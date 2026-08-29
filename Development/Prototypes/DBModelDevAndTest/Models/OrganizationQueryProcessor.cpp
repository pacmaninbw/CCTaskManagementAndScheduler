// Project Header Files
#include "commonUtilities.h"
#include "OrganizationQueryProcessor.h"
#include "OrganizationModel.h"
#include "chronoBoostConversions.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <vector>


OrganizationQueryProcessor::OrganizationQueryProcessor()
: QueryProcessor<OrganizationModel, OrganizationDbQueryValues>("OrganizationModel")
{

}

OrganizationQueryProcessor::~OrganizationQueryProcessor()
{

}