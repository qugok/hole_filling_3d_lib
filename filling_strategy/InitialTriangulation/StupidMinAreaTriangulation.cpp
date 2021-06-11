

#include "StupidMinAreaTriangulation.hpp"
#include <tools/Hole.hpp>
#include <sstream>
#include <tools/MeshController.hpp>

std::vector<std::shared_ptr<Vertex>> initial_triangulation::StupidMinAreaTriangulation::ProcessHole(
    const TriangulationStrategy::Hole &h,
    const MeshController &m) const {
  auto v = h.get_vertexes();
  std::vector<Point> ps;
  std::transform(v.begin(), v.end(), std::back_inserter(ps), [](const auto &v) { return v->_coordinates; });

  double min_square = get_square_of_triangulation(h, ps, ps[0]);
  int index = 0;
  for (int i = 1; i < ps.size(); ++i) {
    double cur_square = get_square_of_triangulation(h, ps, ps[i]);
    if (min_square > cur_square) {
      min_square = cur_square;
      index = i;
    }
  }
  std::vector<std::shared_ptr<Triangle>> hole_triangles;
  std::vector<std::shared_ptr<HalfEdge>> edges_inside_hole;
  std::vector<std::shared_ptr<Vertex>> vertices_inside_hole;
  std::unordered_map<std::shared_ptr<Vertex>, double> scale_factor;
  std::unordered_map<std::shared_ptr<Vertex>, int> count;

  auto prev = *v.rbegin();
  for (const auto &cur : v) {
    if (prev == v[index] || cur == v[index]) {
      prev = cur;
      continue;
    }
    auto last = m.add_triangle(prev, v[index], cur); // a , b, c
    hole_triangles.push_back(last);
    if (v[(index + 1) % v.size()] != prev) {
      edges_inside_hole.push_back(last->ab);
      scale_factor[last->a] += Vector(*last->ab).abs();
      ++count[last->a];
      scale_factor[last->b] += Vector(*last->ab).abs();
      ++count[last->b];
    }
    if (v[(index - 1 + v.size()) % v.size()] != cur) {
      edges_inside_hole.push_back(last->bc);
      scale_factor[last->b] += Vector(*last->bc).abs();
      ++count[last->b];
      scale_factor[last->c] += Vector(*last->bc).abs();
      ++count[last->c];
    }
    prev = cur;
  }
  for (const auto&[v, c] : count) {
    scale_factor[v] /= c;
  }

  return {};
  for (const auto &t : hole_triangles) {
    if (split_condition(scale_factor, t)) {
      vertices_inside_hole.push_back(split_triangle(m, edges_inside_hole, t));
    }
  }
//  return;

  for (int i = 0; i < edges_inside_hole.size(); ++i) {
    const auto &e = edges_inside_hole[i];
    auto temp = swap_edge(m, e);
    edges_inside_hole.insert(edges_inside_hole.end(), temp.begin(), temp.end());
  }
//  for (const auto &e : edges_inside_hole) {
//    auto temp = swap_edge(m, e);
////    edges_inside_hole.insert(edges_inside_hole.end(), temp.begin(), temp.end());
//  }
  return vertices_inside_hole;
}
double initial_triangulation::StupidMinAreaTriangulation::get_square_of_triangulation(
    const TriangulationStrategy::Hole &h,
    const std::vector<Point> &ps,
    const Point &point) {
  double ans = 0;
  auto prev = *ps.rbegin();
  for (const auto &p : ps) {
    ans += area(point, p, prev);
    prev = p;
  }
  return ans;
}
std::shared_ptr<Vertex> initial_triangulation::StupidMinAreaTriangulation::split_triangle(
    const TriangulationStrategy::MeshController &m,
    std::vector<std::shared_ptr<HalfEdge>> &edges_inside_hole,
    const std::shared_ptr<Triangle> &t) {
  auto a = t->a;
  auto b = t->b;
  auto c = t->c;
  auto new_p = std::make_shared<Vertex>(t->barycenter() + Point(0, 0, 0));
  m.remove_triangle(t);
  auto last = m.add_triangle(a, b, new_p);
  edges_inside_hole.push_back(last->ab);
  edges_inside_hole.push_back(last->bc);
  edges_inside_hole.push_back(last->ca);

  last = m.add_triangle(b, c, new_p);
  edges_inside_hole.push_back(last->ab);
  edges_inside_hole.push_back(last->bc);
  edges_inside_hole.push_back(last->ca);

  last = m.add_triangle(c, a, new_p);
  edges_inside_hole.push_back(last->ab);
  edges_inside_hole.push_back(last->bc);
  edges_inside_hole.push_back(last->ca);
  return new_p;
}
bool initial_triangulation::StupidMinAreaTriangulation::split_condition(
    std::unordered_map<std::shared_ptr<Vertex>, double> &scale_factor,
    const std::shared_ptr<Triangle> &t) {
  auto points = {t->a, t->b, t->c};
  auto new_p = t->barycenter();
  double new_p_scale_factor = (scale_factor[t->a] + scale_factor[t->b] + scale_factor[t->c]) / 3;
  for (const auto &v : points) {
    if (2 * (v->_coordinates - new_p).abs2() <= new_p_scale_factor) {
      return false;
    }
    if (2 * (v->_coordinates - new_p).abs2() <= scale_factor[v]) {
      return false;
    }
  }
  return true;
}
// хотим получать новые рёбра что надо тоже обработать
std::vector<std::shared_ptr<HalfEdge>> initial_triangulation::StupidMinAreaTriangulation::swap_edge(const TriangulationStrategy::MeshController &m,
                                                               const std::shared_ptr<HalfEdge> &e) {
  auto swap_condition = [](const std::shared_ptr<Triangle> &t, const std::shared_ptr<Vertex> &opposite) {
    auto centre = t->centre_circumscribed_circle();
    auto radius = (t->a->_coordinates - centre).abs2();
    auto opposote_dist = (opposite->_coordinates - centre).abs2();
    return radius > opposote_dist;
  };
  // изначально связаны  ac, а мы хотим проверить можно ли связать bd
  // для этого мы хотим чтобы ближайшая к ac точка из bd была на отрезке и ближайшая к bd точка из ac была на отрезке
  auto visual_condition = [](const Point &a, const Point &b, const Point &c, const Point &d) {
    if (((a - Point({-29.586500, -5.085510, 38.133202})).abs() < 1e-4
        && (b - Point({-29.543100, -12.052900, 39.133202})).abs() < 1e-4
        && (c - Point({-30.178900, -14.500500, 38.133202})).abs() < 1e-4
        && (d - Point({-28.863800, -16.572701, 38.133202})).abs() < 1e-4) ||
        ((c - Point({-29.586500, -5.085510, 38.133202})).abs() < 1e-4
            && (d - Point({-29.543100, -12.052900, 39.133202})).abs() < 1e-4
            && (a - Point({-30.178900, -14.500500, 38.133202})).abs() < 1e-4
            && (b - Point({-28.863800, -16.572701, 38.133202})).abs() < 1e-4)) {
      std::cout << "our triangles \n";
    }
    auto[p1, p2] = nearest_points_of_lines(a, c, b, d);
    return point_on_segment(p2, b, d) and point_on_segment(p1, a, c);
  };
  if (((e->_start.lock()->_coordinates - Point({-29.5865, -5.08551, 38.1332})).abs() < 1e-4
      && (e->_end.lock()->_coordinates - Point({-30.178900, -14.500500, 38.133202})).abs() < 1e-4) ||
      ((e->_end.lock()->_coordinates - Point({-29.5865, -5.08551, 38.1332})).abs() < 1e-4
          && (e->_start.lock()->_coordinates - Point({-30.178900, -14.500500, 38.133202})).abs() < 1e-4)) {
    std::cout << "interest edge is in progress " << Vector(*e) << "\n";
  }
  if (not e || e->_opposite.expired() || e->_owner.expired() || e->_opposite.lock()->_owner.expired()) {
    std::cout << "edge is invalid\n";
    return {};
  }

  std::cout << "edge is valid\n";

  auto t = e->_owner.lock();
  auto opposite_e = e->_opposite.lock();
  auto opposite_t = opposite_e->_owner.lock();

//  return;
  auto temp = std::tuple(&Triangle::a, &Triangle::b, &Triangle::c);
  if (e == t->ab)
    temp = std::tuple(&Triangle::b, &Triangle::c, &Triangle::a);
  if (e == t->bc)
    temp = std::tuple(&Triangle::c, &Triangle::a, &Triangle::b);
  if (e == t->ca)
    temp = std::tuple(&Triangle::a, &Triangle::b, &Triangle::c);

//  return;
  auto[la, lb, lc] = temp;

//  return;

  auto d = opposite_t->a;
  if (opposite_e == opposite_t->ab)
    d = opposite_t->c;
  if (opposite_e == opposite_t->bc)
    d = opposite_t->a;
  if (opposite_e == opposite_t->ca)
    d = opposite_t->b;

  if (not swap_condition(t, d) and not swap_condition(opposite_t, (*t).*lb))
    return {};
  if (not visual_condition(((*t).*la)->_coordinates,
                           ((*t).*lb)->_coordinates,
                           ((*t).*lc)->_coordinates,
                           d->_coordinates)) {
    std::cout << "Hello\n";
    return {};
  }
  std::cout << "OK\n";
//  return;

  std::vector<std::shared_ptr<HalfEdge>> invalided_edges;

  m.remove_triangle(t);
  m.remove_triangle(opposite_t);
  auto tmp = m.add_triangle((*t).*la, (*t).*lb, d);
  invalided_edges.push_back(tmp->ab);
  invalided_edges.push_back(tmp->ca);
  invalided_edges.push_back(tmp->bc);
  tmp = m.add_triangle((*t).*lb, (*t).*lc, d);
  invalided_edges.push_back(tmp->ab);
  invalided_edges.push_back(tmp->bc);
  invalided_edges.push_back(tmp->ca);
  return invalided_edges;
}
std::string initial_triangulation::StupidMinAreaTriangulation::get_all_params()  const {

  std::ostringstream out;
  out << "StupidMinAreaTriangulation: ";
  return out.str();
}
