#include <codac.h>
#include <vibes.h>

using namespace std;
using namespace codac;

int main()
{
//   double dt = 0.01;
//   Interval tdomain(0,5); // temporal domain [t0,tf]
//   TrajectoryVector actual_x(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), dt);
//   TrajectoryVector actual_v(tdomain, TFunction("(-2*sin(t) ; 2*cos(2*t))"), dt);
//   TubeVector v(actual_v, dt);
//   v.inflate(0.03);
//   TubeVector x(tdomain, dt, IntervalVector({{-oo, oo}, {-oo, oo}}));
//   IntervalVector b{{-0.84, -0.83}, {-0.76, -0.75}};
//
//   ContractorNetwork cn;
//   cn.add(ctc::eval, {Interval(2), IntervalVector({{-0.84, -0.83}, {-0.76, -0.75}}), x, v});
//   cn.add(ctc::polar, {})
//   cn.contract();
//
//   vibes::beginDrawing();
//
//   VIBesFigMap fig_map("Map");
//   fig_map.set_properties(100, 100, 600, 300);
//   fig_map.add_trajectory(&actual_x, "x*", 0, 1);
//   fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);
//   fig_map.add_tube(&x, "x", 0, 1);
//   fig_map.add_beacon(b, 0.05);                   // 0.1: landmark width
//   fig_map.show(0.5);
//
//   vibes::endDrawing();

  double dt = 0.01;
  Interval tdomain(0.,3.); // temporal domain [t0,tf]
  TrajectoryVector x_truth(tdomain, TFunction("(10*cos(t) + t; 5*sin(2*t)+t; atan2((10*cos(2*t)+1),(-10*sin(t)+1)); sqrt(sqr(-10*sin(t)+1)+sqr(10*cos(2*t)+1)))"), dt);

  TubeVector x(tdomain, dt, 4);                     // 4d tube for state vectors
  TubeVector v(tdomain, dt, 4);                     // 4d tube for derivatives of the states
  TubeVector u(tdomain, dt, 2);                     // 2d tube for inputs of the system

  Trajectory measured_psi(tdomain, TFunction("atan2((10*cos(2*t)+1),(-10*sin(t)+1))"), dt);
  Trajectory measured_speed(tdomain, TFunction("sqrt(sqr(-10*sin(t)+1)+sqr(10*cos(2*t)+1))"), dt);

  x[2] = Tube(measured_psi, dt).inflate(0.01);      // measured_psi is a set of measurements
  x[3] = Tube(measured_speed, dt).inflate(0.01);

  IntervalVector b1{8, 3};
  IntervalVector b2{0, 5};
  IntervalVector b3{-2, 1};

  Interval y1{1.9};
  y1.inflate(0.1);

  Interval y2{3.6};
  y2.inflate(0.1);

  Interval y3{2.8};
  y3.inflate(0.1);

  Interval t1{0.3};
  Interval t2{1.5};
  Interval t3{2.0};

  IntervalVector p1(4);
  IntervalVector p2(4);
  IntervalVector p3(4);
  CtcFunction ctc_eq(Function("v", "u", "v-u"));

  vibes::beginDrawing();

  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100, 100, 600, 300);
  fig_map.draw_ring(b1[0].mid(), b1[1].mid(), y1, "gray");
  fig_map.draw_ring(b2[0].mid(), b2[1].mid(), y2, "gray");
  fig_map.draw_ring(b3[0].mid(), b3[1].mid(), y3, "gray");
  fig_map.add_beacon(b1, 0.1);                   // 0.1: landmark width
  fig_map.add_beacon(b2, 0.1);
  fig_map.add_beacon(b3, 0.1);
  fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);
  fig_map.add_trajectory(&x_truth, "x*", 0, 1);

  ContractorNetwork cn;

  cn.add(ctc::polar, {v[0], v[1], x[3], x[2]});
  cn.add(ctc_eq, {v[2], u[0]});
  cn.add(ctc_eq, {v[3], u[1]});

  cn.add(ctc::dist, {cn.subvector(p1,0,1), b1, y1});
  cn.add(ctc::dist, {cn.subvector(p2,0,1), b2, y2});
  cn.add(ctc::dist, {cn.subvector(p3,0,1), b3, y3});

  cn.add(ctc::eval, {t1, p1, x, v});
  cn.add(ctc::eval, {t2, p2, x, v});
  cn.add(ctc::eval, {t3, p3, x, v});

  cn.contract();

  fig_map.add_tube(&x, "x", 0, 1);
  fig_map.draw_vehicle(t1.lb(), &x_truth, 0.7);
  fig_map.draw_vehicle(t2.lb(), &x_truth, 0.7);
  fig_map.draw_vehicle(t3.lb(), &x_truth, 0.7);
  fig_map.show(0);

  vibes::endDrawing();
}
