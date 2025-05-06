#include "InMemUserTask.h"

InMemUserTask::InMemUserTask()
{

}

InMemUserTask::~InMemUserTask()
{

}

std::string InMemUserTask::taskStatusString() const
{
    switch (status)
    {
        default:
            return "Unknown TaskStatus Value";
        case TaskStatus::Not_Started :
            return "Not Started";
        case TaskStatus::On_Hold:
            return "On Hold";
        case TaskStatus::Waiting_for_Dependency:
            return "Waiting for Dependency";
        case TaskStatus::Work_in_Progress:
            return "Work in Progress";
        case TaskStatus::Complete:
            return "Completed";
    }
}
