#include "Box.h"

using namespace std;

Box::Box(Interval x, Interval y)
{
  // Create a box from 2 intervals

  _box[0] = x;
  _box[1] = y;
}

#include <algorithm> // Add missing include statement

Interval& Box::operator[](size_t i)
{
  return _box[i];
}

double Box::width() // Add missing class qualifier
{
  return std::max(this->_box[0].ub() - this->_box[0].lb(), this->_box[1].ub() - this->_box[1].lb()); // Qualify _box variable with this
}

vector<Box> Box::split() // Add missing class qualifier and return type
{
  vector<Box> res;

  // Split the box into 4 sub-boxes
  Interval x = _box[0];
  Interval y = _box[1];

  double mid_x = (x.lb() + x.ub()) / 2;
  double mid_y = (y.lb() + y.ub()) / 2;

  res.push_back(Box(Interval(x.lb(), mid_x), Interval(y.lb(), mid_y)));
  res.push_back(Box(Interval(mid_x, x.ub()), Interval(y.lb(), mid_y)));
  res.push_back(Box(Interval(x.lb(), mid_x), Interval(mid_y, y.ub())));
  res.push_back(Box(Interval(mid_x, x.ub()), Interval(mid_y, y.ub())));

  return res;
}