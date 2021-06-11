#pragma once

/*
 * https://segeval.cs.princeton.edu/public/off_format.html
 */

#include "BaseIStream.hpp"
namespace io {
class OffIStream : public BaseIStream {

 public:
  OffIStream();
  explicit OffIStream(const std::string &path, bool force = false);

  ~OffIStream() override;

  OffIStream &operator>>(size_t &n);
  OffIStream &operator>>(double &d) override;
  OffIStream &operator>>(std::string &s) override;
  OffIStream &operator>>(Point &p) override;
  OffIStream &operator>>(Vertex &v) override;
  OffIStream &operator>>(Triangle &t) override;
  OffIStream &operator>>(Mesh &m) override;
 private:
  void skip_comments();
  bool skip_single_comment();

 private:
  std::istream *in;
  bool force;
};
}