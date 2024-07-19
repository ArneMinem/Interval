#include <codac.h>
#include <vibes.h>
#include <codac-rob.h>

using namespace std;
using namespace codac;

int main()
{
  double dt = 0.05;
  Interval tdomain(0.,6.); // temporal domain [t0,tf]
  TrajectoryVector actual_x(tdomain, TFunction("(10*cos(t) + t; 5*sin(2*t)+t; atan2((10*cos(2*t)+1),(-10*sin(t)+1)); sqrt(sqr(-10*sin(t)+1)+sqr(10*cos(2*t)+1)))"), dt);

  // Creating random map of landmarks
  int nb_landmarks = 150;
  IntervalVector map_area(actual_x.codomain().subvector(0,1));
  map_area.inflate(2);
  vector<IntervalVector> v_map = DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);

  // Generating observations obs=(t,range,bearing) of these landmarks
  int max_nb_obs = 20;
  Interval visi_range(0,4); // [0m,75m]
  Interval visi_angle(-M_PI/4,M_PI/4); // frontal sonar
  vector<IntervalVector> v_obs = DataLoader::generate_observations(actual_x, v_map, max_nb_obs, true, visi_range, visi_angle);
  for (auto& obs : v_obs) {
    obs[1].inflate(0.1);
    obs[2].inflate(0.04);
  }

  TubeVector x(tdomain, dt, 4);                     // 4d tube for state vectors
  TubeVector v(tdomain, dt, 4);                     // 4d tube for derivatives of the states
  TubeVector u(tdomain, dt, 2);                     // 2d tube for inputs of the system

  Trajectory measured_psi(tdomain, TFunction("atan2((10*cos(2*t)+1),(-10*sin(t)+1))"), dt);
  Trajectory measured_speed(tdomain, TFunction("sqrt(sqr(-10*sin(t)+1)+sqr(10*cos(2*t)+1))"), dt);
  measured_psi.make_continuous();
  x[2] = Tube(actual_x[2], dt).inflate(0.01);      // measured_psi is a set of measurements
  x[3] = Tube(measured_speed, dt).inflate(0.01);

  ContractorNetwork cn;

  Function cplus("x", "y", "th", "x+y-th");
  Function cmoins("x", "m", "d", "m-x-d");
  CtcFunction ctc_th(cplus);
  CtcFunction ctc_d(cmoins);
  CtcConstell ctc_constell(v_map);
  cn.add(ctc::polar, {v[0], v[1], x[3], x[2]}); // v0 = x2cos(x3); v1 = x2sin(x3)
  cn.add(ctc::deriv, {x, v}); // dx = v

  // cn.add(ctc_eq, {v[2], u[0]}); // v2 = u0
  // cn.add(ctc_eq, {v[3], u[1]}); // v3 = u1
  for (int i = 0; i < v_obs.size(); i++) {
    IntervalVector& p = cn.create_interm_var(IntervalVector(4)); // to evaluate x(ti)
    Interval& theta = cn.create_interm_var(Interval()); // range-bearing : theta = x3 + y2
    IntervalVector& d = cn.create_interm_var(IntervalVector({{-oo, oo}, {-oo, oo}})); // to evaluate d0=y1cos(theta); d1=y1sin(theta)
    IntervalVector& c = cn.create_interm_var(IntervalVector({{-oo, oo}, {-oo, oo}})); // to evaluate constellation

    cn.add(ctc_th, {p[2], v_obs[i][2], theta}); // theta = x3 + y2
    cn.add(ctc::eval, {v_obs[i][0], p, x, v}); // pi = x(ti)
    cn.add(ctc_d, {p[0], c[0], d[0]}); // m1 - x1 = d0
    cn.add(ctc_d, {p[1], c[1], d[1]}); // m2 - x2 = d1
    cn.add(ctc::polar, {d[0], d[1], v_obs[i][1], theta}); // d0=y1cos(theta); d1=y1sin(theta)
    cn.add(ctc_constell, {c}); // ID a beacon
  }

  cn.contract(true);
  vibes::beginDrawing();

  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100, 100, 600, 300);
  fig_map.axis_limits(-20,25,-10,15, true);
  fig_map.add_trajectory(&actual_x, "x*", 0, 1);
  fig_map.add_tube(&x, "x", 0, 1);
  fig_map.add_observations(v_obs, &actual_x); // drawing obs
  for (const auto& b : v_map) {
    fig_map.add_beacon(b.mid(), 0.1);
  }
  fig_map.smooth_tube_drawing(true);
  fig_map.show(1);

  vibes::endDrawing();
}
