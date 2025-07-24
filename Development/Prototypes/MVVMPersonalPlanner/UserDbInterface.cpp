#include "CommandLineParser.h"
#include "DBInterface.h"
#include "UserDbInterface.h"
#include "UserModel.h"

UserDbInterface::UserDbInterface(ProgramOptions &programOptions)
: DBInterface(programOptions)
{
}
