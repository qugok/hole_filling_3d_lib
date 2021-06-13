#include <fstream>
#include <../library.h>

using namespace std;

/**
 * @brief  функция запускающая эксперимент и выводящия все настраиваемые параметры в stdout
 * @param filling_strategy стратегия, которая будет использоваться для заполнения отверстий
 * @param number номер эксперимента - информация о нем сохранится в файл experiments/@number.txt
 * @param readpath путь откуда надо будет прочитать исходную модель - рекомендуется файл формата stl
 * @param writepath путь по которому будет записан результат - файл формата stl
 * @param save нужно ли сохранять информацию про эксперимент в файл в папке experiments/
 */
void exec_experiment(const FillingStrategy &filling_strategy, int number, const std::string& readpath, const std::string& writepath, bool save = false){
  std::string exp_path = "../experiments/" + to_string(number) + ".txt";
  Mesh m = Mesh::read_from(readpath);

  m.fillHolesWithStrategy(filling_strategy);
  std::string info = "in model: " + readpath + "\n" + "out model: " + writepath + "\n" + filling_strategy.get_all_params();
  if (save){
    ofstream out(exp_path);
    out << info;
  }
  std::cout << info;
  Mesh::write_to(m, writepath, "stl");
}

int main() {

  // здесь можно выбрать из какого файла читать модель
//  std::string readpath = "../models/for_test/1sphere_hole.stl";
//  std::string readpath = "../models/for_test/2sphere_dumbbell_hole.STL";
//  std::string readpath = "../models/for_test/3bunnyhole.off";
  std::string readpath = "../models/for_test/4bun_high.stl";
//  std::string readpath = "../models/for_test/5sphere_small.STL";

  // здесь можно выбрать в какой файл записать результат
//  std::string writepath = "../models/for_test/test_result/1potato.stl";
  std::string writepath = "../models/for_test/test_result/bunny.stl";
//  std::string writepath = "../models/for_test/test_result/16sphere_hole.stl";

  // номер эксперимента
  int number = 35;
  bool save = false;
  // здесь можно выбрать стратегию для применения к отверстиям
  // это метод, описанный в статье (с исправлениями)
//  strategies::FastHoleFillingMethod filling_strategy = strategies::FastHoleFillingMethod();

  // здесь можно собрать свою стратегию из различных частей
  // 1 - начальная триангуляция
  // 2 - улучшения триангуляции, применяемые последовательно
  strategies::SequenceFillingStrategy filling_strategy = strategies::SequenceFillingStrategy(
      new initial_triangulation::advancing_front::ModernAdvancingFront(-1, sqrt(3.5)),
//      new initial_triangulation::MinAreaTriangulation(),
      {
//          new refinement::DeloneLike(100),
              new fairing::StupidHarmonicUmbrella(1, true, true),
//              new fairing::StupidUniformUmbrella(1, true, false),
      }
  );

  // здесь происходит запуск эксперимента
  exec_experiment(filling_strategy, number, readpath, writepath, save);
  return 0;
}