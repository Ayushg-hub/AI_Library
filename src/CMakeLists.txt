add_subdirectory(common)

add_library(AI STATIC  "ML/linearRegression.h" "ML/linearRegression.cpp"  )

target_link_libraries(AI PUBLIC Tensor)

target_include_directories(AI PUBLIC common)