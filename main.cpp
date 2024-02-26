#include "csconverter/CSConvertableStruct.h"
#include <iostream>
#include <set>

struct StructTest : public ConvertableStruct
{
	int value;
	std::string type;
	std::map<std::string, int> testMap;
	std::vector<std::string> testVector;

	StructTest()
	{
		pushValue("value", &value);
		pushValue("type", &type);
		pushValue("testMap", &testMap);
		pushValue("testVector", &testVector);
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

	test.testVector.push_back("ar_test");
	test.testVector.push_back("ar_test2");
	test.testVector.push_back("ar_tes3");
	test.testVector.push_back("ar_tes4");

	std::cout << test.convertObjectToLineSave() << "\n\n";

	std::cout << "-- Updating object --" << "\n\n";

	test.value = 20;
	test.type = "cat";
	test.testVector.push_back("new_ar_test");
	test.testMap.emplace("test10", 800);

	std::cout << test.convertObjectToLineSave() << "\n\n";
	
	std::cout << "-- Converting line to Object Info --" << "\n\n";

	StructTest convertToObj;
	convertToObj.convertLineSaveToObject("m$s^i$test1^200#test2^300#test6^500#test7^800|v$s$ar_test^ar_test2^ar_tes3^ar_tes4|s$cat|i$20");

	std::cout << convertToObj.type << "\n";
	std::cout << convertToObj.value<< "\n";

	std::cout << "Map: ";

	for (auto& [key, value] : convertToObj.testMap)
	{
		std::cout << key << ":" << value << ", ";
	}

	std::cout << "\n";
	std::cout << "Array: ";

	for (auto& value : convertToObj.testVector)
	{
		std::cout << value << ", ";
	}

	std::cout << "\n";
}