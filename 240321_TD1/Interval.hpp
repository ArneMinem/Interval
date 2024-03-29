#ifndef __INTERVAL_HPP__
#define __INTERVAL_HPP__

#include <iostream>
#include <limits>

#define oo std::numeric_limits<double>::infinity()
#define empty_value std::numeric_limits<double>::quiet_NaN()

class Interval {
   public:

        Interval();
        Interval(double x);
        Interval(double lb, double ub);

        double get_lb() const;
        double get_ub() const;

        bool is_empty() const;
        bool is_unbounded() const;
        bool is_singleton() const;
        bool contains(double x) const;
        bool contains(const Interval& x) const;
        bool operator==(const Interval& x) const;

        static Interval empty_set();
        friend std::ostream& operator<<(std::ostream& os, const Interval& x);

    protected:

        double m_lb = -oo, m_ub = oo;

};

Interval min(Interval i1, Interval i2); 
Interval max(Interval i1, Interval i2);


#endif