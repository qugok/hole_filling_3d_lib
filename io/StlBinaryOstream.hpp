#pragma once

#include "BaseOStream.hpp"
namespace io {
class StlBinaryOstream : public BaseOStream {
 public:

  StlBinaryOstream();
  explicit StlBinaryOstream(const std::string &path);
  ~StlBinaryOstream() override;

  StlBinaryOstream &operator<<(const std::string &s) override;
  StlBinaryOstream &operator<<(const Point &p) override;
  StlBinaryOstream &operator<<(const Vertex &v) override;
  StlBinaryOstream &operator<<(const Triangle &t) override;
  StlBinaryOstream &operator<<(const Mesh &m) override;
  StlBinaryOstream &operator<<(const double &d) override;
  StlBinaryOstream &operator<<(const float &d);
  StlBinaryOstream &operator<<(const size_t &n) override;
  StlBinaryOstream &operator<<(const uint32_t &d);
  StlBinaryOstream &operator<<(const uint16_t &d);
  StlBinaryOstream &operator<<(const uint8_t &d);
  StlBinaryOstream &operator<<(const char *s) override;
  template<class T>
  StlBinaryOstream &operator<<(const std::vector<T> &d);

 private:

  std::ostream *out;

  union primitive_types {
    double v = 0;
    float f;
    uint32_t ui32;
    uint16_t ui16;
    uint8_t c[8];
  };
};

template<class T>
StlBinaryOstream &StlBinaryOstream::operator<<(const std::vector<T> &d) {
  for (const T& v: d){
    *this << v;
  }
  return *this;
}
}