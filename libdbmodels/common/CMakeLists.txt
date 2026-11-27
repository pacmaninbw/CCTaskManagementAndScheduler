
target_include_directories(unit_tests PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_sources(unit_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commonUtilities.h
    ${CMAKE_CURRENT_SOURCE_DIR}/commonUtilities.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commonTestValues.h
    ${CMAKE_CURRENT_SOURCE_DIR}/commonTestValues.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chronoBoostConversions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/chronoBoostConversions.cpp
)

target_sources(libdbmodels PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commonUtilities.h
    ${CMAKE_CURRENT_SOURCE_DIR}/commonUtilities.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commonTestValues.h
    ${CMAKE_CURRENT_SOURCE_DIR}/commonTestValues.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chronoBoostConversions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/chronoBoostConversions.cpp
)

