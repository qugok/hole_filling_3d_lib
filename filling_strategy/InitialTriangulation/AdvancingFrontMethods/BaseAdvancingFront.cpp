#include <FillSmall.hpp>
#include <Hole.hpp>
#include <MinAreaTriangulation.hpp>
#include <sstream>
#include "BaseAdvancingFront.hpp"
#include "cmath"
#include "defines.hpp"
#include "DeloneLike.hpp"

initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseAdvancingFront(
    size_t limit, double coef)
    : limit(limit), new_edge_coef(coef) {}

std::vector<std::shared_ptr<Vertex>> initial_triangulation::advancing_front::base::BaseAdvancingFront::process_hole_front(
    const BaseFillingStrategy::Hole &h,
    const BaseFillingStrategy::MeshController &m,
    initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront &f) const {

  f.fill(limit);
  MinAreaTriangulation small;
  auto v = f.get_vertecies_inside();
#ifndef DISABLE_SMALL
  auto temp = small.ProcessHole(Hole(f), m);
  refinement::DeloneLike delone(5);
  delone.Process(temp, Hole(f).get_vertexes(), m);
  v.insert(v.end(), temp.begin(), temp.end());
#endif
  return v;
}

std::vector<std::shared_ptr<Vertex>>
initial_triangulation::advancing_front::base::BaseAdvancingFront::ProcessHole(
    const TriangulationStrategy::Hole &h,
    const BaseFillingStrategy::MeshController &m) const {
  BaseFront f(h, m, new_edge_coef);
  return process_hole_front(h, m, f);
}
std::string initial_triangulation::advancing_front::base::BaseAdvancingFront::get_all_params()  const {
  std::ostringstream out;
  out << "\tBaseAdvancingFront:\n\t\t";
  out << "limit: "<< limit << "\n\t\t";
  out << "new_edge_coef: "<< new_edge_coef << "\n\t\t";
  out << "vertexes_count_end: "<< BaseFront::vertexes_count_end << "\n\t\t";
  out << "min_edge_len: " << BaseFront::min_edge_len;
  return out.str();
}

double initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::Element::calc_angle() const {
  auto ans = get_angle(*v, -Vector(*next->v));
  double sum_angles = 0;
  auto ver = v->_end.lock();
  auto triangles = ver->neighbor_triangles();
  std::for_each(triangles.begin(),
                triangles.end(),
                [&sum_angles, &ver](const auto &t) { sum_angles += t->angle(ver); });
  auto q1 = 2 * M_PI - ans;
  auto q2 = sum_angles + ans;
//  if (sum_angles + ans <= 2 * M_PI){
  if (sum_angles <= M_PI) {
    return 2 * M_PI - ans;
  }
  return ans;
}

Vector initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::Element::calc_hole_normal(
    size_t n) const {
  Vector left;
  std::shared_ptr<Element> cur = next;
  for (int i = 0; i < n; ++i) {
    left += (cur->v->_end.lock()->_coordinates - v->_end.lock()->_coordinates);
    cur = cur->next;
  }
  left = left.norm();

  Vector right;
  cur = prev;
  for (int i = 0; i < n; ++i) {
    right += (cur->v->_end.lock()->_coordinates - v->_end.lock()->_coordinates);
    cur = cur->prev;
  }
  right = right.norm();

  return cross_product(right, left).norm();
}

bool initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::Element::operator<(
    const BaseAdvancingFront::BaseFront::Element &other) const {
  return calc_angle() < other.calc_angle();
}

initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::BaseFront(
    const TriangulationStrategy::Hole &h,
    const TriangulationStrategy::MeshController &m,
    double new_edge_coef)
    : m(m), new_edge_coef(new_edge_coef) {
  std::vector<std::shared_ptr<Element>> temp;
  std::shared_ptr<Element> start = std::make_shared<Element>();
  std::shared_ptr<Element> cur = start;

  for (const auto &e : h) {
    cur->next = std::make_shared<Element>();
    cur->next->v = e;
    edge_min_len += Vector(*e).abs();
    cur->next->prev = cur;
    cur = cur->next;
    temp.push_back(cur);
  }
  edge_min_len /= temp.size();
  if (not min_edge_len){
    edge_min_len = 0;
  }
  cur->next = start->next;
  start->next->prev = cur;
  avg_area = 0;
  for (auto &e : temp) {
    avg_area += e->v->_owner.lock()->area();
    elems.insert(e);
  }
  avg_area /= temp.size();

}
bool initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::step() {
  if (check_is_full()) {
    return false;
  }
//  for(const auto& e : elems){
//    std::cout << e->calc_angle() << "\t";
//  }
//  std::cout << "\n";
  std::shared_ptr<Element> e = *elems.begin();
  std::shared_ptr<Element> prev = e->prev;
  std::shared_ptr<Element> next = e->next;
  elems.erase(e);

  elems.erase(prev); // чтобы сортировка в set не ломалась
  elems.erase(next);
  auto temp = e->calc_angle();
  if (e->calc_angle() <= (75. * M_PI / 180.)) {
//    std::cout << "0\n";
    zero_vertex_case(prev, e, next);
  } else if (e->calc_angle() >= (135. * M_PI / 180.)) {
//    std::cout << "2\n";
    two_vertex_case(prev, e, next);
  } else {
//    std::cout << "1\n";
    one_vertex_case(prev, e, next);
  }

  return true;
}

void initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::zero_vertex_case(
    const std::shared_ptr<Element> &prev,
    const std::shared_ptr<Element> &cur,
    const std::shared_ptr<Element> &next) {
  auto pv = prev->v->_end.lock();
  auto cv = cur->v->_end.lock();
  auto nv = next->v->_end.lock();
  auto n = m.add_triangle(nv, cv, pv);
  next->prev = prev;
  next->v = n->ca;
  prev->next = next;

  elems.insert(next);
  elems.insert(prev);
}
void initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::one_vertex_case(
    const std::shared_ptr<Element> &prev,
    const std::shared_ptr<Element> &cur,
    const std::shared_ptr<Element> &next) {
  auto pv = prev->v->_end.lock();
  auto cv = cur->v->_end.lock();
  auto nv = next->v->_end.lock();
  Vector v1 = Point(*pv) - (*cv);
  Vector v2 = Point(*nv) - (*cv);
  auto new_v = std::make_shared<Vertex>(compute_new_vertex(
      cur,
      bisector(v1, v2) * get_edge_len(v1.abs(), v2.abs())));
  vertices_inside_hole.push_back(new_v);

  auto f = m.add_triangle(new_v, cv, pv);
  auto s = m.add_triangle(new_v, nv, cv);

  cur->v = f->ca;
  next->v = s->ab;

// for debug
//  auto q1 = prev->calc_angle();
//  auto q2 = cur->calc_angle();
//  auto q3 = next->calc_angle();

  elems.insert(prev);
  elems.insert(cur);
  elems.insert(next);
}
void initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::two_vertex_case(
    const std::shared_ptr<Element> &prev,
    const std::shared_ptr<Element> &cur,
    const std::shared_ptr<Element> &next) {

  auto pv = prev->v->_end.lock();
  auto cv = cur->v->_end.lock();
  auto nv = next->v->_end.lock();
  Vector v1 = Point(*pv) - (*cv);
  Vector v4 = Point(*nv) - (*cv);
  auto[v2, v3] = trisector(v1, v4);
  auto p2 = std::make_shared<Vertex>(
      compute_new_vertex(cur, v2 * get_edge_len(v1.abs(), v4.abs())));
  auto p3 = std::make_shared<Vertex>(
      compute_new_vertex(cur, v3 * get_edge_len(v1.abs(), v4.abs())));

  vertices_inside_hole.push_back(p2);
  vertices_inside_hole.push_back(p3);

  auto t1 = m.add_triangle(p2, cv, pv);
  auto t2 = m.add_triangle(p3, cv, p2);
  auto t3 = m.add_triangle(p3, nv, cv);

  auto new_el = std::make_shared<Element>();

  cur->v = t1->ca;
  cur->next = new_el;

  new_el->v = t2->ca;
  new_el->prev = cur;
  new_el->next = next;

  next->v = t3->ab;
  next->prev = new_el;

  elems.insert(prev);
  elems.insert(cur);
  elems.insert(new_el);
  elems.insert(next);
}
bool initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::check_is_full() {
//  return elems.size() < 6;
  if (elems.size() > vertexes_count_end) {
    return false;
  }
  std::vector<Point> v;
  v.reserve(elems.size());
  for (const auto &e : elems) {
    v.push_back(e->v->_end.lock()->_coordinates);
  }
  double areaa = 0;
  Point centre = barycenter(v);
  Point prev = v.back();
  for (auto cur : v) {
    areaa += area(prev, centre, cur);
    prev = cur;
  }
  if (areaa < avg_area * 50) {
    return true;
  }
  return false;
}
initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::operator Hole() {
  auto cur = *elems.begin();
  Hole h(cur->v);
  cur = cur->next;
  while (cur != *elems.begin()) {
    h.insert_next(cur->v);
    cur = cur->next;
  }
  return h;
}
void initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::fill(
    size_t limited) {
  for (size_t i = 0; i < limited; ++i) {
    if (not step()) {
      break;
    }
  }
}

std::vector<std::shared_ptr<Vertex>>
initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::get_vertecies_inside() const {
  return vertices_inside_hole;
}

Point initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::compute_new_vertex(
    const std::shared_ptr<Element> &e, const Vector &d) {
  return e->v->_end.lock()->_coordinates + d;
}
double initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::get_edge_len(
    double left,
    double right) {
  return std::max((left + right) / new_edge_coef, edge_min_len);
}

bool initial_triangulation::advancing_front::base::BaseAdvancingFront::BaseFront::cmp::operator()(
    const std::shared_ptr<Element> &a,
    const std::shared_ptr<Element> &b) const {
  return *a < *b;
}