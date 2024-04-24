#include <codac.h>
#include <math.h>


using namespace std;
using namespace codac;

int main()
{
  // create trajectory actual_x
  double dt = 0.01;
  Interval tdomain(0,5); // temporal domain [t0,tf]
  TrajectoryVector actual_x(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), dt);

  // create landmark b = (0.5, 1)

  IntervalVector b{0.5, 1};

  // project the 2d trajectory

  vibes::beginDrawing();

  // compute the distance signal actual_y between the robot and the landmark

  TFunction f("x", "y","sqrt(sqr(x-0.5)+sqr(y-1))");
  Trajectory actual_y = f.traj_eval(actual_x);
  RandTrajectory n(tdomain, dt, Interval(-0.05,0.05));
  actual_y += n;
    
  // create the velocity signal actual_v

  TrajectoryVector actual_v(tdomain, TFunction("(-2*sin(t) ; 2*cos(2*t))"), dt);

  // With class TubeVector create a 2d tube x around the trajectory actual_x

  TubeVector x(actual_x, dt);

  // inflate the tube x

  x.inflate(0.2);

  // create the tube y around the signal actual_y
  
  Tube y(actual_y, dt);
  y.inflate(0.05);

  // create the tube v around the signal actual_v

  TubeVector v(actual_v, dt);
  v.inflate(0.01);

  // build contractor network cn and contract the first tube x with the distance contracter

  ContractorNetwork cn;
  cn.add(ctc::dist, {x, b, y});

  // add the derivative contracter to the network

  cn.add(ctc::deriv, {x, v});


  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100, 100, 600, 300);
  fig_map.add_trajectory(&actual_x, "x*", 0, 1);
  fig_map.add_beacon(b, 0.1);                   // 0.1: landmark width
  fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);
  fig_map.show(0.5);                            // argument is robot size

  // display the tube x

  fig_map.add_tube(&x, "x", 0, 1);
  fig_map.show(); // this method must be called to display the added objects

  // contract the network

  cn.contract();

  // display the contracted tube x

  fig_map.show();

  // display the distance signal

  VIBesFigTube fig_dist("Distance to the landmark");
  fig_dist.set_properties(100, 100, 600, 300);
  fig_dist.add_trajectory(&actual_y, "y*");
  fig_dist.show();

  // display the tube y

  fig_dist.add_tube(&y, "y");
  fig_dist.show();
  
  vibes::endDrawing();
}


