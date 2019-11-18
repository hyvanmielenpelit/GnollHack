/* GnollHack 4.0 encounter.c */
/* Copyright (c) Janne Gustafsson 2019.                           */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

#define NORANDOMIZEDITEM { 0, 0, 0, 0, 0, 0 }
#define NORANDOMIZEDALTERNATIVES { { NORANDOMIZEDITEM, NORANDOMIZEDITEM, NORANDOMIZEDITEM, NORANDOMIZEDITEM, NORANDOMIZEDITEM } }
#define NOMONSTERITEMS { NORANDOMIZEDALTERNATIVES, NORANDOMIZEDALTERNATIVES, NORANDOMIZEDALTERNATIVES, NORANDOMIZEDALTERNATIVES, NORANDOMIZEDALTERNATIVES }
#define NOMONSTERALTERNATIVE { 0, 0, 0, 0, 0, NOMONSTERITEMS }
#define NORANDOMIZEDMONSTER { { NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE }  }
#define NOMONSTERS { NORANDOMIZEDMONSTER, NORANDOMIZEDMONSTER, NORANDOMIZEDMONSTER, NORANDOMIZEDMONSTER, NORANDOMIZEDMONSTER }
#define NOENCOUNTER { NOMONSTERS, 0, 0 }

struct encounterdef encounter_definitions[] =
{
	NOENCOUNTER,
	{
		{
			/* Monster 1 */
			{ { { PM_GIANT_SPIDER, 1, 1, 0, 100, NOMONSTERITEMS },
			NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE }  },
			/* Monster 2 */
			{ { { PM_CAVE_SPIDER, 2, 6, 0, 100, NOMONSTERITEMS }, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE }  },
			NORANDOMIZEDMONSTER,
			NORANDOMIZEDMONSTER,
			NORANDOMIZEDMONSTER
		},
		10,
		0
	},
	{
		{
			/* Monster 1 */
			{ { { PM_OGRE_LORD, 1, 1, 0, 100, 
					{ { { { INFERNAL_JAGGED_TOOTHED_CLUB, 0, 1, 3, 0, 50 }, { BATTLE_AXE, ART_CLEAVER, 2, 2, 0, 50 }, NORANDOMIZEDITEM, NORANDOMIZEDITEM, NORANDOMIZEDITEM } },
					  NORANDOMIZEDALTERNATIVES, NORANDOMIZEDALTERNATIVES, NORANDOMIZEDALTERNATIVES, NORANDOMIZEDALTERNATIVES } },
			NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE }  },
			/* Monster 2 */
			{ { { PM_OGRE, 2, 6, 0, 100, NOMONSTERITEMS }, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE, NOMONSTERALTERNATIVE }  },
			NORANDOMIZEDMONSTER, 
			NORANDOMIZEDMONSTER, 
			NORANDOMIZEDMONSTER 
		},
		10,
		0
	},
	NOENCOUNTER,
};


static struct encounter zeroencounter = { 0 };
#define MAX_ENCOUNTERS 1000
static struct encounter encounter_list[MAX_ENCOUNTERS];

void encounter_init();

STATIC_DCL void FDECL(write_encounter_monsterdata, (int, int, int*, int*, double));
STATIC_DCL void FDECL(calculate_encounter_difficulty, (int));


void
encounter_init()
{
	for (int j = 0; j < MAX_ENCOUNTERS; j++)
	{
		encounter_list[j] = zeroencounter;
		for (int i = 0; i < MAX_ENCOUNTER_MONSTERS; i++)
		{
			encounter_list[j].encounter_monsters[i].permonstid = NON_PM;
		}
	}

	int encounternum = 0;
	int encountermonsternum = 0;

	/* Write 'spanned' encounter data */
	for (int i = 1; encounter_definitions[i].probability > 0; i++)
	{
		encountermonsternum = 0;
		write_encounter_monsterdata(i, 0, &encountermonsternum, &encounternum, encounter_definitions[i].probability / 100);
	}

	/* Calculate encounter difficulties */
	for (int i = 0; i < MAX_ENCOUNTERS; i++)
	{
		if (encounter_list[i].probability == 0)
			break;

		calculate_encounter_difficulty(i);
	}

    return;
}

STATIC_OVL
void 
write_encounter_monsterdata(encounter_definition_index, monster_type_index, active_encounter_monster_index_ptr, active_encounter_index_ptr, encprob)
int encounter_definition_index;
int monster_type_index;
int* active_encounter_monster_index_ptr;
int* active_encounter_index_ptr;
double encprob;
{
	if (monster_type_index >= MAX_ENCOUNTER_MONSTER_TYPES)
		return;

	if (!active_encounter_index_ptr || !active_encounter_monster_index_ptr)
		return;

	int orig_encounter_index = 0;
	int orig_monster_index = 0;
	struct encounter base_encounter = { 0 };

	orig_encounter_index = *active_encounter_index_ptr;
	base_encounter = encounter_list[*(active_encounter_index_ptr)];
	orig_monster_index = *active_encounter_monster_index_ptr;

	for(int rnd_monster_index = 0; rnd_monster_index < MAX_RANDOM_MONSTER_ALTERNATIVES; rnd_monster_index++)
	{
		int numrnd = encounter_definitions[encounter_definition_index].encounter_monster_types[monster_type_index].random_encounter_monsters[rnd_monster_index].generatednumber_random;
		int numconst = encounter_definitions[encounter_definition_index].encounter_monster_types[monster_type_index].random_encounter_monsters[rnd_monster_index].generatednumber_constant;
		int pmid = encounter_definitions[encounter_definition_index].encounter_monster_types[monster_type_index].random_encounter_monsters[rnd_monster_index].permonstid;

		double monster_type_probability = encounter_definitions[encounter_definition_index].encounter_monster_types[monster_type_index].random_encounter_monsters[rnd_monster_index].probability / 100;

		if (numrnd == 0 && numconst == 0)
		{
			encounter_list[*active_encounter_index_ptr].probability = encprob * 100;
			break;
		}

		for (int i = 0; i <= numrnd; i++)
		{
			double monster_number_probability = 1 / (1 + (double)numrnd);

			/* Make sure that first time everything is written in the same encounter */
			if(i > 0)
			{
				/* Move to a new encounter slot */
				(*active_encounter_index_ptr) = (*active_encounter_index_ptr) + 1;

				/* Check whether there is space to write new encounters, if not, exit */
				if (*active_encounter_index_ptr >= MAX_ENCOUNTERS)
					return;

				/* Copy the data from the original encounter as a basis for the new encounter */
				encounter_list[*active_encounter_index_ptr] = base_encounter;

				/* Set the encounter monster pointer to the original value */
				(*active_encounter_monster_index_ptr) = orig_monster_index;
			}
			/* Now, both randomized monster has been selected (rnd_monster_index)
			 * as well as the number of those monsters (i)
			 */
			int num = numconst + i;

			/* Write num monsters to the current encounter */
			for (int j = 1; j <= num; j++)
			{
				if (*active_encounter_monster_index_ptr >= MAX_ENCOUNTER_MONSTERS)
					break;

				encounter_list[*(active_encounter_index_ptr)].encounter_monsters[*active_encounter_monster_index_ptr].permonstid = pmid;
				encounter_list[*(active_encounter_index_ptr)].encounter_monsters[*active_encounter_monster_index_ptr].miflags = 
					encounter_definitions[encounter_definition_index].encounter_monster_types[monster_type_index].random_encounter_monsters[rnd_monster_index].mflags;
				for(int k = 0; k < MAX_MONSTER_ITEMS; k++)
				{
					encounter_list[*(active_encounter_index_ptr)].encounter_monsters[*active_encounter_monster_index_ptr].monster_items[k] = 
						encounter_definitions[encounter_definition_index].encounter_monster_types[monster_type_index].random_encounter_monsters[rnd_monster_index].monster_items[k];
				}

				/* Move to next free monster slot */
				(*active_encounter_monster_index_ptr) = (*active_encounter_monster_index_ptr) + 1;
			}

			/* Move to the next monster */
			if (monster_type_index < MAX_ENCOUNTER_MONSTER_TYPES - 1)
			{
				write_encounter_monsterdata(encounter_definition_index, monster_type_index + 1, active_encounter_monster_index_ptr, active_encounter_index_ptr,
					encprob * monster_type_probability * monster_number_probability);
			}
			else
			{
				encounter_list[*active_encounter_index_ptr].probability = encprob * monster_type_probability * monster_number_probability * 100;
			}
		}
	}
}



STATIC_OVL
void
calculate_encounter_difficulty(encounter_index)
int encounter_index;
{
	long maxdifficulty = 0;
	long mindifficulty = 0;
	long totaldifficulty = 0;
	long totalsquareddifficulty = 0;

	for (int i = 0; i < MAX_ENCOUNTER_MONSTERS; i++)
	{
		int pmid = encounter_list[encounter_index].encounter_monsters[i].permonstid;
		if (pmid == NON_PM)
			break;

		totaldifficulty += (long)mons[pmid].difficulty;
		totalsquareddifficulty += (long)mons[pmid].difficulty * (long)mons[pmid].difficulty;

	}

	maxdifficulty = totaldifficulty;
	mindifficulty = (long)sqrt((double)totalsquareddifficulty);
	encounter_list[encounter_index].difficulty_max = (int)maxdifficulty;
	encounter_list[encounter_index].difficulty_min = (int)mindifficulty;

	/* Calculate Tommi's point estimate */
	/* This currently assumes that encounter definitions list monsters in descending order of difficulty; otherwise, they need to be sorted into descending order */
	double sum = 0;
	double combatvalue = 0;
	double difficulty_point_estimate = 0;
	long totaldifficulty_of_removed_monsters = 0;

	for (int i = 0; i < MAX_ENCOUNTER_MONSTERS; i++)
	{
		int pmid = encounter_list[encounter_index].encounter_monsters[i].permonstid;
		if (pmid == NON_PM)
			break;

		int current_monster_difficulty = mons[pmid].difficulty;

		sum += ((double)totaldifficulty - (double)totaldifficulty_of_removed_monsters) * ((double)current_monster_difficulty / (double)totaldifficulty);

		/* Move to next and mark this as 'removed' */
		totaldifficulty_of_removed_monsters += (long)current_monster_difficulty;
	}

	combatvalue = sum * (double)totaldifficulty;
	difficulty_point_estimate = sqrt(combatvalue);

	encounter_list[encounter_index].difficulty_point_estimate = (int)difficulty_point_estimate;
}


void
randomize_encounter(x, y)
int x, y;
{
	double totalrollprob = 0;
	int selected_encounter = 0;
	double totalselectedprob = 0;

	/* Select encounters of an appropriate level */
	int zlevel = level_difficulty();

	for (int i = 1; i <= 3; i++)
	{
		int midmlev = 0, maxmlev = 0, minmlev = 0;
		totalselectedprob = 0;
		if (i == 1)
		{
			/* Try first with a tighter range */
			midmlev = (zlevel * 2 + u.ulevel) / (3 * (In_endgame(&u.uz) ? 3 : u.uhave.amulet ? 2 : 1));
			maxmlev = (zlevel * 2 + u.ulevel) / 2;
			minmlev = max(0, midmlev - (maxmlev - midmlev)); //equates to midmlev/2 = (2z+c)/6
		}
		else if (i == 2)
		{
			minmlev = (zlevel * 2 + u.ulevel) / (12 * (In_endgame(&u.uz) ? 3 : u.uhave.amulet ? 2 : 1));
			maxmlev = (zlevel * 2 + u.ulevel) / 2;
		}
		else
		{
			minmlev = 0;
			maxmlev = (zlevel * 2 + u.ulevel);
		}
		for (int j = 1; j < MAX_ENCOUNTERS; j++)
		{
			if (
				(encounter_list[j].difficulty_min >= minmlev && encounter_list[j].difficulty_min <= maxmlev)
				|| (encounter_list[j].difficulty_min >= minmlev && (encounter_list[j].difficulty_min <= maxmlev && !rn2(4)))
			   )
				encounter_list[j].insearch = TRUE, totalselectedprob += encounter_list[j].probability;
			else
				encounter_list[j].insearch = FALSE;
		}
		if (totalselectedprob > 0)
			break;
	}

	int roll_int = rn2(32000); /* totalprob should be 1000, but just in case it is accidently less or more */
	double roll = (double)roll_int / ((double)32000) * totalselectedprob;

	if (totalselectedprob > 0)
	{
		for (int i = 1; i < MAX_ENCOUNTERS; i++)
		{
			if(encounter_list[i].insearch)
				totalrollprob += encounter_list[i].probability;

			if (totalrollprob >= roll)
			{
				selected_encounter = i;
				break;
			}
		}
	}

	if (selected_encounter > 0)
	{
		create_encounter(selected_encounter, x, y);
	}
	else
	{
		makemon((struct permonst*)0, 0, 0, 0);
	}

	return;
}


void
create_encounter(selected_encounter, x, y)
int selected_encounter, x, y;
{
	/* Calculate experience first */
	long encounter_experience = 1 + encounter_list[selected_encounter].difficulty_point_estimate * encounter_list[selected_encounter].difficulty_point_estimate;
	long total_monster_experience = 0;
	//long total_monster_difficulty = 0;

	for (int i = 0; i < MAX_ENCOUNTER_MONSTERS; i++)
	{
		int pmid = encounter_list[selected_encounter].encounter_monsters[i].permonstid;
		if (pmid == NON_PM)
			break;

		total_monster_experience += 1 + mons[pmid].difficulty * mons[pmid].difficulty;
		//total_monster_difficulty += mons[pmid].difficulty;
	}

	long xpdiff = encounter_experience - total_monster_experience;
	if (xpdiff < 0)
		xpdiff = 0;

	int nx = x, ny = y;
	for (int i = 0; i < MAX_ENCOUNTER_MONSTERS; i++)
	{
		int pmid = encounter_list[selected_encounter].encounter_monsters[i].permonstid;
		if (pmid == NON_PM)
			break;

		/*
		int nx, ny, tryct = 0;
		boolean good;
		do {
			nx = x + -1 - tryct/10 + rn2(3 + tryct / 5);
			ny = y + -1 - tryct / 10 + rn2(3 + tryct / 5);
			
			good = isok(nx, ny) && !occupied(nx, ny) &&
				((!in_mklev && cansee(nx, ny)) ? FALSE
				: goodpos(nx, ny, (struct monst*)0, 0));
		} while ((++tryct < 50) && !good);

		*/

		struct monst* mon = (struct monst*)0;
		mon = makemon(&mons[pmid], nx, ny, MM_NOGRP | MM_ADJACENTOK);

		if(mon)
		{
			if (nx == 0 && ny == 0)
			{
				nx = mon->mx;
				ny = mon->my;
			}


			long monster_experience = 1 + mons[pmid].difficulty * mons[pmid].difficulty;

			if(xpdiff > 0 && total_monster_experience > 0)
				mon->extra_encounter_xp = (xpdiff * monster_experience) / total_monster_experience;

			for (int j = 0; j < MAX_MONSTER_ITEMS; j++)
			{
				int roll = rn2(100);
				int selected_item = -1;
				int totalprob = 0;
				for (int k = 0; k < MAX_MONSTER_RANDOM_ITEM_ALTERNATIVES; k++)
				{
					totalprob += encounter_list[selected_encounter].encounter_monsters[i].monster_items[j].random_monster_items[k].probability;
					if (totalprob >= roll)
					{
						selected_item = k;
						break;
					}
				}

				if (totalprob == 0)
					break;

				if (selected_item > -1)
				{
					int otyp = encounter_list[selected_encounter].encounter_monsters[i].monster_items[j].random_monster_items[selected_item].otyp;
					int oartifact = encounter_list[selected_encounter].encounter_monsters[i].monster_items[j].random_monster_items[selected_item].oartifact;
					long flags = encounter_list[selected_encounter].encounter_monsters[i].monster_items[j].random_monster_items[selected_item].iflags;
					if (otyp > STRANGE_OBJECT)
					{
						struct obj* otmp = mksobj(otyp, !!(flags & MI_INITIALIZE), !!(flags & MI_ALLOW_ARTIFACTS), FALSE);

						if (otmp)
						{
							/* Make it into the artifact */
							if (oartifact > 0)
								otmp = oname(otmp, artiname(oartifact));

							if (flags & MI_BLESSED)
								bless(otmp);
							else if (flags & MI_CURSED)
								curse(otmp);
							else if (flags & MI_UNCURSED)
								otmp->blessed = otmp->cursed = 0;

							if (flags & MI_ERODEPROOF)
								otmp->oerodeproof = TRUE;

							if (!(flags & MI_IGNORE_SPE))
							{
								int spe = encounter_list[selected_encounter].encounter_monsters[i].monster_items[j].random_monster_items[selected_item].spe_constant;

								if (encounter_list[selected_encounter].encounter_monsters[i].monster_items[j].random_monster_items[selected_item].spe_random > 0)
									spe += rn2(1 + encounter_list[selected_encounter].encounter_monsters[i].monster_items[j].random_monster_items[selected_item].spe_random);

								otmp->spe = spe;
							}

							(void)mpickobj(mon, otmp);
						}
					}
				}
			}
		}
	}
}


/*encounter.c*/
