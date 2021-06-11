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

class StlOstream : public BaseOStream {
 public:
  StlOstream();
  explicit StlOstream(const std::string &path);

  ~StlOstream() override;

  StlOstream &operator<<(const size_t &n) override;
  StlOstream &operator<<(const Point &p) override;
  StlOstream &operator<<(const Vertex &v) override;
  StlOstream &operator<<(const double &d) override;
  StlOstream &operator<<(const char *s) override;
  StlOstream &operator<<(const std::string &s) override;
  StlOstream &operator<<(const Triangle &t) override;
  StlOstream &operator<<(const Mesh &m) override;

 private:
  std::ostream *out;
};
}