#include <algorithm>
#include "StlBinaryIStream.hpp"
#include "../base/Mesh.hpp"
#include <fstream>

io::StlBinaryIStream::StlBinaryIStream(const std::string& path) {
  in =  new std::ifstream(path, std::ios::binary);
 *in >> std::noskipws;
}

io::StlBinaryIStream &io::StlBinaryIStream::operator>>(Point &p) {
  std::vector<float> coordinates(3);
  *this >> coordinates;
  p.x = coordinates[0];
  p.y = coordinates[1];
  p.z = coordinates[2];
//  *this >> p.x >> p.y >> p.z;
  return *this;
}
io::StlBinaryIStream &io::StlBinaryIStream::operator>>(Vertex &v) {
  *this >> v._coordinates;
  return *this;
}
io::StlBinaryIStream &io::StlBinaryIStream::operator>>(std::string &s) {
  // подумать какое поведение можно хотеть
//  for (char &i : s) {
//    *this >> i;
//  }
  return *this;
}
io::StlBinaryIStream &io::StlBinaryIStream::operator>>(Triangle &t) {
  *this >> t.n;
  t.a = std::make_shared<Vertex>();
  t.b = std::make_shared<Vertex>();
  t.c = std::make_shared<Vertex>();
  *this >> *t.a >> *t.b >> *t.c;
  uint16_t attribute_byte_count;
  *this >> attribute_byte_count;
  return *this;
}
io::StlBinaryIStream &io::StlBinaryIStream::operator>>(Mesh &m) {
  std::vector<uint8_t> header(80);
  *this >> header;
  uint32_t number_of_triagles;
  *this >> number_of_triagles;
//  std::cout << "number = " << number_of_triagles << "\n";
  m._triangles.resize(number_of_triagles);
  std::transform(m._triangles.begin(),
                 m._triangles.end(),
                 m._triangles.begin(),
                 [](const auto &a) { return std::make_shared<Triangle>(); });
  for (auto &t : m._triangles) {
    *this >> *t;
//    std::cout << *t << "\n";
  }
  return *this;
}

io::StlBinaryIStream &io::StlBinaryIStream::operator>>(double &d) {
  primitive_types temp;
  *this >> temp.c[0] >> temp.c[1] >> temp.c[2] >> temp.c[3] >> temp.c[4] >> temp.c[5] >> temp.c[6] >> temp.c[7];
  d = temp.v;
  return *this;
}
io::StlBinaryIStream &io::StlBinaryIStream::operator>>(uint32_t &d) {
  primitive_types temp;
  *this >> temp.c[0] >> temp.c[1] >> temp.c[2] >> temp.c[3];
  d = temp.ui32;
  return *this;
}
io::StlBinaryIStream &io::StlBinaryIStream::operator>>(uint8_t &d) {
  *in >> d;
  return *this;
}
io::StlBinaryIStream &io::StlBinaryIStream::operator>>(uint16_t &d) {
  primitive_types temp;
  *this >> temp.c[0] >> temp.c[1];
  d = temp.ui16;
  return *this;
}
io::StlBinaryIStream &io::StlBinaryIStream::operator>>(float &d) {
  primitive_types temp;
  *this >> temp.c[0] >> temp.c[1] >> temp.c[2] >> temp.c[3];
  d = temp.f;
  return *this;
}
io::StlBinaryIStream::~StlBinaryIStream() {
  delete in;
}

