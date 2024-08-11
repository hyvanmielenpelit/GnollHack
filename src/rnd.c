/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    rnd.c    $NHDT-Date: 1524689470 2018/04/25 20:51:10 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.18 $ */
/*      Copyright (c) 2004 by Robert Patrick Rankin               */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

#ifdef USE_ISAAC64
#include "isaac64.h"

#if 0
static isaac64_ctx rng_state;
#endif

struct rnglist_t {
    int FDECL((*fn), (int));
    boolean init;
    isaac64_ctx rng_state;
};

enum { CORE = 0, DISP = 1 };

static struct rnglist_t rnglist[] = {
    { rn2, FALSE, { 0 } },                      /* CORE */
    { rn2_on_display_rng, FALSE, { 0 } },       /* DISP */
};

int
whichrng(fn)
int FDECL((*fn), (int));
{
    int i;

    for (i = 0; i < SIZE(rnglist); ++i)
        if (rnglist[i].fn == fn)
            return i;
    return -1;
}

void
init_isaac64(seed, fn)
uint64_t seed;
int FDECL((*fn), (int));
{
    unsigned char new_rng_state[sizeof seed];
    unsigned i;
    int rngindx = whichrng(fn);

    if (rngindx < 0)
    {
        panic("Bad rng function passed to init_isaac64().");
        return;
    }

    for (i = 0; i < sizeof seed; i++) {
        new_rng_state[i] = (unsigned char) (seed & 0xFF);
        seed >>= 8;
    }
    isaac64_init(&rnglist[rngindx].rng_state, new_rng_state,
                 (int) sizeof seed);
}

static int
RND(int x)
{
    return (isaac64_next_uint64(&rnglist[CORE].rng_state) % x);
}

/* 0 <= rn2(x) < x, but on a different sequence from the "main" rn2;
   used in cases where the answer doesn't affect gameplay and we don't
   want to give users easy control over the main RNG sequence. */
int
rn2_on_display_rng(x)
register int x;
{
    return (isaac64_next_uint64(&rnglist[DISP].rng_state) % x);
}

#else   /* USE_ISAAC64 */

/* "Rand()"s definition is determined by [OS]conf.h */
#if defined(LINT) && defined(UNIX) /* rand() is int64_t... */
extern int NDECL(rand);
#define RND(x) (rand() % x)
#else /* LINT */
#if defined(UNIX) || defined(RANDOM)
#define RND(x) ((int) (Rand() % (int64_t) (x)))
#else
/* Good luck: the bottom order bits are cyclic. */
#define RND(x) ((int) ((Rand() >> 3) % (x)))
#endif
#endif /* LINT */
int
rn2_on_display_rng(x)
register int x;
{
    static unsigned seed = 1;
    seed *= 2739110765;
    return (int)((seed >> 16) % (unsigned)x);
}
#endif  /* USE_ISAAC64 */

/* 0 <= rn2(x) < x */
int
rn2(x)
register int x;
{
#ifdef BETA
    if (x <= 0) {
        impossible("rn2(%d) attempted", x);
        return 0;
    }
    x = RND(x);
    return x;
#else
    return RND(x);
#endif
}

/* 0 <= rnl(x) < x; sometimes subtracting Luck;
   good luck approaches 0, bad luck approaches (x-1) */
int
rnl(x)
register int x;
{
    register int i, adjustment;

#ifdef BETA
    if (x <= 0) {
        impossible("rnl(%d) attempted", x);
        return 0;
    }
#endif

    adjustment = Luck;
    if (x <= 15) {
        /* for small ranges, use Luck/3 (rounded away from 0);
           also guard against architecture-specific differences
           of integer division involving negative values */
        adjustment = (abs(adjustment) + 1) / 3 * sgn(adjustment);
        /*
         *       11..13 ->  4
         *        8..10 ->  3
         *        5.. 7 ->  2
         *        2.. 4 ->  1
         *       -1,0,1 ->  0 (no adjustment)
         *       -4..-2 -> -1
         *       -7..-5 -> -2
         *      -10..-8 -> -3
         *      -13..-11-> -4
         */
    }

    i = RND(x);
    if (adjustment && rn2(37 + abs(adjustment))) {
        i -= adjustment;
        if (i < 0)
            i = 0;
        else if (i >= x)
            i = x - 1;
    }
    return i;
}

/* 1 <= rnd(x) <= x */
int
rnd(x)
register int x;
{
#ifdef BETA
    if (x <= 0) {
        impossible("rnd(%d) attempted", x);
        return 1;
    }
#endif
    x = RND(x) + 1;
    return x;
}

/* d(N,X) == NdX == dX+dX+...+dX N times; n <= d(n,x) <= (n*x) */
int
d(n, x)
register int n, x;
{
    if (n <= 0 || x <= 0)
        return 0;

    register int tmp = n;

#ifdef BETA
    if (x < 0 || n < 0 || (x == 0 && n != 0)) {
        impossible("d(%d,%d) attempted", n, x);
        return 1;
    }
#endif

    while (n-- > 0)
        tmp += RND(x);
    return tmp; /* Alea iacta est. -- J.C. */
}

/* 1 <= rne(x) <= max(u.ulevel/3,5) */
int
rne(x)
register int x;
{
    register int tmp, utmp;

    utmp = (u.ulevel < 15) ? 5 : u.ulevel / 3;
    tmp = 1;
    while (tmp < utmp && !rn2(x))
        tmp++;
    return tmp;

    /* was:
     *  tmp = 1;
     *  while (!rn2(x))
     *    tmp++;
     *  return min(tmp, (u.ulevel < 15) ? 5 : u.ulevel / 3);
     * which is clearer but less efficient and stands a vanishingly
     * small chance of overflowing tmp
     */
}

/* 1 <= rne(3) <= x */
int
rngh(x, limit)
register int x, limit;
{
    register int tmp, utmp;

    utmp = limit;
    tmp = 1;
    while (tmp < utmp && !rn2(x))
        tmp++;
    return tmp;

}

/* rnz: everyone's favorite! */
/* Note: expected value of rnz(i) is 2i -- JG */
int
rnz(i)
int i;
{
#ifdef LINT
    int x = i;
    int tmp = 1000;
#else
    register int64_t x = (int64_t) i;
    register int64_t tmp = 1000L;
#endif

    tmp += rn2(1000);
    tmp *= rne(4);
    if (rn2(2)) {
        x *= tmp;
        x /= 1000;
    } else {
        x *= 1000;
        x /= tmp;
    }
    return (int) x;
}


int
dd(dice, diesize, plus)
int dice, diesize, plus;
{
    return (dice > 0 && diesize > 0 ? d(dice, diesize) : 0) + plus;
}

/*rnd.c*/
