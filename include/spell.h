/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0    spell.h    $NHDT-Date: 1432512780 2015/05/25 00:13:00 $  $NHDT-Branch: master $:$NHDT-Revision: 1.9 $ */
/* Copyright 1986, M. Stephenson                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SPELL_H
#define SPELL_H

#include "general.h"
#include "onames.h"

#define NO_SPELL 0

/* spellbook re-use control; used when reading and when polymorphing */
#define MAX_SPELL_STUDY 3

struct spell {
    short sp_id;  /* spell id (== object.otyp) */
    schar sp_lev; /* power level */
    int sp_know;  /* knowledge of spell */
    int sp_amount;  /* number of times the spell can be cast */
    int sp_matcomp; /* matcompid, if any */
    int sp_cooldownlength; /* cooldown length */
    int sp_cooldownleft; /* cooldown turns left */
    int sp_skillchance; /* % chance to get skill points upon casting */
    int sp_hotkey; /* value from 1 to 10 */
};

/* levels of memory destruction with a scroll of amnesia */
#define ALL_MAP 0x1
#define ALL_SPELLS 0x2

#define decrnknow(spell) spl_book[spell].sp_know--
#define spellid(spell) spl_book[spell].sp_id
#define spellknow(spell) spl_book[spell].sp_know
#define spell_to_glyph(spell) (spellid(spell) - FIRST_SPELL + GLYPH_SPELL_TILE_OFF)

extern const char* spl_sortchoices[NUM_SPELL_SORTBY];
extern short spl_orderindx[MAXSPELL];

#endif /* SPELL_H */
