#include <codac.h>
#include <math.h>

using namespace std;
using namespace codac;

class MyCtc : public Ctc
{
  public:

    MyCtc(const std::vector<IntervalVector>& M_)
      : Ctc(2), // the contractor acts on 2d boxes
        M(M_)   // attribute needed later on for the contraction
    {

    }

    void contract(IntervalVector& a)
    {
      IntervalVector b(2, Interval::EMPTY_SET);
      for(const auto& m : M)
        b |= (a & m);
      a = b;
    }

  protected:

    const std::vector<IntervalVector> M;
};


int main()
{
  vector<IntervalVector> M;
  M.push_back(IntervalVector{{1.5}, {2.5}});
  M.push_back(IntervalVector{{3}, {1}});
  M.push_back(IntervalVector{{2}, {2}});
  M.push_back(IntervalVector{{2.5}, {3}});
  M.push_back(IntervalVector{{3.5}, {2}});
  M.push_back(IntervalVector{{4}, {1}});
  M.push_back(IntervalVector{{1.5}, {0.5}});
  for(auto& Mi : M)
    Mi.inflate(0.05);

  /******* Vérification du fonctionnement de MyCtc ********/

  vibes::beginDrawing();
  VIBesFigMap fig("Constellation");
  fig.set_properties(100,100,500,500);
  fig.axis_limits(1,4,0,3.5);

  IntervalVector a1{{1.25, 3}, {1.6, 2.75}};
  IntervalVector a2{{2, 3.5}, {0.6, 1.2}};
  IntervalVector a3{{1.1, 3.25}, {0.2, 1.4}};

  for(auto& Mi : M)
    fig.add_beacon(Mi.mid(), 0.1);

  fig.draw_box(a1, "blue[]");
  fig.draw_box(a2, "green[]");
  fig.draw_box(a3, "red[]");

  MyCtc ctc_constell(M);
  ContractorNetwork cn_test;

  cn_test.add(ctc_constell, {a1});
  cn_test.add(ctc_constell, {a2});
  cn_test.add(ctc_constell, {a3});

  cn_test.contract();

  cout << "[a1]: " << a1 << endl;
  cout << "[a2]: " << a2 << endl;
  cout << "[a3]: " << a3 << endl;

  fig.draw_box(a1, "blue[#0000FF40]");
  fig.draw_box(a2, "green[#00FF0040]");
  fig.draw_box(a3, "red[#FF000040]");

  fig.show();

  vibes::endDrawing();

  /******* Fin de la vérification ********/


  Vector x_truth({2.,1.,M_PI/6}); // actual state vector (pose)
  vector<IntervalVector> v_obs = DataLoader::generate_static_observations(x_truth, M, false);

  IntervalVector x = IntervalVector(x_truth);
  x[0].inflate(oo);
  x[1].inflate(oo);
  // Adding uncertainties on the measurements
  for(auto& obs : v_obs)
  {
    obs[0].inflate(0.02); // range
    obs[1].inflate(0.02); // bearing
  }

  // Association set (possible identities)
  vector<IntervalVector> m(v_obs.size(), IntervalVector(2));
  // unknown association for each observation

  // Contractors
  CtcFunction ctc_dist(Function("x", "d", "m", "m-d-x"));
  CtcFunction ctc_theta(Function("x", "y", "theta", "x+y-theta"));
  MyCtc ctc_asso(M); 

  ContractorNetwork cn;

  for(int i = 0 ; i < v_obs.size() ; i++) // for each measurement
  {
    Interval& theta = cn.create_interm_var(Interval());
    IntervalVector& d = cn.create_interm_var(IntervalVector(2));

    cn.add(ctc_dist, {x[0], d[0], m[i][0]});
    cn.add(ctc_dist, {x[1], d[1], m[i][1]});
    cn.add(ctc_theta, {x[2], v_obs[i][1], theta});
    cn.add(ctc::polar, {d, v_obs[i][0], theta});
    cn.add(ctc_asso, {m[i]});
  }

  cn.contract();

  vibes::beginDrawing();

  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100,100,500,500);
  fig_map.draw_vehicle(x_truth,0.5);
  for(const auto& Mi : M)
    fig_map.draw_box(Mi, "red[orange]");

  for(const auto& obs : v_obs) {
    fig_map.draw_pie(x_truth[0], x_truth[1], obs[0], x_truth[2]+obs[1]);
    fig_map.draw_pie(x_truth[0],x_truth[1], (Interval(0.1)|obs[0]), x_truth[2]+obs[1], "lightGray");
  }

  for(const auto& mi : m)
    if(mi.max_diam() <= 0.10001) // if identified
      fig_map.draw_circle(mi[0].mid(), mi[1].mid(), 0.02, "blue[blue]");

  fig_map.draw_box(x.subvector(0,1)); // displays ([x_1],[x_2])

  fig_map.axis_limits(fig_map.view_box(), true, 0.1);
  fig_map.show();

  vibes::endDrawing();
}


