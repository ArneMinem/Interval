#include <codac.h>

using namespace std;
using namespace codac;

int main()
{
  
  /*Interval x1 = Interval(-2,4)*Interval(1,3) ;
  cout << "x1 = " << x1 << endl;

  Interval x2 = Interval(8,10) / Interval(-1,0) ;
  cout << "x2 = " << x2 << endl;

  Interval x3 = Interval(-2,4) | Interval(6,7) ;
  cout << "x3 = " << x3 << endl;

  Interval x4 = max(Interval(2,7), Interval(1,9)) ;
  cout << "x4 = " << x4 << endl;

  Interval x5 = max(Interval::EMPTY_SET, Interval(1,2)) ;
  cout << "x5 = " << x5 << endl;

  Interval x6 = cos(Interval(-oo,oo)) ;
  cout << "x6 = " << x6 << endl;

  Interval x7 = sqr(Interval(-1,4)) ;
  cout << "x7 = " << x7 << endl;

  Interval x8 = (Interval(1,2)*Interval(-1,3)) + max(Interval(1,6)&Interval(5,7), Interval(1,2)) ;
  cout << "x8 = " << x8 << endl;

  IntervalVector y(2) ;
  y[0] = Interval(0,pi) ;
  y[1] = Interval(-pi/6,pi/6) ;
  cout << "|y| = " << abs(y) << endl;

  */

  // Création d'une fonction
  Function g("x[4]", "((x[0]-x[2])^2+(x[1]-x[3])^2)^(1/2)") ;
  IntervalVector X(2) ;
  X[0] = Interval(0,0) ;
  X[1] = Interval(0,0) ;
  // On modifie X
  //X.inflate(0.1) ;
  IntervalVector b(2) ;
  b[0] = Interval(3,4) ;
  b[1] = Interval(2,3) ;
  IntervalVector c = cart_prod(X,b) ;
  Interval d = g.eval(c) ;
  cout << "g(X,b) = " << d << endl;

  vibes::beginDrawing();
  VIBesFigMap fig("Map");
  fig.set_properties(50, 50, 400, 400); // position and size

  fig.draw_circle(0,0, d.ub(),"green");
  fig.draw_circle(0,0, d.lb(),"green");
  fig.draw_box(X, "blue");
  fig.draw_box(b, "red");

  IntervalVector X2 = X + b ;
  fig.draw_box(X2);

  fig.show(); // display all items of the figure
  vibes::endDrawing();

  // Après modification de X, la distance d a bien été modifiée. Le calcul est fiable.

  // Contracteurs
  Interval newd(7,8) ;
  IntervalVector newx(2) ;
  newx[0] = Interval(0,0) ;
  newx[1] = Interval(0,0) ;
  IntervalVector newb1(2) ;
  newb1[0] = Interval(1.5,2.5) ;
  newb1[1] = Interval(4,11) ;
  IntervalVector newb2(2) ;
  newb2[0] = Interval(3,4) ;
  newb2[1] = Interval(4,6.5) ;
  IntervalVector newb3(2) ;
  newb3[0] = Interval(5,7) ;
  newb3[1] = Interval(5.5,8) ;

  vibes::beginDrawing();
  VIBesFigMap fig1("Map for contractor");
  fig1.set_properties(50, 50, 400, 400); // position and size

  fig1.draw_box(newb1);
  fig1.draw_box(newb2);
  fig1.draw_box(newb3);


  // Création du contracteur
  CtcFunction ctc_dist(Function("X[5]", "X[4] - ((X[0]-X[2])^2+(X[1]-X[3])^2)^(1/2)")) ;
  ContractorNetwork cn ;
  cn.add(ctc_dist, {newx[0], newx[1], newb1[0], newb1[1], newd}) ;
  cn.add(ctc_dist, {newx[0], newx[1], newb2[0], newb2[1], newd}) ;
  cn.add(ctc_dist, {newx[0], newx[1], newb3[0], newb3[1], newd}) ;
  cn.contract();

  fig1.draw_box(newb1, "blue[ligtblue]");
  fig1.draw_box(newb2, "blue[ligtblue]");
  fig1.draw_box(newb3, "blue[ligtblue]");

  fig1.draw_circle(0,0, newd.ub(),"black");
  fig1.draw_circle(0,0, newd.lb(),"black");

  fig1.show(); // display all items of the figure
  vibes::endDrawing();

}