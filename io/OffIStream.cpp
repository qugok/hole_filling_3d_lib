#include "OffIStream.hpp"
#include "Mesh.hpp"
#include <fstream>
#include <iostream>

io::OffIStream::OffIStream() : in(&std::cin) {}

io::OffIStream::OffIStream(const std::string &path, bool force)
: in(new std::ifstream(path)) , force(force) {}

io::OffIStream &io::OffIStream::operator>>(double &d) {
  *in >> d;
  return *this;
}
io::OffIStream &io::OffIStream::operator>>(std::string &s) {
  *in >> s;
  return *this;
}
io::OffIStream &io::OffIStream::operator>>(Point &p) {
  *this >> p.x >> p.y >> p.z;
  return *this;
}
io::OffIStream &io::OffIStream::operator>>(Vertex &v) {
  throw std::logic_error("don't use it for off files");
  return *this;
}
io::OffIStream &io::OffIStream::operator>>(Triangle &t) {
  throw std::logic_error("don't use it for off files");
  return *this;
}
io::OffIStream &io::OffIStream::operator>>(Mesh &m) {
  std::string temp;
  *this >> temp;
  // нужно ли проверять что написано OFF?
  if (not force and temp != "OFF" and temp != "COFF"){
    throw std::ios::failure("Wrong file format : not OFF");
  }
  skip_comments();

  auto skip_to_endline = [this](){
    std::string temp;
    getline(*in, temp);};

  size_t vertex_count, face_count, edges_count;
  *this >> vertex_count >> face_count >> edges_count;
  std::vector<Point> vertices(vertex_count);
  for (auto & p : vertices) {
    skip_comments();
    *this >> p;
    skip_to_endline();
  }
  std::vector<std::tuple<size_t, size_t , size_t>> triangles;
  auto read_face = [&triangles, this, &skip_to_endline](){
    size_t vertex_count;
    *this >> vertex_count;
    if (vertex_count < 3){
      return;
    }
    size_t first_vertex, cur_v, prev_v;
    *this >> first_vertex >> prev_v >> cur_v;
    triangles.emplace_back(first_vertex, prev_v, cur_v);
    for (int i = 3; i < vertex_count; ++i) {
      prev_v = cur_v;
      *this >> cur_v;
      triangles.emplace_back(first_vertex, prev_v, cur_v);
    }
    skip_to_endline();
  };
  for (int i = 0; i < face_count; ++i) {
    skip_comments();
    read_face();
  }
  m = Mesh::create_from(vertices, triangles);
  return *this;
}
bool io::OffIStream::skip_single_comment() {
  char temp;
  *in >> temp;
  in->seekg(-1, std::ios_base::cur);
  if (temp != '#'){
    return false;
  }
  std::string t;
  getline(*in, t);
  return true;
}
void io::OffIStream::skip_comments() {
  while (skip_single_comment()){}
}
io::OffIStream &io::OffIStream::operator>>(size_t &n) {
  *in >> n;
  return *this;
}
io::OffIStream::~OffIStream() {
  if (in != &std::cin){
    delete in;
  }
}
