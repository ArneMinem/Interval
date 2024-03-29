#ifndef __INTERVAL_H__
#define __INTERVAL_H__

#include <limits>
#include <iostream>

const double oo = std::numeric_limits<double>::infinity();
const double empty_value = std::numeric_limits<double>::quiet_NaN();

class Interval
{ 
  public:

    Interval();
    Interval(double x);
    Interval(double lb, double ub);

    double lb() const;
    double ub() const;
    bool is_empty() const;
    bool contains(double x) const;
    bool operator==(const Interval& x) const;

    static Interval empty_set();
    friend std::ostream& operator<<(std::ostream& os, const Interval& x);

  protected:

    double m_lb = -oo, m_ub = oo;
};

Interval operator+(const Interval& x, const Interval& y);
Interval operator-(const Interval& x, const Interval& y);
Interval operator*(const Interval& x, const Interval& y);
Interval operator/(const Interval& x, const Interval& y);
Interval operator&(const Interval& x, const Interval& y);
Interval operator|(const Interval& x, const Interval& y);

Interval inverse(const Interval& x);
Interval sqr(const Interval& x);
Interval sqrt(const Interval& x);
Interval exp(const Interval& x);
Interval log(const Interval& x);

Interval min(const Interval& x, const Interval& y);
Interval max(const Interval& x, const Interval& y);

#endif