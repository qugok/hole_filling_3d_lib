#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <sstream>
#include "DeloneLike.hpp"
#include "MeshController.hpp"

void refinement::DeloneLike::Process(std::vector<std::shared_ptr<Vertex>> &inside_hole,
                                     const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
                                     const BaseFillingStrategy::MeshController &m) const {

  std::unordered_map<std::shared_ptr<Vertex>, double> scale_factor;

  std::unordered_set<std::shared_ptr<Vertex>> all;
  all.insert(hole_boundary.begin(), hole_boundary.end());
  all.insert(inside_hole.begin(), inside_hole.end());

  std::unordered_set<std::shared_ptr<HalfEdge>> boundary;

  compute_scale_factor(scale_factor, inside_hole, hole_boundary);
  std::vector<std::weak_ptr<Triangle>> triangles_inside;
  for (const auto &v : all) {
    for (const auto &t : v->neighbor_triangles()) {
      if (all.find(t->a) == all.end() ||
          all.find(t->b) == all.end() ||
          all.find(t->c) == all.end()) {
        continue;
      }
      triangles_inside.emplace_back(t);
    }
  }
  for (int step = 0; step < limit; ++step) {
    if (not split_triangles(scale_factor, inside_hole, triangles_inside, m)) {
      return;
    };
    for (int swap_count = 0; swap_count < limit; ++swap_count) {
      boundary.clear();
      for (int i = 0; i < hole_boundary.size(); ++i) {
        boundary.insert(hole_boundary[(i + 1) % hole_boundary.size()]->find_edge_to(hole_boundary[i]));
        boundary.insert(hole_boundary[i]->find_edge_to(hole_boundary[(i + 1) % hole_boundary.size()]));
      }
      if (not swap_edges(m, triangles_inside, boundary)) {
        break;
      }
    }
  }
}
void refinement::DeloneLike::compute_scale_factor(std::unordered_map<std::shared_ptr<Vertex>, double> &scale_factor,
                                                  const std::vector<std::shared_ptr<Vertex>> &inside_hole,
                                                  const std::vector<std::shared_ptr<Vertex>> &hole_boundary) {
  std::unordered_set<std::shared_ptr<Vertex>> not_compute(hole_boundary.begin(), hole_boundary.end());
  not_compute.insert(inside_hole.begin(), inside_hole.end());
  std::queue<std::shared_ptr<Vertex>> to_compute;
  for (int i = 0; i < hole_boundary.size(); ++i) {
    auto &v = hole_boundary[i];
    double len = 0;
    int count = 0;
    for (const auto &cur_v : v->neighbor_vertices()) {
      if (not_compute.find(cur_v) != not_compute.end() and
          cur_v != hole_boundary[(i + 1) % hole_boundary.size()] and
          cur_v != hole_boundary[(i + hole_boundary.size() - 1) % hole_boundary.size()]) {
        to_compute.push(cur_v);
        continue;
      }
      len += (v->_coordinates - cur_v->_coordinates).abs();
      ++count;
    }
    scale_factor[v] = len / count;
  }
  while (not to_compute.empty()) {
    auto cur = to_compute.front();
    to_compute.pop();
    if (scale_factor.find(cur) != scale_factor.end()) {
      continue;
    }
    double len = 0;
    int count = 0;
    for (const auto &v : cur->neighbor_vertices()) {
      if (scale_factor.find(v) == scale_factor.end()) {
        to_compute.push(v);
        continue;
      }
      len += (v->_coordinates - cur->_coordinates).abs();
      ++count;
    }
    scale_factor[cur] = len / count;
  }
}
bool refinement::DeloneLike::split_triangles(std::unordered_map<std::shared_ptr<Vertex>, double> &scale_factor,
                                             std::vector<std::shared_ptr<Vertex>> &inside_hole,
                                             std::vector<std::weak_ptr<Triangle>> &triangles_inside,
                                             const BaseFillingStrategy::MeshController &m) {
  auto split_condition = [&scale_factor](const std::weak_ptr<Triangle> &wt) {
    if (wt.expired()) {
      return false;
    }
    auto t = wt.lock();
    auto new_p = t->barycenter();
    double new_sf = (
        scale_factor[t->a] +
            scale_factor[t->b] +
            scale_factor[t->c]) / 3;
    for (const auto &v : {t->a, t->b, t->c}) {
      if (alpha2 * (v->_coordinates - new_p).abs2() <= new_sf * new_sf) {
        return false;
      }
      if (alpha2 * (v->_coordinates - new_p).abs2() <= scale_factor[v] * scale_factor[v]) {
        return false;
      }
    }
    return true;
  };

  auto split_triangle = [&m, &triangles_inside, &inside_hole, &scale_factor](const std::shared_ptr<Triangle> &t) {
    auto a = t->a;
    auto b = t->b;
    auto c = t->c;
    auto new_p = std::make_shared<Vertex>(t->barycenter());
    m.remove_triangle(t);

    inside_hole.push_back(new_p);
    scale_factor[new_p] = (scale_factor[a] + scale_factor[b] + scale_factor[c]) / 3;

    triangles_inside.emplace_back(m.add_triangle(a, b, new_p));
    triangles_inside.rbegin()->lock()->ab->fix_opposite();
    triangles_inside.rbegin()->lock()->bc->fix_opposite();
    triangles_inside.rbegin()->lock()->ca->fix_opposite();
    triangles_inside.emplace_back(m.add_triangle(b, c, new_p));
    triangles_inside.rbegin()->lock()->ab->fix_opposite();
    triangles_inside.rbegin()->lock()->bc->fix_opposite();
    triangles_inside.rbegin()->lock()->ca->fix_opposite();
    triangles_inside.emplace_back(m.add_triangle(c, a, new_p));
    triangles_inside.rbegin()->lock()->ab->fix_opposite();
    triangles_inside.rbegin()->lock()->bc->fix_opposite();
    triangles_inside.rbegin()->lock()->ca->fix_opposite();
  };

  size_t fixed_size = triangles_inside.size();
  bool splitted = false;
  for (int i = 0; i < fixed_size; ++i) {
    if (split_condition(triangles_inside[i])) {
      split_triangle(triangles_inside[i].lock());
      splitted = true;
    }
  }
  return splitted;
}
bool refinement::DeloneLike::swap_edge(
    const BaseFillingStrategy::MeshController &m,
    std::vector<std::weak_ptr<Triangle>> &triangles_inside,
    const std::shared_ptr<HalfEdge> &e) {

  auto swap_condition = [](const std::shared_ptr<Triangle> &t, const std::shared_ptr<Vertex> &opposite) {
    auto centre = t->centre_circumscribed_circle();
    auto radius = (t->a->_coordinates - centre).abs2();
    auto opposote_dist = (opposite->_coordinates - centre).abs2();
    return radius > opposote_dist;
  };
  if (not e) {
    return false;
  }
  if (e->_opposite.expired()) {
    e->fix_opposite(); // это костыль, должно хватать fix_opposite в add_triangle
  }
  if (not e || e->_opposite.expired() || e->_owner.expired() || e->_opposite.lock()->_owner.expired()) {
    return false;
  }

  auto t = e->_owner.lock();
  auto opposite_e = e->_opposite.lock();
  auto opposite_t = opposite_e->_owner.lock();

  auto temp = std::tuple(&Triangle::a, &Triangle::b, &Triangle::c);
  if (e == t->ab)
    temp = std::tuple(&Triangle::b, &Triangle::c, &Triangle::a);
  if (e == t->bc)
    temp = std::tuple(&Triangle::c, &Triangle::a, &Triangle::b);
  if (e == t->ca)
    temp = std::tuple(&Triangle::a, &Triangle::b, &Triangle::c);

  auto[la, lb, lc] = temp;

  auto d = opposite_t->a;
  if (opposite_e == opposite_t->ab)
    d = opposite_t->c;
  if (opposite_e == opposite_t->bc)
    d = opposite_t->a;
  if (opposite_e == opposite_t->ca)
    d = opposite_t->b;
  if (not swap_condition(t, d) and not swap_condition(opposite_t, (*t).*lb))
    return false;

  m.remove_triangle(t);
  m.remove_triangle(opposite_t);

  triangles_inside.emplace_back(m.add_triangle((*t).*la, (*t).*lb, d));
  triangles_inside.rbegin()->lock()->ab->fix_opposite();
  triangles_inside.rbegin()->lock()->bc->fix_opposite();
  triangles_inside.rbegin()->lock()->ca->fix_opposite();
  triangles_inside.emplace_back(m.add_triangle((*t).*lb, (*t).*lc, d));
  triangles_inside.rbegin()->lock()->ab->fix_opposite();
  triangles_inside.rbegin()->lock()->bc->fix_opposite();
  triangles_inside.rbegin()->lock()->ca->fix_opposite();

  return true;
}
bool refinement::DeloneLike::swap_edges(
    const BaseFillingStrategy::MeshController &m,
    std::vector<std::weak_ptr<Triangle>> &triangles_inside,
    const std::unordered_set<std::shared_ptr<HalfEdge>> &boundary) {

  std::unordered_set<std::shared_ptr<HalfEdge>> edges_inside;
  for (const auto &wt : triangles_inside) {
    if (wt.expired()) {
      continue;
    }
    auto t = wt.lock();
    for (const auto &e : {t->ab, t->bc, t->ca}) {
      if (boundary.find(e) == boundary.end()) {
        edges_inside.insert(e);
      }
    }
  }
  bool swapped = false;
  for (const auto &e : edges_inside) {
    swapped |= swap_edge(m, triangles_inside, e);
  }
  return swapped;
}
refinement::DeloneLike::DeloneLike(size_t limit) : limit(limit) {}

std::string refinement::DeloneLike::get_all_params()  const {

  std::ostringstream out;
  out << "\t\tDeloneLike:\n\t\t";
  out << "alpha2:\t" << alpha2 << "\n\t\t";
  out << "limit:\t" << limit;
  return out.str();
}
