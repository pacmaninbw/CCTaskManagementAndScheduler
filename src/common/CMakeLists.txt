

foreach(target IN LISTS SHARED_SOURCE_TARGETS)
    target_sources(${target} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/commonUtilities.h
        ${CMAKE_CURRENT_SOURCE_DIR}/commonUtilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/commonTestValues.h
        ${CMAKE_CURRENT_SOURCE_DIR}/commonTestValues.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/chronoBoostConversions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/chronoBoostConversions.cpp
    )
    target_include_directories(${target} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
endforeach()
