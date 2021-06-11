#pragma once
#include "BaseOStream.hpp"
/*
 * https://segeval.cs.princeton.edu/public/off_format.html
 */
namespace io {

class OffOStream : public BaseOStream {

 public:
  OffOStream();
  explicit OffOStream(const std::string& path);

  ~OffOStream() override;

  OffOStream &operator<<(const double &d) override;
  OffOStream &operator<<(const std::string &s) override;
  OffOStream &operator<<(const char *s) override;
  OffOStream &operator<<(const Point &p) override;
  OffOStream &operator<<(const size_t &n) override;
  OffOStream &operator<<(const Vertex &v) override;
  OffOStream &operator<<(const Triangle &t) override;
  OffOStream &operator<<(const Mesh &m) override;

 private:
  std::ostream* out;
};
}
