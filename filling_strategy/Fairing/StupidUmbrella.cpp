#include "StupidUmbrella.hpp"

#include <utility>
#include <MeshController.hpp>
#include <sstream>
#include "Tools.hpp"

fairing::StupidUniformUmbrella::StupidUniformUmbrella(int count, bool iterative, bool include_boundary)
    : StupidUmbrella(fairing::uniform_umbrella_operator, count, iterative, include_boundary) {}
fairing::StupidUniformUmbrella::StupidUniformUmbrella(bool iterative, bool include_boundary)
    : StupidUniformUmbrella(1, iterative, include_boundary) {}
std::string fairing::StupidUniformUmbrella::get_all_params()  const {

  std::ostringstream out;
  out << StupidUmbrella::get_all_params() << "\n";
  out << "\t\tStupidUniformUmbrella:";
  return out.str();
}

//void fairing::StupidUniformUmbrella::Process(const std::vector<std::shared_ptr<Vertex>> &inside_hole,
//                                             const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
//                                             const BaseFillingStrategy::MeshController &m) const {
////  std::cout << "size: " << inside_hole.size() << "\n";
//
//  for (int i = 0; i < count; ++i) {
//    for (const auto &v : inside_hole) {
////      std::cout << "umbrella operator " << fairing::uniform_umbrella_operator(v)  << "\n";
//      v->_coordinates += fairing::uniform_umbrella_operator(v);
//    }
//  }
//}

fairing::StupidScaleDependentUmbrella::StupidScaleDependentUmbrella(int count, bool iterative, bool include_boundary)
    : StupidUmbrella(fairing::scale_dependent_umbrella_operator, count, iterative, include_boundary) {}
fairing::StupidScaleDependentUmbrella::StupidScaleDependentUmbrella(bool iterative, bool include_boundary)
    : StupidScaleDependentUmbrella(1, iterative, include_boundary) {}
std::string fairing::StupidScaleDependentUmbrella::get_all_params()  const {

  std::ostringstream out;
  out << StupidUmbrella::get_all_params() << "\n";
  out << "\t\tStupidScaleDependentUmbrella:";
  return out.str();
}

//void fairing::StupidScaleDependentUmbrella::Process(const std::vector<std::shared_ptr<Vertex>> &inside_hole,
//                                                    const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
//                                                    const BaseFillingStrategy::MeshController &m) const {
//
//  for (int i = 0; i < count; ++i) {
//    for (const auto &v : inside_hole) {
////      std::cout << "umbrella operator " << fairing::scale_dependent_umbrella_operator(v)  << "\n";
//      v->_coordinates += fairing::scale_dependent_umbrella_operator(v);
//    }
//  }
//}

fairing::StupidHarmonicUmbrella::StupidHarmonicUmbrella(int count, bool iterative, bool include_boundary)
    : StupidUmbrella(fairing::harmonic_umbrella_operator, count, iterative, include_boundary) {}
fairing::StupidHarmonicUmbrella::StupidHarmonicUmbrella(bool iterative, bool include_boundary)
    : StupidHarmonicUmbrella(1, iterative, include_boundary) {}
std::string fairing::StupidHarmonicUmbrella::get_all_params()  const {
  std::ostringstream out;
  out << StupidUmbrella::get_all_params() << "\n";
  out << "\t\tStupidHarmonicUmbrella:";
  return out.str();
}

//void fairing::StupidHarmonicUmbrella::Process(const std::vector<std::shared_ptr<Vertex>> &inside_hole,
//                                              const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
//                                              const BaseFillingStrategy::MeshController &m) const {
//  for (int i = 0; i < count; ++i) {
//    for (const auto &v : inside_hole) {
//      v->_coordinates += fairing::harmonic_umbrella_operator(v);
//    }
//  }
//}


fairing::StupidUmbrella::StupidUmbrella(
    std::function<Vector(const std::shared_ptr<Vertex> &)> umbrella_operator,
    int count,
    bool iterative,
    bool include_boundary)
    : umbrella_operator(std::move(umbrella_operator)), count(count), iterative(iterative), include_boundary(include_boundary) {}

void fairing::StupidUmbrella::Process(std::vector<std::shared_ptr<Vertex>> &inside_hole,
                                      const std::vector<std::shared_ptr<Vertex>> &hole_boundary,
                                      const BaseFillingStrategy::MeshController &m) const {

  std::vector<Vector> delta(inside_hole.size());
  auto process_vector = [&delta, this](const std::vector<std::shared_ptr<Vertex>> &vertexes){
    delta.clear();
    for (const auto &v : vertexes) {
      if (iterative) {
        v->_coordinates += umbrella_operator(v);
      } else {
        delta.push_back(umbrella_operator(v));
      }
    }
    if (not iterative) {
      fairing::tools::add(vertexes, delta);
    }
  };
  for (int i = 0; i < count; ++i) {
   process_vector(inside_hole);
   if (include_boundary){
     process_vector(hole_boundary);
   }
  }
}
std::string fairing::StupidUmbrella::get_all_params()  const {
  std::ostringstream out;
  out << "\t\tStupidUmbrella:\n\t\t";
  out << "iterative:\t" << iterative << "\n\t\t";
  out << "include_boundary:\t" << include_boundary << "\n\t\t";
  out << "count:\t" << count;
  return out.str();
}
