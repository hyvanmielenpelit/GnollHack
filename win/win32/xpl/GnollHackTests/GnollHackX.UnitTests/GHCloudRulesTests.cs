using System;
using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHCloudRules: grant batching and expiry, endpoint derivation, the replay upload rule,
       the classification of server and Azure refusals, and the age limit of queued game posts. */
    public class GHCloudRulesTests
    {
        private static List<int> Numbers(int count)
        {
            List<int> list = new List<int>();
            int i;
            for (i = 0; i < count; i++)
                list.Add(i);
            return list;
        }

        [Theory]
        [InlineData(0, 0)]
        [InlineData(1, 1)]
        [InlineData(32, 1)]
        [InlineData(33, 2)]
        [InlineData(65, 3)]
        public void SplitIntoBatches_MakesBatchesOfAtMostThirtyTwo(int count, int expectedBatches)
        {
            List<List<int>> batches = GHCloudRules.SplitIntoBatches(Numbers(count), GHCloudRules.MaxPathsPerGrantRequest);

            Assert.Equal(expectedBatches, batches.Count);
            int total = 0;
            int next = 0;
            foreach (List<int> batch in batches)
            {
                Assert.InRange(batch.Count, 1, GHCloudRules.MaxPathsPerGrantRequest);
                foreach (int n in batch)
                {
                    Assert.Equal(next, n);
                    next++;
                }
                total += batch.Count;
            }
            Assert.Equal(count, total);
        }

        [Fact]
        public void SplitIntoBatches_NullGivesNoBatches()
        {
            Assert.Empty(GHCloudRules.SplitIntoBatches<int>(null, 32));
        }

        [Fact]
        public void SplitIntoBatches_NonPositiveSizeMeansOne()
        {
            Assert.Equal(3, GHCloudRules.SplitIntoBatches(Numbers(3), 0).Count);
        }

        [Fact]
        public void IsGrantUsable_TrueUntilOneMinuteBeforeExpiry()
        {
            DateTimeOffset expires = new DateTimeOffset(2026, 11, 26, 12, 0, 0, TimeSpan.Zero);

            Assert.True(GHCloudRules.IsGrantUsable(expires, expires.AddMinutes(-1).AddSeconds(-1)));
            Assert.False(GHCloudRules.IsGrantUsable(expires, expires.AddMinutes(-1)));
            Assert.False(GHCloudRules.IsGrantUsable(expires, expires.AddSeconds(-59)));
            Assert.False(GHCloudRules.IsGrantUsable(expires, expires.AddMinutes(5)));
        }

        [Theory]
        [InlineData("https://example.test/xlogfile", "/api/cloud/replay/list", "https://example.test/api/cloud/replay/list")]
        [InlineData("https://test-example.test/xlogfile", "/api/forumpost", "https://test-example.test/api/forumpost")]
        [InlineData("https://example.test/sub/xlogfile", "/api/forumpost", "https://example.test/sub/api/forumpost")]
        public void TryDeriveEndpoint_ReplacesTheXlogPath(string address, string apiPath, string expected)
        {
            Assert.Equal(expected, GHCloudRules.TryDeriveEndpoint(address, apiPath));
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData("http://example.test/xlogfile")]
        [InlineData("https://example.test/xlog")]
        [InlineData("https://example.test/xlogfile/")]
        [InlineData("https://example.test/xlogfiles")]
        [InlineData("https:///xlogfile")]
        public void TryDeriveEndpoint_RefusesOtherAddresses(string address)
        {
            Assert.Null(GHCloudRules.TryDeriveEndpoint(address, "/api/forumpost"));
        }

        [Fact]
        public void TryDeriveEndpoint_RefusesAnEmptyApiPath()
        {
            Assert.Null(GHCloudRules.TryDeriveEndpoint("https://example.test/xlogfile", ""));
        }

        [Theory]
        [InlineData("conn", false, "", "", true)]
        [InlineData("conn", true, "", "", false)]
        [InlineData("conn", true, "user", "pass", true)]
        [InlineData("", false, "user", "pass", true)]
        [InlineData("", false, "user", "", false)]
        [InlineData("", false, "", "pass", false)]
        [InlineData("  ", false, " ", " ", false)]
        [InlineData(null, false, null, null, false)]
        [InlineData("", true, "user", "pass", true)]
        public void CanUploadReplays_NeedsCustomStorageOrAnAccount(string custom, bool tournament, string user, string password, bool expected)
        {
            Assert.Equal(expected, GHCloudRules.CanUploadReplays(custom, tournament, user, password));
        }

        [Theory]
        [InlineData(200, GHCloudRefusal.None)]
        [InlineData(204, GHCloudRefusal.None)]
        [InlineData(401, GHCloudRefusal.Credentials)]
        [InlineData(403, GHCloudRefusal.Credentials)]
        [InlineData(412, GHCloudRefusal.Credentials)]
        [InlineData(429, GHCloudRefusal.RateLimited)]
        [InlineData(400, GHCloudRefusal.BadRequest)]
        [InlineData(404, GHCloudRefusal.BadRequest)]
        [InlineData(423, GHCloudRefusal.BadRequest)]
        [InlineData(500, GHCloudRefusal.ServerError)]
        [InlineData(502, GHCloudRefusal.ServerError)]
        [InlineData(0, GHCloudRefusal.ServerError)]
        public void ClassifyServerResponse_MapsStatusCodes(int status, GHCloudRefusal expected)
        {
            Assert.Equal(expected, GHCloudRules.ClassifyServerResponse(status));
        }

        [Theory]
        [InlineData(409, "BlobAlreadyExists", true)]
        [InlineData(403, "AuthorizationPermissionMismatch", true)]
        [InlineData(409, "ContainerAlreadyExists", false)]
        [InlineData(403, "AuthenticationFailed", false)]
        [InlineData(412, "ConditionNotMet", false)]
        [InlineData(409, null, false)]
        public void IsLockHeld_RecognizesBothAnswersToACreateOnlyUpload(int status, string errorCode, bool expected)
        {
            Assert.Equal(expected, GHCloudRules.IsLockHeld(status, errorCode));
        }

        [Theory]
        [InlineData(403, "AuthenticationFailed", true)]
        [InlineData(403, "AuthorizationPermissionMismatch", false)]
        [InlineData(401, "AuthenticationFailed", false)]
        [InlineData(403, null, false)]
        public void IsExpiredGrant_IsOnlyAnAuthenticationFailure(int status, string errorCode, bool expected)
        {
            Assert.Equal(expected, GHCloudRules.IsExpiredGrant(status, errorCode));
        }

        [Fact]
        public void IsStaleQueuedGamePost_AfterTwentyFourHours()
        {
            DateTime queued = new DateTime(2026, 11, 26, 12, 0, 0, DateTimeKind.Utc);

            Assert.False(GHCloudRules.IsStaleQueuedGamePost(queued, queued.AddHours(24)));
            Assert.True(GHCloudRules.IsStaleQueuedGamePost(queued, queued.AddHours(24).AddSeconds(1)));
            Assert.False(GHCloudRules.IsStaleQueuedGamePost(queued, queued.AddMinutes(5)));
        }
    }
}
