
#pragma once

#include "BaseIStream.hpp"
namespace io {
class StlBinaryIStream : public BaseIStream {
  // перепистаь нафиг чтобы не были членами класса!
 public:
  explicit StlBinaryIStream(const std::string &path);
  StlBinaryIStream() = delete;
  ~StlBinaryIStream() override;
  template<class T>
  StlBinaryIStream &operator>>(std::vector<T> &v);
  StlBinaryIStream &operator>>(Point &p) override;
  StlBinaryIStream &operator>>(Vertex &v) override;
  StlBinaryIStream &operator>>(double &d) override;
  StlBinaryIStream &operator>>(std::string &s) override;
  StlBinaryIStream &operator>>(Triangle &t) override;
  StlBinaryIStream &operator>>(Mesh &m) override;
  StlBinaryIStream &operator>>(float &d);
  StlBinaryIStream &operator>>(uint32_t &d);
  StlBinaryIStream &operator>>(uint16_t &d);
  StlBinaryIStream &operator>>(uint8_t &d);
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
StlBinaryIStream &StlBinaryIStream::operator>>(std::vector<T> &v) {
  for (T &i : v) {
    *this >> i;
  }
  return *this;
}
}