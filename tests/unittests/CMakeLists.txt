
target_sources(unit_tests PRIVATE unit_test_main.cpp)

target_compile_definitions(unit_tests PRIVATE TESTCOMMANDLINEENABLED=1)

target_include_directories(unit_tests PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_compile_options(unit_tests PRIVATE -Wall -Wextra -pedantic -Werror -g -fprofile-arcs -ftest-coverage -lgcov)

target_compile_features(unit_tests PRIVATE cxx_std_23)

target_link_libraries(unit_tests ${PROJECT_NAME}::libdbmodels ${Boost_LIBRARIES} ssl crypto gcov)