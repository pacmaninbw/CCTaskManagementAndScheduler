#ifndef GENERICDICTIONARY_H_
#define GENERICDICTIONARY_H_

#include <algorithm>
#include <exception>
#include <expected>
#include <initializer_list>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>

/*
 * This class provides a basic conversion of enums or integers to strings.  
 * 
 * This class makes an attempt to find any missing definitions during construction.
 * 
 */

/******************************************************************************
 * For errors in lookup functions
 *****************************************************************************/
enum class DictionaryLookUpError
{
    Id_Not_Found,
    Name_Not_Found
};

/******************************************************************************
 * Data structures and class Declarations
 *****************************************************************************/
template <typename DictID, typename DictName>
class GenericDictionary
{
public:
    struct DictType
    {
        DictID id;
        DictName names;
    };

// The following constructors will throw exceptions if there are problems in the
// list of definitions.
    GenericDictionary(std::initializer_list<DictType> definitions)
    {
        for (auto const& newDef : definitions)
        {
            userInputList.push_back(newDef);
        }

        if (!commonInternalListBuilder("Constructor"))
        {
            std::logic_error constructorError(exceptionWhatMsg);
            throw constructorError;
        }
    }

    template<std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, DictType>
    GenericDictionary(R&& definitions)
    : userInputList{std::ranges::begin(definitions), std::ranges::end(definitions)}
    {
        if (!commonInternalListBuilder("Constructor"))
        {
            std::logic_error constructorError(exceptionWhatMsg);
            throw constructorError;
        }
    }

    virtual ~GenericDictionary() = default;

    auto lookupID(DictName itemName) const -> std::expected<DictID, DictionaryLookUpError>
    {
        auto definition = nameSearchTable.find(itemName);
        if (definition != nameSearchTable.end())
        {
            return definition->second;
        }

        return std::unexpected{DictionaryLookUpError::Name_Not_Found};
    };

    auto lookupName(DictID id) const -> std::expected<DictName, DictionaryLookUpError>
    {
        auto definition = idSearchTable.find(id);
        if (definition != idSearchTable.end())
        {
            return definition->second;
        }

        return std::unexpected{DictionaryLookUpError::Id_Not_Found};
    }

#ifdef GD_UNIT_TEST
    std::vector<DictType> getUserInput() const noexcept { return userInputList; }
#endif

private:
    [[nodiscard]] bool commonInternalListBuilder(std::string funcName) noexcept
    {
        bool hasErrors = false;
        if (!testForIDNoneLinearDefinitions(funcName))
        {
            hasErrors = true;
        }

        if (reportDuplicateNameErrors(funcName))
        {
            hasErrors = true;
        }

        if (hasErrors)
        {
            return false;
        }
        
        // Errors have been found, there is no need to add the definitions to the maps.
        for (auto newDef: userInputList)
        {
            if (!addDefinition(newDef))
            {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] bool alreadyDefined(DictType candidate) noexcept { return hasID(candidate.id) || hasName(candidate.names); };

    [[nodiscard]] bool reportDuplicateNameErrors(std::string funcName) noexcept
    {
        std::vector<DictType> duplicates;
        // Find all duplicate names, store the name and the ID to generate an
        // error message after all duplicates are found.
        for (auto defintion: userInputList)
        {
            std::size_t countNames = std::ranges::count(userInputList, defintion.names, &DictType::names);
            if (countNames > 1) 
            {
                duplicates.push_back(defintion);
            }
        }

        if (duplicates.size() == 0)
        {
            return false;
        }

        exceptionWhatMsg += "In GenericDictionary::" + funcName + ": Duplicate names found: \n\t";
        for (auto duplicate: duplicates)
        {
            exceptionWhatMsg += "Name: " + duplicate.names + " Integer Value of ID: " +
                std::to_string(static_cast<std::size_t>(duplicate.id)) + "\n\t";
        }

        return true;
    }

    [[nodiscard]] bool hasID(DictID id) noexcept
    {
        auto found = idSearchTable.find(id);
        return (found != idSearchTable.end());
    }

    [[nodiscard]] bool hasName(DictName name) noexcept
    {
        auto found = nameSearchTable.find(name);
        return (found != nameSearchTable.end());
    }

    /*
    * Sorts the user input. After sorting the input performs various tests
    * on the validity of the input, finding duplicate IDs or missing IDs. The
    * input must be sorted before the the error checking can be performed.
    */
    [[nodiscard]] bool sortAndTestIds() noexcept
    {
        std::sort(userInputList.begin(), userInputList.end(),
            [](DictType defIter1, DictType defIter2) {return  defIter1.id < defIter2.id;});

        bool noErrors = true;

        for (auto defIter2 = userInputList.begin() + 1; defIter2 < userInputList.end(); defIter2++ )
        {
            auto defIter1 = defIter2 - 1;
            std::size_t id1 = static_cast<std::size_t>(defIter1->id);
            std::size_t id2 = static_cast<std::size_t>(defIter2->id);
            if (id1 != (id2 - 1))
            {
                if (id1 == id2)
                {
                    exceptionWhatMsg += "duplicate enum values: " + std::to_string(id1) + 
                        " Name: " + defIter1->names + " - " + std::to_string(id2) +
                        " Name: " + defIter2->names + "\n\t";
                }
                else
                {
                    exceptionWhatMsg += "missing enum value: " + std::to_string((id2 - 1)) + "\n\t";
                }
                noErrors = false;
            }
        }

        return noErrors;
    }
    /*
    * Find and report any missing or duplicated enums
    */
    [[nodiscard]] bool testForIDNoneLinearDefinitions(std::string funcName) noexcept
    {
        bool noErrors = true; 
        exceptionWhatMsg += "In GenericDictionary::" + funcName + ": \n\t";

        noErrors = sortAndTestIds();

        return noErrors;
    }

    [[nodiscard]] bool addDefinition(DictType newDef) noexcept
    {
        if (!alreadyDefined(newDef))
        {
            idSearchTable.insert({newDef.id, newDef.names});
            nameSearchTable.insert({newDef.names, newDef.id});
            return true;
        }

        return false;
    }

    std::unordered_map<DictID, DictName> idSearchTable;
    std::unordered_map<DictName, DictID> nameSearchTable;
    std::vector<DictType> userInputList;
    std::string exceptionWhatMsg;
};

#endif // GENERICDICTIONARY_H_

