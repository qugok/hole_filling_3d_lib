#include "Mesh.hpp"
#include "ModernAdvancingFrontFilling.hpp"

/*
 * здесь мы просто заделываем все отверстия
 * с помощью модернизированного метода наступательного фронта
 */
int main() {
  std::string readpath = "../models/for_test/1sphere_hole.stl";
  Mesh m = Mesh::read_from(readpath);

  m.fillHolesWithStrategy(strategies::ModernAdvancingFrontFilling());

  std::string writepath = "../temp/simple_example.stl";
  Mesh::write_to(m, writepath);
  return 0;
}