#pragma once

#include <BaseFillingStrategy.hpp>
#include <base.hpp>


/*
 * специальный proxy - класс чтобы у человека,
 * пишущего триангулирование или улучшение отверстия
 * не было доступа к внутренносятям меша
 *
 * (это скорее как напоминалка, ибо кому надо - всё-равно доберется))
 */
class BaseFillingStrategy::MeshController {
  friend class Mesh;
 private:
  MeshController(Mesh& m);

 public:
  void remove_triangle(const std::shared_ptr<Triangle>& t) const;
  std::shared_ptr<Triangle> add_triangle(const std::shared_ptr<Vertex>& a,
                                         const std::shared_ptr<Vertex>& b,
                                         const std::shared_ptr<Vertex>& c,
                                         Vector n = {0,0,0}) const;
  void get_structure(
      std::vector<std::shared_ptr<Vertex>>& vertices,
      std::vector<std::tuple<size_t, size_t , size_t>>& triangles) const;


 private:
  Mesh & m;
};
