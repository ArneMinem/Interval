#include <codac.h>

using namespace std;
using namespace codac;

int main()
{
  // Truth (unknown pose)
  Vector x_truth({2., 1., M_PI/6}); // actual state vector (pose)
  Vector y({6,M_PI/6});
  Vector m({5,6.2});

  // Create the bounded sets related to the state, the measurement and the landamrk position
  IntervalVector setx(3); // state vector  
  setx[0] = Interval(x_truth[0],x_truth[0]); // x position
  setx[1] = Interval(x_truth[1],x_truth[1]); // y position
  setx.inflate(oo); // uncertainty on the state vector (pose
  setx[2] = Interval(x_truth[2],x_truth[2]); // cap

  IntervalVector sety(2); // measurement vector
  sety[0] = Interval(y[0]-0.3,y[0]+0.3); // range
  sety[1] = Interval(y[1]-0.1,y[1]+0.1); // bearing

  IntervalVector setm(2); // landmark position
  setm[0] = Interval(m[0],m[0]); // x position
  setm[1] = Interval(m[1],m[1]); // y position
  setm.inflate(0.2); // uncertainty on the landmark position

  Interval interval_rho = Interval(sety[0].lb(),sety[0].ub());
  Interval interval_theta = Interval(sety[1].lb(),sety[1].ub()) + Interval(x_truth[2]) ;

  CtcFunction ctc_g(Function("x[3]", "y[2]", "m[2]",
  "(x[0]+y[0]*cos(x[2]+y[1])-m[0] ; x[1]+y[0]*sin(x[2]+y[1])-m[1])"));
  ContractorNetwork cn;
  cn.add(ctc_g, {setx, sety, setm});

  IntervalVector d(2) ;
  CtcPolar ctc_polar ;

  vibes::beginDrawing();
  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100,100,500,500);
  fig_map.axis_limits(0,7,0,7);
  fig_map.draw_vehicle(x_truth, 1);
  fig_map.draw_box(setm, "red");
  fig_map.draw_box(setx.subvector(0,1)); // does not display anything if unbounded
  fig_map.draw_pie(x_truth[0], x_truth[1],interval_rho,interval_theta);
  fig_map.draw_pie(x_truth[0], x_truth[1], (Interval(0.1)|interval_rho), interval_theta, "lightGray");
  cn.contract();
  ctc_polar.contract(d[0], d[1], interval_rho, interval_theta) ;
  fig_map.draw_box(setm, "red");
  fig_map.draw_box(setx.subvector(0,1)); // does not display anything if unbounded
  fig_map.draw_pie(x_truth[0], x_truth[1],interval_rho,interval_theta);
  fig_map.draw_pie(x_truth[0], x_truth[1], (Interval(0.1)|interval_rho), interval_theta, "lightGray");
  fig_map.show();
  vibes::endDrawing();
}