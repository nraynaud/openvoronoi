SET(test_name "cpptest_minimal" )

MESSAGE(STATUS "configuring c++ test: " ${test_name})
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(SOURCE_FILES minimal.cpp)
add_executable( ${test_name} ${SOURCE_FILES} )
add_dependencies(${test_name}  libopenvoronoi)

target_link_libraries(${test_name} libopenvoronoi ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

ADD_TEST(${test_name} ${test_name})
