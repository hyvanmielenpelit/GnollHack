/* GnollHack 4.0	do.c	$NHDT-Date: 1548978604 2019/01/31 23:50:04 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.189 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Derek S. Ray, 2015. */
/* GnollHack may be freely redistributed.  See license for details. */

/* Contains code for 'd', 'D' (drop), '>', '<' (up, down) */

#include "hack.h"
#include "artifact.h"
#include "artilist.h"
#include "lev.h"

STATIC_DCL void FDECL(trycall, (struct obj *));
STATIC_DCL void NDECL(polymorph_sink);
STATIC_DCL boolean NDECL(teleport_sink);
STATIC_DCL void FDECL(dosinkring, (struct obj *));
STATIC_PTR int FDECL(drop, (struct obj *));
STATIC_PTR int FDECL(itemdescription, (struct obj*));
STATIC_PTR int NDECL(wipeoff);
STATIC_DCL int FDECL(menu_drop, (int));
STATIC_DCL int NDECL(currentlevel_rewrite);
STATIC_DCL void NDECL(final_level);
/* static boolean FDECL(badspot, (XCHAR_P,XCHAR_P)); */

extern int n_dgns; /* number of dungeons, from dungeon.c */

static NEARDATA const char drop_types[] = { ALLOW_COUNT, COIN_CLASS,
                                            ALL_CLASSES, 0 };

/* 'd' command: drop one inventory item */
int
dodrop()
{
    int result, i = (invent) ? 0 : (SIZE(drop_types) - 1);

    if (*u.ushops)
        sellobj_state(SELL_DELIBERATE);
    result = drop(getobj(&drop_types[i], "drop", 3, ""));
    if (*u.ushops)
        sellobj_state(SELL_NORMAL);
    if (result)
        reset_occupations();

    return result;
}

/* the M('y') command - Character statistics */
int
docharacterstatistics()
{
	winid datawin = WIN_ERR;
	datawin = create_nhwindow(NHW_MENU);

	char buf[BUFSZ];
	char buf2[BUFSZ];
	const char* txt;

	/* Name and Rank*/
	strcpy(buf, plname);
	*buf = highc(*buf);

	strcpy(buf2, rank());
	*buf2 = highc(*buf2);
	Sprintf(eos(buf), " the %s", buf2);

	txt = buf;
	putstr(datawin, 0, txt);

	/* Level and Role */
	strcpy(buf2, (flags.female && urole.name.f) ? urole.name.f : urole.name.m);
	*buf2 = highc(*buf2);
	Sprintf(buf, "Level %d %s", u.ulevel, buf2);
	txt = buf;
	putstr(datawin, 0, txt);

	/* Empty line */
	Sprintf(buf, "");
	txt = buf;
	putstr(datawin, 0, txt);

	/* Race */
	strcpy(buf2, urace.noun);
	*buf2 = highc(*buf2);
	if (Upolyd)
		Sprintf(buf, "Original race:     %s", buf2);
	else
		Sprintf(buf, "Race:              %s", buf2);

	/* Gender */
	if (Upolyd && u.mfemale != flags.female)
	{
		Sprintf(buf, "Original gender:   %s", u.mfemale ? "Female" : "Male");
		txt = buf;
		putstr(datawin, 0, txt);
	}

	txt = buf;
	putstr(datawin, 0, txt);

	if (Upolyd)
	{
		strcpy(buf2, mons[u.umonnum].mname);
		*buf2 = highc(*buf2);
		Sprintf(buf, "Polymorphed into:  %s", buf2);
		txt = buf;
		putstr(datawin, 0, txt);
	}


	strcpy(buf2, flags.female ? "Female" : "Male");
	if (Upolyd && u.mfemale != flags.female)
		Sprintf(buf, "Polymorph gender:  %s", buf2);
	else
		Sprintf(buf, "Gender:            %s", buf2);
	txt = buf;
	putstr(datawin, 0, txt);

	/* Alignment */
	Strcpy(buf2, (u.ualign.type == A_CHAOTIC)
		? "Chaotic"
		: (u.ualign.type == A_NEUTRAL)
		? "Neutral"
		: "Lawful");
	*buf2 = highc(*buf2);
	Sprintf(buf, "Alignment:         %s", buf2);
	txt = buf;
	putstr(datawin, 0, txt);

	/* God */
	Sprintf(buf, "God:               %s", u_gname());
	txt = buf;
	putstr(datawin, 0, txt);

	/* Experience */
	Sprintf(buf, "Experience:        %d", u.uexp);

	if (u.ulevel < MAXULEV)
	{
		int ulvl = (int)u.ulevel;
		long exp_for_nxt_lvl = newuexp(ulvl);

		Sprintf(eos(buf), " / %d", exp_for_nxt_lvl);
	}
	txt = buf;
	putstr(datawin, 0, txt);

	/* Max level */
	if (u.ulevel != u.ulevelmax)
	{
		Sprintf(buf, "Maximum level:     %d", u.ulevelmax);
		txt = buf;
		putstr(datawin, 0, txt);
	}

	/* Empty line */
	Sprintf(buf, "");
	txt = buf;
	putstr(datawin, 0, txt);


	/* Max attributes */
	Sprintf(buf, "Maximum attribute scores:");
	txt = buf;
	putstr(datawin, 0, txt);

	Sprintf(buf, "  St:%s Dx:%d Co:%d In:%d Wi:%d Ch:%d", get_strength_string(urace.attrmax[A_STR]),
		urace.attrmax[A_DEX], urace.attrmax[A_CON], urace.attrmax[A_INT], urace.attrmax[A_WIS], urace.attrmax[A_CHA]);
	txt = buf;
	putstr(datawin, 0, txt);

	/* Race, role and alignment traits */
	Sprintf(buf, "Character traits:");
	txt = buf;
	putstr(datawin, 0, txt);

	int trait_count = 0;

	for (int i = 0; i < MAX_TRAIT_DESCRIPTIONS; i++)
	{
		if (urace.trait_descriptions[i] && strcmp(urace.trait_descriptions[i], ""))
		{
			trait_count++;

			char buf2[BUFSIZ] = "";

			strcpy(buf2, urace.trait_descriptions[i]);
			*buf2 = highc(*buf2);

			Sprintf(buf, " %2d - %s (Race)", trait_count, buf2);
			txt = buf;
			putstr(datawin, 0, txt);

		}
		else
			break;
	}

	for (int i = 0; i < MAX_TRAIT_DESCRIPTIONS; i++)
	{
		if (urole.trait_descriptions[i] && strcmp(urole.trait_descriptions[i], ""))
		{
			trait_count++;

			char buf2[BUFSIZ] = "";

			strcpy(buf2, urole.trait_descriptions[i]);
			*buf2 = highc(*buf2);

			Sprintf(buf, " %2d - %s (Role)", trait_count, buf2);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		else
			break;
	}

	if (u.ualign.type == A_LAWFUL || u.ualign.type == A_NEUTRAL)
	{
		trait_count++;
		Sprintf(buf, " %2d - Loses telepathy and luck if commits murder (Alignment)", trait_count);
		txt = buf;
		putstr(datawin, 0, txt);
	}

	if (u.ualign.type == A_CHAOTIC)
	{
		trait_count++;
		Sprintf(buf, " %2d - Can sacrifice own race (Alignment)", trait_count);
		txt = buf;
		putstr(datawin, 0, txt);
	}

	if (trait_count == 0)
	{
		Sprintf(buf, " (None)");
		txt = buf;
		putstr(datawin, 0, txt);
	}



	/* Current intrinsics */
	Sprintf(buf, "Intrinsic abilities:");
	txt = buf;
	putstr(datawin, 0, txt);

	int intrinsic_count = 0;
	for (int i = 1; i <= LAST_PROP; i++)
	{
		long innate_intrinsic = u.uprops[i].intrinsic & (INTRINSIC | FROM_FORM);
		if (innate_intrinsic)
		{
			intrinsic_count++;

			char buf2[BUFSIZ] = "";
			char buf3[BUFSIZ] = "";

			strcpy(buf2, get_property_name(i));
			*buf2 = highc(*buf2);

			if (innate_intrinsic & FROM_RACE)
			{
				Sprintf(buf3, "race");
			}
			else if (innate_intrinsic & FROM_ROLE)
			{
				if(strcmp(buf3, ""))
					Sprintf(eos(buf3), ", ");

				Sprintf(eos(buf3), "role");
			}
			else if (innate_intrinsic & FROM_ACQUIRED)
			{
				if (strcmp(buf3, ""))
					Sprintf(eos(buf3), ", ");

				Sprintf(eos(buf3), "acquired");
			}
			else if (innate_intrinsic & FROM_FORM)
			{
				if (strcmp(buf3, ""))
					Sprintf(eos(buf3), ", ");

				Sprintf(eos(buf3), "polymorphed form");
			}

			if (strcmp(buf3, ""))
				*buf3 = highc(*buf3);

			Sprintf(buf, " %2d - %s (%s)", intrinsic_count, buf2, buf3);
			txt = buf;
			putstr(datawin, 0, txt);


		}
	}
	if (intrinsic_count == 0)
	{
		Sprintf(buf, " (None)");
		txt = buf;
		putstr(datawin, 0, txt);
	}



	/* Level-up intrinsics */
	for(int i = 1; i <= 2; i++)
	{
		Sprintf(buf, "Abilities to be acquired from %s:", i == 1 ? "race" : "role");
		txt = buf;
		putstr(datawin, 0, txt);

		const struct innate* intrinsic_ability = (i == 1 ? race_abil(urace.malenum) : role_abil(urole.malenum));
		int abil_count = 0;
		int table_index = 0;
		if (intrinsic_ability)
		{
			while (intrinsic_ability[table_index].ulevel > 0)
			{
				if (intrinsic_ability[table_index].ulevel > u.ulevel&& intrinsic_ability[table_index].propid > 0)
				{
					abil_count++;

					char buf2[BUFSIZ] = "";
					strcpy(buf2, get_property_name(intrinsic_ability[table_index].propid));
					*buf2 = highc(*buf2);

					Sprintf(buf, " Level %2d - %s", intrinsic_ability[table_index].ulevel, buf2);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				table_index++;
			}
		}
		if (abil_count == 0)
		{
			Sprintf(buf, " (None)");
			txt = buf;
			putstr(datawin, 0, txt);
		}
	}



	display_nhwindow(datawin, FALSE);
	destroy_nhwindow(datawin), datawin = WIN_ERR;

	return 0;
}



static NEARDATA const char item_description_objects[] = { ALL_CLASSES, ALLOW_NONE, 0 };

/* the M('x') command - Item descriptions*/
int
doitemdescriptions()
{
	boolean proceedtoinventory = floorexamine();
	if (!proceedtoinventory)
		return 0;
		
	int i = (invent) ? 0 : (SIZE(item_description_objects) - 1);

	return itemdescription(getobj(&item_description_objects[i], "examine", 1, ""));

}

/* Returns TRUE if we proceed to inventory.
 * Object may be either on floor or in inventory.
 */
boolean
floorexamine()
{
	register struct obj* otmp;
	char qbuf[QBUFSZ];
	char c;
	boolean res = TRUE;
	int cnt = 0;

	for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere)
		cnt++;

	if (!cnt)
		return TRUE;

	if (cnt > 1)
	{
		Sprintf(qbuf, "There are several objects here. Do you want to examine them?");
		if ((c = yn_function(qbuf, ynqchars, 'n')) != 'y')
			return  TRUE;
	}

	for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere) 
	{
		char qsfx[QBUFSZ];
		boolean one = (otmp->quan == 1L);

		/* "There is <an object> here; <verb> it?" or
			"There are <N objects> here; <verb> one?" */
		Sprintf(qbuf, "There %s ", otense(otmp, "are"));
		Sprintf(qsfx, " here; %s %s?", "examine", one ? "it" : "them");
		(void)safe_qbuf(qbuf, qbuf, qsfx, otmp, doname, ansimpleoname,
			one ? something : (const char*) "things");
		if ((c = yn_function(qbuf, ynqchars, 'n')) == 'y')
		{
			(void)itemdescription(otmp);
			res = FALSE;
		}
		else if (c == 'q')
			return res;
	}

	return res;
}


STATIC_PTR int
itemdescription(obj)
register struct obj* obj;
{
	if (!obj)
		return 0;

	winid datawin = WIN_ERR;

	datawin = create_nhwindow(NHW_MENU);

	int otyp = obj->otyp;
	if (obj->oartifact && artilist[obj->oartifact].maskotyp != STRANGE_OBJECT)
	{
		if (!obj->known) //!objects[obj->otyp].oc_name_known)
			otyp = artilist[obj->oartifact].maskotyp;
	}
	
	boolean stats_known = object_stats_known(obj);
	boolean uses_spell_flags = object_uses_spellbook_wand_flags_and_properties(obj);

	char buf[BUFSZ];
	char buf2[BUFSZ];
	char buf3[BUFSZ];
	const char* txt;

	/* Name */
	strcpy(buf, cxname(obj));
	*buf = highc(*buf);
	txt = buf;
	putstr(datawin, 0, txt);

	/* Type */
	strcpy(buf3, def_oc_syms[obj->oclass].name);
	*buf3 = highc(*buf3);
	//Sprintf(buf, "Class:                  %s", buf2);
	Sprintf(buf, "%s", makesingular(buf3));
	//txt = buf;
	//putstr(datawin, 0, txt);
	boolean hidemainclass = FALSE;

	strcpy(buf2, ""); /* Prefix */
	strcpy(buf3, ""); /* Postfix preceded by : */
	if (objects[otyp].oc_class == WEAPON_CLASS)
	{
		if (is_ammo(obj))
		{
			strcpy(buf2, "Ammunition");
			hidemainclass = TRUE;
		}
		else if (is_launcher(obj))
		{
			strcpy(buf2, "Ranged");
		}
		else if(objects[otyp].oc_flags & O1_THROWN_WEAPON_ONLY)
		{
			strcpy(buf2, "Thrown");
		}
		else if (objects[otyp].oc_flags & O1_MELEE_AND_THROWN_WEAPON)
		{
			strcpy(buf2, "Melee and Thrown");
		}
		else
		{
			strcpy(buf2, "Melee");
		}
		strcpy(buf3, weapon_type_names[objects[otyp].oc_subtyp]);
		*buf3 = highc(*buf3);
	}
	else if (objects[otyp].oc_class == ARMOR_CLASS)
	{
		strcpy(buf3, armor_class_simple_name(obj));
		*buf3 = highc(*buf3);

		if (is_weapon(obj))
		{
			strcat(buf3, ", Melee weapon");
		}
	}
	else if (objects[otyp].oc_class == MISCELLANEOUS_CLASS && objects[otyp].oc_subtyp > MISC_MULTIPLE_PERMITTED)
	{
		strcpy(buf3, misc_type_names[objects[otyp].oc_subtyp]);
		*buf3 = highc(*buf3);
	}
	else if (objects[otyp].oc_class == FOOD_CLASS && objects[otyp].oc_subtyp > FOODTYPE_GENERAL)
	{
		strcpy(buf3, food_type_names[objects[otyp].oc_subtyp]);
		*buf3 = highc(*buf3);
	}
	else if (objects[otyp].oc_class == TOOL_CLASS)
	{
		if (is_weptool(obj))
		{
			strcpy(buf2, "Weapon-like");
		}
		if (is_wand_like_tool(obj))
		{
			strcpy(buf2, "Wand-like");
		}
		if (is_candle(obj))
		{
			strcpy(buf3, "Candle");
		}
		if (Is_container(obj))
		{
			strcpy(buf2, "Container");
			hidemainclass = TRUE;
		}
	}
	else if (objects[otyp].oc_class == AMULET_CLASS || objects[otyp].oc_class == RING_CLASS)
	{
		if (stats_known)
		{
			if (objects[otyp].oc_magic)
				strcpy(buf2, "Magical");
			else
				strcpy(buf2, "Non-magical");
		}
	}
	else if (objects[otyp].oc_class == GEM_CLASS)
	{
		/*
		if (is_graystone(obj))
		{
			strcpy(buf3, "Gray stone");
		}
		if (is_rock(obj))
		{
			strcpy(buf3, "Rock");
		}
		if (is_ore(obj))
		{
			strcpy(buf3, "Ore");
		}
		*/
	}

	if (strcmp(buf2, "") != 0)
	{
		char buf4[BUFSIZ] = "";
		strcpy(buf4, buf);
		//Sprintf(buf, "Category:               %s", buf2);
		if (hidemainclass)
			strcpy(buf, buf2);
		else
			Sprintf(buf, "%s %s", buf2, buf4);
	}
	if (strcmp(buf3, "") != 0)
	{
		Sprintf(eos(buf), ": %s", buf3);
	}
	if (obj->nknown && obj->known && obj->oartifact)
	{
		Sprintf(eos(buf), " - Artifact");
	}
	txt = buf;
	putstr(datawin, 0, txt);

	Sprintf(buf, "");
	txt = buf;
	putstr(datawin, 0, txt);

	/* Weight */
	double objweight = ((double)obj->owt) / 16;

	/* Show loadstone incorrectly if not known and not carried */
	if(otyp == LOADSTONE && !carried(obj) && !objects[otyp].oc_name_known)
		objweight = ((double)objects[LUCKSTONE].oc_weight) / 16;

	if (objweight >= 1000)
		Sprintf(buf2, "%.0f cwt", objweight / 100);
	else if (objweight >= 10)
		Sprintf(buf2, "%.0f lbs", objweight);
	else
		Sprintf(buf2, "%.1f %s", objweight, objweight == 1 ? "lb " : "lbs");

	Sprintf(buf, "Weight:                 %s", buf2);
	txt = buf;
	putstr(datawin, 0, txt);

	if(stats_known)
	{
		/* Gold value */
		if(obj->oartifact)
			Sprintf(buf2, "%d gold", artilist[obj->oartifact].cost);
		else
			Sprintf(buf2, "%d gold", objects[otyp].oc_cost);

		Sprintf(buf, "Value:                  %s", buf2);
		txt = buf;
		putstr(datawin, 0, txt);

		/* Nutritinal value */
		if (is_edible(obj))
		{
			Sprintf(buf2, "%d rounds", obj->oeaten ? obj->oeaten : obj_nutrition(obj));

			Sprintf(buf, "Nutritional value:      %s", buf2);
			txt = buf;
			putstr(datawin, 0, txt);
		}
	}

	/* Material */
	strcpy(buf2, materialnm[objects[otyp].oc_material]);
	*buf2 = highc(*buf2);
	Sprintf(buf, "Material:               %s", buf2);
	txt = buf;
	putstr(datawin, 0, txt);


	if (!uses_spell_flags && (is_weapon(obj) || ((is_gloves(obj) || is_boots(obj)) && stats_known) || objects[obj->otyp].oc_class == GEM_CLASS))
	{
		char plusbuf[BUFSZ];
		boolean maindiceprinted = FALSE;

		if (!is_ammo(obj) && !is_gloves(obj) && !is_boots(obj))
		{
			/* Single or two-handed */
			if (bimanual(obj))
				strcpy(buf2, "Two-handed");
			else
				strcpy(buf2, "Single-handed");

			Sprintf(buf, "Hands to use:           %s", buf2);
			txt = buf;
			putstr(datawin, 0, txt);
		}

		/* Skill */
		if(objects[obj->otyp].oc_skill != P_NONE)
		{
			strcpy(buf2, weapon_descr(obj));
			*buf2 = highc(*buf2);
			Sprintf(buf, "Skill:                  %s", buf2);
			txt = buf;
			putstr(datawin, 0, txt);
		}


		int baserange = 0;

		/* Ammunition range */
		if (objects[obj->otyp].oc_multishot_style > 0) {

			Sprintf(buf, "%s  %s", 
				is_launcher(obj) ? "Shots per round:      " : 
				nonmelee_throwing_weapon(obj) ? "Throws per round:     " : "Attacks per round:    ", 
				multishot_style_names[objects[obj->otyp].oc_multishot_style]);
			/*
			if((objects[obj->otyp].oc_flags3 & O3_MULTISHOT_REQUIRES_SKILL_MASK) == O3_MULTISHOT_REQUIRES_EXPERT_SKILL)
				Sprintf(eos(buf), " (requires expert skill)");
			else if ((objects[obj->otyp].oc_flags3 & O3_MULTISHOT_REQUIRES_SKILL_MASK) == O3_MULTISHOT_REQUIRES_SKILLED_SKILL)
				Sprintf(eos(buf), " (requires skilled skill)");
			else if ((objects[obj->otyp].oc_flags3 & O3_MULTISHOT_REQUIRES_SKILL_MASK) == O3_MULTISHOT_REQUIRES_BASIC_SKILL)
				Sprintf(eos(buf), " (requires basic skill)");
			*/

			txt = buf;
			putstr(datawin, 0, txt);
		}

		/* Ammunition range */
		if (is_launcher(obj)) {

			baserange = weapon_range((struct obj*)0, obj);

			Sprintf(buf, "Ammunition range:       %d'", max(1, baserange) * 5);
			txt = buf;
			putstr(datawin, 0, txt);
		}



		/* Range and throw distance */
		if (!is_gloves(obj) && !is_boots(obj))
		{
			int range = 0;
			boolean thrown = TRUE;

			/* Throw distance and fire distance for ammo */
			if (is_ammo(obj) && uwep && ammo_and_launcher(obj, uwep))
			{
				thrown = FALSE;
				range = weapon_range(obj, uwep);
			}
			else
				range = weapon_range(obj, (struct obj*)0);

			Sprintf(buf2, "%d", max(1, range) * 5);
			if(thrown)
				Sprintf(buf, "Throw distance:         %s'", buf2);
			else
				Sprintf(buf, "Range when fired:       %s'", buf2);
			txt = buf;
			putstr(datawin, 0, txt);

		}

		boolean printmaindmgtype = FALSE;

		/* Damage - Small */
		if((objects[otyp].oc_wsdice > 0 && objects[otyp].oc_wsdam > 0) || objects[otyp].oc_wsdmgplus != 0)
		{
			printmaindmgtype = TRUE;
			Sprintf(buf, "Base damage - Small:    ");

			if (objects[otyp].oc_wsdice > 0 && objects[otyp].oc_wsdam > 0)
			{
				maindiceprinted = TRUE;
				Sprintf(plusbuf, "%dd%d", objects[otyp].oc_wsdice, objects[otyp].oc_wsdam);
				Strcat(buf, plusbuf);
			}

			if (objects[otyp].oc_wsdmgplus != 0)
			{
				if (maindiceprinted && objects[otyp].oc_wsdmgplus > 0)
				{
					Sprintf(plusbuf, "+");
					Strcat(buf, plusbuf);
				}
				Sprintf(plusbuf, "%d", objects[otyp].oc_wsdmgplus);
				Strcat(buf, plusbuf);
			}
			if (stats_known && (objects[otyp].oc_aflags & A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS))
			{
				/* Damage - Doubled */
				Sprintf(eos(buf), " (x2)");
			}
			if (objects[otyp].oc_aflags & A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA)
			{
				if (objects[otyp].oc_aflags & A1_WOUNDING)
				{
					Sprintf(plusbuf, " (permanent)");
					Strcat(buf, plusbuf);
				}

				if (objects[otyp].oc_aflags & A1_LIFE_LEECH)
				{
					Sprintf(plusbuf, " (confers HP)");
					Strcat(buf, plusbuf);
				}
			}

			txt = buf;
			putstr(datawin, 0, txt);

		}


		/* Damage - Large */
		if((objects[otyp].oc_wldice > 0 && objects[otyp].oc_wldam > 0) || objects[otyp].oc_wldmgplus != 0)
		{
			printmaindmgtype = TRUE;
			maindiceprinted = FALSE;
			Sprintf(buf, "Base damage - Large:    ");

			if (objects[otyp].oc_wldice > 0 && objects[otyp].oc_wldam > 0)
			{
				maindiceprinted = TRUE;
				Sprintf(plusbuf, "%dd%d", objects[otyp].oc_wldice, objects[otyp].oc_wldam);
				Strcat(buf, plusbuf);
			}

			if (objects[otyp].oc_wldmgplus != 0)
			{
				if (maindiceprinted && objects[otyp].oc_wldmgplus > 0)
				{
					Sprintf(plusbuf, "+");
					Strcat(buf, plusbuf);
				}
				Sprintf(plusbuf, "%d", objects[otyp].oc_wldmgplus);
				Strcat(buf, plusbuf);
			}
			if (stats_known && (objects[otyp].oc_aflags & A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS))
			{
				/* Damage - Doubled */
				Sprintf(eos(buf), " (x2)");
			}

			if(objects[otyp].oc_aflags & A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA)
			{
				if (objects[otyp].oc_aflags & A1_WOUNDING)
				{
					Sprintf(plusbuf, " (permanent)");
					Strcat(buf, plusbuf);
				}

				if (objects[otyp].oc_aflags & A1_LIFE_LEECH)
				{
					Sprintf(plusbuf, " (confers HP)");
					Strcat(buf, plusbuf);
				}
			}

			txt = buf;
			putstr(datawin, 0, txt);
		}

		/* Damage type - Main */
		if (printmaindmgtype && objects[otyp].oc_damagetype != AD_PHYS)
		{
			char dmgttext[BUFSZ] = "";
			strcpy(dmgttext, get_damage_type_text(objects[otyp].oc_damagetype));
			*dmgttext = highc(*dmgttext);
			if (strcmp(dmgttext, "") != 0)
			{
				Sprintf(buf, "Damage type:            %s", dmgttext);
				txt = buf;
				putstr(datawin, 0, txt);
			}
		}



		if (stats_known
			&& ((objects[otyp].oc_wedice > 0 && objects[otyp].oc_wedam > 0) || objects[otyp].oc_wedmgplus != 0))
		{

			/* Damage - Extra */
			maindiceprinted = FALSE;
			Sprintf(buf, "Extra damage:           ");

			if (objects[otyp].oc_wedice > 0 && objects[otyp].oc_wedam > 0)
			{
				maindiceprinted = TRUE;
				Sprintf(plusbuf, "%dd%d", objects[otyp].oc_wedice, objects[otyp].oc_wedam);
				Strcat(buf, plusbuf);
			}

			if (objects[otyp].oc_wedmgplus != 0)
			{
				if (maindiceprinted && objects[otyp].oc_wedmgplus > 0)
				{
					Sprintf(plusbuf, "+");
					Strcat(buf, plusbuf);
				}
				Sprintf(plusbuf, "%d", objects[otyp].oc_wedmgplus);
				Strcat(buf, plusbuf);
			}

			if (objects[otyp].oc_aflags & A1_WOUNDING)
			{
				Sprintf(plusbuf, " (permanent)");
				Strcat(buf, plusbuf);
			}

			if (objects[otyp].oc_aflags & A1_LIFE_LEECH)
			{
				Sprintf(plusbuf, " (confers HP)");
				Strcat(buf, plusbuf);
			}

			txt = buf;
			putstr(datawin, 0, txt);

			/* Damage type - Extra */
			if (objects[otyp].oc_extra_damagetype != AD_PHYS)
			{
				char dmgttext[BUFSZ] = "";
				strcpy(dmgttext, get_damage_type_text(objects[otyp].oc_extra_damagetype));
				*dmgttext = highc(*dmgttext);
				if (strcmp(dmgttext, "") != 0)
				{
					Sprintf(buf, "Extra damage type:      %s", dmgttext);
					txt = buf;
					putstr(datawin, 0, txt);
				}
			}
		}


		/* Damage - Silver*/
		if (objects[otyp].oc_material == MAT_SILVER)
		{
			Sprintf(buf, "Silver bonus damage:    ");
			maindiceprinted = TRUE;
			Sprintf(plusbuf, "%dd%d", 1, 20);
			Strcat(buf, plusbuf);
			txt = buf;
			putstr(datawin, 0, txt);
		}

		/* Fixed damage bonus */
		if (is_launcher(obj) && (objects[otyp].oc_flags3 & O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH)) 
		{

			Sprintf(buf, "Fixed damage bonus:     %s%d (instead of strength)", 
				objects[otyp].oc_fixed_damage_bonus >= 0 ? "+" : "", objects[otyp].oc_fixed_damage_bonus);
			txt = buf;
			putstr(datawin, 0, txt);
		}

		if (objects[otyp].oc_hitbonus != 0)
		{
			if (objects[otyp].oc_hitbonus > 0)
				Sprintf(buf, "To hit bonus:           +%d", objects[otyp].oc_hitbonus);
			else if (objects[otyp].oc_hitbonus < 0)
				Sprintf(buf, "To hit bonus:           %d", objects[otyp].oc_hitbonus);

			txt = buf;
			putstr(datawin, 0, txt);
		}
	}

	int mcadj = objects[otyp].oc_mc_adjustment + (objects[otyp].oc_flags & O1_SPE_AFFECTS_MC_ADJUSTMENT) ? obj->spe : 0;
	if (objects[otyp].oc_mc_adjustment != 0 || mcadj != 0)
	{
		if (mcadj >= 0)
			Sprintf(buf, "Target MC adjustment:   +%d", mcadj);
		else
			Sprintf(buf, "Target MC adjustment:   %d", mcadj);

		txt = buf;
		putstr(datawin, 0, txt);
	}

	if (objects[otyp].oc_class == ARMOR_CLASS || (objects[otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED))
	{
		Sprintf(buf2, "%d", 10 - objects[otyp].oc_armor_class);
		Sprintf(buf, "Base armor class:       %s", buf2);
		txt = buf;
		putstr(datawin, 0, txt);

		int mc = objects[otyp].oc_magic_cancellation;
		//if (objects[otyp].oc_flags & O1_SPE_AFFECTS_MC)
		//	mc+= obj->spe;

		/* magic cancellation */
		Sprintf(buf2, "%s%d", mc >= 0 ? "+" : "", mc);
		Sprintf(buf, "Magic cancellation:     %s", buf2);
		txt = buf;
		putstr(datawin, 0, txt);
	}

	if (objects[otyp].oc_class == WAND_CLASS || (objects[otyp].oc_class == TOOL_CLASS && is_wand_like_tool(obj)))
	{
		if (stats_known)
		{
			if (objects[otyp].oc_wsdice > 0 || objects[otyp].oc_wsdam > 0 || objects[otyp].oc_wsdmgplus > 0)
			{
				boolean maindiceprinted = FALSE;
				char plusbuf[BUFSZ];
				Sprintf(buf, "Wand effect damage:     ");

				if (objects[otyp].oc_wsdice > 0 && objects[otyp].oc_wsdam > 0)
				{
					maindiceprinted = TRUE;
					Sprintf(plusbuf, "%dd%d", objects[otyp].oc_wsdice, objects[otyp].oc_wsdam);
					Strcat(buf, plusbuf);
				}

				if (objects[otyp].oc_wsdmgplus != 0)
				{
					if (maindiceprinted && objects[otyp].oc_wsdmgplus > 0)
					{
						Sprintf(plusbuf, "+");
						Strcat(buf, plusbuf);
					}
					Sprintf(plusbuf, "%d", objects[otyp].oc_wsdmgplus);
					Strcat(buf, plusbuf);
				}
				txt = buf;
				putstr(datawin, 0, txt);

			}
			if (objects[otyp].oc_spell_range > 0)
			{
				Sprintf(buf, "Wand effect range:      %d'", objects[otyp].oc_spell_range * 5);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_spell_radius > 0)
			{
				Sprintf(buf, "Wand effect radius:     %d'", objects[otyp].oc_spell_radius * 5);
				txt = buf;
				putstr(datawin, 0, txt);
			}
		}
	}

	if (objects[otyp].oc_class != SPBOOK_CLASS && objects[otyp].oc_class != WAND_CLASS &&
		(objects[otyp].oc_class == ARMOR_CLASS || (objects[otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED) || objects[otyp].oc_spell_casting_penalty != 0))
	{
		int splcaster = objects[otyp].oc_spell_casting_penalty;

		Sprintf(buf2, "%s%d%%", splcaster < 0 ? "+" : "", -splcaster * 5);
		if (splcaster < 0)
			Sprintf(buf, "Spell casting bonus:    %s", buf2);
		else
			Sprintf(buf, "Spell casting penalty:  %s", buf2);

		txt = buf;
		putstr(datawin, 0, txt);
	}



	if (obj->known && objects[otyp].oc_charged)
	{
		strcpy(buf, "");

		if(objects[otyp].oc_class == WAND_CLASS || (objects[otyp].oc_class == TOOL_CLASS && is_wand_like_tool(obj)))
			Sprintf(buf, "Charges left:           %d", obj->spe);
		else
		{
			char bonusbuf[BUFSZ] = "";
			if (obj->oclass == WEAPON_CLASS || is_weptool(obj))
			{
				int enchplus = obj->spe;
				if (!uses_spell_flags && stats_known && (objects[otyp].oc_aflags & A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS))
				{
					enchplus *= 2;
				}

				Sprintf(bonusbuf, " (%s%d to hit and damage)", enchplus >= 0 ? "+" : "", enchplus);
			}

			if (obj->oclass == ARMOR_CLASS || (stats_known && (objects[obj->otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED)))
			{
				if (objects[otyp].oc_flags & O1_SPE_AFFECTS_MC)
				{
					Sprintf(eos(bonusbuf), " (%s%d to AC and %s%d to MC)", 
						obj->spe <= 0 ? "+" : "",
						-obj->spe,
						obj->spe >= 0 ? "+" : "",
						obj->spe
					);
				}
				else
				{
					Sprintf(eos(bonusbuf), " (%s%d %s to AC)", 
						obj->spe <= 0 ? "+" : "", 
						-obj->spe,
						obj->spe >= 0 ? "bonus" : "penalty");
				}
			}

			Sprintf(buf, "Enchantment status:     %s%d%s", obj->spe >= 0 ? "+" : "", obj->spe, bonusbuf);
		}

		txt = buf;
		putstr(datawin, 0, txt);
	}
	if (obj->rknown && (obj->oeroded || obj->oeroded2 || obj->oerodeproof))
	{
		char erodebuf[BUFSZ] = "";
		char penaltybuf[BUFSZ] = "";
		int penalty = 0;

		add_erosion_words(obj, erodebuf);
		*erodebuf = highc(*erodebuf);

		if (obj->oeroded > 0 || obj->oeroded2 > 0)
		{
			if (obj->oclass == WEAPON_CLASS || is_weptool(obj))
			{
				penalty = greatest_erosion(obj);
				Sprintf(penaltybuf, "(%d to damage) ", -penalty);
			}

			if (obj->oclass == ARMOR_CLASS || (stats_known && (objects[obj->otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED)))
			{
				penalty = min(greatest_erosion(obj), objects[obj->otyp].oc_armor_class);
				Sprintf(eos(penaltybuf), "(+%d penalty to AC)", penalty);
			}
		}
		Sprintf(buf, "Erosion status:         %s%s", erodebuf, penaltybuf);

		txt = buf;
		putstr(datawin, 0, txt);
	}
	if (obj->bknown)
	{
		Sprintf(buf, "Blessing status:        %s", obj->blessed ? "Blessed" : obj->cursed ? "Cursed" : "Uncursed");
		txt = buf;
		putstr(datawin, 0, txt);
	}
	if (obj->opoisoned)
	{
		Sprintf(buf, "Poisoned status:        Poisoned (+6d6 poison damage)");
		txt = buf;
		putstr(datawin, 0, txt);
	}
	if (obj->elemental_enchantment)
	{
		Sprintf(buf, "Elemental enchantment:  %s", obj->elemental_enchantment == FIRE_ENCHANTMENT ? "Flaming (+2d6 fire damage)" :
			obj->elemental_enchantment == COLD_ENCHANTMENT ? "Freezing (+1d6 cold damage)" :
			obj->elemental_enchantment == LIGHTNING_ENCHANTMENT ? "Electrified (+3d6 lightning damage)" :
			obj->elemental_enchantment == DEATH_ENCHANTMENT ? "Deathly (kills on hit)" : "Unknown enchantment"
		);
		txt = buf;
		putstr(datawin, 0, txt);
	}

	if (obj->cooldownleft > 0)
	{
		Sprintf(buf, "Cooldown left:          %d rounds", obj->cooldownleft);
		txt = buf;
		putstr(datawin, 0, txt);
	}

	if (stats_known && obj->invokeon)
	{
		Sprintf(buf, "Invoked ability:        Activated");
		txt = buf;
		putstr(datawin, 0, txt);
	}

	/* Various extra info is the item is known */
	if (stats_known && !uses_spell_flags)
	{
		if (objects[otyp].oc_oprop > 0
			|| objects[otyp].oc_oprop2 > 0 
			|| objects[otyp].oc_oprop3 > 0 
			|| objects[otyp].oc_mana_bonus > 0 
			|| objects[otyp].oc_hp_bonus > 0
			|| objects[otyp].oc_bonus_attributes > 0
			|| objects[otyp].oc_aflags > 0
			|| objects[otyp].oc_pflags & P1_CONFERS_LUCK
			|| objects[otyp].oc_pflags & P1_CONFERS_UNLUCK
			)
		{
			Sprintf(buf, "Conferred powers:");
			txt = buf;
			putstr(datawin, 0, txt);

			int powercnt = 0;
			for (int j = 1; j <= 6; j++)
			{
				int prop = 0;
				if (j == 1)
					prop = objects[otyp].oc_oprop;
				else if (j == 2)
					prop = objects[otyp].oc_oprop2;
				else if (j == 3)
					prop = objects[otyp].oc_oprop3;
				else if (j == 4)
					prop = objects[otyp].oc_mana_bonus;
				else if (j == 5)
					prop = objects[otyp].oc_hp_bonus;
				else if (j == 6)
					prop = objects[otyp].oc_bonus_attributes;

				char pwbuf[BUFSZ] = "";
				if (j == 1)
				{
					if (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_WHEN_CARRIED)
						Sprintf(eos(pwbuf), " when carried");
					else
						Sprintf(eos(pwbuf), " when %s", is_weapon(obj)? "wielded" : "worn");

					if (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_ALL_CHARACTERS)
						Sprintf(eos(pwbuf), " (applies to all)");
					else if (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
						Sprintf(eos(pwbuf), " (applies to inappropriate only)");
				}
				else if (j == 2)
				{
					if (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_WHEN_CARRIED)
						Sprintf(eos(pwbuf), " when carried");
					else
						Sprintf(eos(pwbuf), " when %s", is_weapon(obj) ? "wielded" : "worn");

					if (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_ALL_CHARACTERS)
						Sprintf(eos(pwbuf), " (applies to all)");
					else if (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
						Sprintf(eos(pwbuf), " (applies to inappropriate only)");
				}
				else if (j == 3)
				{
					if (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_WHEN_CARRIED)
						Sprintf(eos(pwbuf), " when carried");
					else
						Sprintf(eos(pwbuf), " when %s", is_weapon(obj) ? "wielded" : "worn");

					if (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_ALL_CHARACTERS)
						Sprintf(eos(pwbuf), " (applies to all)");
					else if (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
						Sprintf(eos(pwbuf), " (applies to inappropriate only)");
				}
				else if (j == 4)
				{
					if (objects[otyp].oc_pflags & P1_HP_BONUS_APPLIES_WHEN_CARRIED)
						Sprintf(eos(pwbuf), " when carried");
					else
						Sprintf(eos(pwbuf), " when %s", is_weapon(obj) ? "wielded" : "worn");

					if (objects[otyp].oc_pflags & P1_HP_BONUS_APPLIES_TO_ALL_CHARACTERS)
						Sprintf(eos(pwbuf), " (applies to all)");
					else if (objects[otyp].oc_pflags & P1_HP_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
						Sprintf(eos(pwbuf), " (applies to inappropriate only)");
					else if (objects[otyp].oc_pflags & P1_HP_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)
						Sprintf(eos(pwbuf), " (negative to inappropriate)");
				}
				else if (j == 5)
				{
					if (objects[otyp].oc_pflags & P1_MANA_BONUS_APPLIES_WHEN_CARRIED)
						Sprintf(eos(pwbuf), " when carried");
					else
						Sprintf(eos(pwbuf), " when %s", is_weapon(obj) ? "wielded" : "worn");

					if (objects[otyp].oc_pflags & P1_MANA_BONUS_APPLIES_TO_ALL_CHARACTERS)
						Sprintf(eos(pwbuf), " (applies to all)");
					else if (objects[otyp].oc_pflags & P1_MANA_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
						Sprintf(eos(pwbuf), " (applies to inappropriate only)");
					else if (objects[otyp].oc_pflags & P1_MANA_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)
						Sprintf(eos(pwbuf), " (negative to inappropriate)");
				}
				else if (j == 6)
				{
					if (objects[otyp].oc_pflags & P1_ATTRIBUTE_BONUS_APPLIES_WHEN_CARRIED)
						Sprintf(eos(pwbuf), " when carried");
					else
						Sprintf(eos(pwbuf), " when %s", is_weapon(obj) ? "wielded" : "worn");

					if (objects[otyp].oc_pflags & P1_ATTRIBUTE_BONUS_APPLIES_TO_ALL_CHARACTERS)
						Sprintf(eos(pwbuf), " (applies to all)");
					else if (objects[otyp].oc_pflags & P1_ATTRIBUTE_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
						Sprintf(eos(pwbuf), " (applies to inappropriate only)");
					else if (objects[otyp].oc_pflags & P1_ATTRIBUTE_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)
						Sprintf(eos(pwbuf), " (negative to inappropriate)");
				}

				if (prop > 0)
				{
					if (j < 6)
					{
						powercnt++;

						strcpy(buf2, "");
						Sprintf(buf3, " %2d - ", powercnt);
					}
					if(j <= 3)
					{
						strcpy(buf2, get_property_name(prop));
						*buf2 = highc(*buf2);
					}
					else if(j == 4)
					{
						if (objects[otyp].oc_pflags & P1_MANA_PERCENTAGE_BONUS)
						{
							Sprintf(buf2, "Mana pool is increased by %d%%", prop);
						}
						else
						{
							Sprintf(buf2, "Mana pool increased by %d", prop);
						}
					}
					else if (j == 5)
					{
						if (objects[otyp].oc_pflags & P1_HP_PERCENTAGE_BONUS)
						{
							Sprintf(buf2, "Maximum hit points are increased by %d%%", prop);
						}
						else
						{
							Sprintf(buf2, "Maximum hit points increased by %d", prop);
						}
					}
					else if (j == 6)
					{
						for (int k = 0; k < 12; k++)
						{
							strcpy(buf2, "");
							int stat = objects[otyp].oc_attribute_bonus;
							if (objects[otyp].oc_charged && !(prop & IGNORE_SPE))
								stat += obj->spe;

							if(prop & SETS_FIXED_ATTRIBUTE)
								stat = min((k == 0 ? STR19(25) : 25), max(1, stat));

							char raisebuf[BUFSZ];
							if (prop & FIXED_IS_MAXIMUM)
								strcpy(raisebuf, "Lowers");
							else
								strcpy(raisebuf, "Raises");

							char grantbuf[BUFSZ];
							if (stat < 0)
								strcpy(grantbuf, "Causes");
							else
								strcpy(grantbuf, "Grants");

							char bonusbuf[BUFSZ];
							if (stat < 0)
								strcpy(bonusbuf, "penalty");
							else
								strcpy(bonusbuf, "bonus");

							if (k == 0 && prop & BONUS_TO_STR)
							{
								powercnt++;

								if (prop & SETS_FIXED_ATTRIBUTE)
								{
									char strbuf[BUFSZ] = "";
									if (stat <= 18)
										Sprintf(strbuf, "%d", stat);
									else if(stat < STR18(10))
										Sprintf(strbuf, "18/0%d", stat - 18);
									else if (stat < STR18(100))
										Sprintf(strbuf, "18/%d", stat - 18);
									else if (stat == STR18(100))
										Sprintf(strbuf, "18/**");
									else
										Sprintf(strbuf, "%d", stat - 100);
									Sprintf(buf2, "%s strength to %s", raisebuf, strbuf);
								}
								else
									Sprintf(buf2, "%s %s%d %s to strength", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
							}
							if (k == 1 && prop & BONUS_TO_DEX)
							{
								powercnt++;

								if (prop & SETS_FIXED_ATTRIBUTE)
									Sprintf(buf2, "%s dexterity to %d", raisebuf, stat);
								else
									Sprintf(buf2, "%s %s%d %s to dexterity", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
							}
							if (k == 2 && prop & BONUS_TO_CON)
							{
								powercnt++;

								if (prop & SETS_FIXED_ATTRIBUTE)
									Sprintf(buf2, "%s constitution to %d", raisebuf, stat);
								else
									Sprintf(buf2, "%s %s%d %s to constitution", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
							}
							if (k == 3 && prop & BONUS_TO_INT)
							{
								powercnt++;

								if (prop & SETS_FIXED_ATTRIBUTE)
									Sprintf(buf2, "%s intelligence to %d", raisebuf, stat);
								else
									Sprintf(buf2, "%s %s%d %s to intelligence", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
							}
							if (k == 4 && prop & BONUS_TO_WIS)
							{
								powercnt++;

								if (prop & SETS_FIXED_ATTRIBUTE)
									Sprintf(buf2, "%s wisdom to %d", raisebuf, stat);
								else
									Sprintf(buf2, "%s %s%d %s to wisdom", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
							}
							if (k == 5 && prop & BONUS_TO_CHA)
							{
								powercnt++;

								if (prop & SETS_FIXED_ATTRIBUTE)
									Sprintf(buf2, "%s charisma to %d", raisebuf, stat);
								else
									Sprintf(buf2, "%s %s%d %s to charisma", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
							}
							if (k == 6 && prop & BONUS_TO_AC)
							{
								powercnt++;

								Sprintf(buf2, "%s %s%d %s to armor class", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
							}
							if (k == 7 && prop & BONUS_TO_DAMAGE)
							{
								powercnt++;

								Sprintf(buf2, "%s %s%d %s to damage", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
							}
							if (k == 8 && prop & BONUS_TO_HIT)
							{
								powercnt++;

								Sprintf(buf2, "%s %s%d %s to hit", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
							}
							if (k == 9 && prop & BONUS_TO_MC)
							{
								powercnt++;

								Sprintf(buf2, "%s %s%d %s to magic cancellation", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
							}
							if (k == 10 && prop & BONUS_TO_SPELL_CASTING)
							{
								powercnt++;

								Sprintf(buf2, "%s %s%d%% %s to spell casting", grantbuf, stat >= 0 ? "+" : "", stat * 5, bonusbuf);
							}
							if (k == 11 && prop & BONUS_TO_EXPERIENCE)
							{
								powercnt++;

								Sprintf(buf2, "%s %s%d%% %s to experience", grantbuf, stat >= 0 ? "+" : "", stat * 10, bonusbuf);
							}

							if (strcmp(buf2, "") != 0) // Something else than ""
							{
								Sprintf(buf3, " %2d - ", powercnt);
								Sprintf(buf, "%s%s%s", buf3, buf2, pwbuf);
								txt = buf;
								putstr(datawin, 0, txt);
							}

						}
					}

					if (j < 6)
					{
						if (strcmp(buf2, "") != 0) // Something else than ""
						{
							Sprintf(buf, "%s%s%s", buf3, buf2, pwbuf);
							txt = buf;
							putstr(datawin, 0, txt);
						}
					}
				}
			}

			/* Power-like flags here */
			if (objects[otyp].oc_pflags & P1_CONFERS_LUCK)
			{
				char pwbuf[BUFSZ] = "";
				if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_WHEN_CARRIED)
					Sprintf(eos(pwbuf), " when carried");
				else
					Sprintf(eos(pwbuf), " when %s", is_weapon(obj) ? "wielded" : "worn");

				if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_ALL_CHARACTERS)
					Sprintf(eos(pwbuf), " (applies to all)");
				else if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
					Sprintf(eos(pwbuf), " (applies to inappropriate only)");
				else if (objects[otyp].oc_pflags & P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)
					Sprintf(eos(pwbuf), " (negative to inappropriate)");

				powercnt++;
				Sprintf(buf, " %2d - Confers luck%s", powercnt, pwbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_pflags & P1_CONFERS_UNLUCK)
			{
				char pwbuf[BUFSZ] = "";
				if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_WHEN_CARRIED)
					Sprintf(eos(pwbuf), " when carried");
				else
					Sprintf(eos(pwbuf), " when %s", is_weapon(obj) ? "wielded" : "worn");

				if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_ALL_CHARACTERS)
					Sprintf(eos(pwbuf), " (applies to all)");
				else if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
					Sprintf(eos(pwbuf), " (applies to inappropriate only)");
				else if (objects[otyp].oc_pflags & P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)
					Sprintf(eos(pwbuf), " (negative to inappropriate)");

				powercnt++;
				Sprintf(buf, " %2d - Confers bad luck%s", powercnt, pwbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS))
			{
				powercnt++;
				Sprintf(buf, " %2d - Deals double damage on hit", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_SVB_MASK) == A1_VORPAL)
			{
				powercnt++;
				Sprintf(buf, " %2d - Beheads target on hit", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_SVB_MASK) == A1_BISECT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Besects target on hit", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_SVB_MASK) == A1_SHARPNESS)
			{
				powercnt++;
				Sprintf(buf, " %2d - 10%% chance of permanent damage equal to 25%% of target's maximum hit points", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_CRITICAL_STRIKE) && (objects[otyp].oc_aflags & A1_CRITICAL_STRIKE_IS_DEADLY))
			{
				powercnt++;
				int critchance = objects[otyp].oc_critical_strike_percentage;
				char chancebuf[BUFSZ] = "";
				Sprintf(chancebuf, " at %d%% chance", critchance);
				if((objects[otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK)
					Sprintf(buf, " %2d - Disintegrates the target on hit%s", powercnt, critchance < 100 ? chancebuf : "");
				else if ((objects[otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK)
					Sprintf(buf, " %2d - Slays the target on hit%s", powercnt, critchance < 100 ? chancebuf : "");
				else
					Sprintf(buf, " %2d - Causes lethal %s damage to the target %s", powercnt,
						get_damage_type_text((objects[otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_USES_EXTRA_DAMAGE_TYPE) ? objects[otyp].oc_extra_damagetype : AD_PHYS), 
						critchance < 100 ? chancebuf : "");
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_LEVEL_DRAIN))
			{
				powercnt++;
				Sprintf(buf, " %2d - Drains a level on hit", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_LIFE_LEECH))
			{
				powercnt++;
				Sprintf(buf, " %2d - Transfers hit points to the wielder on hit", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_WOUNDING))
			{
				powercnt++;
				Sprintf(buf, " %2d - Reduces hit points permanently on hit", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags3 & O3_PREVENTS_REVIVAL_OF_PERMITTED_TARGETS)
			{
				powercnt++;
				Sprintf(buf, " %2d - Prevents revival of permitted targets", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags3 & O3_PREVENTS_SUMMONING_BY_PERMITTED_TARGETS)
			{
				powercnt++;
				Sprintf(buf, " %2d - Prevents summoning by permitted targets", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}

			if (powercnt == 0)
			{
				Sprintf(buf, " (None)");
				txt = buf;
				putstr(datawin, 0, txt);
			}



			/* Power confer limitations */
			if (objects[otyp].oc_power_permissions)
			{
				powercnt = 0;

				Sprintf(buf, "Powers are conferred only to:");
				txt = buf;
				putstr(datawin, 0, txt);

				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_ARCHEOLOGIST)
				{
					powercnt++;
					Sprintf(buf, " %2d - Archeologists", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_BARBARIAN)
				{
					powercnt++;
					Sprintf(buf, " %2d - Barbarians", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_CAVEMAN)
				{
					powercnt++;
					Sprintf(buf, " %2d - Cavemen and cavewomen", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_HEALER)
				{
					powercnt++;
					Sprintf(buf, " %2d - Healers", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_KNIGHT)
				{
					powercnt++;
					Sprintf(buf, " %2d - Knights", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_MONK)
				{
					powercnt++;
					Sprintf(buf, " %2d - Monks", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_PRIEST)
				{
					powercnt++;
					Sprintf(buf, " %2d - Priests and priestesses", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_RANGER)
				{
					powercnt++;
					Sprintf(buf, " %2d - Rangers", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_ROGUE)
				{
					powercnt++;
					Sprintf(buf, " %2d - Rogues", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_SAMURAI)
				{
					powercnt++;
					Sprintf(buf, " %2d - Samurais", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_TOURIST)
				{
					powercnt++;
					Sprintf(buf, " %2d - Tourists", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_VALKYRIE)
				{
					powercnt++;
					Sprintf(buf, " %2d - Valkyries", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_WIZARD)
				{
					powercnt++;
					Sprintf(buf, " %2d - Wizards", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_RACE_DWARF)
				{
					powercnt++;
					Sprintf(buf, " %2d - Dwarves", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_RACE_ELF)
				{
					powercnt++;
					Sprintf(buf, " %2d - Elves", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_RACE_GNOLL)
				{
					powercnt++;
					Sprintf(buf, " %2d - Gnolls", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_RACE_GNOME)
				{
					powercnt++;
					Sprintf(buf, " %2d - Gnomes", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_RACE_HUMAN)
				{
					powercnt++;
					Sprintf(buf, " %2d - Humans", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_RACE_ORC)
				{
					powercnt++;
					Sprintf(buf, " %2d - Orcs", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_GENDER_FEMALE)
				{
					powercnt++;
					Sprintf(buf, " %2d - Females", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_GENDER_MALE)
				{
					powercnt++;
					Sprintf(buf, " %2d - Males", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ALIGNMENT_CHAOTIC)
				{
					powercnt++;
					Sprintf(buf, " %2d - Chaotic", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ALIGNMENT_LAWFUL)
				{
					powercnt++;
					Sprintf(buf, " %2d - Lawful", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (objects[otyp].oc_power_permissions & PERMITTED_ALIGNMENT_NEUTRAL)
				{
					powercnt++;
					Sprintf(buf, " %2d - Neutral", powercnt);
					txt = buf;
					putstr(datawin, 0, txt);
				}
				if (powercnt == 0)
				{
					Sprintf(buf, " (None)");
					txt = buf;
					putstr(datawin, 0, txt);
				}

			}
		}


		/* Target permissions */
		if (objects[otyp].oc_target_permissions != ALL_TARGETS || (objects[otyp].oc_flags3 & (O3_PERMTTED_TARGET_CHAOTIC | O3_PERMTTED_TARGET_NEUTRAL | O3_PERMTTED_TARGET_LAWFUL)))
		{
			int powercnt = 0;

			Sprintf(buf, "Permitted targets:");
			txt = buf;
			putstr(datawin, 0, txt);

			/* Flags here */
			if (objects[otyp].oc_target_permissions != ALL_TARGETS)
			{
				if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M1_FLAG)
				{
					if (objects[otyp].oc_target_permissions & M1_AMORPHOUS)
					{
						powercnt++;
						Sprintf(buf, " %2d - Amorphous beings", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M1_AMPHIBIOUS)
					{
						powercnt++;
						Sprintf(buf, " %2d - Amphibious beings", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M1_HUMANOID)
					{
						powercnt++;
						Sprintf(buf, " %2d - Humanoids", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M1_ANIMAL)
					{
						powercnt++;
						Sprintf(buf, " %2d - Animals", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
				}
				else if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M2_FLAG)
				{
					if (objects[otyp].oc_target_permissions & M2_DEMON)
					{
						powercnt++;
						Sprintf(buf, " %2d - Demons", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M2_DWARF)
					{
						powercnt++;
						Sprintf(buf, " %2d - Dwarves", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M2_ELF)
					{
						powercnt++;
						Sprintf(buf, " %2d - Elves", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M2_GIANT)
					{
						powercnt++;
						Sprintf(buf, " %2d - Giants", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M2_GNOLL)
					{
						powercnt++;
						Sprintf(buf, " %2d - Gnolls", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M2_GNOME)
					{
						powercnt++;
						Sprintf(buf, " %2d - Gnomes", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M2_HUMAN)
					{
						powercnt++;
						Sprintf(buf, " %2d - Human beings", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M2_ORC)
					{
						powercnt++;
						Sprintf(buf, " %2d - Orcs", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M2_UNDEAD)
					{
						powercnt++;
						Sprintf(buf, " %2d - Undead", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
					if (objects[otyp].oc_target_permissions & M2_WERE)
					{
						powercnt++;
						Sprintf(buf, " %2d - Lycanthropes", powercnt);
						txt = buf;
						putstr(datawin, 0, txt);
					}
				}
				else if (objects[otyp].oc_flags3 & O3_TARGET_PERMISSION_IS_M3_FLAG)
				{
					//Nothing here yet
				}
				else if (objects[otyp].oc_target_permissions > 0 && objects[otyp].oc_target_permissions < MAXMCLASSES)
				{
					char monsymbuf[BUFSZ];
					strcpy(monsymbuf, def_monsyms[objects[otyp].oc_target_permissions].explain);
					*monsymbuf = highc(*monsymbuf);

					powercnt++;
					Sprintf(buf, " %2d - %s", powercnt, makeplural(monsymbuf));
					txt = buf;
					putstr(datawin, 0, txt);
				}
			}

			if (objects[otyp].oc_flags3 & O3_PERMTTED_TARGET_CHAOTIC)
			{
				powercnt++;
				Sprintf(buf, " %2d - Chaotic creatures", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags3 & O3_PERMTTED_TARGET_NEUTRAL)
			{
				powercnt++;
				Sprintf(buf, " %2d - Neutral creatures", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags3 & O3_PERMTTED_TARGET_LAWFUL)
			{
				powercnt++;
				Sprintf(buf, " %2d - Lawful creatures", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (powercnt == 0)
			{
				Sprintf(buf, " (None)");
				txt = buf;
				putstr(datawin, 0, txt);
			}


		}




		/* Item properties */
		if (objects[otyp].oc_flags & ~(O1_THROWN_WEAPON_ONLY | O1_MELEE_AND_THROWN_WEAPON
			| O1_WAND_LIKE_TOOL | O1_NON_SPELL_SPELLBOOK | O1_EDIBLE_NONFOOD))
		{
			int powercnt = 0;

			Sprintf(buf, "Item properties:");
			txt = buf;
			putstr(datawin, 0, txt);

			/* Flags here */
			if (objects[otyp].oc_flags & O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED)
			{
				powercnt++;
				Sprintf(buf, " %2d - Becomes cursed when picked up", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_BECOMES_CURSED_WHEN_WORN)
			{
				powercnt++;
				Sprintf(buf, " %2d - Becomes cursed when worn", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED)
			{
				powercnt++;
				Sprintf(buf, " %2d - Undroppable when cursed", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_COLD_RESISTANT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Cold resistant", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_CORROSION_RESISTANT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Corrosion resistant", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_DISINTEGRATION_RESISTANT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Disintegration resistant", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_FIRE_RESISTANT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Fire resistant", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_INDESTRUCTIBLE)
			{
				powercnt++;
				Sprintf(buf, " %2d - Indestructible", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_LIGHTNING_RESISTANT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Lightning resistant", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_NOT_CURSEABLE)
			{
				powercnt++;
				Sprintf(buf, " %2d - Cannot be cursed", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_POLYMORPH_RESISTANT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Polymorph resistant", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_RUST_RESISTANT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Rust-proof", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED)
			{
				powercnt++;
				Sprintf(buf, " %2d - Is armor when wielded", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags & O1_IS_WEAPON_WHEN_WIELDED)
			{
				powercnt++;
				Sprintf(buf, " %2d - Is weapon when wielded", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags2 & O2_SHINES_MAGICAL_LIGHT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Shines magical light when wielded", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (objects[otyp].oc_flags3 & O3_ELEMENTAL_ENCHANTABLE)
			{
				powercnt++;
				Sprintf(buf, " %2d - Can be specially enchanted", powercnt);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (powercnt == 0)
			{
				Sprintf(buf, " (None)");
				txt = buf;
				putstr(datawin, 0, txt);
			}
		}


		/* Description*/
		if (objects[otyp].oc_short_description && !(obj->oartifact && obj->nknown))
		{
			/* One empty line here */
			Sprintf(buf, "");
			txt = buf;
			putstr(datawin, 0, txt);

			Sprintf(buf, "Description:");
			txt = buf;
			putstr(datawin, 0, txt);
			Sprintf(buf, objects[otyp].oc_short_description);
			txt = buf;
			putstr(datawin, 0, txt);
		}
	}


	/* Artifact powers */
	if (obj->oartifact && stats_known)
	{
		int powercnt = 0;
		Sprintf(buf, "Artifact properties:");
		txt = buf;
		putstr(datawin, 0, txt);

		const char* alingstr = align_str(artilist[obj->oartifact].alignment);
		powercnt++;
		Sprintf(buf, " %2d - Alignment is %s", powercnt, alingstr);
		txt = buf;
		putstr(datawin, 0, txt);

		if (artilist[obj->oartifact].role > NON_PM)
		{
			powercnt++;
			Sprintf(buf, " %2d - Artifact is associated with %s", powercnt, makeplural(mons[artilist[obj->oartifact].role].mname));
			txt = buf;
			putstr(datawin, 0, txt);
		}

		if (artilist[obj->oartifact].race > NON_PM)
		{
			powercnt++;
			Sprintf(buf, " %2d - Artifact is associated with %s", powercnt, makeplural(mons[artilist[obj->oartifact].race].mname));
			txt = buf;
			putstr(datawin, 0, txt);
		}

		if (artilist[obj->oartifact].attk.damn != 0 || artilist[obj->oartifact].attk.damd != 0 || artilist[obj->oartifact].attk.damp != 0)
		{
			char dmgttext[BUFSZ] = "";
			strcpy(dmgttext, get_damage_type_text(artilist[obj->oartifact].attk.adtyp));

			powercnt++;
			Sprintf(buf, " %2d - Artifact damage type is %s damage", powercnt, dmgttext);
			txt = buf;
			putstr(datawin, 0, txt);

			char dmgbuf[BUFSIZ] = "";
			char plusbuf[BUFSIZ] = "";
			if (artilist[obj->oartifact].attk.damp != 0)
				Sprintf(plusbuf, "%s%d", artilist[obj->oartifact].attk.damp >= 0 ? "+" : "", artilist[obj->oartifact].attk.damp);

			if (artilist[obj->oartifact].attk.damn > 0 && artilist[obj->oartifact].attk.damd > 0)
			{
				Sprintf(dmgbuf, "Artifact damage bonus is %dd%d%s", artilist[obj->oartifact].attk.damn, artilist[obj->oartifact].attk.damd, plusbuf);
			}
			else if (artilist[obj->oartifact].attk.damn == ARTDMG_DOUBLE_DAMAGE)
				Sprintf(dmgbuf, "Artifact deals double normal damage");
			else if (artilist[obj->oartifact].attk.damn == ARTDMG_TRIPLE_DAMAGE)
				Sprintf(dmgbuf, "Artifact deals triple normal damage");
			else if (artilist[obj->oartifact].attk.damn == ARTDMG_QUADRUPLE_DAMAGE)
				Sprintf(dmgbuf, "Artifact deals quadruple normal damage");
			else
				Sprintf(dmgbuf, "Artifact damage bonus is %s", plusbuf);

			powercnt++;
			Sprintf(buf, " %2d - %s", powercnt, dmgbuf);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (artilist[obj->oartifact].tohit_dice != 0 || artilist[obj->oartifact].tohit_diesize != 0 || artilist[obj->oartifact].tohit_plus != 0)
		{
			char tohitbuf[BUFSIZ] = "";
			char plusbuf[BUFSIZ] = "";
			if (artilist[obj->oartifact].tohit_plus != 0)
				Sprintf(plusbuf, "%s%d", artilist[obj->oartifact].tohit_plus >= 0 ? "+" : "", artilist[obj->oartifact].tohit_plus);

			if (artilist[obj->oartifact].tohit_dice > 0 && artilist[obj->oartifact].tohit_diesize > 0)
			{
				Sprintf(tohitbuf, "Artifact to-hit bonus is %dd%d%s", artilist[obj->oartifact].tohit_dice, artilist[obj->oartifact].tohit_diesize, plusbuf);
			}
			else
				Sprintf(tohitbuf, "Artifact to-hit bonus is %s", plusbuf);

			powercnt++;
			Sprintf(buf, " %2d - %s", powercnt, tohitbuf);
			txt = buf;
			putstr(datawin, 0, txt);
		}

		if (artilist[obj->oartifact].worn_prop > 0)
		{
			char defensetext[BUFSZ] = "";
			strcpy(defensetext, get_property_name(artilist[obj->oartifact].worn_prop));
			*defensetext = highc(*defensetext);

			powercnt++;
			if (is_weapon(obj))
				Sprintf(buf, " %2d - %s when wielded", powercnt, defensetext);
			else
				Sprintf(buf, " %2d - %s when worn", powercnt, defensetext);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (artilist[obj->oartifact].carried_prop > 0)
		{
			char defensetext[BUFSZ] = "";
			strcpy(defensetext, get_property_name(artilist[obj->oartifact].carried_prop));
			*defensetext = highc(*defensetext);

			powercnt++;
			Sprintf(buf, " %2d - %s when carried", powercnt, defensetext);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (artilist[obj->oartifact].inv_prop > 0)
		{
			char invoketext[BUFSIZ] = "";
			if (artilist[obj->oartifact].inv_prop > LAST_PROP)
			{
				const char* invprop = get_artifact_invoke_name(artilist[obj->oartifact].inv_prop);
				strcpy(invoketext, invprop);
				*invoketext = highc(*invoketext);
			}
			else
			{
				strcpy(invoketext, get_property_name(artilist[obj->oartifact].inv_prop));
				*invoketext = highc(*invoketext);
			}

			powercnt++;
			Sprintf(buf, " %2d - %s when invoked", powercnt, invoketext);
			txt = buf;
			putstr(datawin, 0, txt);
		}


		unsigned long aflags = artilist[obj->oartifact].aflags;
		unsigned long mtype = artilist[obj->oartifact].mtype;
		if (aflags & AF_BEHEAD)
		{
			powercnt++;
			Sprintf(buf, " %2d - May behead target on hit", powercnt);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_BISECT)
		{
			powercnt++;
			Sprintf(buf, " %2d - May bisect target on hit", powercnt);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_HITS_ADJACENT_SQUARES)
		{
			powercnt++;
			Sprintf(buf, " %2d - Hits also adjacent squares", powercnt);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_BLOODTHIRSTY)
		{
			powercnt++;
			Sprintf(buf, " %2d - Bloodthirsty", powercnt);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_PREVENTS_REVIVAL_OF_MON)
		{
			powercnt++;
			Sprintf(buf, " %2d - Prevents revival of %s", powercnt,
				((aflags & AF_DFLAG2) && mtype & M2_ORC) ? "orcs" : 
				((aflags & AF_DFLAG2) && mtype & M2_ELF) ? "elves" :
				((aflags & AF_DFLAG2) && mtype & M2_DEMON) ? "demons" :
				((aflags & AF_DFLAG2) && mtype & M2_UNDEAD) ? "undead" :
				((aflags & AF_DFLAG2) && mtype & M2_GIANT) ? "giants" :
				((aflags & AF_DFLAG2) && mtype & M2_WERE) ? "lycanthropes" :
				((aflags & AF_DCLAS) && mtype == S_TROLL) ? "trolls" :
				((aflags & AF_DCLAS) && mtype == S_OGRE) ? "ogres" :
				((aflags & AF_DCLAS) && mtype == S_DRAGON) ? "dragons" :
				"a specific type of monsters");
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_PREVENTS_SUMMONING_OF_MON)
		{
			powercnt++;
			Sprintf(buf, " %2d - Prevents summoning by %s", powercnt,
				((aflags & AF_DFLAG2) && mtype & M2_ORC) ? "orcs" :
				((aflags & AF_DFLAG2) && mtype & M2_ELF) ? "elves" :
				((aflags & AF_DFLAG2) && mtype & M2_DEMON) ? "demons" :
				((aflags & AF_DFLAG2) && mtype & M2_UNDEAD) ? "undead" :
				((aflags & AF_DFLAG2) && mtype & M2_GIANT) ? "giants" :
				((aflags & AF_DFLAG2) && mtype & M2_WERE) ? "lycanthropes" :
				((aflags & AF_DCLAS) && mtype == S_TROLL) ? "trolls" :
				((aflags & AF_DCLAS) && mtype == S_OGRE) ? "ogres" :
				((aflags & AF_DCLAS) && mtype == S_DRAGON) ? "dragons" :
				"a specific type of monsters");
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_MAGIC_ABSORBING)
		{
			powercnt++;
			Sprintf(buf, " %2d - Magic absorbing", powercnt);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_MONSTERS_CAN_TRACK_ARTIFACT)
		{
			powercnt++;
			Sprintf(buf, " %2d - Monsters can track the artifact", powercnt);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_ANGERS_DEMONS)
		{
			powercnt++;
			Sprintf(buf, " %2d - Angers demons on sight", powercnt);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_INTEL)
		{
			powercnt++;
			Sprintf(buf, " %2d - Intelligent", powercnt);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_SPEAK)
		{
			powercnt++;
			Sprintf(buf, " %2d - Speaking", powercnt);
			txt = buf;
			putstr(datawin, 0, txt);
		}
		if (aflags & AF_DRLI)
		{
			powercnt++;
			Sprintf(buf, " %2d - Drains life levels on hit", powercnt);
			txt = buf;
			putstr(datawin, 0, txt);
		}



		for (int i = 1; i <= 2; i++)
		{
			unsigned long specialeffect = 0;
			char endbuf[BUFSIZ] = "";
			if (i == 1)
			{
				specialeffect = artilist[obj->oartifact].spfx;
				if (is_weapon(obj))
					strcpy(endbuf, "when wielded");
				else
					strcpy(endbuf, "when worn");
			}
			else
			{
				specialeffect = artilist[obj->oartifact].cspfx;
				strcpy(endbuf, "when carried");
			}
			if (specialeffect & SPFX_SEEK)
			{
				powercnt++;
				Sprintf(buf, " %2d - Seeking %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_SEARCH)
			{
				powercnt++;
				Sprintf(buf, " %2d - Searching %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_WARN_OF_MON)
			{
				powercnt++;
				Sprintf(buf, " %2d - Warning of the presence of %s %s", powercnt,
					(context.warntype.obj& M2_ORC) ? "orcs"
					: (context.warntype.obj & M2_ELF) ? "elves"
					: (context.warntype.obj & M2_WERE) ? "lycanthropes"
					: (context.warntype.obj & M2_GIANT) ? "giants"
					: (context.warntype.obj & M2_DEMON) ? "demons" : something, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_ORC_WARNING)
			{
				powercnt++;
				Sprintf(buf, " %2d - Warning of the presence of orcs %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_ELF_WARNING)
			{
				powercnt++;
				Sprintf(buf, " %2d - Warning of the presence of elves %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_WARNING)
			{
				powercnt++;
				Sprintf(buf, " %2d - Warning %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_HALRES)
			{
				powercnt++;
				Sprintf(buf, " %2d - Hallucination resistance %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_ESP)
			{
				powercnt++;
				Sprintf(buf, " %2d - ESP %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_STLTH)
			{
				powercnt++;
				Sprintf(buf, " %2d - Stealth %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_REGEN)
			{
				powercnt++;
				Sprintf(buf, " %2d - Regeneration %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_STLTH)
			{
				powercnt++;
				Sprintf(buf, " %2d - Mana regeneration %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_HSPDAM)
			{
				powercnt++;
				Sprintf(buf, " %2d - Half spell damage %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_HPHDAM)
			{
				powercnt++;
				Sprintf(buf, " %2d - Half physical damage %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS)
			{
				powercnt++;
				Sprintf(buf, " %2d - Half physical damage against undead and demons %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_TCTRL)
			{
				powercnt++;
				Sprintf(buf, " %2d - Teleport control %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_LUCK)
			{
				powercnt++;
				Sprintf(buf, " %2d - Confers luck %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_XRAY)
			{
				powercnt++;
				Sprintf(buf, " %2d - X-ray vision %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_REFLECT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Reflection %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_PROTECT)
			{
				powercnt++;
				Sprintf(buf, " %2d - Protection %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_AGGRAVATE_MONSTER)
			{
				powercnt++;
				Sprintf(buf, " %2d - Aggravates monsters %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
			if (specialeffect & SPFX_UNLUCK)
			{
				powercnt++;
				Sprintf(buf, " %2d - Confers bad luck %s", powercnt, endbuf);
				txt = buf;
				putstr(datawin, 0, txt);
			}
		}

		if (artilist[obj->oartifact].mtype > 0)
		{
			char endbuf[BUFSIZ] = "";
			if ((artilist[obj->oartifact].aflags & AF_DMONS) && artilist[obj->oartifact].mtype < NUMMONS)
			{
				strcpy(endbuf, mons[artilist[obj->oartifact].mtype].mname);
			}
			else if (artilist[obj->oartifact].aflags & AF_DFLAG1)
			{
			}
			else if (artilist[obj->oartifact].aflags & AF_DFLAG2)
			{
				if (artilist[obj->oartifact].mtype == M2_DWARF)
					strcpy(endbuf, "dwarves");
				else if (artilist[obj->oartifact].mtype == M2_DEMON)
					strcpy(endbuf, "demons");
				else if (artilist[obj->oartifact].mtype == M2_ELF)
					strcpy(endbuf, "elves");
				else if (artilist[obj->oartifact].mtype == M2_GIANT)
					strcpy(endbuf, "giants");
				else if (artilist[obj->oartifact].mtype == M2_GNOLL)
					strcpy(endbuf, "gnolls");
				else if (artilist[obj->oartifact].mtype == M2_GNOME)
					strcpy(endbuf, "gnomes");
				else if (artilist[obj->oartifact].mtype == M2_HUMAN)
					strcpy(endbuf, "human beings");
				else if (artilist[obj->oartifact].mtype == M2_MODRON)
					strcpy(endbuf, "modrons");
				else if (artilist[obj->oartifact].mtype == M2_ORC)
					strcpy(endbuf, "orcs");
				else if (artilist[obj->oartifact].mtype == M2_UNDEAD)
					strcpy(endbuf, "undead creatures");
				else if (artilist[obj->oartifact].mtype == M2_WERE)
					strcpy(endbuf, "lycanthropes");
			}
			else if (artilist[obj->oartifact].aflags & AF_DCLAS && artilist[obj->oartifact].mtype < MAXMCLASSES)
			{
				strcpy(endbuf, def_monsyms[artilist[obj->oartifact].mtype].explain);
			}
			else if (artilist[obj->oartifact].aflags & AF_DALIGN)
			{
				strcpy(endbuf, "non-aligned creatures");
			}
			powercnt++;
			Sprintf(buf, " %2d - Attack bonus applies to %s", powercnt, endbuf);
			txt = buf;
			putstr(datawin, 0, txt);
		}
	}

	display_nhwindow(datawin, FALSE);
	destroy_nhwindow(datawin), datawin = WIN_ERR;

	return 0;
}

static const char* damage_type_names[] = {
	"physical", "magic", "fire", "cold", "",
	"", "", "electrical", "", "",
	"", "", "stunning", "", "",
	"life draining",
};


const char* get_damage_type_text(damagetype)
int damagetype;
{
	if (damagetype < 0 || damagetype >= SIZE(damage_type_names))
		return empty_string;

	return damage_type_names[damagetype];
}

static const char* defense_type_names[] = {
	"physical damage resistance", "magic resistance", "fire resistance", "cold resistance", "",
	"", "", "shock resistance", "", "",
	"", "", "stun resistance", "", "",
	"drain resistance",
};

const char* get_defense_type_text(defensetype)
int defensetype;
{
	if (defensetype < 0 || defensetype >= SIZE(defense_type_names))
		return empty_string;

	return defense_type_names[defensetype];
}

static const char* artifact_invoke_names[] = { 
	"taming", "healing", "mana replenishment", "untrapping", "charging",
	"level teleportation", "portal creation", "enlightenment", "arrow creation", "death ray", "blessing of contents",
};

const char* get_artifact_invoke_name(specialpropindex)
int specialpropindex;
{
	if (specialpropindex < TAMING || specialpropindex >= TAMING + SIZE(artifact_invoke_names))
		return empty_string;

	return artifact_invoke_names[specialpropindex - TAMING];
}

/* Called when a boulder is dropped, thrown, or pushed.  If it ends up
 * in a pool, it either fills the pool up or sinks away.  In either case,
 * it's gone for good...  If the destination is not a pool, returns FALSE.
 */
boolean
boulder_hits_pool(otmp, rx, ry, pushing)
struct obj *otmp;
register int rx, ry;
boolean pushing;
{
    if (!otmp || otmp->otyp != BOULDER) {
        impossible("Not a boulder?");
    } else if (!Is_waterlevel(&u.uz) && is_pool_or_lava(rx, ry)) {
        boolean lava = is_lava(rx, ry), fills_up;
        const char *what = waterbody_name(rx, ry);
        schar ltyp = levl[rx][ry].typ;
        int chance = rn2(10); /* water: 90%; lava: 10% */
        fills_up = lava ? chance == 0 : chance != 0;

        if (fills_up) {
            struct trap *ttmp = t_at(rx, ry);

            if (ltyp == DRAWBRIDGE_UP) {
                levl[rx][ry].drawbridgemask &= ~DB_UNDER; /* clear lava */
                levl[rx][ry].drawbridgemask |= DB_FLOOR;
            } else
                levl[rx][ry].typ = ROOM, levl[rx][ry].flags = 0;

            if (ttmp)
                (void) delfloortrap(ttmp);
            bury_objs(rx, ry);

            newsym(rx, ry);
            if (pushing) {
                char whobuf[BUFSZ];

                Strcpy(whobuf, "you");
                if (u.usteed)
                    Strcpy(whobuf, y_monnam(u.usteed));
                pline("%s %s %s into the %s.", upstart(whobuf),
                      vtense(whobuf, "push"), the(xname(otmp)), what);
                if (flags.verbose && !Blind)
                    pline("Now you can cross it!");
                /* no splashing in this case */
            }
        }
        if (!fills_up || !pushing) { /* splashing occurs */
            if (!u.uinwater) {
                if (pushing ? !Blind : cansee(rx, ry)) {
                    There("is a large splash as %s %s the %s.",
                          the(xname(otmp)), fills_up ? "fills" : "falls into",
                          what);
                } else if (!Deaf)
                    You_hear("a%s splash.", lava ? " sizzling" : "");
                wake_nearto(rx, ry, 40);
            }

            if (fills_up && u.uinwater && distu(rx, ry) == 0) {
                u.uinwater = 0;
                docrt();
                vision_full_recalc = 1;
                You("find yourself on dry land again!");
            } else if (lava && distu(rx, ry) <= 2) {
                int dmg;
                You("are hit by molten %s%c",
                    hliquid("lava"), Fire_resistance ? '.' : '!');
                burn_away_slime();
                dmg = d((Fire_resistance ? 1 : 3), 6);
                losehp(Maybe_Half_Phys(dmg), /* lava damage */
                       "molten lava", KILLED_BY);
            } else if (!fills_up && flags.verbose
                       && (pushing ? !Blind : cansee(rx, ry)))
                pline("It sinks without a trace!");
        }

        /* boulder is now gone */
        if (pushing)
            delobj(otmp);
        else
            obfree(otmp, (struct obj *) 0);
        return TRUE;
    }
    return FALSE;
}

/* Used for objects which sometimes do special things when dropped; must be
 * called with the object not in any chain.  Returns TRUE if the object goes
 * away.
 */
boolean
flooreffects(obj, x, y, verb)
struct obj *obj;
int x, y;
const char *verb;
{
    struct trap *t;
    struct monst *mtmp;
    struct obj *otmp;
    boolean tseen;
    int ttyp = NO_TRAP;

    if (obj->where != OBJ_FREE)
        panic("flooreffects: obj not free");

    /* make sure things like water_damage() have no pointers to follow */
    obj->nobj = obj->nexthere = (struct obj *) 0;

    if (obj->otyp == BOULDER && boulder_hits_pool(obj, x, y, FALSE)) {
        return TRUE;
    } else if (obj->otyp == BOULDER && (t = t_at(x, y)) != 0
               && (is_pit(t->ttyp) || is_hole(t->ttyp))) {
        ttyp = t->ttyp;
        tseen = t->tseen ? TRUE : FALSE;
        if (((mtmp = m_at(x, y)) && mtmp->mtrapped)
            || (u.utrap && u.ux == x && u.uy == y)) {
            if (*verb && (cansee(x, y) || distu(x, y) == 0))
                pline("%s boulder %s into the pit%s.",
                      Blind ? "A" : "The",
                      vtense((const char *) 0, verb),
                      mtmp ? "" : " with you");
            if (mtmp) {
                if (!passes_walls(mtmp->data) && !throws_rocks(mtmp->data)) {
                    /* dieroll was rnd(20); 1: maximum chance to hit
                       since trapped target is a sitting duck */
                    int damage, dieroll = 1;

                    /* 3.6.2: this was calling hmon() unconditionally
                       so always credited/blamed the hero but the boulder
                       might have been thrown by a giant or launched by
                       a rolling boulder trap triggered by a monster or
                       dropped by a scroll of earth read by a monster */
                    if (context.mon_moving) {
                        /* normally we'd use ohitmon() but it can call
                           drop_throw() which calls flooreffects() */
						if (is_launcher(obj))
							damage = d(1, 2);
						else
							damage = weapon_total_dmg_value(obj, mtmp, &youmonst);

                        mtmp->mhp -= damage;
                        if (DEADMONSTER(mtmp)) {
                            if (canspotmon(mtmp))
                                pline("%s is %s!", Monnam(mtmp),
                                      (is_not_living(mtmp->data) || is_vampshifter(mtmp))
                                      ? "destroyed" : "killed");
                            mondied(mtmp);
                        }
                    } else {
						boolean obj_destroyed = FALSE;
                        (void) hmon(mtmp, obj, HMON_THROWN, dieroll, &obj_destroyed);
						if (obj_destroyed)
							obj = 0;
                    }
                    if (!DEADMONSTER(mtmp) && !is_whirly(mtmp->data))
                        return FALSE; /* still alive */
                }
                mtmp->mtrapped = 0;
            } else {
                if (!Passes_walls && !throws_rocks(youmonst.data)) {
                    losehp(Maybe_Half_Phys(rnd(15)),
                           "squished under a boulder", NO_KILLER_PREFIX);
                    return FALSE; /* player remains trapped */
                } else
                    reset_utrap(TRUE);
            }
        }
        if (*verb) {
            if (Blind && (x == u.ux) && (y == u.uy)) {
                You_hear("a CRASH! beneath you.");
            } else if (!Blind && cansee(x, y)) {
                pline_The("boulder %s%s.",
                          (ttyp == TRAPDOOR && !tseen)
                              ? "triggers and " : "",
                          (ttyp == TRAPDOOR)
                              ? "plugs a trap door"
                              : (ttyp == HOLE) ? "plugs a hole"
                                               : "fills a pit");
            } else {
                You_hear("a boulder %s.", verb);
            }
        }
        /*
         * Note:  trap might have gone away via ((hmon -> killed -> xkilled)
         *  || mondied) -> mondead -> m_detach -> fill_pit.
         */
        if ((t = t_at(x, y)) != 0)
            deltrap(t);
		if(obj)
	        useupf(obj, 1L);
        bury_objs(x, y);
        newsym(x, y);
        return TRUE;
    } else if (is_lava(x, y)) {
        return lava_damage(obj, x, y);
    } else if (is_pool(x, y)) {
        /* Reasonably bulky objects (arbitrary) splash when dropped.
         * If you're floating above the water even small things make
         * noise.  Stuff dropped near fountains always misses */
        if ((Blind || (Levitation || Flying)) && !Deaf
            && ((x == u.ux) && (y == u.uy))) {
            if (!Underwater) {
                if (weight(obj) > 9) {
                    pline("Splash!");
                } else if (Levitation || Flying) {
                    pline("Plop!");
                }
            }
            map_background(x, y, 0);
            newsym(x, y);
        }
        return water_damage(obj, NULL, FALSE) == ER_DESTROYED;
    } else if (u.ux == x && u.uy == y && (t = t_at(x, y)) != 0
               && uteetering_at_seen_pit(t)) {
        if (Blind && !Deaf)
            You_hear("%s tumble downwards.", the(xname(obj)));
        else
            pline("%s %s into %s pit.", The(xname(obj)),
                  otense(obj, "tumble"), the_your[t->madeby_u]);
    } else if (obj->globby) {
        /* Globby things like puddings might stick together */
        while (obj && (otmp = obj_nexto_xy(obj, x, y, TRUE)) != 0) {
            pudding_merge_message(obj, otmp);
            /* intentionally not getting the melded object; obj_meld may set
             * obj to null. */
            (void) obj_meld(&obj, &otmp);
        }
        return (boolean) !obj;
    }
    return FALSE;
}

/* obj is an object dropped on an altar */
void
doaltarobj(obj)
register struct obj *obj;
{
    if (Blind)
        return;

    if (obj->oclass != COIN_CLASS) {
        /* KMH, conduct */
        u.uconduct.gnostic++;
    } else {
        /* coins don't have bless/curse status */
        obj->blessed = obj->cursed = 0;
    }

    if (obj->blessed || obj->cursed) {
        There("is %s flash as %s %s the altar.",
              an(hcolor(obj->blessed ? NH_AMBER : NH_BLACK)), doname(obj),
              otense(obj, "hit"));
        if (!Hallucination)
            obj->bknown = 1;
    } else {
        pline("%s %s on the altar.", Doname2(obj), otense(obj, "land"));
        if (obj->oclass != COIN_CLASS)
            obj->bknown = 1;
    }
}

STATIC_OVL void
trycall(obj)
register struct obj *obj;
{
    if (!objects[obj->otyp].oc_name_known && !objects[obj->otyp].oc_uname)
        docall(obj);
}

/* Transforms the sink at the player's position into
   a fountain, throne, altar or grave. */
STATIC_DCL void
polymorph_sink()
{
    uchar sym = S_sink;
    boolean sinklooted;

    if (levl[u.ux][u.uy].typ != SINK)
        return;

    sinklooted = levl[u.ux][u.uy].looted != 0;
    level.flags.nsinks--;
    levl[u.ux][u.uy].doormask = 0; /* levl[][].flags */
    switch (rn2(4)) {
    default:
    case 0:
        sym = S_fountain;
        levl[u.ux][u.uy].typ = FOUNTAIN;
        levl[u.ux][u.uy].blessedftn = 0;
        if (sinklooted)
            SET_FOUNTAIN_LOOTED(u.ux, u.uy);
        level.flags.nfountains++;
        break;
    case 1:
        sym = S_throne;
        levl[u.ux][u.uy].typ = THRONE;
        if (sinklooted)
            levl[u.ux][u.uy].looted = T_LOOTED;
        break;
    case 2:
        sym = S_altar;
        levl[u.ux][u.uy].typ = ALTAR;
        levl[u.ux][u.uy].altarmask = Align2amask(rn2((int) A_LAWFUL + 2) - 1);
        break;
    case 3:
        sym = S_room;
        levl[u.ux][u.uy].typ = ROOM;
        make_grave(u.ux, u.uy, (char *) 0);
        if (levl[u.ux][u.uy].typ == GRAVE)
            sym = S_grave;
        break;
    }
    /* give message even if blind; we know we're not levitating,
       so can feel the outcome even if we can't directly see it */
    if (levl[u.ux][u.uy].typ != ROOM)
        pline_The("sink transforms into %s!", an(defsyms[sym].explanation));
    else
        pline_The("sink vanishes.");
    newsym(u.ux, u.uy);
}

/* Teleports the sink at the player's position;
   return True if sink teleported. */
STATIC_DCL boolean
teleport_sink()
{
    int cx, cy;
    int cnt = 0;
    struct trap *trp;
    struct engr *eng;

    do {
        cx = rnd(COLNO - 1);
        cy = rn2(ROWNO);
        trp = t_at(cx, cy);
        eng = engr_at(cx, cy);
    } while ((levl[cx][cy].typ != ROOM || trp || eng || cansee(cx, cy))
             && cnt++ < 200);

    if (levl[cx][cy].typ == ROOM && !trp && !eng) {
        /* create sink at new position */
        levl[cx][cy].typ = SINK;
        levl[cx][cy].looted = levl[u.ux][u.uy].looted;
        newsym(cx, cy);
        /* remove old sink */
        levl[u.ux][u.uy].typ = ROOM;
        levl[u.ux][u.uy].looted = 0;
        newsym(u.ux, u.uy);
        return TRUE;
    }
    return FALSE;
}

/* obj is a ring being dropped over a kitchen sink */
STATIC_OVL void
dosinkring(obj)
register struct obj *obj;
{
    struct obj *otmp, *otmp2;
    boolean ideed = TRUE;
    boolean nosink = FALSE;

    You("drop %s down the drain.", doname(obj));
    obj->in_use = TRUE;  /* block free identification via interrupt */
    switch (obj->otyp) { /* effects that can be noticed without eyes */
    case RIN_SEARCHING:
        You("thought %s got lost in the sink, but there it is!", yname(obj));
        goto giveback;
    case RIN_SLOW_DIGESTION:
        pline_The("ring is regurgitated!");
 giveback:
        obj->in_use = FALSE;
        dropx(obj);
        trycall(obj);
        return;
    case RIN_LEVITATION:
        pline_The("sink quivers upward for a moment.");
        break;
    case RIN_POISON_RESISTANCE:
        You("smell rotten %s.", makeplural(fruitname(FALSE)));
        break;
    case RIN_AGGRAVATE_MONSTER:
        pline("Several %s buzz angrily around the sink.",
              Hallucination ? makeplural(rndmonnam(NULL)) : "flies");
        break;
    case RIN_SHOCK_RESISTANCE:
        pline("Static electricity surrounds the sink.");
        break;
	case RIN_LIFE_PROTECTION:
		pline("A pillar of smoke arises from the sink.");
		break;
	case RIN_CONFLICT:
        You_hear("loud noises coming from the drain.");
        break;
    case RIN_SUSTAIN_ABILITY: /* KMH */
        pline_The("%s flow seems fixed.", hliquid("water"));
        break;
    case RIN_GAIN_STRENGTH:
        pline_The("%s flow seems %ser now.",
                  hliquid("water"),
                  (obj->spe < 0) ? "weak" : "strong");
        break;
	case RIN_GAIN_DEXTERITY:
		pline_The("%s flow seems %ser now.",
			hliquid("water"),
			(obj->spe < 0) ? "stiff" : "nimbl");
		break;
	case RIN_GAIN_CONSTITUTION:
        pline_The("%s flow seems %ser now.",
                  hliquid("water"),
                  (obj->spe < 0) ? "less" : "great");
        break;
	case RIN_GAIN_INTELLIGENCE:
		pline_The("%s flow seems %ser now.",
			hliquid("water"),
			(obj->spe < 0) ? "coars" : "fin");
		break;
	case RIN_GAIN_WISDOM:
		pline_The("%s flow seems %s now.",
			hliquid("water"),
			(obj->spe < 0) ? "less sensible" : "more sensible");
		break;
	case RIN_POWER:
		pline_The("%s flow seems %s now.",
			hliquid("water"),
			(obj->spe < 0) ? "much worse" : "much better");
		break;
	case RIN_INCREASE_ACCURACY: /* KMH */
        pline_The("%s flow %s the drain.",
                  hliquid("water"),
                  (obj->spe < 0) ? "misses" : "hits");
        break;
    case RIN_INCREASE_DAMAGE:
        pline_The("water's force seems %ser now.",
                  (obj->spe < 0) ? "small" : "great");
        break;
    case RIN_HUNGER:
        ideed = FALSE;
        for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp2) {
            otmp2 = otmp->nexthere;
            if (otmp != uball && otmp != uchain
                && !obj_resists(otmp, 1, 99)) {
                if (!Blind) {
                    pline("Suddenly, %s %s from the sink!", doname(otmp),
                          otense(otmp, "vanish"));
                    ideed = TRUE;
                }
                delobj(otmp);
            }
        }
        break;
    case MEAT_RING:
        /* Not the same as aggravate monster; besides, it's obvious. */
        pline("Several flies buzz around the sink.");
        break;
    case RIN_TELEPORTATION:
        nosink = teleport_sink();
        /* give message even if blind; we know we're not levitating,
           so can feel the outcome even if we can't directly see it */
        pline_The("sink %svanishes.", nosink ? "" : "momentarily ");
        ideed = FALSE;
        break;
    case RIN_POLYMORPH:
        polymorph_sink();
        nosink = TRUE;
        /* for S_room case, same message as for teleportation is given */
        ideed = (levl[u.ux][u.uy].typ != ROOM);
        break;
    default:
        ideed = FALSE;
        break;
    }
    if (!Blind && !ideed) {
        ideed = TRUE;
        switch (obj->otyp) { /* effects that need eyes */
        case RIN_ADORNMENT:
            pline_The("faucets flash brightly for a moment.");
            break;
        case RIN_REGENERATION:
            pline_The("sink looks as good as new.");
            break;
		case RIN_REPLENISHMENT:
			pline_The("sink shines with a blue aura for a while.");
			break;
		case RIN_INVISIBILITY:
            You("don't see anything happen to the sink.");
            break;
        case RIN_FREE_ACTION:
            You_see("the ring slide right down the drain!");
            break;
        case RIN_SEE_INVISIBLE:
            You_see("some %s in the sink.",
                    Hallucination ? "oxygen molecules" : "air");
            break;
        case RIN_STEALTH:
            pline_The("sink seems to blend into the floor for a moment.");
            break;
        case RIN_FIRE_RESISTANCE:
            pline_The("hot %s faucet flashes brightly for a moment.",
                      hliquid("water"));
            break;
        case RIN_COLD_RESISTANCE:
            pline_The("cold %s faucet flashes brightly for a moment.",
                      hliquid("water"));
            break;
        case RIN_PROTECTION_FROM_SHAPE_CHANGERS:
            pline_The("sink looks nothing like a fountain.");
            break;
        case RIN_PROTECTION:
            pline_The("sink glows %s for a moment.",
                      hcolor((obj->spe < 0) ? NH_BLACK : NH_SILVER));
            break;
        case RIN_WARNING:
            pline_The("sink glows %s for a moment.", hcolor(NH_WHITE));
            break;
        case RIN_TELEPORT_CONTROL:
            pline_The("sink looks like it is being beamed aboard somewhere.");
            break;
        case RIN_POLYMORPH_CONTROL:
            pline_The(
                  "sink momentarily looks like a regularly erupting geyser.");
            break;
        default:
            break;
        }
    }
    if (ideed)
        trycall(obj);
    else if (!nosink)
        You_hear("the ring bouncing down the drainpipe.");

    if (!rn2(20) && !nosink) {
        pline_The("sink backs up, leaving %s.", doname(obj));
        obj->in_use = FALSE;
        dropx(obj);
    } else if (!rn2(5)) {
        freeinv(obj);
        obj->in_use = FALSE;
        obj->ox = u.ux;
        obj->oy = u.uy;
        add_to_buried(obj);
    } else
        useup(obj);
}

/* some common tests when trying to drop or throw items */
boolean
canletgo(obj, word)
struct obj *obj;
const char *word;
{
	if (obj->owornmask & ((W_ARMOR & ~W_ARMS) | W_ACCESSORY)) {
		if (*word)
			Norep("You cannot %s %s you are wearing.", word, something);
		return FALSE;
	}
	if (objects[obj->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED && obj->cursed) {
        /* getobj() kludge sets corpsenm to user's specified count
           when refusing to split a stack of cursed loadstones */
        if (*word) {
            /* getobj() ignores a count for throwing since that is
               implicitly forced to be 1; replicate its kludge... */
            if (!strcmp(word, "throw") && obj->quan > 1L)
                obj->corpsenm = 1;
            pline("For some reason, you cannot %s%s %s%s!", word,
                  obj->corpsenm ? " any of" : "", is_graystone(obj) ? "the stone" : "the item", plur(obj->quan));
        }
        obj->corpsenm = 0; /* reset */
        obj->bknown = 1;
        return FALSE;
    }
    if (obj->otyp == LEASH && obj->leashmon != 0) {
        if (*word)
            pline_The("leash is tied around your %s.", body_part(HAND));
        return FALSE;
    }
    if (obj->owornmask & W_SADDLE) {
        if (*word)
            You("cannot %s %s you are sitting on.", word, something);
        return FALSE;
    }
    return TRUE;
}

STATIC_PTR int
drop(obj)
register struct obj *obj;
{
    if (!obj)
        return 0;
    if (!canletgo(obj, "drop"))
        return 0;
    if (obj == uwep) {
        if (welded(uwep, &youmonst)) {
            weldmsg(obj);
            return 0;
        }
        setuwep((struct obj *) 0, W_WEP);
    }
	if (obj == uarms) {
		if (welded(uarms, &youmonst)) {
			weldmsg(obj);
			return 0;
		}
		setuwep((struct obj*) 0, obj->owornmask);
	}
	if (obj == uquiver) {
        setuqwep((struct obj *) 0);
    }
    if (obj == uswapwep) {
        setuswapwep((struct obj *) 0, W_SWAPWEP);
    }
	if (obj == uswapwep2) {
		setuswapwep((struct obj*) 0, W_SWAPWEP2);
	}

    if (u.uswallow) {
        /* barrier between you and the floor */
        if (flags.verbose) {
            char *onam_p, monbuf[BUFSZ];

            /* doname can call s_suffix, reusing its buffer */
            Strcpy(monbuf, s_suffix(mon_nam(u.ustuck)));
            onam_p = is_unpaid(obj) ? yobjnam(obj, (char *) 0) : doname(obj);
            You("drop %s into %s %s.", onam_p, monbuf,
                mbodypart(u.ustuck, STOMACH));
        }
    } else {
        if ((obj->oclass == RING_CLASS || obj->otyp == MEAT_RING)
            && IS_SINK(levl[u.ux][u.uy].typ)) {
            dosinkring(obj);
            return 1;
        }
        if (!can_reach_floor(TRUE)) {
            /* we might be levitating due to #invoke Heart of Ahriman;
               if so, levitation would end during call to freeinv()
               and we want hitfloor() to happen before float_down() */
            //boolean levhack = finesse_ahriman(obj);

            //if (levhack)
             //   ELevitation = W_ARTIFACT_CARRIED; /* other than W_ARTIFACT_INVOKED */
            if (flags.verbose)
                You("drop %s.", doname(obj));
            /* Ensure update when we drop gold objects */
            if (obj->oclass == COIN_CLASS)
                context.botl = 1;
            freeinv(obj);
            hitfloor(obj, TRUE);
            //if (levhack)
            //    float_down(I_SPECIAL | TIMEOUT, W_ARTIFACT_INVOKED | W_ARTIFACT_CARRIED);
            return 1;
        }
        if (!IS_ALTAR(levl[u.ux][u.uy].typ) && flags.verbose)
            You("drop %s.", doname(obj));
    }
    dropx(obj);
    return 1;
}

/* dropx - take dropped item out of inventory;
   called in several places - may produce output
   (eg ship_object() and dropy() -> sellobj() both produce output) */
void
dropx(obj)
register struct obj *obj;
{
    /* Ensure update when we drop gold objects */
    if (obj->oclass == COIN_CLASS)
        context.botl = 1;
    freeinv(obj);
    if (!u.uswallow) {
        if (ship_object(obj, u.ux, u.uy, FALSE))
            return;
        if (IS_ALTAR(levl[u.ux][u.uy].typ))
            doaltarobj(obj); /* set bknown */
    }
    dropy(obj);
}

/* dropy - put dropped object at destination; called from lots of places */
void
dropy(obj)
struct obj *obj;
{
    dropz(obj, FALSE);
}

/* dropz - really put dropped object at its destination... */
void
dropz(obj, with_impact)
struct obj *obj;
boolean with_impact;
{
    if (obj == uwep)
        setuwep((struct obj *) 0, W_WEP);
	if (obj == uarms)
		setuwep((struct obj*) 0, uarms->owornmask);
	if (obj == uquiver)
        setuqwep((struct obj *) 0);
    if (obj == uswapwep)
        setuswapwep((struct obj *) 0, W_SWAPWEP);
	if (obj == uswapwep2)
		setuswapwep((struct obj*) 0, W_SWAPWEP2);

    if (!u.uswallow && flooreffects(obj, u.ux, u.uy, "drop"))
        return;
    /* uswallow check done by GAN 01/29/87 */
    if (u.uswallow) {
        boolean could_petrify = FALSE;
        boolean could_poly = FALSE;
        boolean could_slime = FALSE;
        boolean could_grow = FALSE;
        boolean could_heal = FALSE;

        if (obj != uball) { /* mon doesn't pick up ball */
            if (obj->otyp == CORPSE) {
                could_petrify = touch_petrifies(&mons[obj->corpsenm]);
                could_poly = polyfodder(obj);
                could_slime = (obj->corpsenm == PM_GREEN_SLIME);
                could_grow = (obj->corpsenm == PM_WRAITH || obj->corpsenm == PM_SPECTRE || obj->corpsenm == PM_KING_WRAITH);
                could_heal = (obj->corpsenm == PM_NURSE);
            }
            if (is_unpaid(obj))
                (void) stolen_value(obj, u.ux, u.uy, TRUE, FALSE);
            (void) mpickobj(u.ustuck, obj);
            if (is_animal(u.ustuck->data)) {
                if (could_poly || could_slime) {
                    (void) newcham(u.ustuck,
                                   could_poly ? (struct permonst *) 0
                                              : &mons[PM_GREEN_SLIME],
                                   FALSE, could_slime);
                    delobj(obj); /* corpse is digested */
                } else if (could_petrify) {
					int existing_stoning = get_mon_property(u.ustuck, STONED);
					set_mon_property_verbosely(u.ustuck, STONED, max(1, min(existing_stoning - 1, 5)));
					//minstapetrify(u.ustuck, TRUE);
                    /* Don't leave a cockatrice corpse in a statue */
                    if (!u.uswallow)
                        delobj(obj);
                } else if (could_grow) {
                    (void) grow_up(u.ustuck, (struct monst *) 0);
                    delobj(obj); /* corpse is digested */
                } else if (could_heal) {
                    u.ustuck->mhp = u.ustuck->mhpmax;
                    delobj(obj); /* corpse is digested */
                }
            }
        }
    } else {
        place_object(obj, u.ux, u.uy);
        if (with_impact)
            container_impact_dmg(obj, u.ux, u.uy);
        if (obj == uball)
            drop_ball(u.ux, u.uy);
        else if (level.flags.has_shop)
            sellobj(obj, u.ux, u.uy);
        stackobj(obj);
        if (Blind && Levitation)
            map_object(obj, 0);
        newsym(u.ux, u.uy); /* remap location under self */
    }
}

/* things that must change when not held; recurse into containers.
   Called for both player and monsters */
void
obj_no_longer_held(obj)
struct obj *obj;
{
    if (!obj) {
        return;
    } else if (Has_contents(obj)) {
        struct obj *contents;

        for (contents = obj->cobj; contents; contents = contents->nobj)
            obj_no_longer_held(contents);
    }
    switch (obj->otyp) {
    case CRYSKNIFE:
        /* Normal crysknife reverts to worm tooth when not held by hero
         * or monster; fixed crysknife has only 10% chance of reverting.
         * When a stack of the latter is involved, it could be worthwhile
         * to give each individual crysknife its own separate 10% chance,
         * but we aren't in any position to handle stack splitting here.
         */
        if (!obj->oerodeproof || !rn2(10)) {
            /* if monsters aren't moving, assume player is responsible */
            if (!context.mon_moving && !program_state.gameover)
                costly_alteration(obj, COST_DEGRD);
            obj->otyp = WORM_TOOTH;
            obj->oerodeproof = 0;
        }
        break;
    }
}

/* 'D' command: drop several things */
int
doddrop()
{
    int result = 0;

    if (!invent) {
        You("have nothing to drop.");
        return 0;
    }
    add_valid_menu_class(0); /* clear any classes already there */
    if (*u.ushops)
        sellobj_state(SELL_DELIBERATE);
    if (flags.menu_style != MENU_TRADITIONAL
        || (result = ggetobj("drop", drop, 0, FALSE, (unsigned *) 0, 3)) < -1)
        result = menu_drop(result);
    if (*u.ushops)
        sellobj_state(SELL_NORMAL);
    if (result)
        reset_occupations();

    return result;
}

/* Drop things from the hero's inventory, using a menu. */
STATIC_OVL int
menu_drop(retry)
int retry;
{
    int n, i, n_dropped = 0;
    long cnt;
    struct obj *otmp, *otmp2;
    menu_item *pick_list;
    boolean all_categories = TRUE;
    boolean drop_everything = FALSE;

    if (retry) {
        all_categories = (retry == -2);
    } else if (flags.menu_style == MENU_FULL) {
        all_categories = FALSE;
        n = query_category("Drop what type of items?", invent,
                           UNPAID_TYPES | ALL_TYPES | CHOOSE_ALL | BUC_BLESSED
                               | BUC_CURSED | BUC_UNCURSED | BUC_UNKNOWN,
                           &pick_list, PICK_ANY);
        if (!n)
            goto drop_done;
        for (i = 0; i < n; i++) {
            if (pick_list[i].item.a_int == ALL_TYPES_SELECTED)
                all_categories = TRUE;
            else if (pick_list[i].item.a_int == 'A')
                drop_everything = TRUE;
            else
                add_valid_menu_class(pick_list[i].item.a_int);
        }
        free((genericptr_t) pick_list);
    } else if (flags.menu_style == MENU_COMBINATION) {
        unsigned ggoresults = 0;

        all_categories = FALSE;
        /* Gather valid classes via traditional GnollHack method */
        i = ggetobj("drop", drop, 0, TRUE, &ggoresults, 3);
        if (i == -2)
            all_categories = TRUE;
        if (ggoresults & ALL_FINISHED) {
            n_dropped = i;
            goto drop_done;
        }
    }

    if (drop_everything) {
        /*
         * Dropping a burning potion of oil while levitating can cause
         * an explosion which might destroy some of hero's inventory,
         * so the old code
         *      for (otmp = invent; otmp; otmp = otmp2) {
         *          otmp2 = otmp->nobj;
         *          n_dropped += drop(otmp);
         *      }
         * was unreliable and could lead to an "object lost" panic.
         *
         * Use the bypass bit to mark items already processed (hence
         * not droppable) and rescan inventory until no unbypassed
         * items remain.
         */
        bypass_objlist(invent, FALSE); /* clear bypass bit for invent */
        while ((otmp = nxt_unbypassed_obj(invent)) != 0)
            n_dropped += drop(otmp);
        /* we might not have dropped everything (worn armor, welded weapon,
           cursed loadstones), so reset any remaining inventory to normal */
        bypass_objlist(invent, FALSE);
    } else {
        /* should coordinate with perm invent, maybe not show worn items */
        n = query_objlist("What would you like to drop?", &invent,
                          (USE_INVLET | INVORDER_SORT), &pick_list, PICK_ANY,
                          all_categories ? allow_all : allow_category, 3);
        if (n > 0) {
            /*
             * picklist[] contains a set of pointers into inventory, but
             * as soon as something gets dropped, they might become stale
             * (see the drop_everything code above for an explanation).
             * Just checking to see whether one is still in the invent
             * chain is not sufficient validation since destroyed items
             * will be freed and items we've split here might have already
             * reused that memory and put the same pointer value back into
             * invent.  Ditto for using invlet to validate.  So we start
             * by setting bypass on all of invent, then check each pointer
             * to verify that it is in invent and has that bit set.
             */
            bypass_objlist(invent, TRUE);
            for (i = 0; i < n; i++) {
                otmp = pick_list[i].item.a_obj;
                for (otmp2 = invent; otmp2; otmp2 = otmp2->nobj)
                    if (otmp2 == otmp)
                        break;
                if (!otmp2 || !otmp2->bypass)
                    continue;
                /* found next selected invent item */
                cnt = pick_list[i].count;
                if (cnt < otmp->quan) {
                    if (welded(otmp, &youmonst)) {
                        ; /* don't split */
                    } else if ((objects[otmp->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && otmp->cursed) {
                        /* same kludge as getobj(), for canletgo()'s use */
                        otmp->corpsenm = (int) cnt; /* don't split */
                    } else {
                        otmp = splitobj(otmp, cnt);
                    }
                }
                n_dropped += drop(otmp);
            }
            bypass_objlist(invent, FALSE); /* reset invent to normal */
            free((genericptr_t) pick_list);
        }
    }

 drop_done:
    return n_dropped;
}

/* on a ladder, used in goto_level */
static NEARDATA boolean at_ladder = FALSE;

/* the '>' command */
int
dodown()
{
    struct trap *trap = 0;
    boolean stairs_down = ((u.ux == xdnstair && u.uy == ydnstair)
                           || (u.ux == sstairs.sx && u.uy == sstairs.sy
                               && !sstairs.up)),
            ladder_down = (u.ux == xdnladder && u.uy == ydnladder);

    if (u_rooted())
        return 1;

    if (stucksteed(TRUE)) {
        return 0;
    }
    /* Levitation might be blocked, but player can still use '>' to
       turn off controlled levitation */
    if (HLevitation || ELevitation) {
        if ((HLevitation & I_SPECIAL) || (ELevitation & W_ARTIFACT_INVOKED)) {
            /* end controlled levitation */
            if (ELevitation & W_ARTIFACT_INVOKED) {
                struct obj *obj;

                for (obj = invent; obj; obj = obj->nobj) {
                    if (obj->oartifact
                        && artifact_has_invprop(obj, LEVITATION)) {
                        if (obj->age < monstermoves)
                            obj->age = monstermoves;
                        obj->age += rnz(100);
                    }
                }
            }
            if (float_down(I_SPECIAL | TIMEOUT, W_ARTIFACT_INVOKED)) {
                return 1; /* came down, so moved */
            } else if (!HLevitation && !ELevitation) {
                Your("latent levitation ceases.");
                return 1; /* did something, effectively moved */
            }
        }
        if (BLevitation) {
            ; /* weren't actually floating after all */
        } else if (Blind) {
            /* Avoid alerting player to an unknown stair or ladder.
             * Changes the message for a covered, known staircase
             * too; staircase knowledge is not stored anywhere.
             */
            if (stairs_down)
                stairs_down =
                    (glyph_to_cmap(levl[u.ux][u.uy].glyph) == S_dnstair);
            else if (ladder_down)
                ladder_down =
                    (glyph_to_cmap(levl[u.ux][u.uy].glyph) == S_dnladder);
        }
        if (Is_airlevel(&u.uz))
            You("are floating in the %s.", surface(u.ux, u.uy));
        else if (Is_waterlevel(&u.uz))
            You("are floating in %s.",
                is_pool(u.ux, u.uy) ? "the water" : "a bubble of air");
        else
            floating_above(stairs_down ? "stairs" : ladder_down
                                                    ? "ladder"
                                                    : surface(u.ux, u.uy));
        return 0; /* didn't move */
    }
    if (!stairs_down && !ladder_down) {
        trap = t_at(u.ux, u.uy);
        if (trap && uteetering_at_seen_pit(trap)) {
            dotrap(trap, TOOKPLUNGE);
            return 1;
        } else if (!trap || !is_hole(trap->ttyp)
                   || !Can_fall_thru(&u.uz) || !trap->tseen) {
            if (flags.autodig && !context.nopick && uwep && is_pick(uwep)) {
                return use_pick_axe2(uwep);
            } else {
                You_cant("go down here.");
                return 0;
            }
        }
    }
    if (u.ustuck) {
        You("are %s, and cannot go down.",
            !u.uswallow ? "being held" : is_animal(u.ustuck->data)
                                             ? "swallowed"
                                             : "engulfed");
        return 1;
    }
    if (on_level(&valley_level, &u.uz) && !u.uevent.gehennom_entered) {
        You("are standing at the gate to Gehennom.");
        pline("Unspeakable cruelty and harm lurk down there.");
        if (yn("Are you sure you want to enter?") != 'y')
            return 0;
        else
            pline("So be it.");
        u.uevent.gehennom_entered = 1; /* don't ask again */
    }

    if (!next_to_u()) {
        You("are held back by your pet!");
        return 0;
    }

    if (trap) {
        const char *down_or_thru = trap->ttyp == HOLE ? "down" : "through";
        const char *actn = Flying ? "fly" : locomotion(youmonst.data, "jump");

        if (youmonst.data->msize >= MZ_HUGE) {
            char qbuf[QBUFSZ];

            You("don't fit %s easily.", down_or_thru);
            Sprintf(qbuf, "Try to squeeze %s?", down_or_thru);
            if (yn(qbuf) == 'y') {
                if (!rn2(3)) {
                    actn = "manage to squeeze";
                    losehp(Maybe_Half_Phys(rnd(4)),
                           "contusion from a small passage", KILLED_BY);
                } else {
                    You("were unable to fit %s.", down_or_thru);
                    return 0;
                }
            } else {
                return 0;
            }
        }
        You("%s %s the %s.", actn, down_or_thru,
            trap->ttyp == HOLE ? "hole" : "trap door");
    }
    if (trap && Is_stronghold(&u.uz)) {
        goto_hell(FALSE, TRUE);
    } else {
        at_ladder = (boolean) (levl[u.ux][u.uy].typ == LADDER);
        next_level(!trap);
        at_ladder = FALSE;
    }
    return 1;
}

/* the '<' command */
int
doup()
{
    if (u_rooted())
        return 1;

    /* "up" to get out of a pit... */
    if (u.utrap && u.utraptype == TT_PIT) {
        climb_pit();
        return 1;
    }

    if ((u.ux != xupstair || u.uy != yupstair)
        && (!xupladder || u.ux != xupladder || u.uy != yupladder)
        && (!sstairs.sx || u.ux != sstairs.sx || u.uy != sstairs.sy
            || !sstairs.up)) {
        You_cant("go up here.");
        return 0;
    }
    if (stucksteed(TRUE)) {
        return 0;
    }
    if (u.ustuck) {
        You("are %s, and cannot go up.",
            !u.uswallow ? "being held" : is_animal(u.ustuck->data)
                                             ? "swallowed"
                                             : "engulfed");
        return 1;
    }
    if (near_capacity() > SLT_ENCUMBER) {
        /* No levitation check; inv_weight() already allows for it */
        Your("load is too heavy to climb the %s.",
             levl[u.ux][u.uy].typ == STAIRS ? "stairs" : "ladder");
        return 1;
    }
    if (ledger_no(&u.uz) == 1) {
        if (iflags.debug_fuzzer)
            return 0;
        if (yn("Beware, there will be no return!  Still climb?") != 'y')
            return 0;
    }
    if (!next_to_u()) {
        You("are held back by your pet!");
        return 0;
    }
    at_ladder = (boolean) (levl[u.ux][u.uy].typ == LADDER);
    prev_level(TRUE);
    at_ladder = FALSE;
    return 1;
}

d_level save_dlevel = { 0, 0 };

/* check that we can write out the current level */
STATIC_OVL int
currentlevel_rewrite()
{
    register int fd;
    char whynot[BUFSZ];

    /* since level change might be a bit slow, flush any buffered screen
     *  output (like "you fall through a trap door") */
    mark_synch();

    fd = create_levelfile(ledger_no(&u.uz), whynot);
    if (fd < 0) {
        /*
         * This is not quite impossible: e.g., we may have
         * exceeded our quota. If that is the case then we
         * cannot leave this level, and cannot save either.
         * Another possibility is that the directory was not
         * writable.
         */
        pline1(whynot);
        return -1;
    }

#ifdef MFLOPPY
    if (!savelev(fd, ledger_no(&u.uz), COUNT_SAVE)) {
        (void) nhclose(fd);
        delete_levelfile(ledger_no(&u.uz));
        pline("GnollHack is out of disk space for making levels!");
        You("can save, quit, or continue playing.");
        return -1;
    }
#endif
    return fd;
}

#ifdef INSURANCE
void
save_currentstate()
{
    int fd;

    if (flags.ins_chkpt) {
        /* write out just-attained level, with pets and everything */
        fd = currentlevel_rewrite();
        if (fd < 0)
            return;
        bufon(fd);
        savelev(fd, ledger_no(&u.uz), WRITE_SAVE);
        bclose(fd);
    }

    /* write out non-level state */
    savestateinlock();
}
#endif

/*
static boolean
badspot(x, y)
register xchar x, y;
{
    return (boolean) ((levl[x][y].typ != ROOM
                       && levl[x][y].typ != AIR
                       && levl[x][y].typ != CORR)
                      || MON_AT(x, y));
}
*/

/* when arriving on a level, if hero and a monster are trying to share same
   spot, move one; extracted from goto_level(); also used by wiz_makemap() */
void
u_collide_m(mtmp)
struct monst *mtmp;
{
    coord cc;

    if (!mtmp || mtmp == u.usteed || mtmp != m_at(u.ux, u.uy)) {
        impossible("level arrival collision: %s?",
                   !mtmp ? "no monster"
                     : (mtmp == u.usteed) ? "steed is on map"
                       : "monster not co-located");
        return;
    }

    /* There's a monster at your target destination; it might be one
       which accompanied you--see mon_arrive(dogmove.c)--or perhaps
       it was already here.  Randomly move you to an adjacent spot
       or else the monster to any nearby location.  Prior to 3.3.0
       the latter was done unconditionally. */
    if (!rn2(2) && enexto(&cc, u.ux, u.uy, youmonst.data)
        && distu(cc.x, cc.y) <= 2)
        u_on_newpos(cc.x, cc.y); /*[maybe give message here?]*/
    else
        mnexto(mtmp);

    if ((mtmp = m_at(u.ux, u.uy)) != 0) {
        /* there was an unconditional impossible("mnexto failed")
           here, but it's not impossible and we're prepared to cope
           with the situation, so only say something when debugging */
        if (wizard)
            pline("(monster in hero's way)");
        if (!rloc(mtmp, TRUE) || (mtmp = m_at(u.ux, u.uy)) != 0)
            /* no room to move it; send it away, to return later */
            m_into_limbo(mtmp);
    }
}

void
goto_level(newlevel, at_stairs, falling, portal)
d_level *newlevel;
boolean at_stairs, falling, portal;
{
    int fd, l_idx;
    xchar new_ledger;
    boolean cant_go_back, great_effort,
            up = (depth(newlevel) < depth(&u.uz)),
            newdungeon = (u.uz.dnum != newlevel->dnum),
            was_in_W_tower = In_W_tower(u.ux, u.uy, &u.uz),
            familiar = FALSE,
            new = FALSE; /* made a new level? */
    struct monst *mtmp;
    char whynot[BUFSZ];
    char *annotation;

    if (dunlev(newlevel) > dunlevs_in_dungeon(newlevel))
        newlevel->dlevel = dunlevs_in_dungeon(newlevel);
    if (newdungeon && In_endgame(newlevel)) { /* 1st Endgame Level !!! */
        if (!u.uhave.amulet)
            return;  /* must have the Amulet */
        if (!wizard) /* wizard ^V can bypass Earth level */
            assign_level(newlevel, &earth_level); /* (redundant) */
    }
    new_ledger = ledger_no(newlevel);
    if (new_ledger <= 0)
        done(ESCAPED); /* in fact < 0 is impossible */

    /* If you have the amulet and are trying to get out of Gehennom,
     * going up a set of stairs sometimes does some very strange things!
     * Biased against law and towards chaos.  (The chance to be sent
     * down multiple levels when attempting to go up are significantly
     * less than the corresponding comment in older versions indicated
     * due to overlooking the effect of the call to assign_rnd_lvl().)
     *
     * Odds for making it to the next level up, or of being sent down:
     *  "up"    L      N      C
     *   +1   75.0   75.0   75.0
     *    0    6.25   8.33  12.5
     *   -1   11.46  12.50  12.5
     *   -2    5.21   4.17   0.0
     *   -3    2.08   0.0    0.0
     */
    if (Inhell && up && u.uhave.amulet && !newdungeon && !portal
        && (dunlev(&u.uz) < dunlevs_in_dungeon(&u.uz) - 3)) {
        if (!rn2(4)) {
            int odds = 3 + (int) u.ualign.type,   /* 2..4 */
                diff = odds <= 1 ? 0 : rn2(odds); /* paranoia */

            if (diff != 0) {
                assign_rnd_level(newlevel, &u.uz, diff);
                /* if inside the tower, stay inside */
                if (was_in_W_tower && !On_W_tower_level(newlevel))
                    diff = 0;
            }
            if (diff == 0)
                assign_level(newlevel, &u.uz);

            new_ledger = ledger_no(newlevel);

            pline("A mysterious force momentarily surrounds you...");
            if (on_level(newlevel, &u.uz)) {
                (void) safe_teleds(FALSE);
                (void) next_to_u();
                return;
            } else
                at_stairs = at_ladder = FALSE;
        }
    }

    /* Prevent the player from going past the first quest level unless
     * (s)he has been given the go-ahead by the leader.
     */
    if (on_level(&u.uz, &qstart_level) && !newdungeon && !ok_to_quest()) {
        pline("A mysterious force prevents you from descending.");
        return;
    }

    if (on_level(newlevel, &u.uz))
        return; /* this can happen */

    /* tethered movement makes level change while trapped feasible */
    if (u.utrap && u.utraptype == TT_BURIEDBALL)
        buried_ball_to_punishment(); /* (before we save/leave old level) */

    fd = currentlevel_rewrite();
    if (fd < 0)
        return;

    /* discard context which applies to the level we're leaving;
       for lock-picking, container may be carried, in which case we
       keep context; if on the floor, it's about to be saved+freed and
       maybe_reset_pick() needs to do its carried() check before that */
    maybe_reset_pick((struct obj *) 0);
    reset_trapset(); /* even if to-be-armed trap obj is accompanying hero */
    iflags.travelcc.x = iflags.travelcc.y = 0; /* travel destination cache */
    context.polearm.hitmon = (struct monst *) 0; /* polearm target */
    /* digging context is level-aware and can actually be resumed if
       hero returns to the previous level without any intervening dig */

    if (falling) /* assuming this is only trap door or hole */
        impact_drop((struct obj *) 0, u.ux, u.uy, newlevel->dlevel);

    check_special_room(TRUE); /* probably was a trap door */
    if (Punished)
        unplacebc();
    reset_utrap(FALSE); /* needed in level_tele */
    fill_pit(u.ux, u.uy);
    u.ustuck = 0; /* idem */
    u.uinwater = 0;
    u.uundetected = 0; /* not hidden, even if means are available */
    keepdogs(FALSE);
    if (u.uswallow) /* idem */
        u.uswldtim = u.uswallow = 0;
    recalc_mapseen(); /* recalculate map overview before we leave the level */
    /*
     *  We no longer see anything on the level.  Make sure that this
     *  follows u.uswallow set to null since uswallow overrides all
     *  normal vision.
     */
    vision_recalc(2);

    /*
     * Save the level we're leaving.  If we're entering the endgame,
     * we can get rid of all existing levels because they cannot be
     * reached any more.  We still need to use savelev()'s cleanup
     * for the level being left, to recover dynamic memory in use and
     * to avoid dangling timers and light sources.
     */
    cant_go_back = (newdungeon && In_endgame(newlevel));
    if (!cant_go_back) {
        update_mlstmv(); /* current monsters are becoming inactive */
        bufon(fd);       /* use buffered output */
    }
    savelev(fd, ledger_no(&u.uz),
            cant_go_back ? FREE_SAVE : (WRITE_SAVE | FREE_SAVE));
    bclose(fd);
    if (cant_go_back) {
        /* discard unreachable levels; keep #0 */
        for (l_idx = maxledgerno(); l_idx > 0; --l_idx)
            delete_levelfile(l_idx);
        /* mark #overview data for all dungeon branches as uninteresting */
        for (l_idx = 0; l_idx < n_dgns; ++l_idx)
            remdun_mapseen(l_idx);
    }

    if (Is_rogue_level(newlevel) || Is_rogue_level(&u.uz))
        assign_graphics(Is_rogue_level(newlevel) ? ROGUESET : PRIMARY);
#ifdef USE_TILES
    substitute_tiles(newlevel);
#endif
    check_gold_symbol();
    /* record this level transition as a potential seen branch unless using
     * some non-standard means of transportation (level teleport).
     */
    if ((at_stairs || falling || portal) && (u.uz.dnum != newlevel->dnum))
        recbranch_mapseen(&u.uz, newlevel);
    assign_level(&u.uz0, &u.uz);
    assign_level(&u.uz, newlevel);
    assign_level(&u.utolev, newlevel);
    u.utotype = 0;
    if (!builds_up(&u.uz)) { /* usual case */
        if (dunlev(&u.uz) > dunlev_reached(&u.uz))
            dunlev_reached(&u.uz) = dunlev(&u.uz);
    } else {
        if (dunlev_reached(&u.uz) == 0
            || dunlev(&u.uz) < dunlev_reached(&u.uz))
            dunlev_reached(&u.uz) = dunlev(&u.uz);
    }
    reset_rndmonst(NON_PM); /* u.uz change affects monster generation */

    /* set default level change destination areas */
    /* the special level code may override these */
    (void) memset((genericptr_t) &updest, 0, sizeof updest);
    (void) memset((genericptr_t) &dndest, 0, sizeof dndest);

    if (!(level_info[new_ledger].flags & LFILE_EXISTS)) {
        /* entering this level for first time; make it now */
        if (level_info[new_ledger].flags & (FORGOTTEN | VISITED)) {
            impossible("goto_level: returning to discarded level?");
            level_info[new_ledger].flags &= ~(FORGOTTEN | VISITED);
        }
        mklev();
        new = TRUE; /* made the level */
    } else {
        /* returning to previously visited level; reload it */
        fd = open_levelfile(new_ledger, whynot);
        if (tricked_fileremoved(fd, whynot)) {
            /* we'll reach here if running in wizard mode */
            error("Cannot continue this game.");
        }
        reseed_random(rn2);
        reseed_random(rn2_on_display_rng);
        minit(); /* ZEROCOMP */
        getlev(fd, hackpid, new_ledger, FALSE);
        (void) nhclose(fd);
        oinit(); /* reassign level dependent obj probabilities */
    }
    reglyph_darkroom();
    /* do this prior to level-change pline messages */
    vision_reset();         /* clear old level's line-of-sight */
    vision_full_recalc = 0; /* don't let that reenable vision yet */
    flush_screen(-1);       /* ensure all map flushes are postponed */

    if (portal && !In_endgame(&u.uz)) {
        /* find the portal on the new level */
        register struct trap *ttrap;

        for (ttrap = ftrap; ttrap; ttrap = ttrap->ntrap)
            if (ttrap->ttyp == MAGIC_PORTAL)
                break;

        if (!ttrap)
            panic("goto_level: no corresponding portal!");
        seetrap(ttrap);
        u_on_newpos(ttrap->tx, ttrap->ty);
    } else if (at_stairs && !In_endgame(&u.uz)) {
        if (up) {
            if (at_ladder)
                u_on_newpos(xdnladder, ydnladder);
            else if (newdungeon)
                u_on_sstairs(1);
            else
                u_on_dnstairs();
            /* you climb up the {stairs|ladder};
               fly up the stairs; fly up along the ladder */
            great_effort = (Punished && !Levitation);
            if (flags.verbose || great_effort)
                pline("%s %s up%s the %s.",
                      great_effort ? "With great effort, you" : "You",
                      Levitation ? "float" : Flying ? "fly" : "climb",
                      (Flying && at_ladder) ? " along" : "",
                      at_ladder ? "ladder" : "stairs");
        } else { /* down */
            if (at_ladder)
                u_on_newpos(xupladder, yupladder);
            else if (newdungeon)
                u_on_sstairs(0);
            else
                u_on_upstairs();
            if (!u.dz) {
                ; /* stayed on same level? (no transit effects) */
            } else if (Flying) {
                if (flags.verbose)
                    You("fly down %s.",
                        at_ladder ? "along the ladder" : "the stairs");
            } else if (near_capacity() > UNENCUMBERED
                       || Punished || Fumbling) {
                You("fall down the %s.", at_ladder ? "ladder" : "stairs");
                if (Punished) {
                    drag_down();
                    ballrelease(FALSE);
                }
                /* falling off steed has its own losehp() call */
                if (u.usteed)
                    dismount_steed(DISMOUNT_FELL);
                else
                    losehp(Maybe_Half_Phys(rnd(3)),
                           at_ladder ? "falling off a ladder"
                                     : "tumbling down a flight of stairs",
                           KILLED_BY);
                selftouch("Falling, you");
            } else { /* ordinary descent */
                if (flags.verbose)
                    You("%s.", at_ladder ? "climb down the ladder"
                                         : "descend the stairs");
            }
        }
    } else { /* trap door or level_tele or In_endgame */
        u_on_rndspot((up ? 1 : 0) | (was_in_W_tower ? 2 : 0));
        if (falling) {
            if (Punished)
                ballfall();
            selftouch("Falling, you");
        }
    }

    if (Punished)
        placebc();
    obj_delivery(FALSE);
    losedogs();
    kill_genocided_monsters(); /* for those wiped out while in limbo */
    /*
     * Expire all timers that have gone off while away.  Must be
     * after migrating monsters and objects are delivered
     * (losedogs and obj_delivery).
     */
    run_timers();

    /* hero might be arriving at a spot containing a monster;
       if so, move one or the other to another location */
    if ((mtmp = m_at(u.ux, u.uy)) != 0)
        u_collide_m(mtmp);

    initrack();

    /* initial movement of bubbles just before vision_recalc */
    if (Is_waterlevel(&u.uz) || Is_airlevel(&u.uz))
        movebubbles();
    else if (Is_firelevel(&u.uz))
        fumaroles();

    if (level_info[new_ledger].flags & FORGOTTEN) {
        forget_map(ALL_MAP); /* forget the map */
        forget_traps();      /* forget all traps too */
        familiar = TRUE;
        level_info[new_ledger].flags &= ~FORGOTTEN;
    }

    /* Reset the screen. */
    vision_reset(); /* reset the blockages */
    docrt();        /* does a full vision recalc */
    flush_screen(-1);

    /*
     *  Move all plines beyond the screen reset.
     */

    /* special levels can have a custom arrival message */
    deliver_splev_message();

    /* give room entrance message, if any */
    check_special_room(FALSE);

    /* deliver objects traveling with player */
    obj_delivery(TRUE);

    /* Check whether we just entered Gehennom. */
    if (!In_hell(&u.uz0) && Inhell) {
        if (Is_valley(&u.uz)) {
            You("arrive at the Valley of the Dead...");
            pline_The("odor of burnt flesh and decay pervades the air.");
#ifdef MICRO
            display_nhwindow(WIN_MESSAGE, FALSE);
#endif
            You_hear("groans and moans everywhere.");
        } else
            pline("It is hot here.  You smell smoke...");
        u.uachieve.enter_gehennom = 1;
    }
    /* in case we've managed to bypass the Valley's stairway down */
    if (Inhell && !Is_valley(&u.uz))
        u.uevent.gehennom_entered = 1;

    if (familiar) {
        static const char *const fam_msgs[4] = {
            "You have a sense of deja vu.",
            "You feel like you've been here before.",
            "This place %s familiar...", 0 /* no message */
        };
        static const char *const halu_fam_msgs[4] = {
            "Whoa!  Everything %s different.",
            "You are surrounded by twisty little passages, all alike.",
            "Gee, this %s like uncle Conan's place...", 0 /* no message */
        };
        const char *mesg;
        char buf[BUFSZ];
        int which = rn2(4);

        if (Hallucination)
            mesg = halu_fam_msgs[which];
        else
            mesg = fam_msgs[which];
        if (mesg && index(mesg, '%')) {
            Sprintf(buf, mesg, !Blind ? "looks" : "seems");
            mesg = buf;
        }
        if (mesg)
            pline1(mesg);
    }

    /* special location arrival messages/events */
    if (In_endgame(&u.uz)) {
        if (new &&on_level(&u.uz, &astral_level))
            final_level(); /* guardian angel,&c */
        else if (newdungeon && u.uhave.amulet)
            resurrect(); /* force confrontation with Wizard */
    } else if (In_quest(&u.uz)) {
        onquest(); /* might be reaching locate|goal level */
    } else if (In_V_tower(&u.uz)) {
        if (newdungeon && In_hell(&u.uz0))
            pline_The("heat and smoke are gone.");
    } else if (Is_knox(&u.uz)) {
        /* alarm stops working once Croesus has died */
        if (new || !mvitals[PM_CROESUS].died) {
            You("have penetrated a high security area!");
            pline("An alarm sounds!");
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
                if (DEADMONSTER(mtmp))
                    continue;
                mtmp->msleeping = 0;
            }
        }
    } else {
        if (new && Is_rogue_level(&u.uz))
            You("enter what seems to be an older, more primitive world.");
        /* main dungeon message from your quest leader */
        if (!In_quest(&u.uz0) && at_dgn_entrance("The Quest")
            && !(u.uevent.qcompleted || u.uevent.qexpelled
                 || quest_status.leader_is_dead)) {
            if (!u.uevent.qcalled) {
                u.uevent.qcalled = 1;
                com_pager(2); /* main "leader needs help" message */
            } else {          /* reminder message */
                com_pager(Role_if(PM_ROGUE) ? 4 : 3);
            }
        }
    }

    assign_level(&u.uz0, &u.uz); /* reset u.uz0 */
#ifdef INSURANCE
    save_currentstate();
#endif

    if ((annotation = get_annotation(&u.uz)) != 0)
        You("remember this level as %s.", annotation);

    /* assume this will always return TRUE when changing level */
    (void) in_out_region(u.ux, u.uy);
    (void) pickup(1);
}

STATIC_OVL void
final_level()
{
    struct monst *mtmp;

    /* reset monster hostility relative to player */
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        reset_hostility(mtmp);
    }

    /* create some player-monsters */
    create_mplayers(rn1(4, 3), TRUE);

    /* create a guardian angel next to player, if worthy */
    gain_guardian_angel(FALSE);
}

static char *dfr_pre_msg = 0,  /* pline() before level change */
            *dfr_post_msg = 0; /* pline() after level change */

/* change levels at the end of this turn, after monsters finish moving */
void
schedule_goto(tolev, at_stairs, falling, portal_flag, pre_msg, post_msg)
d_level *tolev;
boolean at_stairs, falling;
int portal_flag;
const char *pre_msg, *post_msg;
{
    int typmask = 0100; /* non-zero triggers `deferred_goto' */

    /* destination flags (`goto_level' args) */
    if (at_stairs)
        typmask |= 1;
    if (falling)
        typmask |= 2;
    if (portal_flag)
        typmask |= 4;
    if (portal_flag < 0)
        typmask |= 0200; /* flag for portal removal */
    u.utotype = typmask;
    /* destination level */
    assign_level(&u.utolev, tolev);

    if (pre_msg)
        dfr_pre_msg = dupstr(pre_msg);
    if (post_msg)
        dfr_post_msg = dupstr(post_msg);
}

/* handle something like portal ejection */
void
deferred_goto()
{
    if (!on_level(&u.uz, &u.utolev)) {
        d_level dest;
        int typmask = u.utotype; /* save it; goto_level zeroes u.utotype */

        assign_level(&dest, &u.utolev);
        if (dfr_pre_msg)
            pline1(dfr_pre_msg);
        goto_level(&dest, !!(typmask & 1), !!(typmask & 2), !!(typmask & 4));
        if (typmask & 0200) { /* remove portal */
            struct trap *t = t_at(u.ux, u.uy);

            if (t) {
                deltrap(t);
                newsym(u.ux, u.uy);
            }
        }
        if (dfr_post_msg)
            pline1(dfr_post_msg);
    }
    u.utotype = 0; /* our caller keys off of this */
    if (dfr_pre_msg)
        free((genericptr_t) dfr_pre_msg), dfr_pre_msg = 0;
    if (dfr_post_msg)
        free((genericptr_t) dfr_post_msg), dfr_post_msg = 0;
}

/*
 * Return TRUE if we created a monster for the corpse.  If successful, the
 * corpse is gone.
 */
boolean
revive_corpse(corpse)
struct obj *corpse;
{
    struct monst *mtmp, *mcarry;
    boolean is_uwep, chewed;
    xchar where;
    char cname[BUFSZ];
    struct obj *container = (struct obj *) 0;
    int container_where = 0;

    where = corpse->where;
    is_uwep = (corpse == uwep);
    chewed = (corpse->oeaten != 0);
    Strcpy(cname, corpse_xname(corpse,
                               chewed ? "bite-covered" : (const char *) 0,
                               CXN_SINGULAR));
    mcarry = (where == OBJ_MINVENT) ? corpse->ocarry : 0;

    if (where == OBJ_CONTAINED) {
        struct monst *mtmp2;

        container = corpse->ocontainer;
        mtmp2 = get_container_location(container, &container_where, (int *) 0);
        /* container_where is the outermost container's location even if
         * nested */
        if (container_where == OBJ_MINVENT && mtmp2)
            mcarry = mtmp2;
    }
    mtmp = revive(corpse, FALSE, -1, FALSE); /* corpse is gone if successful */

    if (mtmp) {
        switch (where) {
        case OBJ_INVENT:
            if (is_uwep)
                pline_The("%s writhes out of your grasp!", cname);
            else
                You_feel("squirming in your backpack!");
            break;

        case OBJ_FLOOR:
            if (cansee(mtmp->mx, mtmp->my))
                pline("%s rises from the dead!",
                      chewed ? Adjmonnam(mtmp, "bite-covered")
                             : Monnam(mtmp));
            break;

        case OBJ_MINVENT: /* probably a nymph's */
            if (cansee(mtmp->mx, mtmp->my)) {
                if (canseemon(mcarry))
                    pline("Startled, %s drops %s as it revives!",
                          mon_nam(mcarry), an(cname));
                else
                    pline("%s suddenly appears!",
                          chewed ? Adjmonnam(mtmp, "bite-covered")
                                 : Monnam(mtmp));
            }
            break;
        case OBJ_CONTAINED: {
            char sackname[BUFSZ];

            if (container_where == OBJ_MINVENT && cansee(mtmp->mx, mtmp->my)
                && mcarry && canseemon(mcarry) && container) {
                pline("%s writhes out of %s!", Amonnam(mtmp),
                      yname(container));
            } else if (container_where == OBJ_INVENT && container) {
                Strcpy(sackname, an(xname(container)));
                pline("%s %s out of %s in your pack!",
                      Blind ? Something : Amonnam(mtmp),
                      locomotion(mtmp->data, "writhes"), sackname);
            } else if (container_where == OBJ_FLOOR && container
                       && cansee(mtmp->mx, mtmp->my)) {
                Strcpy(sackname, an(xname(container)));
                pline("%s escapes from %s!", Amonnam(mtmp), sackname);
            }
            break;
        }
        default:
            /* we should be able to handle the other cases... */
            impossible("revive_corpse: lost corpse @ %d", where);
            break;
        }
        return TRUE;
    }
    return FALSE;
}

/*
 * Return TRUE if we created a monster for the corpse.  If successful, the
 * corpse is gone.
 */
boolean
animate_corpse(corpse, animateintomon)
struct obj* corpse;
int animateintomon; // monstid to be animated into
{
	if (animateintomon < 0 || animateintomon >= NUMMONS || !corpse || corpse->corpsenm < 0 || corpse->corpsenm >= NUMMONS)
		return FALSE;

	struct monst* mtmp, * mcarry;
	boolean is_uwep, chewed;
	xchar where;
	char cname[BUFSZ];
	struct obj* container = (struct obj*) 0;
	int container_where = 0;
	int oldcorpsenum = corpse->corpsenm;

	where = corpse->where;
	is_uwep = (corpse == uwep);
	chewed = (corpse->oeaten != 0);
	Strcpy(cname, corpse_xname(corpse,
		chewed ? "bite-covered" : (const char*)0,
		CXN_SINGULAR));
	mcarry = (where == OBJ_MINVENT) ? corpse->ocarry : 0;

	if (where == OBJ_CONTAINED) {
		struct monst* mtmp2;

		container = corpse->ocontainer;
		mtmp2 = get_container_location(container, &container_where, (int*)0);
		/* container_where is the outermost container's location even if
		 * nested */
		if (container_where == OBJ_MINVENT && mtmp2)
			mcarry = mtmp2;
	}
	mtmp = revive(corpse, FALSE, animateintomon, FALSE); /* corpse is gone if successful */

	if (mtmp) {
		switch (where) {
		case OBJ_INVENT:
			if (is_uwep)
				pline_The("%s writhes out of your grasp!", cname);
			else
				You_feel("squirming in your backpack!");
			break;

		case OBJ_FLOOR:
			if (cansee(mtmp->mx, mtmp->my))
				pline("%s rises from the dead as %s!",
					The(mons[oldcorpsenum].mname), an(mons[animateintomon].mname));
			break;

		case OBJ_MINVENT: /* probably a nymph's */
			if (cansee(mtmp->mx, mtmp->my)) {
				if (canseemon(mcarry))
					pline("Startled, %s drops %s as it gets animated!",
						mon_nam(mcarry), an(cname));
				else
					pline("%s rises from the dead as %s!",
						The(mons[oldcorpsenum].mname), an(mons[animateintomon].mname));
			}
			break;
		case OBJ_CONTAINED: {
			char sackname[BUFSZ];

			if (container_where == OBJ_MINVENT && cansee(mtmp->mx, mtmp->my)
				&& mcarry && canseemon(mcarry) && container) {
				pline("%s writhes out of %s!", Amonnam(mtmp),
					yname(container));
			}
			else if (container_where == OBJ_INVENT && container) {
				Strcpy(sackname, an(xname(container)));
				pline("%s %s out of %s in your pack!",
					Blind ? Something : Amonnam(mtmp),
					locomotion(mtmp->data, "writhes"), sackname);
			}
			else if (container_where == OBJ_FLOOR && container
				&& cansee(mtmp->mx, mtmp->my)) {
				Strcpy(sackname, an(xname(container)));
				pline("%s escapes from %s!", Amonnam(mtmp), sackname);
			}
			break;
		}
		default:
			/* we should be able to handle the other cases... */
			impossible("animate_corpse: lost corpse @ %d", where);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

/* Revive the corpse via a timeout. */
/*ARGSUSED*/
void
revive_mon(arg, timeout)
anything *arg;
long timeout UNUSED;
{
    struct obj *body = arg->a_obj;
    struct permonst *mptr = &mons[body->corpsenm];
    struct monst *mtmp;
    xchar x, y;

    /* corpse will revive somewhere else if there is a monster in the way;
       Riders get a chance to try to bump the obstacle out of their way */
    if ((mptr->mflags3 & M3_DISPLACES) != 0 && body->where == OBJ_FLOOR
        && get_obj_location(body, &x, &y, 0) && (mtmp = m_at(x, y)) != 0) {
        boolean notice_it = canseemon(mtmp); /* before rloc() */
        char *monname = Monnam(mtmp);

        if (rloc(mtmp, TRUE)) {
            if (notice_it && !canseemon(mtmp))
                pline("%s vanishes.", monname);
            else if (!notice_it && canseemon(mtmp))
                pline("%s appears.", Monnam(mtmp)); /* not pre-rloc monname */
            else if (notice_it && dist2(mtmp->mx, mtmp->my, x, y) > 2)
                pline("%s teleports.", monname); /* saw it and still see it */
        }
    }

    /* if we succeed, the corpse is gone */
    if (!revive_corpse(body)) {
        long when;
        int action;

        if (is_rider(mptr) && rn2(99)) { /* Rider usually tries again */
            action = REVIVE_MON;
            for (when = 3L; when < 67L; when++)
                if (!rn2(3))
                    break;
        } else { /* rot this corpse away */
            You_feel("%sless hassled.", is_rider(mptr) ? "much " : "");
            action = ROT_CORPSE;
            when = 250L - (monstermoves - body->age);
            if (when < 1L)
                when = 1L;
        }
        (void) start_timer(when, TIMER_OBJECT, action, arg);
    }
}

int
donull()
{
    return 1; /* Do nothing, but let other things happen */
}

STATIC_PTR int
wipeoff(VOID_ARGS)
{
    if (u.ucreamed < 4)
        u.ucreamed = 0;
    else
        u.ucreamed -= 4;
    if (Blinded < 4)
        Blinded = 0;
    else
        Blinded -= 4;
    if (!Blinded) {
        pline("You've got the glop off.");
        u.ucreamed = 0;
        if (!gulp_blnd_check()) {
            Blinded = 1;
            make_blinded(0L, TRUE);
        }
        return 0;
    } else if (!u.ucreamed) {
        Your("%s feels clean now.", body_part(FACE));
        return 0;
    }
    return 1; /* still busy */
}

int
dowipe()
{
    if (u.ucreamed) {
        static NEARDATA char buf[39];

        Sprintf(buf, "wiping off your %s", body_part(FACE));
        set_occupation(wipeoff, buf, 0);
        /* Not totally correct; what if they change back after now
         * but before they're finished wiping?
         */
        return 1;
    }
    Your("%s is already clean.", body_part(FACE));
    return 1;
}

void
set_wounded_legs(side, timex)
register long side;
register int timex;
{
    /* KMH -- STEED
     * If you are riding, your steed gets the wounded legs instead.
     * You still call this function, but don't lose hp.
     * Caller is also responsible for adjusting messages.
     */

	/*
    if (!Wounded_legs) {
        ATEMP(A_DEX)--;
        context.botl = 1;
    }
	*/

    if (!Wounded_legs || (HWounded_legs & TIMEOUT))
        HWounded_legs = timex;
    EWounded_legs = side;
    (void) encumber_msg();
}

void
heal_legs(how)
int how; /* 0: ordinary, 1: dismounting steed, 2: limbs turn to stone */
{
    if (Wounded_legs) {
		
		/* Moved to updateabon */
		/*
        if (ATEMP(A_DEX) < 0) {
            ATEMP(A_DEX)++;
            context.botl = 1;
        }
		*/

        /* when mounted, wounded legs applies to the steed;
           during petrification countdown, "your limbs turn to stone"
           before the final stages and that calls us (how==2) to cure
           wounded legs, but we want to suppress the feel better message */
        if (!u.usteed && how != 2) {
            const char *legs = body_part(LEG);

            if ((EWounded_legs & BOTH_SIDES) == BOTH_SIDES)
                legs = makeplural(legs);
            /* this used to say "somewhat better" but that was
               misleading since legs are being fully healed */
            Your("%s %s better.", legs, vtense(legs, "feel"));
        }

        HWounded_legs = EWounded_legs = 0L;

        /* Wounded_legs reduces carrying capacity, so we want
           an encumbrance check when they're healed.  However,
           while dismounting, first steed's legs get healed,
           then hero is dropped to floor and a new encumbrance
           check is made [in dismount_steed()].  So don't give
           encumbrance feedback during the dismount stage
           because it could seem to be shown out of order and
           it might be immediately contradicted [able to carry
           more when steed becomes healthy, then possible floor
           feedback, then able to carry less when back on foot]. */
        if (how == 0)
            (void) encumber_msg();
    }
}

/*do.c*/
