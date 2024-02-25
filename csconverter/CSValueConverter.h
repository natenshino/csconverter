#ifndef __CS_VALUE_CONVERTER_H__
#define __CS_VALUE_CONVERTER_H__

#include "CSHelper.h"

template<typename Type, typename Definition = void>
struct ValueConverter {};

// @ STRING CONVERTER @ 

template<typename StringType>
struct ValueConverter<StringType, CS::TypeHelper::isSame<StringType, std::string>> 
{
private:
	const std::string getType() { return CSTypeDefines::csStringType;  }
public:
	std::string cast(const std::string& aValue)
	{
		StringType castedValue{};
		
		auto [type, value] = CS::DataHelper::splitToTypeAndValue(aValue);
		if (CS::TypeHelper::checkType(getType(), type))
		{
			castedValue = std::move(value);
		}
		else
		{
			CS::Errors::throwExceptionWithTypeMismatch(type, getType());
		}

		return castedValue;
	}

	std::string toString(StringType aValue) { return aValue; }
	std::string toTyped(StringType aValue) { return CS::TypeHelper::typifyValue(getType(), toString(aValue)); };
};

// @ INTEGRAL CONVERTER @ 

template<typename IntegralType>
struct ValueConverter<IntegralType, CS::TypeHelper::isIntergal<IntegralType>>
{
private:
	const std::string getType()
	{
		return CSTypeDefines::csIntegralType;
	}

public:
	IntegralType cast(const std::string& aValue)
	{
		IntegralType castedValue{};

		auto [type, value] = CS::DataHelper::splitToTypeAndValue(aValue);
		if (CS::TypeHelper::checkType(getType(), type))
		{
			std::stringstream ss(aValue);
			ss >> castedValue;
		}
		else
		{
			CS::Errors::throwExceptionWithTypeMismatch(type, getType());
		}

		return castedValue;
	}

	std::string toString(IntegralType aValue) { return std::to_string(aValue); }
	std::string toTyped(IntegralType aValue) { return CS::TypeHelper::typifyValue(getType(), toString(aValue)); };
};

// @ FLOATING CONVERTER @ 

template<typename FloatingType>
struct ValueConverter<FloatingType, CS::TypeHelper::isFloating<FloatingType>> 
{
private:
	const std::string getType() noexcept
	{
		return CSTypeDefines::csFloatingType;
	}
public:
	FloatingType cast(const std::string& aValue)
	{
		FloatingType castedValue{};

		auto [type, value] = CS::DataHelper::splitToTypeAndValue(aValue);
		if (CS::TypeHelper::checkType(getType(), type))
		{
			std::stringstream ss(aValue);
			ss >> castedValue;
		}
		else
		{
			CS::Errors::throwExceptionWithTypeMismatch(type, getType());
		}

		return castedValue;
	}

	std::string toString(FloatingType aValue) { return std::to_string(aValue); }
	std::string toStringTyped(FloatingType aValue) { return CS::TypeHelper::typifyValue(getType(), aValue); }
};

// @ BOOL CONVERTER @ 

template<typename BoolType>
struct ValueConverter<BoolType, CS::TypeHelper::isSame<BoolType, bool>>
{
private:
	const std::string getType() noexcept
	{
		return CSTypeDefines::csFloatingType;
	}
public:
	BoolType cast(const std::string& aValue)
	{
		BoolType castedValue = false;

		auto [type, value] = CS::DataHelper::splitToTypeAndValue(aValue);
		if (CS::TypeHelper::checkType(getType(), type))
		{
			castedValue = value == "+" ? true : false;
		}
		else
		{
			CS::Errors::throwExceptionWithTypeMismatch(type, getType());
		}

		return castedValue;
	}

	std::string toString(BoolType aValue) { return aValue ? "+" : "-"; }
	std::string toTyped(BoolType aValue) { return CS::TypeHelper::typifyValue(getType(), toString(aValue)); }
};

// @ ENUM CONVERTER @ 

template<typename EnumType>
struct ValueConverter<EnumType, CS::TypeHelper::isEnum<EnumType>>
{
private:
	std::string getType() noexcept
	{
		return CSTypeDefines::csFloatingType;
	}
public:
	EnumType cast(const std::string& aValue)
	{
		typedef std::underlying_type<EnumType>::type UnderlyingType;
		ValueConverter<UnderlyingType> converter;
		return static_cast<EnumType>(converter.cast(aValue));
	}

	std::string toString(EnumType aValue) { return std::to_string(static_cast<std::underlying_type<EnumType>::type>(aValue)); }
	std::string toTyped(EnumType aValue) { return CS::TypeHelper::typifyValue(getType(), toString(aValue)); }
};

#endif // !__CS_VALUE_CONVERTER_H__