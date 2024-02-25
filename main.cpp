#include "csconverter/CSConvertableStruct.h"
#include <iostream>

struct StructTest : public ConvertableStruct
{
	int value;
	std::string type;

	StructTest()
	{
		pushValue("value", &value);
		pushValue("type", &type);

	}
};

int main()
{
	StructTest test;

	test.value = 10;
	test.type = "work";


	// Output: i$10|s$work
	std::cout << test.convertObjectToLineSave() << "\n";

	test.value = 20;
	test.type = "cat";

	// Output: i$20|s$cat
	std::cout << test.convertObjectToLineSave() << "\n";

}