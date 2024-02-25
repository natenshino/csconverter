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

	std::string toString(StringType aValue) { return aValue; }
	std::string toTyped(StringType aValue) { return CS::TypeHelper::typifyValue(type(), toString(aValue)); };
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

	std::string toString(IntegralType aValue) { return std::to_string(aValue); }
	std::string toTyped(IntegralType aValue) { return CS::TypeHelper::typifyValue(type(), toString(aValue)); };
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

	std::string toString(FloatingType aValue) { return std::to_string(aValue); }
	std::string toStringTyped(FloatingType aValue) { return CS::TypeHelper::typifyValue(type(), aValue); }
};

// @ BOOL CONVERTER @ 

template<typename BoolType>
struct ValueConverter<BoolType, CS::TypeHelper::isSame<BoolType, bool>> : public IValueConverter
{
	const std::string& type() override { return CSTypeDefines::csFloatingType; }
	
	BoolType cast(const std::string& aValue)
	{
		return aValue = aValue == "+" ? true : false;
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
	
	std::string toString(BoolType aValue) { return aValue ? "+" : "-"; }
	std::string toTyped(BoolType aValue) { return CS::TypeHelper::typifyValue(type(), toString(aValue)); }
};

// @ ENUM CONVERTER @ 

template<typename EnumType>
struct ValueConverter<EnumType, CS::TypeHelper::isEnum<EnumType>> : public IValueConverter
{
	const std::string& type() override { return CSTypeDefines::csFloatingType; }

	EnumType cast(const std::string& aValue)
	{
		typedef std::underlying_type<EnumType>::type UnderlyingType;
		ValueConverter<UnderlyingType> converter;
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

	std::string toString(EnumType aValue) { return std::to_string(static_cast<std::underlying_type<EnumType>::type>(aValue)); }
	std::string toTyped(EnumType aValue) { return CS::TypeHelper::typifyValue(type(), toString(aValue)); }
};

// @ VECTOR CONVERTER @ 
template<typename VectorType>
struct ValueConverter<VectorType, CS::TypeHelper::isSame<VectorType, std::vector<typename VectorType::value_type>>> : public IValueConverter
{
	const std::string& type() override { return CSTypeDefines::csVectorType; }

	VectorType cast(const std::string& aValue)
	{
		VectorType casteValue{};
		
		auto splittedValues = CS::DataHelper::splitString(aValue, CSTypeDefines::csContainerValuesDelimiter);
		if (!splittedValues.empty())
		{
			ValueConverter<VectorType::value_type> valueConverter;
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
				ValueConverter<VectorType::value_type> valueConverter;
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

	std::string toString(VectorType aValue) 
	{ 
		std::string string;

		if (!aValue.empty())
		{
			ValueConverter<VectorType::value_type> valueConverter;
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

	std::string toTyped(VectorType aValue) 
	{ 
		ValueConverter<VectorType::value_type> valueConverter;
		std::string formatedType = CS::TypeHelper::formatType({ type(), valueConverter.type() });
		return CS::TypeHelper::typifyValue(std::move(formatedType), toString(aValue));
	}
};
#endif // !__CS_VALUE_CONVERTER_H__