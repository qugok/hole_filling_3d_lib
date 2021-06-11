

#include "MinAreaTriangulation.hpp"
#include <tools/Hole.hpp>
#include <sstream>
#include <tools/MeshController.hpp>

std::vector<std::shared_ptr<Vertex>> initial_triangulation::MinAreaTriangulation::ProcessHole(
    const TriangulationStrategy::Hole &h,
    const MeshController &m) const {
  auto v = h.get_vertexes();
  std::vector<Point> ps;
  std::transform(v.begin(), v.end(), std::back_inserter(ps), [](const auto &v) { return v->_coordinates; });
  auto get_p = [&ps](size_t ind){return ps[ind % ps.size()];};
  auto get_v = [&v](size_t ind){return v[ind % v.size()];};

  std::vector<std::vector<double>> d(ps.size(), std::vector<double> (ps.size()));
  std::vector<std::vector<int>> ans(ps.size(), std::vector<int> (ps.size()));
  for (int i = 0; i < d.size(); ++i) {
    d[i][0] = 0;
    d[i][1] = 0;
    d[i][2] = area(get_p(i), get_p(i+1), get_p(i+2));
    ans[i][2] = i + 1;
  }
  for (int i = 3; i < d[0].size(); ++i) {
    for (int j = 0; j < d.size(); ++j) {
      int k = j + i;
      d[j][i] = d[j][i-1] + area(get_p(j), get_p(k - 1), get_p(k));
      ans[j][i] = k - 1;
      for (int mi = j + 1; mi < k; ++mi) {
        double new_area = d[j][(mi - j)%ps.size()] + area(get_p(j), get_p(mi), get_p(k))  + d[mi%ps.size()][k - mi];
        if (new_area < d[j][i]){
          d[j][i] = new_area;
          ans[j][i] = mi%ps.size();
        }
      }

    }
  }
  std::function<void(int,int)> add_triangles;
  add_triangles = [&ans, &v, &get_v, &m, &add_triangles](size_t i, size_t shift) {
    if (shift < 2){
      return;
    }
    m.add_triangle(get_v(i), get_v(i + shift), get_v(ans[i][shift]));
    add_triangles(i, (ans[i][shift] - i + v.size())%v.size());
    add_triangles(ans[i][shift], (i + shift - ans[i][shift] + v.size())%v.size());
  };
  add_triangles(0, v.size() - 1);
  return {};
}
std::string initial_triangulation::MinAreaTriangulation::get_all_params()  const {
  std::ostringstream out;
  out << "\tMinAreaTriangulation: ";
  return out.str();
}
