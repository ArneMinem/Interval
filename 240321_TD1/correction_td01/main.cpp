#include <cstdlib>
#include <iostream>
#include <list>
#include <array>
#include "Interval.h"

using namespace std;

Interval f(const Interval& x)
{
  return sqr(x) + 2*x - exp(x);
}

int main()
{
  cout << "Section A" << endl << endl;

  // 1.
  Interval x(-1,3), y(2,5);
  cout << "  [x]+[y]=" << (x+y) << endl;
  cout << "  [x]-[y]=" << (x-y) << endl;
  cout << "  [x]·[y]=" << (x*y) << endl;
  cout << "  [x]/[y]=" << (x/y) << endl;
  cout << "  min([x],[y])=" << min(x,y) << endl;
  cout << "  max([x],[y])=" << max(x,y) << endl << endl;

  // 2.
  cout << "  [-2,4]·[1,3]=" << (Interval(-2,4)*Interval(1,3)) << endl;
  cout << "  [-2,4]⊔[6,7]=" << (Interval(-2,4)|Interval(6,7)) << endl;
  cout << "  max([2,7],[1,9])=" << max(Interval(2,7),Interval(1,9)) << endl;
  cout << "  max(∅,[1,2])=" << max(Interval::empty_set(),Interval(1,2)) << endl;
  cout << "  [-1,3]/[0,∞]=" << (Interval(-1,3)*Interval(0,oo)) << endl;
  cout << "  ([1,2]·[-1,3])+max([1,3]∩[6,7],[1,2])="
       << (Interval(1,2)*Interval(-1,3))+max(Interval(1,3)&Interval(6,7),Interval(1,2)) << endl << endl;

  // 3.
  cout << "  [-1,3]²=" << sqr(Interval(-1,3)) << endl;
  cout << "  sqrt([-10,4])=" << sqrt(Interval(-10,4)) << endl;
  cout << "  log([-2,-1])=" << log(Interval(-2,-1)) << endl << endl;

  // 4.
  cout << "  ([1,2]+[-3,4])·[-1,5]=" << (Interval(1,2)+Interval(-3,4))*Interval(-1,5) << endl;
  cout << "  exp([1,2]/[0,∞])=" << exp(Interval(1,2)/Interval(0,oo)) << endl << endl;


  cout << "Section B" << endl << endl;

  // 10.
  cout << "  f([-2,2])=" << f(Interval(-2,2)) << endl;

  return EXIT_SUCCESS;
}