#ifndef USERQUERYPROCESSOR_H_
#define USERQUERYPROCESSOR_H_

// Project Header Files
#include "QueryProcessor.h"
#include "UserModel.h"

// Standard C++ Header Files
#include <format>
#include <iostream>

using UserQueryProcessorValues = std::vector<UserModel_shp>;

class UserQueryProcessor : public QueryProcessor<UserModel>
{
public:
    UserQueryProcessor();
    virtual ~UserQueryProcessor() = default;
    UserQueryProcessorValues getAllUsers() noexcept;

private:
    virtual std::vector<ListExceptionTestElement> initListExceptionTests() noexcept override;
    TestStatus testExceptionsGetAllUsers() noexcept;
};

#endif // USERQUERYPROCESSOR_H_

