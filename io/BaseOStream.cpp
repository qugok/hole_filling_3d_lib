#include "BaseOStream.hpp"
#include <fstream>

io::StlOStream &io::StlOStream::operator<<(const Mesh &m) {
  *this << "solid " << m.name << "\n";
  for (const auto &t : m._triangles) {
    *this << *t << "\n";
  }
  *this << "endsolid " << m.name;
  return *this;
}

io::StlOStream &io::StlOStream::operator<<(const Triangle &t) {
  *this << " facet normal " << t.n << "\n";
  *this << "  outer loop\n  ";
  *this << *t.a << "\n  " << *t.b << "\n  " << *t.c << "\n";
  *this << "  endloop\n endfacet\n";
  return *this;
}

io::StlOStream &io::StlOStream::operator<<(const Vertex &v) {
  *this << "vertex " << v._coordinates;
  return *this;
}
io::StlOStream &io::StlOStream::operator<<(const Point &p) {
  *this << p.x << " " << p.y << " " << p.z;
  return *this;
}
io::StlOStream &io::StlOStream::operator<<(const double &d) {
//  out << std::fixed << d;
  *out << d;
  return *this;
}
io::StlOStream &io::StlOStream::operator<<(const std::string &s) {
  *out << s;
  return *this;
}

io::StlOStream &io::StlOStream::operator<<(const char *s) {
  *out << s;
  return *this;
}
io::StlOStream &io::StlOStream::operator<<(const size_t &n) {
  *out << n;
  return *this;
}
io::StlOStream::StlOStream(const std::string &path) {
  out = new std::ofstream(path);
}

io::StlOStream::StlOStream() : out(&std::cout) {}

io::StlOStream::~StlOStream() {
  if (out != &std::cout) {
    delete out;
  }
}

