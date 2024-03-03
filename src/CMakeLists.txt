cmake_minimum_required(VERSION 3.12)
project(AtaxxDotCpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_executable(AtaxxDotCpp "main.cpp" "position.cpp" "fens.cpp" "movegen.cpp" "perft.cpp" "move.cpp" "uai.cpp" "search.cpp"  "timer.cpp" "moveorder.cpp" "datagen.cpp" "display.cpp" "evaluation_nn.cpp" "evaluation_hce.cpp" "evaluation_nn_base.cpp")

target_link_libraries(AtaxxDotCpp PRIVATE Threads::Threads)