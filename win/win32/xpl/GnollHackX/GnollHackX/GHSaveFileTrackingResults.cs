using System.Net;

namespace GnollHackX
{
    public enum SaveFileTrackingRejection
    {
        /* The server rejected the save file; the credentials are known to be correct */
        SaveFileNotAccepted,
        /* As SaveFileNotAccepted, for a caller that reports the failure itself */
        SaveFileNotAcceptedSilently,
        /* The credentials were refused */
        CredentialsRefused,
        /* The credential check failed for another reason: no connection, a timeout, or another status */
        CheckFailed
    }

    /* Decides what a rejected api/savefiletracking/use request means. The server answers 403 both for
       a refused sign-in and for a save file that does not match its record, so a 403 is followed by a
       credential check; a 422 is sent only after a successful sign-in. */
    public static class GHSaveFileTrackingResults
    {
        /* netstandard2.0's HttpStatusCode has no members for these */
        public const int UnprocessableEntityStatusCode = 422;
        public const int LockedStatusCode = 423;

        public static bool IsRejection(HttpStatusCode useStatus)
        {
            return useStatus == HttpStatusCode.Forbidden || (int)useStatus == UnprocessableEntityStatusCode;
        }

        public static bool NeedsCredentialCheck(HttpStatusCode useStatus)
        {
            return useStatus == HttpStatusCode.Forbidden;
        }

        /* The check fields are ignored unless useStatus is 403. A 423 from the check comes only after a
           successful sign-in. */
        public static SaveFileTrackingRejection Decide(HttpStatusCode useStatus, bool checkSucceeded,
            bool checkHasStatusCode, HttpStatusCode checkStatusCode, bool isSilent)
        {
            bool isForbidden = useStatus == HttpStatusCode.Forbidden;
            bool credentialsAccepted = (int)useStatus == UnprocessableEntityStatusCode
                || (isForbidden && (checkSucceeded || (checkHasStatusCode && (int)checkStatusCode == LockedStatusCode)));

            if (credentialsAccepted)
                return isSilent ? SaveFileTrackingRejection.SaveFileNotAcceptedSilently : SaveFileTrackingRejection.SaveFileNotAccepted;
            if (isForbidden && checkHasStatusCode && checkStatusCode == HttpStatusCode.Forbidden)
                return SaveFileTrackingRejection.CredentialsRefused;
            return SaveFileTrackingRejection.CheckFailed;
        }
    }
}
