#include <cstdlib>
#include <iostream>
#include <list>
#include <array>
#include "Interval.h"
#include "Box.h"
#include "vibes.h"

using namespace std;

Interval f(const Interval& x)
{
  return sqr(x) + 2*x - exp(x);
}

Interval f2(Box& X, const pair<double, double>& b)
{
  return sqrt(sqr(X[0]-b.first) + sqr(X[1]-b.second));
}

void sivia(Box& x, const Interval& Y, const double epsilon, vector<Box>& Xm, vector<Box>& Xp, const pair<double, double> b)
{
  // vibes::drawBox(x[0].lb(), x[0].ub(), x[1].lb(), x[1].ub(), "[b]");

  if (intersection(f2(x, b), Y).is_empty())
  {
  }

  else
  {
    if (Y.contains(f2(x, b)))
    {
      vibes::drawBox(x[0].lb(), x[0].ub(), x[1].lb(), x[1].ub(), "[g]");
      Xm.push_back(x);
      Xp.push_back(x);
    }
    else if (x.width() < epsilon)
    {
      vibes::drawBox(x[0].lb(), x[0].ub(), x[1].lb(), x[1].ub(), "[y]");
      Xp.push_back(x);
    }
    else
    {
      vector<Box> L1 = x.split();
      sivia(L1[0], Y, epsilon, Xm, Xp, b);
      sivia(L1[1], Y, epsilon, Xm, Xp, b);
      sivia(L1[2], Y, epsilon, Xm, Xp, b);
      sivia(L1[3], Y, epsilon, Xm, Xp, b);
    }    
  }

}

void sivia(Box& x, const Interval& Y1, const Interval& Y2, const double epsilon, vector<Box>& Xm, vector<Box>& Xp, const pair<double, double> b1, const pair<double, double> b2)
{
  // vibes::drawBox(x[0].lb(), x[0].ub(), x[1].lb(), x[1].ub(), "[b]");

  if (intersection(f2(x, b1), Y1).is_empty() || intersection(f2(x,b2), Y2).is_empty())
  {
  }

  else
  {
    if (Y1.contains(f2(x, b1)) && Y2.contains(f2(x, b2)))      
    {
      vibes::drawBox(x[0].lb(), x[0].ub(), x[1].lb(), x[1].ub(), "[b]");
      Xm.push_back(x);
      Xp.push_back(x);
    }
    else if (x.width() < epsilon)
    {
      vibes::drawBox(x[0].lb(), x[0].ub(), x[1].lb(), x[1].ub(), "[r]");
      Xp.push_back(x);
    }
    else
    {
      vector<Box> L1 = x.split();
      sivia(L1[0], Y1, Y2, epsilon, Xm, Xp, b1, b2);
      sivia(L1[1], Y1, Y2, epsilon, Xm, Xp, b1, b2);
      sivia(L1[2], Y1, Y2, epsilon, Xm, Xp, b1, b2);
      sivia(L1[3], Y1, Y2, epsilon, Xm, Xp, b1, b2);
    }    
  }
}

void sivia(Box& x, const Interval& Y1, const Interval& Y2, const Interval& Y3, const double epsilon, vector<Box>& Xm, vector<Box>& Xp, const pair<double, double> b1, const pair<double, double> b2, const pair<double, double> b3)
{
  // vibes::drawBox(x[0].lb(), x[0].ub(), x[1].lb(), x[1].ub(), "[b]");

  if (intersection(f2(x, b1), Y1).is_empty() || intersection(f2(x,b2), Y2).is_empty() || intersection(f2(x,b3), Y3).is_empty())
  {
  }

  else
  {
    if (Y1.contains(f2(x, b1)) && Y2.contains(f2(x, b2)) && Y3.contains(f2(x, b3)))      
    {
      vibes::drawBox(x[0].lb(), x[0].ub(), x[1].lb(), x[1].ub(), "[b]");
      Xm.push_back(x);
      Xp.push_back(x);
    }
    else if (x.width() < epsilon)
    {
      vibes::drawBox(x[0].lb(), x[0].ub(), x[1].lb(), x[1].ub(), "[r]");
      Xp.push_back(x);
    }
    else
    {
      vector<Box> L1 = x.split();
      sivia(L1[0], Y1, Y2, Y3, epsilon, Xm, Xp, b1, b2, b3);
      sivia(L1[1], Y1, Y2, Y3, epsilon, Xm, Xp, b1, b2, b3);
      sivia(L1[2], Y1, Y2, Y3, epsilon, Xm, Xp, b1, b2, b3);
      sivia(L1[3], Y1, Y2, Y3, epsilon, Xm, Xp, b1, b2, b3);
    }    
  }
}

int main()
{
  #if 0

    cout << "Correction du TD 01" << endl << endl;
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
    cout << "  max(∅,[1,2])=" << max(EMPTY_SET,Interval(1,2)) << endl;
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

  #endif


  vibes::beginDrawing();
  vibes::newFigure("SIVIA");
  vibes::setFigureProperties(vibesParams("x",100,"y",100,"width",600,"height",600));
  vibes::axisLimits(-10,10,-10,10);

  Interval x(-10,10), y(-10,10);
  Box X(x,y);

  double epsilon = 0.001;
  vector<Box> Xm, Xp;

  pair<double, double> b1 = {1,2};
  pair<double, double> b2 = {3.6,2.4};
  pair<double, double> b3 = {-4,-3};

  Interval Y1 = Interval(2, 2.4);
  Interval Y2 = Interval(4.1, 4.5);
  Interval Y3 = Interval(4.8, 5.2);


  sivia(X, Y1, epsilon, Xm, Xp, b1);
  sivia(X, Y2, epsilon, Xm, Xp, b2);
  sivia(X, Y3, epsilon, Xm, Xp, b3);

  sivia (X, Y1, Y2, epsilon, Xm, Xp, b1, b2);
  sivia (X, Y2, Y3, epsilon, Xm, Xp, b2, b3);
  sivia (X, Y1, Y3, epsilon, Xm, Xp, b1, b3);

  sivia(X, Y1, Y2, Y3, epsilon, Xm, Xp, b1, b2, b3);

  vibes::endDrawing();

  return EXIT_SUCCESS;
}