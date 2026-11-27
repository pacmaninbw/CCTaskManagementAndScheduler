

add_subdirectory(common)

add_subdirectory(dbmodels)

target_include_directories(libdbmodels PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/dbmodels
)

target_include_directories(unit_tests PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_compile_definitions(libdbmodels PRIVATE TESTCOMMANDLINEENABLED=1)

target_compile_options(libdbmodels PRIVATE -Wall -Wextra -pedantic -Werror -g -fprofile-arcs -ftest-coverage -lgcov)

target_compile_features(libdbmodels PRIVATE cxx_std_23)
