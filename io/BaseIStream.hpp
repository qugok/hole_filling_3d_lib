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

class StlIstream : public BaseIStream {
 public:
  StlIstream();
  explicit StlIstream(const std::string &path);

//  StlIstream(const StlIstream& ) = delete;
//  StlIstream(StlIstream&& ) = delete;
//  StlIstream& operator=(const StlIstream& ) = delete;
//  StlIstream& operator=(StlIstream&& ) = delete;
  ~StlIstream() override;

  StlIstream &operator>>(Point &p) override;
  StlIstream &operator>>(Vertex &v) override;
  StlIstream &operator>>(double &d) override;
  StlIstream &operator>>(std::string &s) override;
  StlIstream &operator>>(Triangle &t) override;
  StlIstream &operator>>(Mesh &m) override;

 private:
  std::istream *in;
};
}