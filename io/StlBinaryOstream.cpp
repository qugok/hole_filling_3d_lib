#include "StlBinaryOstream.hpp"
#include <fstream>

io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const std::string &s) {
  for (uint8_t c : s) {
    *this << c;
  }
  return *this;
}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const Point &p) {
  *this << static_cast<float>(p.x) << static_cast<float>(p.y) << static_cast<float>(p.z);
  return *this;
}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const Vertex &v) {
  *this << v._coordinates;
  return *this;
}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const Triangle &t) {
  *this << t.n << *t.a << *t.b << *t.c << static_cast<uint16_t>(0);
  return *this;
}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const Mesh &m) {
  std::string to_header = "binary stl file";
  *this << to_header << std::vector<uint8_t>(80 - to_header.size() - 1, ' ') << static_cast<uint8_t >(0);
  *this << static_cast<uint32_t>(m._triangles.size());
  for (const auto& t : m._triangles){
    *this << *t;
  }
  return *this;
}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const double &d) {
  primitive_types temp;
  temp.v = d;
  *this << temp.c[0] << temp.c[1] << temp.c[2] << temp.c[3]
  << temp.c[4] << temp.c[5] << temp.c[6] << temp.c[7];
  return *this;
}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const float &d) {
  primitive_types temp;
  temp.f = d;
  *this << temp.c[0] << temp.c[1] << temp.c[2] << temp.c[3];
  return *this;
}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const uint32_t &d) {
  primitive_types temp;
  temp.ui32 = d;
  *this << temp.c[0] << temp.c[1] << temp.c[2] << temp.c[3];
  return *this;
}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const uint16_t &d) {
  primitive_types temp;
  temp.ui16 = d;
  *this << temp.c[0] << temp.c[1];
  return *this;
}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const uint8_t &d) {
  *out << d;
  return *this;
}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const char *s) {
  *out << s;
  return *this;
}

io::StlBinaryOstream::StlBinaryOstream(const std::string &path) {
  out =  new std::ofstream(path, std::ios::binary);
}
io::StlBinaryOstream::~StlBinaryOstream() {
  delete out;
}
io::StlBinaryOstream::StlBinaryOstream() : out(&std::cout) {}
io::StlBinaryOstream &io::StlBinaryOstream::operator<<(const size_t &n) {
  *out << n;
  return *this;
}
