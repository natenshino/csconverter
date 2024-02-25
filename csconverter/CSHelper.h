#ifndef _CS_HELPER_H__
#define _CS_HELPER_H__

#include <type_traits>
#include <string>
#include <map>
#include <vector>
#include <sstream>

namespace CSTypeDefines
{
    const std::string csStringType = "s";
    const std::string csIntegralType = "i";
    const std::string csFloatingType = "f";
    const std::string csMapType = "m";
    const std::string csBoolType = "b";

    const std::string csTypeDelimiter = "$";

    const std::string csMapKeyValueDelimiter = ":";
    const std::string csMapValueDelimiter = ",";

    const std::string csValueDelimiter = "|";
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
        using isMap = typename std::enable_if_t<std::is_same<typename T::value_type, std::pair<const typename T::key_type, typename T::mapped_type>>::value>;

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
            else
            {
                return "Undefined";
            }

            return "Undefined";
        }

        static std::string typifyValue(const std::string& aType, const std::string& aValue)
        {
            std::string typifyedValue;
            typifyedValue.append(aType);
            typifyedValue.append(CSTypeDefines::csTypeDelimiter);
            typifyedValue.append(aValue);

            return typifyedValue;
        }

        static bool checkType(const std::string& aLeft, const std::string& aRight)
        {
            return aLeft == aRight;
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

        static std::pair<std::string, std::string> splitToTypeAndValue(const std::string& aStr)
        {
            auto splited = splitString(aStr, CSTypeDefines::csTypeDelimiter);
            if (splited.size() > 2)
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
            std::runtime_error("Type mismatch. Trying to convert from " + usedFullType + " to " + requiredFullType);
        }

        static void throwExceptionTypeRedefinition(const std::string& aKeyWrited)
        {
            std::runtime_error("Type Redefinition. Trying to write " + aKeyWrited + " when already defined.");
        }
    };

};

#endif // !_CS_HELPER_H__