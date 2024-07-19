#include <codac.h>
#include <vibes.h>

#include <unistd.h> // used for usleep

using namespace std;
using namespace codac;

int main()
{
  // Initial pose x0=(0,0,2)
  Vector x0({0,0,2});

  // Landmarks

  vector<IntervalVector> b = {{6, 12},{-2, -5},{-3, 20},{3, 4}};

  // Temporal domain [t0,tf]
  Interval tdomain(0.,15.);
  double dt = 0.05;

  // Initialize random seed
  double iteration_dt = 0.2; // elapsed animation time between each dt

  // System input
  Trajectory u_truth(tdomain, TFunction("3*(sin(t)^2)+t/100"), dt);

  // Add noise on u
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

  // Add noise on x3
  RandTrajectory nx3(tdomain, dt, Interval(-0.03,0.03));  

  // Create tubes x, v and u
  TubeVector x({Tube(tdomain, dt), Tube(tdomain, dt), Tube(tdomain, dt)});
  TubeVector v({Tube(tdomain, dt), Tube(tdomain, dt), Tube(tdomain, dt)});
  Tube u(u_truth, dt);
  v[2] = u.inflate(0.03);
  x[2] = Tube(x_truth[2], dt);
  x[2].inflate(0.03);
  v[0] = 10 * cos(x[2]);
  v[1] = 10 * sin(x[2]);
  x = v.primitive() + x0;

  // Create the contractor network
  ContractorNetwork cn;

  // Create the distance function
  Function g("x[4]", "sqrt((x[0]-x[2])^2+(x[1]-x[3])^2)");

  // Create the contractor
  CtcFunction ctc_f(Function("x[3]", "v[3]", "u", "(v[0] - 10*cos(x[2]) ; v[1] - 10*sin(x[2]) ; v[2] - u)"));

  // Add the contractors to the network
  cn.add(ctc_f, {x, v, u.inflate(0.03)});
  cn.add(ctc::deriv, {x, v});
  cn.add(ctc::eval, {Interval(0),IntervalVector({{0,0}, {0,0}, {2,2}}),x,v});

  // Create the box landmarks
  vector<IntervalVector> b_box;
  for (int i=0; i<b.size(); i++)
    b_box.push_back(IntervalVector(2));

  // // Add the landmarks to the network
  // int b_id;
  // for (double t = 2*dt; t < tdomain.ub(); t += 2*dt) {
  //   b_id = rand() % b.size();
  //   IntervalVector& p = cn.create_interm_var(IntervalVector(3));
  //   Interval& distance = cn.create_interm_var(g.eval(cart_prod(x_truth(t).subvector(0, 1), b[b_id]))).inflate(0.03);

  //   cn.add(ctc::dist, {cn.subvector(p, 0, 1), b_box[b_id], distance});
  //   cn.add(ctc::eval, {Interval(t), p, x, v});
  // }


  // Begin drawing
  vibes::beginDrawing();


  // Show the trajectory
  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100, 100, 600, 300);
  fig_map.axis_limits(-20, 25, -10, 15, true);
  fig_map.add_trajectory(&x_truth, "x*", 0, 1, "black");
  fig_map.add_tube(&x, "x", 0, 1);

  // Smooth the tube drawing
  fig_map.smooth_tube_drawing(true);

  // Add the beacons to the map
  for (const auto& beacon : b)
    fig_map.add_beacon(beacon, 0.5);

  // Draw the box landmarks
  for (const auto& beacon : b_box)
    fig_map.draw_box(beacon, "black[]");

  // Initialize the previous observation time
  double prev_t_obs = tdomain.lb();
  for(double t = tdomain.lb() ; t < tdomain.ub() ; t+=dt)
  {
    if(t - prev_t_obs > 2*dt) // new observation each 2*dt
    {
      // Creating new observation to a random landmark
      int b_id = rand() % b.size();
      IntervalVector& p = cn.create_interm_var(IntervalVector(3));
      Interval& distance = cn.create_interm_var(g.eval(cart_prod(x_truth(t).subvector(0, 1), b[b_id]))).inflate(0.03);

      // Adding related observation constraints to the network
      cn.add(ctc::dist, {cn.subvector(p, 0, 1), b_box[b_id], distance});
      cn.add(ctc::eval, {Interval(t), p, x, v});

      // Updated last iteration time
      prev_t_obs = t;

      // Contract the network
      cn.contract(true);
    }

    double contraction_dt = cn.contract_during(iteration_dt);
    usleep(max(0.,iteration_dt-contraction_dt)*1e6); // pause for the animation

    // Display the current slice [x](t)
    fig_map.draw_box(x(max(0.,ibex::previous_float(t))).subvector(0,1));
  }

  // Show the map
  fig_map.show(1);

  // End drawing
  vibes::endDrawing();

}
