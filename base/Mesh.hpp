#pragma once

#include <unordered_set>
#include <Hole.hpp>
#include <MeshController.hpp>
#include <FillingStrategy.hpp>
#include "base.hpp"

namespace io{
class StlOStream;
class StlIStream;
class StlBinaryIStream;
class StlBinaryOStream;
}


class Mesh {
  friend class BaseFillingStrategy::MeshController;
  typedef BaseFillingStrategy::Hole Hole;
 public:
  // точки - 3 координаты, треугольники - 3 индекса точек в порядке против часовой
  static Mesh create_from(const std::vector<Point>& points, const std::vector<std::tuple<size_t, size_t , size_t>>& triangles);
  static Mesh read_from(const std::string& path, std::string type = "auto");
  static void write_to(const Mesh& m, const std::string& path, const std::string& type = "bin_stl");
  void get_structure(std::vector<Point>& points, std::vector<std::tuple<size_t, size_t , size_t>>& triangles) const;

  void fillHolesWithStrategy(const FillingStrategy& s, bool clean_hole_boundary = true);

  Mesh();

  void set_name(const std::string& s);

  friend std::ostream& operator<<(std::ostream& out, const Mesh& m);
  friend std::istream& operator>>(std::istream& in, Mesh& m);

  void normalize();

  friend class io::StlOStream;
  friend class io::StlIStream;
  friend class io::StlBinaryIStream;
  friend class io::StlBinaryOStream;

 private:
  void get_structure(std::vector<std::shared_ptr<Vertex>>& vertices, std::vector<std::tuple<size_t, size_t , size_t>>& triangles);

  void hole_boundary_clean(Hole& h);
  void debug_check();

  void remove_triangle(const std::shared_ptr<Triangle>& t);
  std::shared_ptr<Triangle> add_triangle(const std::shared_ptr<Vertex>& a,
                    const std::shared_ptr<Vertex>& b,
                    const std::shared_ptr<Vertex>& c,
                    Vector n = {0,0,0});

  /**
   * поиск всех отверстий внутри меша
   * @param holes вектор в который запишутся все найденные отверсия
   */
  void find_holes(std::vector<Hole>& holes);
  /**
   * есть дырка с недозаполненной границей
   * функция её дозаполнит и замкнет
   * @param hole полузаполненная дырка
   * @param e множество доступных граничных полуребер
   * @return сообщает о корректности завершения операции
   * false, если    дырка не закончилась
   *         или    она изначально была пустая
   *
   */
  static bool find_whole_hole_boundary(Hole& hole, std::unordered_set<std::shared_ptr<HalfEdge>>& e);

 private:
  std::string name;
  std::vector<std::shared_ptr<Triangle>> _triangles;
};