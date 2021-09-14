/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0	qt_clust.h	$NHDT-Date: 1432512779 2015/05/25 00:12:59 $  $NHDT-Branch: master $:$NHDT-Revision: 1.8 $ */
/* Copyright (c) Warwick Allison, 1999. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef clusterizer_H
#define clusterizer_H

#include <qrect.h>

class Clusterizer
{
  public:
    Clusterizer(int maxclusters);
    ~Clusterizer();

    void add(int x, int y); // 1x1 rectangle (point)
    void add(int x, int y, int w, int h);
    void add(const QRect &rect);

    void clear();
    int
    clusters()
    {
        return count;
    }
    const QRect &operator[](int i);

  private:
    QRect *cluster;
    int count;
    const int max;
};

#endif
