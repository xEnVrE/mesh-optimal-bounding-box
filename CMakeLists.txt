cmake_minimum_required(VERSION 3.16)

project(mesh-optimal-bounding-box)

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")

set(TARGET_NAME ${PROJECT_NAME})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(PkgConfig REQUIRED)

# Eigen
find_package(Eigen3 REQUIRED)

# CGAL
find_package(CGAL REQUIRED)

# libconfig
pkg_check_modules(LIBCONFIG++ REQUIRED IMPORTED_TARGET libconfig++)

# tclap
pkg_check_modules(TCLAP REQUIRED tclap)

add_executable(${TARGET_NAME}
               src/main.cpp
               src/ConfigParser.cpp
               src/OptimalBoundingBox.cpp
               include/ConfigParser.h
               include/OptimalBoundingBox.h
)

target_include_directories(${TARGET_NAME} PRIVATE
                           ${TCLAP_INCLUDEDIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${TARGET_NAME} PRIVATE
                      CGAL::CGAL
                      Eigen3::Eigen
                      PkgConfig::LIBCONFIG++
)
install(TARGETS ${TARGET_NAME} DESTINATION bin)

include(AddUninstallTarget)
