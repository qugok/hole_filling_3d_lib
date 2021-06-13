#pragma once

#include "BaseOStream.hpp"
namespace io {
class StlBinaryOStream : public BaseOStream {
 public:

  StlBinaryOStream();
  explicit StlBinaryOStream(const std::string &path);
  ~StlBinaryOStream() override;

  StlBinaryOStream &operator<<(const std::string &s) override;
  StlBinaryOStream &operator<<(const Point &p) override;
  StlBinaryOStream &operator<<(const Vertex &v) override;
  StlBinaryOStream &operator<<(const Triangle &t) override;
  StlBinaryOStream &operator<<(const Mesh &m) override;
  StlBinaryOStream &operator<<(const double &d) override;
  StlBinaryOStream &operator<<(const float &d);
  StlBinaryOStream &operator<<(const size_t &n) override;
  StlBinaryOStream &operator<<(const uint32_t &d);
  StlBinaryOStream &operator<<(const uint16_t &d);
  StlBinaryOStream &operator<<(const uint8_t &d);
  StlBinaryOStream &operator<<(const char *s) override;
  template<class T>
  StlBinaryOStream &operator<<(const std::vector<T> &d);

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
StlBinaryOStream &StlBinaryOStream::operator<<(const std::vector<T> &d) {
  for (const T& v: d){
    *this << v;
  }
  return *this;
}
}