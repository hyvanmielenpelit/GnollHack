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
    otypdata.nh_color = objects[otmp->otyp].oc_color;

    otypdata.lamplit = otmp->lamplit;
    otypdata.poisoned = otmp->opoisoned;
    otypdata.eroded = otmp->oeroded;
    otypdata.eroded2 = otmp->oeroded2;

    otypdata.corrodeable = is_corrodeable(otmp);
    otypdata.rottable = is_rottable(otmp);
    otypdata.flammable = is_flammable(otmp);
    otypdata.rustprone = is_rustprone(otmp);
    otypdata.poisonable = is_poisonable(otmp);

    if (otmp == uchain || otmp == uball)
    {
        otypdata.is_uchain = otmp == uchain;
        otypdata.is_uball = otmp == uball;

        xchar ox = 0, oy = 0;
        (void)get_obj_location(otmp, &ox, &oy, BURIED_TOO);
        otypdata.obj_loc_x = ox;
        otypdata.obj_loc_y = oy;
    }

    return otypdata;
}
