
include(FetchContent)
set(DOCTEST_VERSION v2.4.11)
FetchContent_Declare(doctest
    URL  https://raw.githubusercontent.com/doctest/doctest/${DOCTEST_VERSION}/doctest/doctest.h
    DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(doctest)

set(DOCTEST_INCLUDE_DIR ${doctest_SOURCE_DIR})
# Include doctest_discover_tests(<target>) function
#include("${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake")

# Use doctest in a target
#target_link_libraries(my_target PUBLIC doctest::doctest)

# Or use it globally
#link_libraries(doctest::doctest)

add_subdirectory(unittests)

add_subdirectory(integrationtests)
