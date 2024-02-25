#include "csconverter/CSConvertableStruct.h"
#include <iostream>

struct StructTest : public ConvertableStruct
{
	int value;
	std::string type;
	std::map<std::string, int> testMap;

	StructTest()
	{
		pushValue("value", &value);
		pushValue("type", &type);
		pushValue("testMap", &testMap);
	}
};

int main()
{
	std::cout << "-- Converting object to line Info --" << "\n\n";

	StructTest test;
	test.value = 10;
	test.type = "work";
	test.testMap.emplace("test1", 200);
	test.testMap.emplace("test2", 300);
	test.testMap.emplace("test6", 500);
	test.testMap.emplace("test7", 800);

	// Output: i$10|s$work
	std::cout << test.convertObjectToLineSave() << "\n\n";

	std::cout << "-- Updating object --" << "\n\n";

	test.value = 20;
	test.type = "cat";

	// Output: i$20|s$cat
	std::cout << test.convertObjectToLineSave() << "\n\n";

	
	std::cout << "-- Converting line to Object Info --" << "\n\n";

	StructTest convertToObj;
	convertToObj.convertLineSaveToObject("m$s^i$test1^200#test2^300#test6^500#test7^800|s$cat|i$20");

	std::cout << convertToObj.type << "\n";
	std::cout << convertToObj.value<< "\n";
	for (auto& [key, value] : convertToObj.testMap)
	{
		std::cout << key << " : " << value << "\n";
	}
}