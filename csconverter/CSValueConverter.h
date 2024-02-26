#ifndef __CS_VALUE_CONVERTER_H__
#define __CS_VALUE_CONVERTER_H__

#include "CSHelper.h"

struct IValueConverter
{
	virtual const std::string& type() = 0;
	virtual bool typeOf(const std::string& aTypeCheck) { return aTypeCheck == type(); }
};

template<typename Type, typename Definition = void>
struct ValueConverter {};

// @ STRING CONVERTER @ 

template<typename StringType>
struct ValueConverter<StringType, CS::TypeHelper::isSame<StringType, std::string>> : public IValueConverter 
{
	const std::string& type() override { return CSTypeDefines::csStringType;  }

	StringType cast(const std::string& aValue)
	{
		return aValue;
	}

	StringType castTyped(const StringType& aValue)
	{
		StringType castedValue{};

		auto [valueType, value] = CS::DataHelper::splitTwoValues(aValue, CSTypeDefines::csTypeDelimiter);
		if (typeOf(valueType))
		{
			castedValue = cast(value);
		}
		else
		{
			CS::Errors::throwExceptionWithTypeMismatch(valueType, type());
		}

		return castedValue;
	}

	std::string toString(const StringType& aValue) { return aValue; }
	std::string toTyped(const StringType& aValue) { return CS::TypeHelper::typifyValue(type(), toString(aValue)); };
};

// @ INTEGRAL CONVERTER @ 

template<typename IntegralType>
struct ValueConverter<IntegralType, CS::TypeHelper::isIntergal<IntegralType>> : public IValueConverter
{
	const std::string& type() override { return CSTypeDefines::csIntegralType; }

	IntegralType cast(const std::string& aValue)
	{
		IntegralType castedValue{};
		std::stringstream ss(aValue);
		ss >> castedValue;
		return castedValue;
	}

	IntegralType castTyped(const std::string& aValue)
	{
		IntegralType castedValue{};

		auto [valueType, value] = CS::DataHelper::splitTwoValues(aValue, CSTypeDefines::csTypeDelimiter);
		if (typeOf(valueType))
		{
			castedValue = cast(value);
		}
		else
		{
			CS::Errors::throwExceptionWithTypeMismatch(valueType, type());
		}

		return castedValue;
	}

	std::string toString(const IntegralType& aValue) { return std::to_string(aValue); }
	std::string toTyped(const IntegralType& aValue) { return CS::TypeHelper::typifyValue(type(), toString(aValue)); };
};

// @ FLOATING CONVERTER @ 

template<typename FloatingType>
struct ValueConverter<FloatingType, CS::TypeHelper::isFloating<FloatingType>> : public IValueConverter
{
	const std::string& type() override { return CSTypeDefines::csFloatingType; }
	
	FloatingType cast(const std::string& aValue)
	{
		FloatingType castedValue{};
		std::stringstream ss(aValue);
		ss >> castedValue;
		return castedValue;
	}

	FloatingType castTyped(const std::string& aValue)
	{
		FloatingType castedValue{};

		auto [valueType, value] = CS::DataHelper::splitTwoValues(aValue, CSTypeDefines::csTypeDelimiter);
		if (typeOf(valueType))
		{
			castedValue = cast(value);
		}
		else
		{
			CS::Errors::throwExceptionWithTypeMismatch(valueType, type());
		}

		return castedValue;
	}

	std::string toString(const FloatingType& aValue) { return std::to_string(aValue); }
	std::string toStringTyped(const FloatingType& aValue) { return CS::TypeHelper::typifyValue(type(), aValue); }
};

// @ BOOL CONVERTER @ 

template<typename BoolType>
struct ValueConverter<BoolType, CS::TypeHelper::isSame<BoolType, bool>> : public IValueConverter
{
	const std::string& type() override { return CSTypeDefines::csFloatingType; }
	
	BoolType cast(const std::string& aValue)
	{
		return aValue == "+" ? true : false;
	}

	BoolType castTyped(const std::string& aValue)
	{
		BoolType castedValue{};

		auto [valueType, value] = CS::DataHelper::splitTwoValues(aValue, CSTypeDefines::csTypeDelimiter);
		if (typeOf(valueType))
		{
			castedValue = cast(value);
		}
		else
		{
			CS::Errors::throwExceptionWithTypeMismatch(valueType, type());
		}

		return castedValue;
	}
	
	std::string toString(const BoolType& aValue) { return aValue ? "+" : "-"; }
	std::string toTyped(const BoolType& aValue) { return CS::TypeHelper::typifyValue(type(), toString(aValue)); }
};

// @ ENUM CONVERTER @ 

template<typename EnumType>
struct ValueConverter<EnumType, CS::TypeHelper::isEnum<EnumType>> : public IValueConverter
{
	using EnumUnderlyingType = typename std::underlying_type<EnumType>::type;

	const std::string& type() override { return CSTypeDefines::csIntegralType; }

	EnumType cast(const std::string& aValue)
	{
		ValueConverter<EnumUnderlyingType> converter;
		return static_cast<EnumType>(converter.cast(aValue));
	}

	EnumType castTyped(const std::string& aValue)
	{
		EnumType castedValue{};

		auto [valueType, value] = CS::DataHelper::splitTwoValues(aValue, CSTypeDefines::csTypeDelimiter);
		if (typeOf(valueType))
		{
			castedValue = cast(value);
		}
		else
		{
			CS::Errors::throwExceptionWithTypeMismatch(valueType, type());
		}

		return castedValue;
	}

	std::string toString(const EnumType& aValue) { return std::to_string(static_cast<EnumUnderlyingType>(aValue)); }
	std::string toTyped(const EnumType& aValue) { return CS::TypeHelper::typifyValue(type(), toString(aValue)); }
};

// @ VECTOR CONVERTER @ 
template<typename VectorType>
struct ValueConverter<VectorType, CS::TypeHelper::isSame<VectorType, std::vector<typename VectorType::value_type>>> : public IValueConverter
{
	using ValueType = typename VectorType::value_type;

	const std::string& type() override { return CSTypeDefines::csVectorType; }

	VectorType cast(const std::string& aValue)
	{
		VectorType casteValue{};
		
		auto splittedValues = CS::DataHelper::splitString(aValue, CSTypeDefines::csContainerValuesDelimiter);
		if (!splittedValues.empty())
		{
			ValueConverter<ValueType> valueConverter;
			for (auto& value : splittedValues)
			{
				casteValue.push_back(valueConverter.cast(value));
			}
		}

		return casteValue;
	}

	VectorType castTyped(const std::string& aValue)
	{
		VectorType casteValue{};

		auto splittedValues = CS::DataHelper::splitString(aValue, CSTypeDefines::csTypeDelimiter);
		if (splittedValues.size() == 3)
		{
			const auto& vectorType = splittedValues[0];
			const auto& innerType = splittedValues[1];
			const auto& vectorValues = splittedValues[2];

			if (typeOf(vectorType))
			{
				ValueConverter<ValueType> valueConverter;
				if (!valueConverter.typeOf(innerType))
				{
					CS::Errors::throwExceptionWithTypeMismatch(innerType, type());
				}

				casteValue = cast(vectorValues);
			}
			else
			{
				CS::Errors::throwExceptionWithTypeMismatch(vectorType, type());
			}
		}

		return casteValue;
	}

	std::string toString(const VectorType& aValue) 
	{ 
		std::string string;

		if (!aValue.empty())
		{
			ValueConverter<ValueType> valueConverter;
			auto valuesAmount = aValue.size();

			for (auto& value : aValue)
			{
				string.append(valueConverter.toString(value));
				if (--valuesAmount != 0) 
				{
					string.append(CSTypeDefines::csContainerValuesDelimiter);
				}
			}
		}

		return string;
	}

	std::string toTyped(const VectorType& aValue) 
	{ 
		ValueConverter<ValueType> valueConverter;
		std::string formatedType = CS::TypeHelper::formatType({ type(), valueConverter.type() });
		return CS::TypeHelper::typifyValue(std::move(formatedType), toString(aValue));
	}
};
#endif // !__CS_VALUE_CONVERTER_H__