cmake_minimum_required (VERSION 2.8)

project(templateSpecialization) 

# eigen in gdb does not make fun with optimization
add_definitions('-O0')
add_definitions('-g')
add_definitions('-Wall')

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)

# eye-candy: nice symlink...
add_custom_target(${PROJECT_NAME}-symlink ALL ln --force -s ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/${PROJECT_NAME} DEPENDS ${PROJECT_NAME})
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})

