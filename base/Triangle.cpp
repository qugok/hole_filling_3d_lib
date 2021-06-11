#include <cmath>
#include "base.hpp"

void Triangle::fill(const std::shared_ptr<Vertex> &va,
                    const std::shared_ptr<Vertex> &vb,
                    const std::shared_ptr<Vertex> &vc,
                    Vector vn) {
  this->a = va;
  this->b = vb;
  this->c = vc;
  if (vn == Vector()) {
    this->n = normal_to_triangle(*a, *b, *c);
    if (std::isnan(this->n.x) or std::isnan(this->n.x) or std::isnan(this->n.x)) {
      std::cout << "we have problems\n";
    }
  } else {
    this->n = vn;
  }

  ab = std::make_shared<HalfEdge>(a, b, b->find_edge_to(a), shared_from_this());
  bc = std::make_shared<HalfEdge>(b, c, c->find_edge_to(b), shared_from_this());
  ca = std::make_shared<HalfEdge>(c, a, a->find_edge_to(c), shared_from_this());
  ab->fix_opposite();
  bc->fix_opposite();
  ca->fix_opposite();
  this->a->add_edge(ab);
  this->b->add_edge(bc);
  this->c->add_edge(ca);
}
Triangle::Triangle() : a(std::make_shared<Vertex>()), b(std::make_shared<Vertex>()), c(std::make_shared<Vertex>()) {}

std::ostream &operator<<(std::ostream &out, const Triangle &t) {
  out << "A: " << *t.a << "\tB: " << *t.b << "\tC: " << *t.c;
  return out;
}
std::istream &operator>>(std::istream &in, Triangle &t) {
  Vertex ta, tb, tc;
  in >> ta >> tb >> tc;
  t.a = std::make_shared<Vertex>(ta);
  t.b = std::make_shared<Vertex>(tb);
  t.c = std::make_shared<Vertex>(tc);
  return in;
}
Triangle::Triangle(const Triangle &t) : enable_shared_from_this(t) {
  a = t.a;
  b = t.b;
  c = t.c;
  n = t.n;
  ab = t.ab;
  bc = t.bc;
  ca = t.ca;
}
std::shared_ptr<Triangle> Triangle::from(const std::shared_ptr<Vertex> &a,
                                         const std::shared_ptr<Vertex> &b,
                                         const std::shared_ptr<Vertex> &c,
                                         Vector n) {
  auto t = std::make_shared<Triangle>();
  t->fill(a, b, c, n);
  return t;
}
Point Triangle::barycenter() {
  return ::barycenter({a->_coordinates, b->_coordinates, c->_coordinates});
}

Point Triangle::centre_circumscribed_circle() {
//  auto ra = this->a->_coordinates;
//  auto rb = this->b->_coordinates;
//  auto rc = this->c->_coordinates;
//  auto alpha_a = (rb-rc).abs2() * inner_product(ra - rb, ra - rc) / 8 / area() / area();
//  auto alpha_b = (ra-rc).abs2() * inner_product(rb - rc, rb - ra) / 8 / area() / area();
//  auto alpha_c = (rb-ra).abs2() * inner_product(rc - ra, rc - rb) / 8 / area() / area();
//  return alpha_a * ra + alpha_b * rb + alpha_c *rc;
  auto ra = this->a->_coordinates;
  auto rb = this->b->_coordinates;
  auto rc = this->c->_coordinates;
  auto a2 = (rb - rc).abs2();
  auto b2 = (ra - rc).abs2();
  auto c2 = (rb - ra).abs2();
  auto alpha_a = a2 * (b2 + c2 - a2);
  auto alpha_b = b2 * (a2 + c2 - b2);
  auto alpha_c = c2 * (a2 + b2 - c2);
  return (alpha_a * ra + alpha_b * rb + alpha_c * rc) / (alpha_a + alpha_b + alpha_c);
}
double Triangle::area() {
  return ::area(a->_coordinates, b->_coordinates, c->_coordinates);
}

std::shared_ptr<Vertex> Triangle::get_opposite(const std::shared_ptr<HalfEdge> &e) const {
  auto d = std::shared_ptr<Vertex>();
  if (e == ab)
    d = c;
  if (e == bc)
    d = a;
  if (e == ca)
    d = b;
  return d;
}
double Triangle::angle(const std::shared_ptr<Vertex> &v) {
  if (a == v) {
    return get_angle(*ca, -Vector(*ab));
  }
  if (b == v) {
    return get_angle(*ab, -Vector(*bc));
  }
  if (c == v) {
    return get_angle(*bc, -Vector(*ca));
  }
  return 0;
}
