// License
//
// Created by [Daniil Puzdrach]
//
// Repository https://github.com/natenshino/csconverter
// 
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain.We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors.We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#ifndef __CS_CONVERTABLE_STRUCT_H__
#define __CS_CONVERTABLE_STRUCT_H__

#include "CSHelper.h"
#include "CSConvetableValue.h"

struct ConvertableStruct
{
private:
	std::map<std::string, IConvertableValue*> keyValue;
public:
	ConvertableStruct() {};
	virtual ~ConvertableStruct() {};

	void convertLineSaveToObject(const std::string& aLineSave)
	{
		auto splitedProgress = CS::DataHelper::splitString(aLineSave, CSTypeDefines::csValueDelimiter);
		std::reverse(splitedProgress.begin(), splitedProgress.end());

		for (auto& [key, valueVar] : keyValue)
		{
			valueVar->setValue(splitedProgress.back());
			splitedProgress.pop_back();
		}
	}

	std::string convertObjectToLineSave()
	{
		std::string string;
		auto keysAmount = keyValue.size();

		for (auto& [key, valueVar] : keyValue)
		{
			string += valueVar->asString();
			if (--keysAmount != 0)
			{
				string += CSTypeDefines::csValueDelimiter;
			}
		}
		return string;
	}

	template<typename Type>
	void pushValue(const std::string& aKey, Type* aValuePointer)
	{
		auto findIt = keyValue.find(aKey);
		if (findIt == keyValue.end())
		{
			keyValue.emplace(aKey, new ConvertableValue<Type>(aValuePointer));
		}
		else
		{
			CS::Errors::throwExceptionTypeRedefinition(aKey);
		}
	}
};

#endif // !__CS_CONVERTABLE_STRUCT_H__