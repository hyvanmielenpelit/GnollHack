using System.Reflection;
using GnollHackM;
using SkiaSharp;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* The flush arrays are private, so their length, identity and tail are inspected
       through reflection on the fields GHTileBatch declares. */
    public class GHTileBatchTests : IDisposable
    {
        private const BindingFlags Private = BindingFlags.NonPublic | BindingFlags.Instance;

        private static readonly int FlushGranularity =
            (int)typeof(GHTileBatch).GetField("FlushGranularity", BindingFlags.NonPublic | BindingFlags.Static).GetRawConstantValue();

        private readonly SKSurface _atlasSurface;
        private readonly SKImage _atlas;
        private readonly SKSurface _surface;
        private readonly SKPaint _paint;
        private readonly SKSamplingOptions _sampling = new SKSamplingOptions(SKFilterMode.Nearest);

        public GHTileBatchTests()
        {
            _atlasSurface = SKSurface.Create(new SKImageInfo(64, 64));
            _atlas = _atlasSurface.Snapshot();
            _surface = SKSurface.Create(new SKImageInfo(64, 64));
            _paint = new SKPaint();
        }

        public void Dispose()
        {
            _paint.Dispose();
            _surface.Dispose();
            _atlas.Dispose();
            _atlasSurface.Dispose();
        }

        private static int QuantisedLength(int count)
        {
            return ((count + FlushGranularity - 1) / FlushGranularity) * FlushGranularity;
        }

        private static int Slot(int count)
        {
            return (count + FlushGranularity - 1) / FlushGranularity;
        }

        private static SKRect[][] FlushSprites(GHTileBatch batch)
        {
            return (SKRect[][])typeof(GHTileBatch).GetField("_flushSprites", Private).GetValue(batch);
        }

        private static SKRotationScaleMatrix[][] FlushXforms(GHTileBatch batch)
        {
            return (SKRotationScaleMatrix[][])typeof(GHTileBatch).GetField("_flushXforms", Private).GetValue(batch);
        }

        private static SKColor[][] FlushColors(GHTileBatch batch)
        {
            return (SKColor[][])typeof(GHTileBatch).GetField("_flushColors", Private).GetValue(batch);
        }

        private static SKRect RectFor(int i)
        {
            return new SKRect(i, i + 1, i + 10, i + 20);
        }

        private GHTileBatch NewBatch()
        {
            GHTileBatch batch = new GHTileBatch(16);
            batch.SetFrameGeometry(1f, 0f, 0f, new SKRect(0, 0, 64, 64));
            return batch;
        }

        private void AddSprites(GHTileBatch batch, int sheet, int count)
        {
            int i;
            for (i = 0; i < count; i++)
            {
                SKRect r = RectFor(i);
                batch.Add(sheet, in r, i, i * 2, 1f, 200, 255);
            }
        }

        private int Flush(GHTileBatch batch, int sheet)
        {
            return batch.Flush(_surface.Canvas, sheet, _atlas, _paint, _sampling);
        }

        [Theory]
        [InlineData(1)]
        [InlineData(10)]
        [InlineData(255)]
        [InlineData(256)]
        [InlineData(257)]
        public void Flush_UsesArraysOfQuantisedLength(int count)
        {
            GHTileBatch batch = NewBatch();
            AddSprites(batch, 0, count);
            Assert.Equal(count, batch.Count(0));
            Assert.Equal(count, batch.TotalCount);

            Assert.Equal(count, Flush(batch, 0));
            Assert.Equal(0, batch.Count(0));

            int slot = Slot(count);
            Assert.Equal(QuantisedLength(count), FlushSprites(batch)[slot].Length);
            Assert.Equal(QuantisedLength(count), FlushXforms(batch)[slot].Length);
            Assert.Equal(QuantisedLength(count), FlushColors(batch)[slot].Length);
        }

        [Fact]
        public void Flush_ReusesSlotArrayAcrossFrames()
        {
            GHTileBatch batch = NewBatch();
            AddSprites(batch, 0, 10);
            Flush(batch, 0);
            SKRect[] first = FlushSprites(batch)[Slot(10)];
            SKRotationScaleMatrix[] firstXforms = FlushXforms(batch)[Slot(10)];
            SKColor[] firstColors = FlushColors(batch)[Slot(10)];

            AddSprites(batch, 0, 20);
            Flush(batch, 0);
            Assert.Same(first, FlushSprites(batch)[Slot(20)]);
            Assert.Same(firstXforms, FlushXforms(batch)[Slot(20)]);
            Assert.Same(firstColors, FlushColors(batch)[Slot(20)]);

            /* A different sheet flushes through the same set */
            AddSprites(batch, 1, 5);
            Flush(batch, 1);
            Assert.Same(first, FlushSprites(batch)[Slot(5)]);
        }

        [Fact]
        public void Flush_ZeroesTailPastCount()
        {
            GHTileBatch batch = NewBatch();
            AddSprites(batch, 0, 20);
            Flush(batch, 0);

            AddSprites(batch, 0, 10);
            Flush(batch, 0);

            SKRect[] sprites = FlushSprites(batch)[Slot(10)];
            SKRotationScaleMatrix[] xforms = FlushXforms(batch)[Slot(10)];
            SKColor[] colors = FlushColors(batch)[Slot(10)];
            int i;
            for (i = 0; i < 10; i++)
            {
                Assert.Equal(RectFor(i), sprites[i]);
                Assert.Equal(1f, xforms[i].SCos);
                Assert.Equal(0f, xforms[i].SSin);
                Assert.Equal((float)i, xforms[i].TX);
                Assert.Equal((float)(i * 2), xforms[i].TY);
                Assert.Equal(new SKColor(200, 200, 200, 255), colors[i]);
            }
            for (i = 10; i < sprites.Length; i++)
            {
                Assert.Equal(SKRect.Empty, sprites[i]);
                Assert.Equal(0f, xforms[i].SCos);
                Assert.Equal(0f, xforms[i].SSin);
                Assert.Equal(0f, xforms[i].TX);
                Assert.Equal(0f, xforms[i].TY);
                Assert.Equal(default(SKColor), colors[i]);
            }
        }

        [Fact]
        public void Add_BeyondInitialCapacity_GrowsWithoutLosingEntries()
        {
            GHTileBatch batch = NewBatch();
            int initialSlots = FlushSprites(batch).Length;

            int count = 700;
            AddSprites(batch, 0, count);
            Assert.Equal(count, batch.Count(0));

            Assert.Equal(count, Flush(batch, 0));
            int slot = Slot(count);
            Assert.True(FlushSprites(batch).Length > initialSlots);
            Assert.True(FlushSprites(batch).Length > slot);

            SKRect[] sprites = FlushSprites(batch)[slot];
            Assert.Equal(QuantisedLength(count), sprites.Length);
            int i;
            for (i = 0; i < count; i++)
                Assert.Equal(RectFor(i), sprites[i]);
            for (i = count; i < sprites.Length; i++)
                Assert.Equal(SKRect.Empty, sprites[i]);
        }

        [Fact]
        public void Flush_EmptyBucket_ReturnsZero()
        {
            GHTileBatch batch = NewBatch();
            Assert.Equal(0, Flush(batch, 0));
            Assert.Equal(0, Flush(batch, GHConstants.MaxTileSheets));
            Assert.Equal(0, Flush(batch, -1));
        }

        [Fact]
        public void Flush_NullAtlas_DropsBucketWithoutDrawing()
        {
            GHTileBatch batch = NewBatch();
            AddSprites(batch, 0, 10);
            Assert.Equal(0, batch.Flush(_surface.Canvas, 0, null, _paint, _sampling));
            Assert.Equal(0, batch.Count(0));
            Assert.Null(FlushSprites(batch)[Slot(10)]);
        }

        [Fact]
        public void Add_InvalidSheet_IsIgnored()
        {
            GHTileBatch batch = NewBatch();
            SKRect r = RectFor(0);
            batch.Add(-1, in r, 0, 0, 1f, 255, 255);
            batch.Add(GHConstants.MaxTileSheets, in r, 0, 0, 1f, 255, 255);
            Assert.Equal(0, batch.TotalCount);
            Assert.Equal(0, batch.Count(-1));
            Assert.Equal(0, batch.Count(GHConstants.MaxTileSheets));
        }

        [Fact]
        public void Reset_ClearsEverySheet()
        {
            GHTileBatch batch = NewBatch();
            AddSprites(batch, 0, 3);
            AddSprites(batch, 1, 4);
            Assert.Equal(7, batch.TotalCount);
            batch.Reset();
            Assert.Equal(0, batch.TotalCount);
            Assert.Equal(0, batch.Count(0));
            Assert.Equal(0, batch.Count(1));
        }

        [Fact]
        public void Add_TileOverload_UsesFrameGeometry()
        {
            GHTileBatch batch = new GHTileBatch(16);
            batch.SetFrameGeometry(2f, 0.5f, 0.25f, new SKRect(0, 0, 64, 64));
            batch.Add(0, 64, 96, 3f, 4f, 128, 200);
            Flush(batch, 0);

            SKRect sprite = FlushSprites(batch)[Slot(1)][0];
            Assert.Equal(64f + 0.5f, sprite.Left);
            Assert.Equal(96f + 0.25f, sprite.Top);
            Assert.Equal(64f + GHConstants.TileWidth - 0.5f, sprite.Right);
            Assert.Equal(96f + GHConstants.TileHeight - 0.25f, sprite.Bottom);

            SKRotationScaleMatrix xform = FlushXforms(batch)[Slot(1)][0];
            Assert.Equal(2f, xform.SCos);
            Assert.Equal(3f, xform.TX);
            Assert.Equal(4f, xform.TY);
            Assert.Equal(new SKColor(128, 128, 128, 200), FlushColors(batch)[Slot(1)][0]);
        }
    }
}
