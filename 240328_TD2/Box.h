#ifndef __BOX_H__
#define __BOX_H__

#include "Interval.h"
#include <vector>

class Box
{
  public:

    Box(Interval x, Interval y);
    Interval& operator[](size_t i);

    double width();

    std::vector<Box> split();

  protected:

    Interval _box[2];
};

#endif