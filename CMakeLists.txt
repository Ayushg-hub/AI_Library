# CMakeList.txt : CMake project for AI_Library, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (AI_Library)

#include_directories(src/common)

#link_directories(src/common)

#add_subdirectory(src/common)
add_subdirectory(src)

# Add source to this project's executable.
add_executable (AI_Library "AI_Library.cpp" "AI_Library.h")

#target_link_libraries(AI_Library PUBLIC Tensor)
target_link_libraries(AI_Library PUBLIC AI)

#target_include_directories(AI_Library PUBLIC src/common)
#target_include_directories(AI_Library PUBLIC src)

#target_link_directories(AI_Library PUBLIC src/common)



# TODO: Add tests and install targets if needed.
