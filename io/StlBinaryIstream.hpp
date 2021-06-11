
#pragma once

#include "BaseIStream.hpp"
namespace io {
class StlBinaryIstream : public BaseIStream {
  // перепистаь нафиг чтобы не были членами класса!
 public:
  explicit StlBinaryIstream(const std::string &path);
  StlBinaryIstream() = delete;
  ~StlBinaryIstream() override;
  template<class T>
  StlBinaryIstream &operator>>(std::vector<T> &v);
  StlBinaryIstream &operator>>(Point &p) override;
  StlBinaryIstream &operator>>(Vertex &v) override;
  StlBinaryIstream &operator>>(double &d) override;
  StlBinaryIstream &operator>>(std::string &s) override;
  StlBinaryIstream &operator>>(Triangle &t) override;
  StlBinaryIstream &operator>>(Mesh &m) override;
  StlBinaryIstream &operator>>(float &d);
  StlBinaryIstream &operator>>(uint32_t &d);
  StlBinaryIstream &operator>>(uint16_t &d);
  StlBinaryIstream &operator>>(uint8_t &d);
 private:
  union primitive_types {
    double v = 0;
    float f;
    uint32_t ui32;
    uint16_t ui16;
    uint8_t c[8];
  };
  std::istream *in;
};

template<class T>
StlBinaryIstream &StlBinaryIstream::operator>>(std::vector<T> &v) {
  for (T &i : v) {
    *this >> i;
  }
  return *this;
}
}