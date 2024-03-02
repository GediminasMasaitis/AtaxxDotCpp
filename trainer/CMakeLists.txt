cmake_minimum_required(VERSION 3.12)
project(NnueTrainer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(Torch REQUIRED PATHS "C:/Libs/libtorch12/share/cmake/Torch")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(NnueTrainer "main.cpp")

target_link_libraries(NnueTrainer PRIVATE Threads::Threads "${TORCH_LIBRARIES}")

if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET NnueTrainer
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:NnueTrainer>)
endif (MSVC)