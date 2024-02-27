#ifndef _CS_HELPER_H__
#define _CS_HELPER_H__

#include <type_traits>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <sstream>

namespace CSTypeDefines
{
    const std::string csStringType = "s";
    const std::string csIntegralType = "i";
    const std::string csFloatingType = "f";
    const std::string csMapType = "m";
    const std::string csBoolType = "b";
    const std::string csVectorType = "v";
    const std::string csPairType = "p";

    const std::string csTypeDelimiter = "$";
    const std::string csValueDelimiter = "|";

    const std::string csContainerValuesDelimiter = "^";

    const std::string csMapValueDelimiter = "#";
};

struct CS
{
    struct TypeHelper
    {
        template<typename T>
        using isEnum = typename std::enable_if_t<std::is_enum<T>::value>;

        template<typename T>
        using isArithmetic = typename std::enable_if_t<std::is_arithmetic<T>::value>;

        template<typename T>
        using isIntergal = typename std::enable_if_t<std::is_integral<T>::value>;

        template<typename T>
        using isFloating = typename std::enable_if_t<std::is_floating_point<T>::value>;

        template<typename A, typename B>
        using isSame = typename std::enable_if_t<std::is_same<A, B>::value>;

        template<typename T>
        using isMap = typename std::enable_if_t<std::_Is_any_of_v<T, std::map<typename T::key_type, typename T::mapped_type>, 
            std::unordered_map<typename T::key_type, typename T::mapped_type>>>;

        template<typename T>
        using isPair = isSame<T, std::pair<typename T::first_type, typename T::second_type>>;

        static std::string tryGetFullType(const std::string& aShortType)
        {
            if (aShortType == CSTypeDefines::csStringType)
            {
                return "String";
            }
            else if (aShortType == CSTypeDefines::csIntegralType)
            {
                return "Integral";
            }
            else if (aShortType == CSTypeDefines::csFloatingType)
            {
                return "Float";
            }
            else if (aShortType == CSTypeDefines::csMapType)
            {
                return "Map";
            }
            else if (aShortType == CSTypeDefines::csBoolType)
            {
                return "Bool";
            }
            else if (aShortType == CSTypeDefines::csVectorType)
            {
                return "Vector";
            }
            else
            {
                return "Undefined";
            }

            return "Undefined";
        }

        static std::string typifyValue(const std::string& aType, std::string&& aValue)
        {
            std::string typifyedValue;
            typifyedValue += aType;
            typifyedValue += CSTypeDefines::csTypeDelimiter;
            typifyedValue += std::move<>(aValue);

            return typifyedValue;
        }

        static std::string typifyValue(std::string&& aType, std::string&& aValue)
        {
            std::string typifyedValue;
            typifyedValue += std::move(aType);
            typifyedValue += CSTypeDefines::csTypeDelimiter;
            typifyedValue += std::move(aValue);

            return typifyedValue;
        }

        static std::string formatType(std::vector<std::string>&& aTypes)
        {
            std::string formatedType;
            auto typesCount = aTypes.size();
            for (auto& type : aTypes)
            {
                formatedType += std::move(type);
                if (--typesCount != 0)
                {
                    formatedType += CSTypeDefines::csTypeDelimiter;
                }
            }

            return formatedType;
        }

        static std::string formatContainerTypes(const std::string& aType, std::vector<std::string>&& aInnerTypes)
        {
            std::string innerTyeps;
            innerTyeps += aType;
            innerTyeps += CSTypeDefines::csTypeDelimiter;

            auto typesCount = aInnerTypes.size();
            for (auto& innerType : aInnerTypes)
            {
                innerTyeps += std::move(innerType);
                if (--typesCount != 0)
                {
                    innerTyeps += CSTypeDefines::csContainerValuesDelimiter;
                }
            }
            
            innerTyeps += CSTypeDefines::csTypeDelimiter;

            return innerTyeps;
        }
    };

    struct DataHelper
    {
        static std::vector<std::string> splitString(const std::string& str, const std::string& delimiter) 
        {
            std::vector<std::string> result;
            std::string::size_type start = 0;
            std::string::size_type end = str.find(delimiter);

            while (end != std::string::npos) {
                result.push_back(str.substr(start, end - start));
                start = end + delimiter.length();
                end = str.find(delimiter, start);
            }

            result.push_back(str.substr(start));

            return result;
        }

        static std::pair<std::string, std::string> splitTwoValues(const std::string& aStr, const std::string& aDelimiter)
        {
            auto splited = splitString(aStr, aDelimiter);
            if (splited.size() == 2)
            {
                return { splited[0], splited[1] };
            }

            return {"", ""};
        }
    };

    struct Errors
    {
        static void throwExceptionWithTypeMismatch(const std::string& aTypeUsed, const std::string& aTypeRequired)
        {
            std::string usedFullType = CS::TypeHelper::tryGetFullType(aTypeUsed);
            std::string requiredFullType = CS::TypeHelper::tryGetFullType(aTypeUsed);
            std::string errorMessage = "Type mismatch. Trying to convert from " + usedFullType + " to " + requiredFullType;

            throw std::runtime_error(errorMessage);
        }

        static void throwExceptionTypeRedefinition(const std::string& aKeyWrited)
        {
            std::string errorMessage = "Type Redefinition. Trying to write " + aKeyWrited + " when already defined.";

            throw std::runtime_error(errorMessage);
        }

        static void throwExceptionFailedToWrieNullptr(const std::string& aDataKey)
        {
            std::string errorMessage = "Bad Pointer. Failed to wrie data to " + aDataKey + ". Reffer to nullptr value";

            throw std::runtime_error(errorMessage);
        }

        static void throwExceptionFailedToReadNullptr(const std::string& aDataKey)
        {
            std::string errorMessage = "Bad Pointer. Failed to read data from " + aDataKey + ". Reffer to nullptr value";

            throw std::runtime_error(errorMessage);
        }
    };

};

#endif // !_CS_HELPER_H__