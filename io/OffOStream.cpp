#include "OffOStream.hpp"
#include <fstream>

io::OffOStream::OffOStream() : out(&std::cout) {}

io::OffOStream::OffOStream(const std::string &path) {
  out = new std::ofstream(path);
}
io::OffOStream::~OffOStream() {
  if (out != &std::cout) {
    delete out;
  }
}

io::OffOStream &io::OffOStream::operator<<(const double &d) {
  *out << d;
  return *this;
}
io::OffOStream &io::OffOStream::operator<<(const std::string &s) {
  *out << s;
  return *this;
}
io::OffOStream &io::OffOStream::operator<<(const char *s) {
  *out << s;
  return *this;
}
io::OffOStream &io::OffOStream::operator<<(const Point &p) {
  *this << p.x << " " << p.y << " " << p.z;
  return *this;
}
io::OffOStream &io::OffOStream::operator<<(const Vertex &v) {
  throw std::logic_error("don't use it for off files");
  return *this;
}
io::OffOStream &io::OffOStream::operator<<(const Triangle &t) {
  throw std::logic_error("don't use it for off files");
  return *this;
}
io::OffOStream &io::OffOStream::operator<<(const Mesh &m) {
  std::vector<Point> vertices;
  std::vector<std::tuple<size_t, size_t, size_t>> triangles;
  m.get_structure(vertices, triangles);
  *this << "OFF\n";
  *this << vertices.size() << " " << triangles.size() << " 0\n";
  for (auto v : vertices) {
    *this << v << "\n";
  }
  for (auto t : triangles) {
    *this << "3 " << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << "\n";
  }
  return *this;
}
io::OffOStream &io::OffOStream::operator<<(const size_t &n) {
  *out << n;
  return *this;
}
