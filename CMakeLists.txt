cmake_minimum_required(VERSION 3.15)
project(hole_filling_3d_lib)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD 17)

add_library(hole_filling_3d_lib

        library.cpp library.h

        geometry/Point.cpp base/Vertex.cpp

        base/HalfEdge.cpp base/base.hpp
        base/Triangle.cpp base/Mesh.cpp
        base/Mesh.hpp io/BaseOStream.cpp

        io/OffIStream.cpp io/OffIStream.hpp
        io/OffOStream.cpp io/OffOStream.hpp
        io/BaseOStream.hpp io/BaseIStream.cpp
        io/BaseIStream.hpp io/StlBinaryIstream.hpp
        io/StlBinaryIstream.cpp io/StlBinaryIstream.hpp
        io/StlBinaryOstream.cpp io/StlBinaryOstream.hpp

        filling_strategy/BaseFillingStrategy.hpp
        filling_strategy/FillingStrategy.hpp
        filling_strategy/ImproveStrategy.hpp

        filling_strategy/Strategies/AdvancingFrontFilling.cpp filling_strategy/Strategies/AdvancingFrontFilling.hpp
        filling_strategy/Strategies/StupidFilling.cpp filling_strategy/Strategies/StupidFilling.hpp
        filling_strategy/Strategies/FastHoleFillingMethod.cpp filling_strategy/Strategies/FastHoleFillingMethod.hpp
        filling_strategy/Strategies/SequenceFillingStrategy.cpp filling_strategy/Strategies/SequenceFillingStrategy.hpp
        filling_strategy/Strategies/ClassicFillingStrategy.cpp filling_strategy/Strategies/ClassicFillingStrategy.hpp
        filling_strategy/Strategies/LiepraFilling.cpp filling_strategy/Strategies/LiepraFilling.hpp

        filling_strategy/InitialTriangulation/TriangulationStrategy.hpp
        filling_strategy/InitialTriangulation/FillSmall.cpp filling_strategy/InitialTriangulation/FillSmall.hpp
        filling_strategy/InitialTriangulation/StupidMinAreaTriangulation.cpp filling_strategy/InitialTriangulation/StupidMinAreaTriangulation.hpp
        filling_strategy/InitialTriangulation/MinAreaTriangulation.cpp filling_strategy/InitialTriangulation/MinAreaTriangulation.hpp
        filling_strategy/InitialTriangulation/AdvancingFrontMethods/AdvancingFront.cpp filling_strategy/InitialTriangulation/AdvancingFrontMethods/AdvancingFront.hpp
        filling_strategy/InitialTriangulation/AdvancingFrontMethods/ModernAdvancingFront.cpp filling_strategy/InitialTriangulation/AdvancingFrontMethods/ModernAdvancingFront.hpp
        filling_strategy/InitialTriangulation/WithoutTriangulation.cpp filling_strategy/InitialTriangulation/WithoutTriangulation.hpp
        filling_strategy/InitialTriangulation/AdvancingFrontMethods/BaseAdvancingFront.cpp
        filling_strategy/InitialTriangulation/AdvancingFrontMethods/BaseAdvancingFront.hpp

        filling_strategy/Refinement/RefinementStrategy.hpp
        filling_strategy/Refinement/DeloneLike.cpp filling_strategy/Refinement/DeloneLike.hpp
        filling_strategy/Refinement/WithoutRefinement.cpp filling_strategy/Refinement/WithoutRefinement.hpp

        filling_strategy/Fairing/FairingStrategy.hpp
        filling_strategy/Fairing/WithoutFairing.cpp filling_strategy/Fairing/WithoutFairing.hpp
        filling_strategy/Fairing/Tools.cpp filling_strategy/Fairing/Tools.hpp
        filling_strategy/Fairing/StupidUmbrella.cpp filling_strategy/Fairing/StupidUmbrella.hpp

        filling_strategy/tools/Hole.cpp filling_strategy/tools/Hole.hpp
        filling_strategy/tools/MeshController.cpp filling_strategy/tools/MeshController.hpp

        tools/string_tools.cpp tools/string_tools.hpp

        static/defines.hpp

        filling_strategy/Strategies/ModernAdvancingFrontFilling.cpp filling_strategy/Strategies/ModernAdvancingFrontFilling.hpp)

include_directories(base geometry io filling_strategy
        filling_strategy/InitialTriangulation
        filling_strategy/InitialTriangulation/AdvancingFrontMethods
        filling_strategy/Fairing
        filling_strategy/Refinement
        filling_strategy/tools
        filling_strategy/Strategies
        static
        tools)

add_executable(experiment_example examples/experiment_example.cpp)
add_executable(playground examples/playground.cpp)
add_executable(simple_example examples/simple_example.cpp)

target_link_libraries(experiment_example hole_filling_3d_lib)
target_link_libraries(playground hole_filling_3d_lib)
target_link_libraries(simple_example hole_filling_3d_lib)
