using Xunit;

namespace GnollHackX.UnitTests
{
    public class GHRefListTests
    {
        private struct Item
        {
            public int Value;
            public string Tag;
        }

        [Fact]
        public void Indexer_ReturnsReferenceThatMutatesInPlace()
        {
            GHRefList<Item> list = new GHRefList<Item>();
            list.Add(new Item { Value = 1, Tag = "a" });

            ref Item r = ref list[0];
            r.Value = 42;

            Assert.Equal(42, list[0].Value);
            Assert.Equal("a", list[0].Tag);
        }

        [Fact]
        public void AddDefault_ReturnsReferenceToNewSlot()
        {
            GHRefList<Item> list = new GHRefList<Item>();
            ref Item r = ref list.AddDefault();
            r.Value = 7;
            r.Tag = "seven";

            Assert.Equal(1, list.Count);
            Assert.Equal(7, list[0].Value);
            Assert.Equal("seven", list[0].Tag);
        }

        [Fact]
        public void Clear_ResetsCount()
        {
            GHRefList<int> list = new GHRefList<int>();
            list.Add(1);
            list.Add(2);
            Assert.Equal(2, list.Count);

            list.Clear();
            Assert.Equal(0, list.Count);
            Assert.Throws<ArgumentOutOfRangeException>(() => list[0]);
        }

        [Fact]
        public void Growth_AllocatesNewBackingArray()
        {
            GHRefList<int> list = new GHRefList<int>(2);
            Assert.Equal(2, list.Capacity);
            list.Add(1);
            list.Add(2);

            /* Per the header comment, a reference taken before a growth points into the
               abandoned array: writes through it no longer reach the list. */
            ref int stale = ref list[0];

            list.Add(3);
            Assert.Equal(4, list.Capacity);
            Assert.Equal(3, list.Count);

            stale = 99;
            Assert.Equal(1, list[0]);
        }

        [Fact]
        public void Growth_FromEmpty_UsesDefaultCapacityThenDoubles()
        {
            GHRefList<int> list = new GHRefList<int>();
            Assert.Equal(0, list.Capacity);
            list.Add(1);
            Assert.Equal(4, list.Capacity);
            int i;
            for (i = 0; i < 4; i++)
                list.Add(i);
            Assert.Equal(8, list.Capacity);
            Assert.Equal(5, list.Count);
        }

        [Fact]
        public void EnsureCapacity_GrowsWithoutChangingCount()
        {
            GHRefList<int> list = new GHRefList<int>();
            list.Add(5);
            list.EnsureCapacity(100);
            Assert.True(list.Capacity >= 100);
            Assert.Equal(1, list.Count);
            Assert.Equal(5, list[0]);
        }

        [Fact]
        public void Insert_KeepsOrder()
        {
            GHRefList<int> list = new GHRefList<int>();
            list.Add(1);
            list.Add(3);
            list.Insert(1, 2);
            list.Insert(0, 0);
            list.Insert(list.Count, 4);

            Assert.Equal(new int[] { 0, 1, 2, 3, 4 }, list.ToArray());
        }

        [Fact]
        public void Insert_AliasedElement_CopiesBeforeShift()
        {
            GHRefList<int> list = new GHRefList<int>(8);
            list.Add(10);
            list.Add(20);
            list.Add(30);

            /* The inserted item aliases the slot the shift overwrites */
            list.Insert(0, in list[2]);

            Assert.Equal(new int[] { 30, 10, 20, 30 }, list.ToArray());
        }

        [Fact]
        public void RemoveAt_KeepsOrder()
        {
            GHRefList<int> list = new GHRefList<int>();
            int i;
            for (i = 0; i < 5; i++)
                list.Add(i);

            list.RemoveAt(2);
            Assert.Equal(new int[] { 0, 1, 3, 4 }, list.ToArray());

            list.RemoveAt(0);
            Assert.Equal(new int[] { 1, 3, 4 }, list.ToArray());

            list.RemoveAt(list.Count - 1);
            Assert.Equal(new int[] { 1, 3 }, list.ToArray());
        }

        [Fact]
        public void RemoveAtSwapBack_MovesLastIntoHole()
        {
            GHRefList<int> list = new GHRefList<int>();
            int i;
            for (i = 0; i < 5; i++)
                list.Add(i);

            list.RemoveAtSwapBack(1);
            Assert.Equal(new int[] { 0, 4, 2, 3 }, list.ToArray());
        }

        [Fact]
        public void Remove_FindsAndRemovesFirstMatch()
        {
            GHRefList<int> list = new GHRefList<int>();
            list.Add(1);
            list.Add(2);
            list.Add(1);

            Assert.True(list.Remove(1));
            Assert.Equal(new int[] { 2, 1 }, list.ToArray());
            Assert.False(list.Remove(9));
            Assert.Equal(1, list.IndexOf(1));
            Assert.True(list.Contains(2));
        }

        [Fact]
        public void Enumerator_YieldsAllItems()
        {
            GHRefList<int> list = new GHRefList<int>();
            int i;
            for (i = 1; i <= 5; i++)
                list.Add(i * 10);

            List<int> seen = new List<int>();
            foreach (int v in list)
                seen.Add(v);
            Assert.Equal(new int[] { 10, 20, 30, 40, 50 }, seen.ToArray());

            /* The interface path yields the same sequence */
            Assert.Equal(seen, ((IEnumerable<int>)list).ToList());
        }

        [Fact]
        public void Enumerator_RefIteration_MutatesInPlace()
        {
            GHRefList<Item> list = new GHRefList<Item>();
            list.Add(new Item { Value = 1 });
            list.Add(new Item { Value = 2 });

            foreach (ref Item item in list)
                item.Value *= 10;

            Assert.Equal(10, list[0].Value);
            Assert.Equal(20, list[1].Value);
        }

        [Fact]
        public void AsSpan_CoversLiveElementsOnly()
        {
            GHRefList<int> list = new GHRefList<int>(16);
            list.Add(1);
            list.Add(2);
            Span<int> span = list.AsSpan();
            Assert.Equal(2, span.Length);
            span[1] = 5;
            Assert.Equal(5, list[1]);
        }

        [Fact]
        public void IndexerOutOfRange_Throws()
        {
            GHRefList<int> list = new GHRefList<int>();
            list.Add(1);
            Assert.Throws<ArgumentOutOfRangeException>(() => list[1]);
            Assert.Throws<ArgumentOutOfRangeException>(() => list[-1]);
            Assert.Throws<ArgumentOutOfRangeException>(() => list.RemoveAt(1));
            Assert.Throws<ArgumentOutOfRangeException>(() => list.Insert(2, 0));
        }

        [Fact]
        public void SortReverseAndTrimExcess_Work()
        {
            GHRefList<int> list = new GHRefList<int>(new int[] { 3, 1, 2 });
            list.Sort();
            Assert.Equal(new int[] { 1, 2, 3 }, list.ToArray());
            list.Reverse();
            Assert.Equal(new int[] { 3, 2, 1 }, list.ToArray());
            list.EnsureCapacity(64);
            list.TrimExcess();
            Assert.Equal(3, list.Capacity);
        }
    }
}
