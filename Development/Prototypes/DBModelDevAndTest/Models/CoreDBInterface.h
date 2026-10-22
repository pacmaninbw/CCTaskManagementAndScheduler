/*
 * The CoreDBInterface provides a low level interface for input and output to 
 * the database. It also provides access to the program options to the models.
 * Knowledge of the program options struct and variables is limited to the
 * CoreDBInterface class to reduce external dependencies.
 */

#ifndef COREDBINTERFACECORE_H_
#define COREDBINTERFACECORE_H_

// Project Header Files

// External Libraries
#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <boost/mysql/pfr.hpp>

// Standard C++ Header Files
#include <iostream>
#include <optional>
#include <string>

class CoreDBInterface
{
public:
    CoreDBInterface();
    virtual ~CoreDBInterface() = default;
    std::string getAllErrorMessages() const noexcept { return m_errorMessages; };
    inline void clearErrorMessages() { m_errorMessages.clear(); };
    void setForceExceptions(bool forceIt) { m_forceException = forceIt; };
    void setSelfTest(bool isSelfTest) { m_selfTest = isSelfTest; };
    void testResetFormatOpts() { m_formatOpts.reset(); };

#ifdef SHOW_DEBUG_VARIABLES
    void debugShowVariables(std::string functionName) const noexcept
    {
        std::cout << "In " << functionName << ":\n";
        std::cout << "\tformat_opts: " << (m_formatOpts.has_value()? "TRUE" : "FALSE") << "\n";
        std::cout << "\tinSelfTest: " << (m_selfTest? "TRUE" : "FALSE") << "\n";
        std::cout << "\tforceException: " << (m_forceException? "TRUE" : "FALSE") << "\n";
        std::cout << "\tforceError: " << (m_forceError? "TRUE" : "FALSE") << "\n";
        std::cout << std::endl;
    }
#endif 

protected:
    void initFormatOptions();
    const boost::mysql::format_options& getFormatOptions() {
        initFormatOptions();
        return m_formatOpts.value();
    }
    void appendErrorMessage(const std::string& newError) { m_errorMessages.append(newError); m_errorMessages.append("\n");};
/*
 * for MySQL pattern match of string
 */
    std::string wrapSearchContentSQLPatternMatch(std::string searchString) const noexcept;

/*
 * All calls to runQueryAsync, staticRunQueryAsync and getConnectionFormatOptsAsync
 * should be implemented within try blocks.
 */
    boost::mysql::results runQueryAsync(const std::string& query);
    boost::mysql::format_options getConnectionFormatOptsAsync();
    boost::asio::awaitable<boost::mysql::results> coRoutineExecuteSqlStatement(const std::string& query);
    boost::asio::awaitable<boost::mysql::format_options> coRoutineGetFormatOptions();
    template <typename T>
    boost::asio::awaitable<boost::mysql::static_results<boost::mysql::pfr_by_name<T>>>
        coRoutineExecuteSqlStatement(const std::string& query)
    {
        boost::mysql::static_results<boost::mysql::pfr_by_name<T>> staticResults;
        if (m_forceException)
        {
            std::string forcingException("Forcing Exception in CoreDBInterface::coRoutineExecuteSqlStatement");
            std::domain_error forcedException(forcingException);
            throw forcedException;
        }

        if (m_selfTest)
        {
            if (m_verboseOutput)
            {
                std::cout << "In Self Test Query is: \n\t" << query << std::endl;
            }
            co_return staticResults;
        }

        boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

        co_await conn.async_connect(m_dbConnection);
        
        if (m_verboseOutput)
        {
            std::cout << "Running: \n\t" << query << std::endl;
        }

        co_await conn.async_execute(query, staticResults);

        co_await conn.async_close();

        co_return staticResults;
    };

    template <typename T>
    boost::mysql::static_results<boost::mysql::pfr_by_name<T>> staticRunQueryAsync(const std::string& query)
    {
        boost::mysql::static_results<boost::mysql::pfr_by_name<T>> staticResults;
        boost::asio::io_context ctx;

        boost::asio::co_spawn(ctx, coRoutineExecuteSqlStatement<T>(query),
            [&staticResults, this](std::exception_ptr ptr,
                boost::mysql::static_results<boost::mysql::pfr_by_name<T>> result)
            {
                if (ptr)
                {
                    std::rethrow_exception(ptr);
                }
                staticResults = std::move(result);
            }
        );

        ctx.run();

        return staticResults;
    };


    std::string m_errorMessages;// Provides database access error messsages to the application and for unit testing
    boost::mysql::connect_params m_dbConnection;
    bool m_verboseOutput;       // Used in unit testing and debugging
    bool m_forceError;          // Used in unit testing
    bool m_forceException;      // User in uint testing
    bool m_selfTest;            // used in unit testing
    bool m_showTimeStamps;      // Used in unit testing and debugging
    std::optional<boost::mysql::format_options> m_formatOpts;
};

#endif // COREDBINTERFACECORE_H_


