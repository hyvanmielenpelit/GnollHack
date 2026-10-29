using System;
using System.Collections.Generic;
using System.Globalization;

namespace GnollHackX
{
    /* A rectangle in viewport-relative CSS pixels */
    public struct GHWebRect
    {
        public GHWebRect(double x, double y, double width, double height)
        {
            X = x;
            Y = y;
            Width = width;
            Height = height;
        }

        public double X { get; }
        public double Y { get; }
        public double Width { get; }
        public double Height { get; }

        public double Left => X;
        public double Top => Y;
        public double Right => X + Width;
        public double Bottom => Y + Height;
        public double CenterX => X + Width / 2.0;
        public double CenterY => Y + Height / 2.0;

        /* Zero or negative width or height; such a fragment is never scored */
        public bool IsEmpty => !(Width > 0.0) || !(Height > 0.0);
    }

    public enum GHWebDirection
    {
        Up,
        Down,
        Left,
        Right
    }

    /* A focusable element of the page: its DOM-order index among all candidates, its kind (a link, b button or
       other control, t single-line text field, m multiple-line text field, s select, u summary) and its
       getClientRects() fragments */
    public sealed class GHWebCandidate
    {
        public GHWebCandidate(int index, char kind, GHWebRect[] fragments)
        {
            Index = index;
            Kind = kind;
            Fragments = fragments ?? new GHWebRect[0];
        }

        public int Index { get; }
        public char Kind { get; }
        public GHWebRect[] Fragments { get; }
    }

    /* One result of the page script's snapshot(reach):
       S~seq~vw~vh~dpr~sx~sy~maxX~maxY~focus~flags~items, items separated by ';', each index,kind,x,y,w,h[,x,y,w,h]...
       with one to four fragments. Candidates are in DOM order; FocusPosition is a position in Candidates or -1. */
    public sealed class GHWebSnapshot
    {
        public const int FieldCount = 12;
        public const int MaxFragments = 4;
        public const string Kinds = "abtmsu";

        public GHWebSnapshot()
        {
            Candidates = new List<GHWebCandidate>();
            FocusPosition = -1;
            DevicePixelRatio = 1.0;
        }

        public int Seq { get; set; }
        public double ViewportWidth { get; set; }
        public double ViewportHeight { get; set; }
        public double DevicePixelRatio { get; set; }
        public double ScrollX { get; set; }
        public double ScrollY { get; set; }
        public double MaxScrollX { get; set; }
        public double MaxScrollY { get; set; }
        public int FocusPosition { get; set; }
        public bool RingHidden { get; set; }
        public bool FocusLost { get; set; }
        public bool Editing { get; set; }
        public List<GHWebCandidate> Candidates { get; }

        /* The focused candidate's kind, or '\0' without a focus */
        public char FocusKind
        {
            get
            {
                if (FocusPosition < 0 || FocusPosition >= Candidates.Count)
                    return '\0';
                return Candidates[FocusPosition].Kind;
            }
        }

        /* False, with a null snapshot, for a wrong field count or prefix, an unparsable or non-finite number, a
           negative viewport size or fragment size, a non-positive pixel ratio, an unknown kind, a fragment count
           outside 1..4, an empty item entry, or a focus outside the items. Unknown flag characters are ignored. */
        public static bool TryParse(string text, out GHWebSnapshot snapshot)
        {
            snapshot = null;
            if (string.IsNullOrEmpty(text))
                return false;
            string[] fields = text.Split('~');
            if (fields.Length != FieldCount || fields[0] != "S")
                return false;

            int seq, focus;
            double vw, vh, dpr, sx, sy, maxX, maxY;
            if (!TryParseInt(fields[1], out seq)
                || !TryParseDouble(fields[2], out vw)
                || !TryParseDouble(fields[3], out vh)
                || !TryParseDouble(fields[4], out dpr)
                || !TryParseDouble(fields[5], out sx)
                || !TryParseDouble(fields[6], out sy)
                || !TryParseDouble(fields[7], out maxX)
                || !TryParseDouble(fields[8], out maxY)
                || !TryParseInt(fields[9], out focus))
                return false;
            if (vw < 0.0 || vh < 0.0 || dpr <= 0.0 || focus < -1)
                return false;

            GHWebSnapshot result = new GHWebSnapshot();
            result.Seq = seq;
            result.ViewportWidth = vw;
            result.ViewportHeight = vh;
            result.DevicePixelRatio = dpr;
            result.ScrollX = sx;
            result.ScrollY = sy;
            result.MaxScrollX = maxX;
            result.MaxScrollY = maxY;

            foreach (char flag in fields[10])
            {
                switch (flag)
                {
                case 'h':
                    result.RingHidden = true;
                    break;
                case 'l':
                    result.FocusLost = true;
                    break;
                case 'e':
                    result.Editing = true;
                    break;
                default:
                    break;
                }
            }

            if (fields[11].Length > 0)
            {
                string[] items = fields[11].Split(';');
                foreach (string item in items)
                {
                    GHWebCandidate candidate;
                    if (!TryParseCandidate(item, out candidate))
                        return false;
                    result.Candidates.Add(candidate);
                }
            }

            if (focus >= result.Candidates.Count)
                return false;
            result.FocusPosition = focus;
            snapshot = result;
            return true;
        }

        private static bool TryParseCandidate(string item, out GHWebCandidate candidate)
        {
            candidate = null;
            string[] parts = item.Split(',');
            if (parts.Length < 6 || (parts.Length - 2) % 4 != 0)
                return false;
            int count = (parts.Length - 2) / 4;
            if (count > MaxFragments)
                return false;

            int index;
            if (!TryParseInt(parts[0], out index) || index < 0)
                return false;
            if (parts[1].Length != 1 || Kinds.IndexOf(parts[1][0]) < 0)
                return false;

            GHWebRect[] fragments = new GHWebRect[count];
            for (int i = 0; i < count; i++)
            {
                int p = 2 + 4 * i;
                double x, y, w, h;
                if (!TryParseDouble(parts[p], out x)
                    || !TryParseDouble(parts[p + 1], out y)
                    || !TryParseDouble(parts[p + 2], out w)
                    || !TryParseDouble(parts[p + 3], out h))
                    return false;
                if (w < 0.0 || h < 0.0)
                    return false;
                fragments[i] = new GHWebRect(x, y, w, h);
            }
            candidate = new GHWebCandidate(index, parts[1][0], fragments);
            return true;
        }

        private static bool TryParseInt(string text, out int value)
        {
            return int.TryParse(text, NumberStyles.AllowLeadingSign, CultureInfo.InvariantCulture, out value);
        }

        private static bool TryParseDouble(string text, out double value)
        {
            NumberStyles styles = NumberStyles.AllowLeadingSign | NumberStyles.AllowDecimalPoint
                | NumberStyles.AllowExponent;
            if (!double.TryParse(text, styles, CultureInfo.InvariantCulture, out value))
                return false;
            return !double.IsNaN(value) && !double.IsInfinity(value);
        }
    }

    /* Gamepad focus movement between the candidates of a web page snapshot, after the W3C CSS Spatial Navigation
       draft simplified to rectangles. Positions index GHWebSnapshot.Candidates; reading order and ties use the DOM
       index GHWebCandidate.Index. */
    public static class GHWebSpatialNavigation
    {
        /* Up/Down: score = euclid + OrthogonalWeight * orth - OverlapWeight * overlap / ref.Width */
        public const double OrthogonalWeight = 2.0;
        public const double OverlapWeight = 5.0;

        /* Line tolerance: LineToleranceFactor * the smaller of the two heights */
        public const double LineToleranceFactor = 0.5;

        /* Left/Right: how far a same-line fragment may start inside the reference */
        public const double HorizontalSlack = 1.0;

        /* How far above the viewport top a candidate may start and still count as inside it */
        public const double TopVisibleTolerance = 1.0;

        public const double MinimumScrollStep = 48.0;
        public const double ScrollStepFraction = 0.2;

        /* Scores closer than this are a tie */
        private const double ScoreEpsilon = 1e-9;

        /* A script result as a plain string: null, empty, whitespace, null and undefined give null; a JSON string
           literal (WebView2) is decoded, and an empty decoded value gives null; one with an invalid escape or an
           unescaped quote is returned as is; anything else (WebKitGTK, CEF) is returned trimmed */
        public static string NormalizeScriptResult(string raw)
        {
            if (raw == null)
                return null;
            string trimmed = raw.Trim();
            if (trimmed.Length == 0 || trimmed == "null" || trimmed == "undefined")
                return null;
            if (trimmed.Length >= 2 && trimmed[0] == '"' && trimmed[trimmed.Length - 1] == '"')
            {
                string decoded;
                if (!TryDecodeJsonString(trimmed, 1, trimmed.Length - 1, out decoded))
                    return trimmed;
                return decoded.Length == 0 ? null : decoded;
            }
            return trimmed;
        }

        /* The body of a JSON string literal between start (inclusive) and end (exclusive) */
        private static bool TryDecodeJsonString(string text, int start, int end, out string decoded)
        {
            decoded = null;
            char[] buffer = new char[end - start];
            int n = 0;
            for (int i = start; i < end; i++)
            {
                char c = text[i];
                if (c == '"')
                    return false;
                if (c != '\\')
                {
                    buffer[n++] = c;
                    continue;
                }
                i++;
                if (i >= end)
                    return false;
                switch (text[i])
                {
                case '"':
                    buffer[n++] = '"';
                    break;
                case '\\':
                    buffer[n++] = '\\';
                    break;
                case '/':
                    buffer[n++] = '/';
                    break;
                case 'b':
                    buffer[n++] = '\b';
                    break;
                case 'f':
                    buffer[n++] = '\f';
                    break;
                case 'n':
                    buffer[n++] = '\n';
                    break;
                case 'r':
                    buffer[n++] = '\r';
                    break;
                case 't':
                    buffer[n++] = '\t';
                    break;
                case 'u':
                    {
                        if (i + 4 >= end)
                            return false;
                        int code = 0;
                        for (int k = 1; k <= 4; k++)
                        {
                            int digit = HexValue(text[i + k]);
                            if (digit < 0)
                                return false;
                            code = code * 16 + digit;
                        }
                        buffer[n++] = (char)code;
                        i += 4;
                        break;
                    }
                default:
                    return false;
                }
            }
            decoded = new string(buffer, 0, n);
            return true;
        }

        private static int HexValue(char c)
        {
            if (c >= '0' && c <= '9')
                return c - '0';
            if (c >= 'a' && c <= 'f')
                return c - 'a' + 10;
            if (c >= 'A' && c <= 'F')
                return c - 'A' + 10;
            return -1;
        }

        /* Down/Up without a target scroll by this many CSS px */
        public static double ScrollStep(double viewportHeight)
        {
            return Math.Max(MinimumScrollStep, ScrollStepFraction * viewportHeight);
        }

        /* A focus to move from: set, inside Candidates, not reported lost, and with a non-empty fragment */
        public static bool HasUsableFocus(GHWebSnapshot s)
        {
            if (s == null || s.FocusLost || s.FocusPosition < 0 || s.FocusPosition >= s.Candidates.Count)
                return false;
            return HasUsableFragment(s.Candidates[s.FocusPosition]);
        }

        /* The position of the candidate the focus moves to in direction dir, or -1 when none is eligible or the
           focus is not usable (see HasUsableFocus; the caller then uses FindTopVisible) */
        public static int FindTarget(GHWebSnapshot s, GHWebDirection dir)
        {
            if (!HasUsableFocus(s))
                return -1;
            GHWebCandidate focus = s.Candidates[s.FocusPosition];
            GHWebRect reference = ReferenceFragment(focus, dir);
            int target = FindBestScored(s, reference, dir);
            if (target >= 0 || dir == GHWebDirection.Up || dir == GHWebDirection.Down)
                return target;
            return FindReadingOrder(s, focus.Index, dir == GHWebDirection.Right);
        }

        /* The candidate whose first fragment starts highest within the viewport (top >= -TopVisibleTolerance and
           above its bottom edge), ties to the lower DOM index; else the first in DOM order with a fragment
           overlapping the viewport vertically; else -1 */
        public static int FindTopVisible(GHWebSnapshot s)
        {
            if (s == null)
                return -1;
            int best = -1;
            double bestTop = 0.0;
            int overlapping = -1;
            for (int i = 0; i < s.Candidates.Count; i++)
            {
                GHWebCandidate c = s.Candidates[i];
                GHWebRect first;
                if (!TryGetFirstFragment(c, out first))
                    continue;
                if (first.Top >= -TopVisibleTolerance && first.Top < s.ViewportHeight)
                {
                    if (best < 0 || first.Top < bestTop
                        || (first.Top == bestTop && c.Index < s.Candidates[best].Index))
                    {
                        best = i;
                        bestTop = first.Top;
                    }
                }
                else if (overlapping < 0 && OverlapsViewportVertically(c, s.ViewportHeight))
                {
                    overlapping = i;
                }
            }
            return best >= 0 ? best : overlapping;
        }

        /* The focus's fragment furthest in dir: lowest-bottomed for Down, highest-topped for Up, the last for
           Right, the first for Left. The candidate has at least one non-empty fragment. */
        private static GHWebRect ReferenceFragment(GHWebCandidate focus, GHWebDirection dir)
        {
            GHWebRect best = default(GHWebRect);
            bool found = false;
            foreach (GHWebRect f in focus.Fragments)
            {
                if (f.IsEmpty)
                    continue;
                if (!found)
                {
                    best = f;
                    found = true;
                    continue;
                }
                switch (dir)
                {
                case GHWebDirection.Down:
                    if (f.Bottom > best.Bottom)
                        best = f;
                    break;
                case GHWebDirection.Up:
                    if (f.Top < best.Top)
                        best = f;
                    break;
                case GHWebDirection.Right:
                    best = f;
                    break;
                default:
                    break;
                }
            }
            return best;
        }

        /* The lowest-scoring candidate other than the focus, a candidate scoring as its best fragment; ties to the
           lower DOM index */
        private static int FindBestScored(GHWebSnapshot s, GHWebRect reference, GHWebDirection dir)
        {
            int best = -1;
            double bestScore = 0.0;
            for (int i = 0; i < s.Candidates.Count; i++)
            {
                if (i == s.FocusPosition)
                    continue;
                GHWebCandidate c = s.Candidates[i];
                bool found = false;
                double candidateScore = 0.0;
                foreach (GHWebRect f in c.Fragments)
                {
                    double score;
                    if (f.IsEmpty || !TryScore(reference, f, dir, out score))
                        continue;
                    if (!found || score < candidateScore)
                    {
                        candidateScore = score;
                        found = true;
                    }
                }
                if (!found)
                    continue;
                if (best < 0 || candidateScore < bestScore - ScoreEpsilon
                    || (candidateScore <= bestScore + ScoreEpsilon && c.Index < s.Candidates[best].Index))
                {
                    best = i;
                    bestScore = candidateScore;
                }
            }
            return best;
        }

        /* Eligibility and score of fragment f from reference. Down: f.Top >= ref.Bottom - tol and f below the
           reference's center; Up mirrors it. Right: on the same line (vertical overlap >= tol), f.Left >=
           ref.Right - HorizontalSlack and f right of the reference's center; Left mirrors it. */
        private static bool TryScore(GHWebRect reference, GHWebRect f, GHWebDirection dir, out double score)
        {
            score = 0.0;
            double tol = LineToleranceFactor * Math.Min(reference.Height, f.Height);
            double overlap;
            switch (dir)
            {
            case GHWebDirection.Down:
                if (f.Top < reference.Bottom - tol || f.CenterY <= reference.CenterY)
                    return false;
                score = VerticalScore(reference, f, f.Top - reference.Bottom);
                return true;
            case GHWebDirection.Up:
                if (f.Bottom > reference.Top + tol || f.CenterY >= reference.CenterY)
                    return false;
                score = VerticalScore(reference, f, reference.Top - f.Bottom);
                return true;
            case GHWebDirection.Right:
                overlap = Overlap(reference.Top, reference.Bottom, f.Top, f.Bottom);
                if (overlap < tol || f.Left < reference.Right - HorizontalSlack || f.CenterX <= reference.CenterX)
                    return false;
                score = Math.Max(0.0, f.Left - reference.Right) - OverlapWeight * overlap / reference.Height;
                return true;
            case GHWebDirection.Left:
                overlap = Overlap(reference.Top, reference.Bottom, f.Top, f.Bottom);
                if (overlap < tol || f.Right > reference.Left + HorizontalSlack || f.CenterX >= reference.CenterX)
                    return false;
                score = Math.Max(0.0, reference.Left - f.Right) - OverlapWeight * overlap / reference.Height;
                return true;
            default:
                return false;
            }
        }

        /* euclid + OrthogonalWeight * orth - OverlapWeight * overlap / ref.Width, with primary the gap along the
           axis, orth the gap between the horizontal intervals and overlap their overlap */
        private static double VerticalScore(GHWebRect reference, GHWebRect f, double axisGap)
        {
            double primary = Math.Max(0.0, axisGap);
            double orth = Math.Max(0.0, Math.Max(f.Left - reference.Right, reference.Left - f.Right));
            double overlap = Overlap(reference.Left, reference.Right, f.Left, f.Right);
            double euclid = Math.Sqrt(primary * primary + orth * orth);
            return euclid + OrthogonalWeight * orth - OverlapWeight * overlap / reference.Width;
        }

        /* Right: the lowest DOM index above focusIndex; Left: the highest below it */
        private static int FindReadingOrder(GHWebSnapshot s, int focusIndex, bool forward)
        {
            int best = -1;
            for (int i = 0; i < s.Candidates.Count; i++)
            {
                if (i == s.FocusPosition)
                    continue;
                GHWebCandidate c = s.Candidates[i];
                if (!HasUsableFragment(c))
                    continue;
                if (forward)
                {
                    if (c.Index > focusIndex && (best < 0 || c.Index < s.Candidates[best].Index))
                        best = i;
                }
                else
                {
                    if (c.Index < focusIndex && (best < 0 || c.Index > s.Candidates[best].Index))
                        best = i;
                }
            }
            return best;
        }

        private static double Overlap(double start1, double end1, double start2, double end2)
        {
            return Math.Max(0.0, Math.Min(end1, end2) - Math.Max(start1, start2));
        }

        private static bool HasUsableFragment(GHWebCandidate c)
        {
            GHWebRect first;
            return TryGetFirstFragment(c, out first);
        }

        /* The first non-empty fragment */
        private static bool TryGetFirstFragment(GHWebCandidate c, out GHWebRect first)
        {
            foreach (GHWebRect f in c.Fragments)
            {
                if (!f.IsEmpty)
                {
                    first = f;
                    return true;
                }
            }
            first = default(GHWebRect);
            return false;
        }

        private static bool OverlapsViewportVertically(GHWebCandidate c, double viewportHeight)
        {
            foreach (GHWebRect f in c.Fragments)
            {
                if (!f.IsEmpty && f.Bottom > 0.0 && f.Top < viewportHeight)
                    return true;
            }
            return false;
        }
    }
}
