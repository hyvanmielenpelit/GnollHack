using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Threading;
using GnollHackX;
using SkiaSharp;

namespace GnollHackM
{
    /* GPU objects (surfaces, render targets, snapshots) whose disposal waits until GnollHack code runs
       on Avalonia's render thread with a GL context current. Bury may be called from any thread;
       Drain only from a GnollHack draw operation while the given context is current. */
    public static class GHGLResourceGraveyard
    {
        private struct Entry
        {
            public IDisposable Resource;
            public GRContext Owner;
        }

        private static readonly ConcurrentQueue<Entry> _entries = new ConcurrentQueue<Entry>();
        private static int _foreignContextLogged = 0;

        public static bool IsEmpty => _entries.IsEmpty;

        /* Queues a resource created on the owner context */
        public static void Bury(IDisposable resource, GRContext owner)
        {
            if (resource == null)
                return;

            Entry entry;
            entry.Resource = resource;
            entry.Owner = owner;
            _entries.Enqueue(entry);
        }

        /* Disposes every queued resource: those of the current context because it is current, and those
           of any other context because Avalonia replaces its context only after losing the old one, and
           Skia then frees only the CPU-side state of that context's objects */
        public static void Drain(GRContext current)
        {
            if (current == null || _entries.IsEmpty)
                return;

            int foreign = 0;
            Entry entry;
            while (_entries.TryDequeue(out entry))
            {
                if (entry.Owner != null && !ReferenceEquals(entry.Owner, current))
                    foreign++;

                try
                {
                    entry.Resource.Dispose();
                }
                catch (Exception ex)
                {
                    Debug.WriteLine("GHGLResourceGraveyard: " + ex.Message);
                }
            }

            if (foreign > 0 && Interlocked.Exchange(ref _foreignContextLogged, 1) == 0)
                GHApp.MaybeWriteGHLog("GHGLResourceGraveyard: disposed " + foreign.ToString() + " GPU resource(s) of a previous GRContext");
        }
    }
}
