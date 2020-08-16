/*
* Copyright 2020 by Janne Gustafsson
*/ 

#ifndef FOUNTAIN_H
#define FOUNTAIN_H

/* Fountain types */
enum fountain_subtypes
{
	FOUNTAIN_MAGIC = 0,
	FOUNTAIN_HEALING = 1,
	FOUNTAIN_MANA = 2,
	FOUNTAIN_POWER = 3,
	FOUNTAIN_POISON = 4,
	FOUNTAIN_WATER = 5,
	MAX_FOUNTAIN_SUBTYPES
};

#define LAST_SHUFFLED_FOUNTAIN FOUNTAIN_POISON

#endif /* FOUNTAIN_H */

