using System.Net;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHSaveFileTrackingResults: which use statuses are rejections, which need a credential
       check, and the outcome of a rejection for every check result, silent or not. */
    public class GHSaveFileTrackingResultsTests
    {
        private const HttpStatusCode Unprocessable = (HttpStatusCode)422;
        private const HttpStatusCode Locked = (HttpStatusCode)423;
        private const HttpStatusCode TooManyRequests = (HttpStatusCode)429;
        private const HttpStatusCode NoStatus = (HttpStatusCode)0;

        /* ---- Rejections ---- */

        [Theory]
        [InlineData(HttpStatusCode.Forbidden, true)]
        [InlineData(Unprocessable, true)]
        [InlineData(HttpStatusCode.BadRequest, false)]
        [InlineData(HttpStatusCode.NotFound, false)]
        [InlineData(HttpStatusCode.Conflict, false)]
        [InlineData(HttpStatusCode.Gone, false)]
        [InlineData(HttpStatusCode.PreconditionFailed, false)]
        [InlineData(Locked, false)]
        [InlineData(HttpStatusCode.InternalServerError, false)]
        public void IsRejection_MatchesOnly403And422(HttpStatusCode useStatus, bool expected)
        {
            Assert.Equal(expected, GHSaveFileTrackingResults.IsRejection(useStatus));
        }

        [Theory]
        [InlineData(HttpStatusCode.Forbidden, true)]
        [InlineData(Unprocessable, false)]
        public void NeedsCredentialCheck_OnlyFor403(HttpStatusCode useStatus, bool expected)
        {
            Assert.Equal(expected, GHSaveFileTrackingResults.NeedsCredentialCheck(useStatus));
        }

        /* ---- 403 from use, followed by a credential check ---- */

        [Theory]
        [InlineData(HttpStatusCode.OK, false, SaveFileTrackingRejection.SaveFileNotAccepted)]
        [InlineData(HttpStatusCode.OK, true, SaveFileTrackingRejection.SaveFileNotAcceptedSilently)]
        [InlineData(HttpStatusCode.Created, false, SaveFileTrackingRejection.SaveFileNotAccepted)]
        [InlineData(HttpStatusCode.Created, true, SaveFileTrackingRejection.SaveFileNotAcceptedSilently)]
        public void Decide_Forbidden_CheckSucceeded_IsSaveFileNotAccepted(HttpStatusCode checkStatus, bool isSilent,
            SaveFileTrackingRejection expected)
        {
            Assert.Equal(expected, GHSaveFileTrackingResults.Decide(HttpStatusCode.Forbidden, true, true, checkStatus, isSilent));
        }

        [Theory]
        [InlineData(false, SaveFileTrackingRejection.SaveFileNotAccepted)]
        [InlineData(true, SaveFileTrackingRejection.SaveFileNotAcceptedSilently)]
        public void Decide_Forbidden_Check423_IsSaveFileNotAccepted(bool isSilent, SaveFileTrackingRejection expected)
        {
            Assert.Equal(expected, GHSaveFileTrackingResults.Decide(HttpStatusCode.Forbidden, false, true, Locked, isSilent));
        }

        [Theory]
        [InlineData(false)]
        [InlineData(true)]
        public void Decide_Forbidden_Check403_IsCredentialsRefused(bool isSilent)
        {
            Assert.Equal(SaveFileTrackingRejection.CredentialsRefused,
                GHSaveFileTrackingResults.Decide(HttpStatusCode.Forbidden, false, true, HttpStatusCode.Forbidden, isSilent));
        }

        [Theory]
        [InlineData(false)]
        [InlineData(true)]
        public void Decide_Forbidden_CheckWithoutStatusCode_IsCheckFailed(bool isSilent)
        {
            Assert.Equal(SaveFileTrackingRejection.CheckFailed,
                GHSaveFileTrackingResults.Decide(HttpStatusCode.Forbidden, false, false, NoStatus, isSilent));
        }

        [Theory]
        [InlineData(HttpStatusCode.Unauthorized, false)]
        [InlineData(HttpStatusCode.Unauthorized, true)]
        [InlineData(HttpStatusCode.NotFound, false)]
        [InlineData(HttpStatusCode.PreconditionFailed, false)]
        [InlineData(HttpStatusCode.PreconditionFailed, true)]
        [InlineData(TooManyRequests, false)]
        [InlineData(HttpStatusCode.InternalServerError, false)]
        [InlineData(HttpStatusCode.InternalServerError, true)]
        public void Decide_Forbidden_CheckOtherStatus_IsCheckFailed(HttpStatusCode checkStatus, bool isSilent)
        {
            Assert.Equal(SaveFileTrackingRejection.CheckFailed,
                GHSaveFileTrackingResults.Decide(HttpStatusCode.Forbidden, false, true, checkStatus, isSilent));
        }

        /* ---- 422 from use, without a check ---- */

        [Theory]
        [InlineData(false, SaveFileTrackingRejection.SaveFileNotAccepted)]
        [InlineData(true, SaveFileTrackingRejection.SaveFileNotAcceptedSilently)]
        public void Decide_Unprocessable_IsSaveFileNotAccepted(bool isSilent, SaveFileTrackingRejection expected)
        {
            Assert.Equal(expected, GHSaveFileTrackingResults.Decide(Unprocessable, false, false, NoStatus, isSilent));
        }

        /* ---- Not a rejection ---- */

        [Fact]
        public void Decide_Conflict_IsCheckFailed()
        {
            Assert.Equal(SaveFileTrackingRejection.CheckFailed,
                GHSaveFileTrackingResults.Decide(HttpStatusCode.Conflict, true, true, HttpStatusCode.OK, false));
        }
    }
}
