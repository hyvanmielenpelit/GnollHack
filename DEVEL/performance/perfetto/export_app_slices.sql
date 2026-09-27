-- The app's own GH.Tick, GH.Paint and GH.Flush sections, each with the FrameId of the
-- in-app timeline and, for ticks, the frame token of the Choreographer#doFrame slice that
-- contains it, which is the token export_frames.sql reports for the same frame.
-- Capture-AndroidFrames.ps1 substitutes {PACKAGE}. Times are in the trace clock (ns).
SELECT
  s.ts,
  s.dur,
  s.name,
  t.name AS thread_name,
  (
    SELECT CAST(c.value AS INT)
    FROM counter c
    JOIN process_counter_track ct ON c.track_id = ct.id
    WHERE ct.upid = p.upid
      AND ct.name = CASE WHEN s.name = 'GH.Tick' THEN 'GH.FrameId' ELSE 'GH.PaintFrameId' END
      AND c.ts >= s.ts
      AND c.ts <= s.ts + s.dur
    ORDER BY c.ts
    LIMIT 1
  ) AS frame_id,
  (
    SELECT CAST(SUBSTR(d.name, LENGTH('Choreographer#doFrame ') + 1) AS INT)
    FROM slice d
    WHERE d.track_id = s.track_id
      AND d.name GLOB 'Choreographer#doFrame *'
      AND d.ts <= s.ts
      AND d.ts + d.dur >= s.ts + s.dur
    LIMIT 1
  ) AS vsync_token
FROM slice s
JOIN thread_track tt ON s.track_id = tt.id
JOIN thread t USING (utid)
JOIN process p USING (upid)
WHERE p.name = '{PACKAGE}'
  AND s.name IN ('GH.Tick', 'GH.Paint', 'GH.Flush')
ORDER BY s.ts;
