//
// Created by alex on 18.03.2021.
//

#include "BaseIStream.hpp"
#include "Mesh.hpp"
#include <fstream>
#include <iostream>

io::BaseIStream::operator bool() {
  return !error;
}

io::StlIstream::StlIstream() : in(&std::cin){}

io::StlIstream::StlIstream(const std::string &path) {
  in =  new std::ifstream(path);
}


io::StlIstream &io::StlIstream::operator>>(double &d) {
  *in >> d;
  return *this;
}
io::StlIstream &io::StlIstream::operator>>(std::string &s) {
  *in >> s;
  return *this;
}
io::StlIstream &io::StlIstream::operator>>(Point &p) {
  *this >> p.x >> p.y >> p.z;
  return *this;
}
io::StlIstream &io::StlIstream::operator>>(Vertex &v) {
  std::string temp;
  *this >> temp >> v._coordinates;
  return *this;
}
io::StlIstream &io::StlIstream::operator>>(Triangle &t) {
  error = false;
  std::string temp;
  if (!(*this >> temp)  || temp != "facet"){
    error = true;
    return *this;
  }
  *this >> temp >> t.n;
  *this >> temp >> temp;
  t.a = std::make_shared<Vertex>();
  t.b = std::make_shared<Vertex>();
  t.c = std::make_shared<Vertex>();
  *this >> *t.a >> *t.b >> *t.c;
  *this >> temp >> temp;
  return *this;
}
io::StlIstream &io::StlIstream::operator>>(Mesh &m) {
  std::string temp;
  *this >> temp >> m.name;
  Triangle t;
  while (*this >> t){
    m._triangles.push_back(std::make_shared<Triangle>(std::move(t)));
  }
  return *this;
}
io::StlIstream::~StlIstream() {
  if (in != &std::cin){
    delete in;
  }
}
