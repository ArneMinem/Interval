#include <codac.h>
#include <vibes.h>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace codac;

int main()
{
  double dt = 0.05;
  Interval tdomain(0.,15.); // temporal domain [t0,tf]

  /* initialize random seed: */
  srand (time(NULL));

  // Initial pose x0=(0,0,2)
  Vector x0({0,0,2});

  vector<IntervalVector> b = {{6, 12},{-2, -5},{-3, 20},{3, 4}};

  // System input
  Trajectory u_truth(tdomain, TFunction("3*(sin(t)^2)+t/100"), dt);
  RandTrajectory nu(tdomain, dt, Interval(-0.03,0.03));
  // Actual trajectories (state + derivative)
  TrajectoryVector v_truth(3);
  TrajectoryVector x_truth(3);
  v_truth[2] = u_truth + nu;
  x_truth[2] = v_truth[2].primitive() + x0[2];
  v_truth[0] = 10*cos(x_truth[2]);
  v_truth[1] = 10*sin(x_truth[2]);
  x_truth[0] = v_truth[0].primitive() + x0[0];
  x_truth[1] = v_truth[1].primitive() + x0[1];

  RandTrajectory nx3(tdomain, dt, Interval(-0.03,0.03));
  TubeVector x({Tube(tdomain, dt), Tube(tdomain, dt), Tube(tdomain, dt)});
  TubeVector v({Tube(tdomain, dt), Tube(tdomain, dt), Tube(tdomain, dt)});
  Tube u(u_truth, dt);
  v[2] = u.inflate(0.03);
  x[2] = Tube(x_truth[2], dt);
  x[2].inflate(0.03);
  v[0] = 10 * cos(x[2]);
  v[1] = 10 * sin(x[2]);
  x = v.primitive() + x0;

  ContractorNetwork cn;
  Function g("x[4]", "sqrt((x[0]-x[2])^2+(x[1]-x[3])^2)");
  CtcFunction ctc_f(Function("x[3]", "v[3]", "u", "(v[0] - 10*cos(x[2]) ; v[1] - 10*sin(x[2]) ; v[2] - u)"));
  cn.add(ctc_f, {x, v, u.inflate(0.03)});
  cn.add(ctc::deriv, {x, v});
  cn.add(ctc::eval, {Interval(0),IntervalVector({{0,0}, {0,0}, {2,2}}),x,v});

  vector<IntervalVector> b_box;
  for (int i=0; i<b.size(); i++)
    b_box.push_back(IntervalVector(2));

  int b_id;
  for (double t = 2*dt; t < tdomain.ub(); t += 2*dt) {
    b_id = rand() % b.size();
    IntervalVector& p = cn.create_interm_var(IntervalVector(3));
    Interval& distance = cn.create_interm_var(g.eval(cart_prod(x_truth(t).subvector(0, 1), b[b_id]))).inflate(0.03);

    cn.add(ctc::dist, {cn.subvector(p, 0, 1), b_box[b_id], distance});
    cn.add(ctc::eval, {Interval(t), p, x, v});
  }

  vibes::beginDrawing();

  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100, 100, 600, 300);
  fig_map.axis_limits(-20,25,-10,15, true);
  fig_map.add_trajectory(&x_truth, "x*", 0, 1, "white");
  fig_map.add_tube(&x, "x", 0, 1);
  fig_map.smooth_tube_drawing(true);
  for (const auto& beacon : b)
    fig_map.add_beacon(beacon, 0.5);
  fig_map.show(1);
  cn.contract(true);
  for (const auto& beacon : b_box)
    fig_map.draw_box(beacon, "black[]");
  fig_map.show(1);

  vibes::endDrawing();
}
