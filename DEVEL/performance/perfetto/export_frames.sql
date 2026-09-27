-- The app's frames as SurfaceFlinger's frame timeline saw them, one row per frame token.
-- Capture-AndroidFrames.ps1 substitutes {PACKAGE}. Times are in the trace clock (ns).
-- present_type and jank_type carry SurfaceFlinger's classification, e.g. "On-time Present",
-- "Late Present", "App Deadline Missed", "Buffer Stuffing", "SurfaceFlinger CPU Deadline
-- Missed", "Display HAL", "Prediction Error".
-- Expected frames are aggregated per (name, upid) before the join: SurfaceFlinger can
-- record more than one expected row for the same token, and joining actual rows directly
-- against them would multiply the actual rows one-to-many.
SELECT
  a.name AS token,
  a.ts AS actual_ts,
  a.dur AS actual_dur,
  e.ts AS expected_ts,
  e.dur AS expected_dur,
  a.layer_name,
  a.present_type,
  a.on_time_finish,
  a.gpu_composition,
  a.jank_type,
  a.prediction_type
FROM actual_frame_timeline_slice a
JOIN process p USING (upid)
LEFT JOIN (
  SELECT name, upid, MIN(ts) AS ts, MAX(dur) AS dur
  FROM expected_frame_timeline_slice
  GROUP BY name, upid
) e
  ON e.name = a.name AND e.upid = a.upid
WHERE p.name = '{PACKAGE}'
ORDER BY a.ts;
