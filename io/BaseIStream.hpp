#pragma once

#include <base.hpp>

namespace io {
class BaseIStream {
 public:

  BaseIStream() = default;

  BaseIStream(const BaseIStream &) = delete;
  BaseIStream(BaseIStream &&) = delete;
  BaseIStream &operator=(const BaseIStream &) = delete;
  BaseIStream &operator=(BaseIStream &&) = delete;

  virtual ~BaseIStream() = default;

  virtual BaseIStream &operator>>(double &d) = 0;
  virtual BaseIStream &operator>>(std::string &s) = 0;
  virtual BaseIStream &operator>>(Point &p) = 0;
  virtual BaseIStream &operator>>(Vertex &v) = 0;
  virtual BaseIStream &operator>>(Triangle &t) = 0;
  virtual BaseIStream &operator>>(Mesh &m) = 0;
  operator bool();

 protected:
  bool error = false;
};

class StlIStream : public BaseIStream {
 public:
  StlIStream();
  explicit StlIStream(const std::string &path);

//  StlIStream(const StlIStream& ) = delete;
//  StlIStream(StlIStream&& ) = delete;
//  StlIStream& operator=(const StlIStream& ) = delete;
//  StlIStream& operator=(StlIStream&& ) = delete;
  ~StlIStream() override;

  StlIStream &operator>>(Point &p) override;
  StlIStream &operator>>(Vertex &v) override;
  StlIStream &operator>>(double &d) override;
  StlIStream &operator>>(std::string &s) override;
  StlIStream &operator>>(Triangle &t) override;
  StlIStream &operator>>(Mesh &m) override;

 private:
  std::istream *in;
};
}