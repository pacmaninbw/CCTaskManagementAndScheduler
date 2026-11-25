

target_include_directories(unit_tests PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_sources(unit_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDBConnection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDBConnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UtilityTimer.h
)

add_subdirectory(unittests)

add_subdirectory(integrationtests)
