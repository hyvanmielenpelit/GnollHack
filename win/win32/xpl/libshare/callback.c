/* GnollHack 4.1.0 callback.c */
/* Copyright (c) Janne Gustafsson, 2021. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "callback.h"

struct objclassdata
get_objclassdata(struct obj* otmp)
{
    struct objclassdata otypdata = { 0 };
    if (!otmp)
        return otypdata;

    otypdata.tile_height = get_obj_height(otmp);
    otypdata.special_quality = objects[otmp->otyp].oc_special_quality;
    otypdata.max_charges = get_obj_max_charge(otmp);
    otypdata.semitransparent = (uchar)is_obj_semi_transparent(otmp);

    otypdata.lamplit = (uchar)(otmp->lamplit || (otmp->item_flags & ITEM_FLAGS_MEMORY_OBJECT_LAMPLIT) != 0);
    otypdata.poisoned = (uchar)otmp->opoisoned;
    otypdata.eroded = (uchar)otmp->oeroded;
    otypdata.eroded2 = (uchar)otmp->oeroded2;

    otypdata.corrodeable = (uchar)is_corrodeable(otmp);
    otypdata.rottable = (uchar)is_rottable(otmp);
    otypdata.flammable = (uchar)is_flammable(otmp);
    otypdata.rustprone = (uchar)is_rustprone(otmp);
    otypdata.poisonable = (uchar)is_poisonable(otmp);


    if (otmp == uchain || otmp == uball)
    {
        otypdata.is_uchain = otmp == uchain;
        otypdata.is_uball = otmp == uball;

        xchar ox = 0, oy = 0;
        (void)get_obj_location(otmp, &ox, &oy, BURIED_TOO);
        otypdata.obj_loc_x = (schar)ox;
        otypdata.obj_loc_y = (schar)oy;
    }

    return otypdata;
}
