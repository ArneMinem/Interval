#include "Interval.h"
#include <cmath> // for isnan()
#include <vector>
#include <algorithm> // for min/max_element

using namespace std;

Interval::Interval()
{

}

Interval::Interval(double x) :
  m_lb(x), m_ub(x)
{

}

Interval::Interval(double lb, double ub) :
  m_lb(lb), m_ub(ub)
{
  if(lb > ub)
  {
    m_lb = numeric_limits<double>::quiet_NaN();
    m_ub = numeric_limits<double>::quiet_NaN();
  }
}

double Interval::lb() const
{
  return m_lb;
}

double Interval::ub() const
{
  return m_ub;
}

bool Interval::is_empty() const
{
  return isnan(m_lb);
}

Interval Interval::empty_set()
{
  return Interval(empty_value,empty_value);
}

bool Interval::contains(double x) const
{
  return m_lb <= x && x <= m_ub;
}

bool Interval::contains(const Interval& x) const
{
  return m_lb <= x.lb() && x.ub() <= m_ub;
}

bool Interval::operator==(const Interval& x) const
{
  return m_lb == x.lb() && m_ub == x.ub();
}

ostream& operator<<(ostream& os, const Interval& x)
{
  if(x.is_empty())
    os << "[empty set]";
  else
    os << "[" << x.lb() << "," << x.ub() << "]";
  return os;
}

Interval operator+(const Interval& x, const Interval& y)
{
  return Interval(x.lb() + y.lb(), x.ub() + y.ub());
}

Interval operator-(const Interval& x, const Interval& y)
{
  return Interval(x.lb() - y.ub(), x.ub() - y.lb());
}

Interval operator*(const Interval& x, const Interval& y)
{
  vector<double> bounds { x.lb()*y.lb(), x.lb()*y.ub(), x.ub()*y.lb(), x.ub()*y.ub() };
  return Interval(*min_element(begin(bounds), end(bounds)), *max_element(begin(bounds), end(bounds)));
}

Interval inverse(const Interval& x)
{
  if(x == 0.) // works thanks to "operator=="
    return Interval::empty_set();

  else if(!x.contains(0.))
    return Interval(1./x.ub(), 1./x.lb());

  else if(x.lb() == 0. && x.ub() > 0.)
    return Interval(1./x.ub(), oo);

  else if(x.lb() < 0. && x.ub() == 0.)
    return Interval(-oo, 1./x.lb());

  else
    return Interval(); // [-oo,oo]
}

Interval operator/(const Interval& x, const Interval& y)
{
  return x * inverse(y);
}

Interval operator|(const Interval& x, const Interval& y)
{
  return Interval(min(x.lb(),y.lb()), max(x.ub(),y.ub()));
}

Interval operator&(const Interval& x, const Interval& y)
{
  return Interval(max(x.lb(),y.lb()), min(x.ub(),y.ub()));
}

Interval sqr(const Interval& x)
{
  vector<double> bounds { pow(x.lb(),2), pow(x.ub(),2) };

  if(x.contains(0.))
    return Interval(0., *max_element(begin(bounds), end(bounds)));

  else
    return Interval(*min_element(begin(bounds), end(bounds)), *max_element(begin(bounds), end(bounds)));
}

Interval sqrt(const Interval& x)
{
  Interval x_ = Interval(0,oo) & x;
  return Interval(sqrt(x_.lb()),sqrt(x_.ub()));
}

Interval exp(const Interval& x)
{
  return Interval(exp(x.lb()), exp(x.ub()));
}

Interval log(const Interval& x)
{
  if(x.ub() <= 0.)
    return Interval::empty_set();

  else if(x.contains(0.))
    return Interval(-oo, log(x.ub()));

  else
    return Interval(log(x.lb()), log(x.ub()));
}

Interval min(const Interval& x, const Interval& y)
{
  return Interval(min(x.lb(),y.lb()), min(x.ub(),y.ub()));
}

Interval max(const Interval& x, const Interval& y)
{
  return Interval(max(x.lb(),y.lb()), max(x.ub(),y.ub()));
}

Interval intersection(const Interval& x, const Interval& y)
{
  if (x.ub() < y.lb() || y.ub() < x.lb())
    return Interval::empty_set();
  else
    return Interval(max(x.lb(),y.lb()), min(x.ub(),y.ub()));
}