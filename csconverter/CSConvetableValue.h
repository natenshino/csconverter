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
	Type* valuePointer;
	ConvertableValue(Type* aValuePtr) : valuePointer(aValuePtr) {};

	void setValue(const std::string& aValue) override
	{
		ValueConverter<Type> converter;
		*valuePointer = converter.cast(aValue);
	};

	std::string asString() override
	{
		ValueConverter<Type> converter;
		return converter.toTyped(*valuePointer);
	};
};

template<typename MapType>
struct ConvertableValue<MapType, CS::TypeHelper::isMap<MapType>> : public IConvertableValue
{
	MapType* valuePointer;
	ConvertableValue(MapType* aValuePtr) : valuePointer(aValuePtr) {}
	void setValue(const std::string& aValue) override
	{
		ValueConverter<MapType::key_type> keyConverter;
		ValueConverter<MapType::mapped_type> valueConverter;

		MapType& map = *valuePointer;

		auto splitedValues = CS::DataHelper::splitString(aValue, CSTypeDefines::csMapValueDelimiter);
		for (const auto& keyValue : splitedValues)
		{
			const auto splitedKeyValue = CS::DataHelper::splitString(keyValue, CSTypeDefines::csMapKeyValueDelimiter);
			const auto& key = splitedKeyValue[0];
			const auto& value = splitedKeyValue[1];
			
			map.emplace(keyConverter.cast(key), valueConverter.cast(value));
		}
	};

	std::string asString() override
	{
		std::string string;

		ValueConverter<MapType::key_type> keyConverter;
		ValueConverter<MapType::mapped_type> valueConverter;

		MapType& map = *valuePointer;
		auto valuesAmount = map.size();

		for (auto& [key, value] : map)
		{
			string += keyConverter.toString(key) + CSTypeDefines::csMapValueDelimiter + valueConverter.toString(value);
			if (--valuesAmount != 0)
			{
				string += CSTypeDefines::csMapKeyValueDelimiter;
			}
		}

		return string;;
	};
};

#endif // !__CS_CONVERTABLE_VALUE_H__