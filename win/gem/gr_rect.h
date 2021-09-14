/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* gr_rect.h */
/*
 * $NHDT-Date: 1432512809 2015/05/25 00:13:29 $  $NHDT-Branch: master $:$NHDT-Revision: 1.4 $
 */
#include <e_gem.h>
/********** structs **********/
typedef struct {
    GRECT *rects;
    int max, used;
} dirty_rect;
/********* functions ************/
dirty_rect *new_dirty_rect(int size);
void delete_dirty_rect(dirty_rect *this);
int add_dirty_rect(dirty_rect *dr, GRECT *area);
int get_dirty_rect(dirty_rect *dr, GRECT *area);
int clear_dirty_rect(dirty_rect *dr);
int resize_dirty_rect(dirty_rect *dr, int new_size);
