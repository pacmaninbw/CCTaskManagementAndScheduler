#ifndef MODELDSELFTEST_H_
#define MODELDSELFTEST_H_

// Project Header Files
#include "AttributeSelfTest.h"
#include "ExceptionSelfTest.h"
#include "FunctionalSelfTest.h"
#include "ModelDBInterface.h"
#include "TestStatus.h"

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>

// Standard C++ Header Files
#include <concepts>
#include <type_traits>

template<class Model>
requires std::is_base_of_v<ModelDBInterface, Model>
class ModelSelfTest : public virtual ExceptionSelfTest<Model>,
                        public virtual AttributeSelfTest<Model>,
                        public virtual FunctionalSelfTest<Model>,
                        public virtual Model
{
public:

    ModelSelfTest() : Model() { }

    virtual ~ModelSelfTest() = default;
    virtual TestStatus runSelfTest() noexcept = 0;

protected:
/*****
 * ModelSelfTest::selfTestResetAllValues() should be called by any override.
 * The override should provide resets for any of the model under tests attributes.
 */
    virtual void selfTestResetAllValues()
    {
        ModelDBInterface::primaryKey = 0;
        ModelDBInterface::modified = false;
/*****
 * format_opts needs to be reset before any exception test
 */
        CoreDBInterface::format_opts.reset();
        CoreDBInterface::errorMessages.clear();
    }

    
};

#endif // MODELDSELFTEST_H_


