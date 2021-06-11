//
// Created by alex on 01.05.2021.
//

#include <FillSmall.hpp>
#include <tools/Hole.hpp>
#include <algorithm>
#include <sstream>
#include <tools/MeshController.hpp>

std::vector<std::shared_ptr<Vertex>> initial_triangulation::FillSmall::ProcessHole(
    const TriangulationStrategy::Hole &h,
    const MeshController &m) const {
  auto v = h.get_vertexes();

  if (v.size() == 3){
    m.add_triangle(v[0], v[2], v[1]);
    return {};
  }

  std::vector<Point> ps;
  std::transform(v.begin(), v.end(), std::back_inserter(ps), [](const auto &v) { return v->_coordinates; });
  auto nv = std::make_shared<Vertex>(barycenter(ps));
  auto prev = *v.rbegin();
  for (const auto &cur : v) {
    m.add_triangle(prev, nv, cur);
    prev = cur;
  }
//  nv->_coordinates += nv->get_normal() / 100 / (nv->_coordinates - ps[0]).abs();
  return {nv};
}
std::string initial_triangulation::FillSmall::get_all_params() const {

  std::ostringstream out;
  out << "FillSmall: ";
  return out.str();
}
