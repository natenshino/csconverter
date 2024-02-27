#ifndef __CS_CONVERTABLE_VALUE_H__
#define __CS_CONVERTABLE_VALUE_H__

#include "CSValueConverter.h"

struct IConvertableValue
{
	virtual void setValue(const std::string& aValue) {};
	virtual std::string asString() { return ""; };
	virtual ~IConvertableValue() {};
};

template <typename Type, typename TypeDefiniton = void>
struct ConvertableValue : public IConvertableValue
{
private:
	std::string valueName;
	Type* valuePointer;

public:
	ConvertableValue(const std::string& aValueName, Type* aValuePtr) 
		: valueName{aValueName}, 
		valuePointer{aValuePtr} {};

	void setValue(const std::string& aValue) override
	{
		if (valuePointer)
		{
			ValueConverter<Type> converter;
			*valuePointer = std::move(converter.castTyped(aValue));
		}
		else
		{
			CS::Errors::throwExceptionFailedToWrieNullptr(valueName);
		}
	};

	std::string asString() override
	{
		std::string result;
		if (valuePointer)
		{
			ValueConverter<Type> converter;
			result = converter.toTyped(*valuePointer);
		}
		else
		{
			CS::Errors::throwExceptionFailedToReadNullptr(valueName);
		}

		return result;
	};
};

template<typename MapType>
struct ConvertableValue<MapType, CS::TypeHelper::isMap<MapType>> : public IConvertableValue
{
private:
	std::string valueName;
	MapType* valuePointer;

public:
	using KeyType = typename MapType::key_type;
	using ValueType = typename MapType::mapped_type;

	ConvertableValue(const std::string& aValueName, MapType* aValuePtr) : valueName{ aValueName }, valuePointer{ aValuePtr } {}
	void setValue(const std::string& aValue) override
	{
		ValueConverter<KeyType> keyConverter;
		ValueConverter<ValueType> valueConverter;

		MapType& map = *valuePointer;

		auto splitedMapData = CS::DataHelper::splitString(aValue, CSTypeDefines::csTypeDelimiter);
		if (splitedMapData.size() == 3)
		{
			const auto& mapType = splitedMapData[0];
			const auto& mapInnerType = splitedMapData[1];
			const auto& mapValues = splitedMapData[2];

			if (!mapType.empty() && !mapInnerType.empty() && !mapValues.empty())
			{
				if (mapType == CSTypeDefines::csMapType)
				{
					auto [keyType, valueType] = CS::DataHelper::splitTwoValues(mapInnerType, CSTypeDefines::csContainerValuesDelimiter);
					if (!keyConverter.typeOf(keyType))
					{
						CS::Errors::throwExceptionWithTypeMismatch(keyType, keyConverter.type());
					}

					if (!valueConverter.typeOf(valueType))
					{
						CS::Errors::throwExceptionWithTypeMismatch(valueType, valueConverter.type());
					}

					auto mapEntries = CS::DataHelper::splitString(mapValues, CSTypeDefines::csMapValueDelimiter);
					for (const auto& keyValue : mapEntries)
					{
						const auto [key, value] = CS::DataHelper::splitTwoValues(keyValue, CSTypeDefines::csContainerValuesDelimiter);
						map.emplace(keyConverter.cast(key), valueConverter.cast(value));
					}
				}
				else
				{
					CS::Errors::throwExceptionWithTypeMismatch(mapType, CSTypeDefines::csMapType);
				}
			}
		}
	};

	std::string asString() override
	{
		std::string string;

		ValueConverter<KeyType> keyConverter;
		ValueConverter<ValueType> valueConverter;

		MapType& map = *valuePointer;
		auto valuesAmount = map.size();

		auto keyType = keyConverter.type();
		auto valueType = valueConverter.type();

		string += CS::TypeHelper::formatContainerTypes(CSTypeDefines::csMapType, {keyType, valueType});

		for (auto& [key, value] : map)
		{
			string += keyConverter.toString(key) + CSTypeDefines::csContainerValuesDelimiter + valueConverter.toString(value);
			if (--valuesAmount != 0)
			{
				string += CSTypeDefines::csMapValueDelimiter;
			}
		}

		return string;;
	};
};

#endif // !__CS_CONVERTABLE_VALUE_H__     