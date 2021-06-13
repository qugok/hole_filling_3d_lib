#pragma once
#include "../base/Mesh.hpp"
#include "base.hpp"

namespace io {
class BaseOStream {
 public:

  BaseOStream() = default;

  BaseOStream(const BaseOStream &) = delete;
  BaseOStream(BaseOStream &&) = delete;
  BaseOStream &operator=(const BaseOStream &) = delete;
  BaseOStream &operator=(BaseOStream &&) = delete;

  virtual ~BaseOStream() = default;

  virtual BaseOStream &operator<<(const size_t &n) = 0;
  virtual BaseOStream &operator<<(const double &d) = 0;
  virtual BaseOStream &operator<<(const std::string &s) = 0;
  virtual BaseOStream &operator<<(const char *s) = 0;
  virtual BaseOStream &operator<<(const Point &p) = 0;
  virtual BaseOStream &operator<<(const Vertex &v) = 0;
  virtual BaseOStream &operator<<(const Triangle &t) = 0;
  virtual BaseOStream &operator<<(const Mesh &m) = 0;

};

class StlOStream : public BaseOStream {
 public:
  StlOStream();
  explicit StlOStream(const std::string &path);

  ~StlOStream() override;

  StlOStream &operator<<(const size_t &n) override;
  StlOStream &operator<<(const Point &p) override;
  StlOStream &operator<<(const Vertex &v) override;
  StlOStream &operator<<(const double &d) override;
  StlOStream &operator<<(const char *s) override;
  StlOStream &operator<<(const std::string &s) override;
  StlOStream &operator<<(const Triangle &t) override;
  StlOStream &operator<<(const Mesh &m) override;

 private:
  std::ostream *out;
};
}