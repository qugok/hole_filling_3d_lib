#include "Mesh.hpp"
#include "BaseOStream.hpp"
#include "OffIStream.hpp"
#include <unordered_map>
#include <set>
#include <algorithm>
#include <tools/MeshController.hpp>
#include <string>
#include <fstream>
#include <StlBinaryIstream.hpp>
#include <StlBinaryOstream.hpp>
#include <string_tools.hpp>
#include <OffOStream.hpp>

std::ostream &operator<<(std::ostream &out, const Mesh &m) {
  for (const auto& i : m._triangles) {
    out << *i << "\n";
  }
  return out;
}
std::istream &operator>>(std::istream &in, Mesh &m) {
  int n;
  in >> n;
  for (int i = 0; i < n; ++i) {
    Triangle temp;
    in >> temp;
    m._triangles.push_back(std::make_shared<Triangle>(temp));
  }
  return in;
}
Mesh::Mesh() : name("unnamed") {}

void Mesh::normalize() {
  std::unordered_map<Point, std::shared_ptr<Vertex>> v;

  auto find_vertex = [&v](const Vertex &vc) {
    if (v.find(vc._coordinates) == v.end()) {
      v[vc._coordinates] = std::make_shared<Vertex>(vc._coordinates);
    }
    return v[vc._coordinates];
  };

  std::vector<std::shared_ptr<Triangle>> t;
  for (const auto &tr : _triangles) {
    auto a = find_vertex(*tr->a);
    auto b = find_vertex(*tr->b);
    auto c = find_vertex(*tr->c);
    t.push_back(Triangle::from(a, b, c, tr->n));
  }
  _triangles = t;
}
bool Mesh::find_whole_hole_boundary(Mesh::Hole &hole, std::unordered_set<std::shared_ptr<HalfEdge>> &e) {
  hole.boundary_start->next = nullptr;
  std::shared_ptr<Hole::Element> current = hole.boundary_start;
  if (not current) {
    return false;
  }
  do {
    if (current->next) {
      current = current->next;
    }
    if (not current->next) {
      current->set_next(current->elem->_end.lock()->find_next_boundary_edge(current->elem));
      if (e.find(current->next->elem) == e.end()) {
        throw std::domain_error("found edge is used in other hole");
      }
      e.erase(current->next->elem);
    }
  } while (current->next->elem != hole.boundary_start->elem and not e.empty());
  if (current->next->elem == hole.boundary_start->elem) {
    current->next = hole.boundary_start;
    return true;
  }
  return false;
}
void Mesh::find_holes(std::vector<Hole> &holes) {
  std::unordered_set<std::shared_ptr<HalfEdge>> boundary_edges;
  auto compute_edge = [&boundary_edges](const std::shared_ptr<HalfEdge> &e) {
    if (not e->has_opposite()) {
      boundary_edges.insert(e);
    }
  };

  for (const auto &t : _triangles) {
    compute_edge(t->ab);
    compute_edge(t->bc);
    compute_edge(t->ca);
  }

  while (not boundary_edges.empty()) {
    Hole h(*boundary_edges.begin());
//    boundary_edges.erase(boundary_edges.begin());
    if (find_whole_hole_boundary(h, boundary_edges)) {
      holes.push_back(std::move(h));
    }
  }
}

void Mesh::remove_triangle(const std::shared_ptr<Triangle> &t) {
//  std::cout << "before remove size: " << _triangles.size() << "\n";

  for (int i = 0; i < _triangles.size(); ++i) {
    if (t == _triangles[i]) {
      _triangles.erase(_triangles.begin() + i);
      t->ab->_owner.reset();
      if (not t->ab->_opposite.expired()) {
        t->ab->_opposite.lock()->_opposite.reset();
      }
      t->bc->_owner.reset();
      if (not t->bc->_opposite.expired()) {
        t->bc->_opposite.lock()->_opposite.reset();
      }
      t->ca->_owner.reset();
      if (not t->ca->_opposite.expired()) {
        t->ca->_opposite.lock()->_opposite.reset();
      }
      return;
    }
  }
//  std::cout << "triangle wasn't removed!!!\n";
}
void Mesh::hole_boundary_clean(Mesh::Hole &h) {
  auto is_tooth = [](const std::shared_ptr<HalfEdge> &a,
                     const std::shared_ptr<HalfEdge> &b) {
    return (a->_owner.lock() == b->_owner.lock());
  };

  auto remove_tooth = [this](const std::shared_ptr<HalfEdge> &a,
                             const std::shared_ptr<HalfEdge> &b) {
    auto t = a->_owner.lock();
    std::set<std::shared_ptr<HalfEdge>> edges = {t->ab, t->bc, t->ca};
    edges.erase(a);
    edges.erase(b);
    auto e = (*edges.begin())->_opposite.lock();
//    auto e = (*edges.begin());
    remove_triangle(t);
    e->_opposite.reset();
//    e->_opposite = nullptr;
    return e;
  };

  auto remove_tooth_loop = [&]() {
    std::shared_ptr<Hole::Element> current = h.boundary_start;
    std::shared_ptr<Hole::Element> next = current->next;
    int tooth_count = 0;
    do {
//      std::cout <<"ones   " <<  *(current->elem->_start.lock()) << "   \t";
//      std::cout <<"onee   " <<  *(current->elem->_end.lock()) << " \n";
      if (is_tooth(current->elem, next->elem)) {
        current->next = next->next;
        current->elem = remove_tooth(current->elem, next->elem);
        tooth_count++;
      }
//      std::cout <<"checks " <<  *(current->elem->_start.lock()) << "   \t";
//      std::cout <<"checke " <<  *(current->elem->_end.lock()) << "   \t";
      current = current->next;
      next = current->next;
//      std::cout <<"twos " <<  *(current->elem->_start.lock()) << "\t";
//      std::cout <<"twoe " <<  *(current->elem->_end.lock()) << "\n";
    } while (next != h.boundary_start);
    return tooth_count;
  };
  while (remove_tooth_loop());
  std::cout << "\n";
}
std::shared_ptr<Triangle> Mesh::add_triangle(const std::shared_ptr<Vertex> &a,
                                             const std::shared_ptr<Vertex> &b,
                                             const std::shared_ptr<Vertex> &c,
                                             Vector n) {
  _triangles.push_back(Triangle::from(a, b, c, n));
//  std::cout << "after add size: " << _triangles.size() << "\n";
  return *_triangles.rbegin();
}
Mesh Mesh::read_from(const std::string &path, std::string type) {
  std::string lower;
  std::transform(path.begin(), path.end(),
                 std::back_inserter(lower), [](unsigned char c) { return std::tolower(c); });
  if (type == "auto" and tools::ends_with(lower, ".stl")) {
    type = "stl";
  } else if (type == "auto" and tools::ends_with(lower, ".off")) {
    type = "off";
  }
  if (type == "auto") {
    throw std::logic_error("поддерживаются только файлы *.stl или *.off");
  }
  std::shared_ptr<io::BaseIStream> in;
  if (type == "stl") {
    std::vector<uint8_t> h(5);
    {
      std::ifstream in(path, std::ios::binary);
      if (not in.good()) {
        throw std::logic_error("file is not exists");
      }
      io::StlBinaryIstream bin(path);
      bin >> h;
    }
    if (h == std::vector<uint8_t>({'s', 'o', 'l', 'i', 'd'})) {
      in = std::make_shared<io::StlIstream>(path);
    } else {
      in = std::make_shared<io::StlBinaryIstream>(path);
    }
  } else if (type == "off") {
    in = std::make_shared<io::OffIStream>(path);
  }
  Mesh m;
  *in >> m;
  m.normalize();
  return m;
}

/*
 * types = ascii_stl, stl(=bin_stl), off
 */
void Mesh::write_to(const Mesh &m, const std::string &path, const std::string &type) {
  std::shared_ptr<io::BaseOStream> out;
  if (type == "stl" || type == "bin_stl") {
    out = std::make_shared<io::StlBinaryOstream>(path);
  } else if (type == "ascii_stl") {
    out = std::make_shared<io::StlOstream>(path);
  } else if (type == "off") {
    out = std::make_shared<io::OffOStream>(path);
  } else {
    throw std::logic_error("unknown type");
  }
  *out << m;
}
void Mesh::fillHolesWithStrategy(const FillingStrategy &s, bool clean_hole_boundary) {
  std::vector<Hole> v;
  find_holes(v);
  for (auto &h : v) {
    if (clean_hole_boundary) {
      hole_boundary_clean(h);
    }
    s.Process(h, *this);
  }
}
void Mesh::debug_check() {
  for (auto &t : _triangles) {
    auto ab = t->ab;
    auto bc = t->bc;
    auto ca = t->ca;
    for (auto &e : {ab, bc, ca}) {
      if (e == e->_opposite.lock()) {
        std::cout << *t;
        std::cout << "s " << *(e->_start.lock()) << "   \t";
        std::cout << "e " << *(e->_end.lock()) << " \n";
      }
    }
  }
}

Mesh Mesh::create_from(const std::vector<Point> &points,
                       const std::vector<std::tuple<size_t, size_t, size_t>> &triangles) {
  std::vector<std::shared_ptr<Vertex>> vertices;
  vertices.reserve(points.size());
  for (const auto &point : points) {
    vertices.push_back(std::make_shared<Vertex>(point));
  }
  Mesh m;
  for (const auto &t : triangles) {
    const auto&[a, b, c] = t;
    m.add_triangle(vertices[a], vertices[b], vertices[c]);
  }
  return m;
}

void Mesh::set_name(const std::string &s) {
  name = s;
}
void Mesh::get_structure(std::vector<Point> &points, std::vector<std::tuple<size_t, size_t, size_t>> &triangles) const{
  points.clear();
  triangles.clear();
  triangles.reserve(_triangles.size());
  std::unordered_map<Point, size_t> index;
  auto get_index = [&index, &points](const Point &p) {
    if (index.find(p) == index.end()) {
      index[p] = points.size();
      points.push_back(p);
    }
    return index[p];
  };
  for (const auto &t : _triangles) {
    triangles.emplace_back(
        get_index(t->a->_coordinates),
        get_index(t->b->_coordinates),
        get_index(t->c->_coordinates)
    );
  }
}
void Mesh::get_structure(std::vector<std::shared_ptr<Vertex>> &vertices,
                         std::vector<std::tuple<size_t, size_t, size_t>> &triangles) {
  vertices.clear();
  triangles.clear();
  triangles.reserve(_triangles.size());
  std::unordered_map<std::shared_ptr<Vertex>, size_t> index;
  auto get_index = [&index, &vertices](const std::shared_ptr<Vertex> &p) {
    if (index.find(p) == index.end()) {
      index[p] = vertices.size();
      vertices.push_back(p);
    }
    return index[p];
  };
  for (const auto &t : _triangles) {
    triangles.emplace_back(
        get_index(t->a),
        get_index(t->b),
        get_index(t->c)
    );
  }
}

