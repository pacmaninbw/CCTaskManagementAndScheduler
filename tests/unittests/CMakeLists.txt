
target_sources(unit_tests PRIVATE unit_test_main.cpp)

target_compile_definitions(unit_tests PRIVATE TESTCOMMANDLINEENABLED=1)

target_include_directories(unit_tests PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/common>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/dbmodels>
)

# target_compile_options(unit_tests PRIVATE -Wall -Wextra -pedantic -Werror -pg -fprofile-arcs -ftest-coverage -lgcov)
target_compile_options(unit_tests PRIVATE -Wall -Wextra -pedantic -Werror -fprofile-arcs -ftest-coverage -lgcov)

target_compile_features(unit_tests PRIVATE cxx_std_23)

# target_link_options(unit_tests PRIVATE -pg)

target_link_libraries(unit_tests  ${Boost_LIBRARIES} ssl crypto gcov)