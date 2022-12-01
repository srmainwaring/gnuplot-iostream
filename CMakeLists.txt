cmake_minimum_required(VERSION 3.8)
project(gnuplot-iostream)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#options.
option(GnuPlotIostream_BuildTests "build gnuplot iostream tests" OFF)
option(GnuPlotIostream_BuildExamples "build gnuplot iostream examples" ON)

# find dependencies
find_package(ament_cmake REQUIRED)

find_package(Boost REQUIRED COMPONENTS
    iostreams system filesystem
)

# Target.
add_library(gnuplot-iostream INTERFACE)
target_include_directories(gnuplot-iostream INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(gnuplot-iostream INTERFACE
    Boost::iostreams
    Boost::system
    Boost::filesystem
)
ament_target_dependencies(gnuplot-iostream)

# Tests
if(GnuPlotIostream_BuildTests)
  set(GnuPlotIostream_tests
    test-assert-depth
    test-assert-depth-colmajor
    test-empty
    test-noncopyable
    test-outputs
    )
  foreach(atest ${GnuPlotIostream_tests})
    add_executable(${atest} src/${atest}.cc)
    target_compile_features(${atest} PRIVATE cxx_std_17)
    target_compile_options(${atest} PRIVATE -Wall -Wextra)
    target_link_libraries(${atest} PRIVATE
      gnuplot-iostream
      boost_iostreams
      boost_system
      boost_filesystem
      )
  endforeach()
endif()

# Examples
if(GnuPlotIostream_BuildExamples)
  set(GnuPlotIostream_examples
    example-data-1d
    example-data-2d
    example-interactive
    example-misc
    )
  foreach(an_example ${GnuPlotIostream_examples})
    add_executable(${an_example} src/${an_example}.cc)
    target_compile_features(${an_example} PRIVATE cxx_std_17)
    target_compile_options(${an_example} PRIVATE -Wall -Wextra)
    target_link_libraries(${an_example} PRIVATE 
      gnuplot-iostream
      util
    )
    ament_target_dependencies(${an_example}
      # gnuplot-iostream
      # util
    )
  endforeach()
endif()

# Export ament targets
ament_export_include_directories(include)

# Install includes
install(
  DIRECTORY include/
  DESTINATION include
)

# Install targets
install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# Install executables
install(
  TARGETS
    example-data-1d
    example-data-2d
    example-interactive
    example-misc
  DESTINATION lib/${PROJECT_NAME}
)

# Tests
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
