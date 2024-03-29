#include "Interval.hpp"

#include <limits>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>


Interval::Interval() {}

Interval::Interval(double x)
{
    this->m_lb = x;
    this->m_ub = x;
}

Interval::Interval(double lb, double ub)
{
    if (lb > ub)
    {
        throw std::invalid_argument("min > max");
    }
    this->m_lb = lb;
    this->m_ub = ub;
}

double Interval::get_lb() const
{
    return this->m_lb;
}

double Interval::get_ub() const
{
    return this->m_ub;
}

bool Interval::is_empty() const
{
    return this->m_lb > this->m_ub;
}

bool Interval::is_unbounded() const
{
    return this->m_lb == -oo && this->m_ub == oo;
}

bool Interval::is_singleton() const
{
    return this->m_lb == this->m_ub;
}

bool Interval::contains(double x) const
{
    return this->m_lb <= x && x <= this->m_ub;
}

bool Interval::contains(const Interval& x) const
{
    return this->m_lb <= x.m_lb && x.m_ub <= this->m_ub;
}

bool Interval::operator==(const Interval& x) const
{
    return this->m_lb == x.m_lb && this->m_ub == x.m_ub;
}

Interval Interval::empty_set()
{
    return Interval(empty_value, empty_value);
}

std::ostream& operator<<(std::ostream& os, const Interval& x)
{
    os << "[" << x.m_lb << ", " << x.m_ub << "]";
    return os;
}

Interval min(Interval i1, Interval i2)
{
    return i1.get_lb() < i2.get_lb() ? i1.get_lb() : i2.get_lb();
}

Interval max(Interval i1, Interval i2)
{
    return i1.get_ub() > i2.get_ub() ? i1.get_ub() : i2.get_ub();
}
