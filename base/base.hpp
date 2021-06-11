#pragma once

#include <memory>
#include <vector>
#include <Point.hpp>

struct Vertex;
class Mesh;
class Triangle;

struct HalfEdge : std::enable_shared_from_this<HalfEdge> {
  HalfEdge(const std::shared_ptr<Vertex> &start,
           const std::shared_ptr<Vertex> &end,
           const std::shared_ptr<HalfEdge> &opposite,
           const std::shared_ptr<Triangle> &_owner);
  ~HalfEdge();
  bool has_opposite();
  void fix_opposite();
  operator Vector() const;

  std::weak_ptr<Vertex> _start;
  std::weak_ptr<Vertex> _end;
  std::weak_ptr<HalfEdge> _opposite;
  std::weak_ptr<Triangle> _owner;

};

namespace std {

template<>
struct hash<HalfEdge> {
  std::size_t operator()(const HalfEdge &p) const;
};
}

struct Vertex {
  Vertex();
  Vertex(const Point &coordinates, const std::vector<std::weak_ptr<HalfEdge>> &outgoing_edges = {});
  void add_edge(const std::shared_ptr<HalfEdge>& e);
//  void remove_edge(HalfEdge e);
  [[nodiscard]] std::shared_ptr<HalfEdge> find_edge_to(const std::shared_ptr<Vertex>& to) const;
  [[nodiscard]] std::shared_ptr<HalfEdge> get_edge_to(const std::shared_ptr<Vertex>& to) const;
  [[nodiscard]] std::shared_ptr<HalfEdge> find_next_boundary_edge(const std::shared_ptr<HalfEdge>& prev = nullptr) const;
  [[nodiscard]] std::vector<std::shared_ptr<Vertex>> neighbor_vertices() const;
  [[nodiscard]] std::vector<std::shared_ptr<Triangle>> neighbor_triangles() const;

  operator Point();
  Vector get_normal();

  Point _coordinates;
  std::vector<std::weak_ptr<HalfEdge>> _outgoing_edges;


};

std::ostream &operator<<(std::ostream &out, const Vertex &v);
std::istream &operator>>(std::istream &in, Vertex &v);

/*
направление обхода треугольника против часовой если смотреть снаружи фигуры
 */
struct Triangle :  std::enable_shared_from_this<Triangle> {
 public:
  Triangle();
  Triangle(const Triangle &t);
  Point barycenter();
  double area();
  double angle(const std::shared_ptr<Vertex>& v);
  Point centre_circumscribed_circle();
  static std::shared_ptr<Triangle> from(const std::shared_ptr<Vertex>& a,
                                        const std::shared_ptr<Vertex>& b,
                                        const std::shared_ptr<Vertex>& c,
                                        Vector n);
  std::shared_ptr<Vertex> get_opposite(const std::shared_ptr<HalfEdge>& e) const;

  friend std::ostream &operator<<(std::ostream &out, const Triangle &t);

  friend std::istream &operator>>(std::istream &in, Triangle &t);
  friend class StlOstream;

  friend class StlIstream;
  friend class StlBinaryIstream;
  friend class StlBinaryOstream;
  friend class Mesh;

 private:
  void fill(const std::shared_ptr<Vertex>& a,
            const std::shared_ptr<Vertex>& b,
            const std::shared_ptr<Vertex>& c,
            Vector n);

 public:
  Vector n;
  std::shared_ptr<Vertex> a, b, c;
  std::shared_ptr<HalfEdge> ab, bc, ca;
};
