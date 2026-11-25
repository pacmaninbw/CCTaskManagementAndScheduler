

set (DBMODEL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CoreDBInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CoreDBInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GoalQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GoalQueryProcessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelDBInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelDBInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/NoteModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NoteModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/NoteQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NoteQueryProcessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OrganizationModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OrganizationModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OrganizationQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OrganizationQueryProcessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/QueryProcessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ScheduleItemModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ScheduleItemModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ScheduleItemQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ScheduleItemQueryProcessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TaskModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TaskModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TaskQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TaskQueryProcessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/UserGoalModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserGoalModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/UserModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/UserQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserQueryProcessor.h
)

foreach(target IN LISTS SHARED_SOURCE_TARGETS)
    target_sources(${target} PRIVATE ${DBMODEL_SOURCES})
    target_include_directories(${target} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
endforeach()
