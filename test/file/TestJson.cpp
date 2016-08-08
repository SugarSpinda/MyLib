#include "common/Log.hpp"
#include "common/Exception.hpp"
#include "file/Json.hpp"

const std::string jsonFile =
"{\n"
"	\"key1\": \"\\\"value1\\\"\",\n"
"	\"key2\": [ [ null, null ] ],\n"
"	\"obj2\":\n"
"	{\n"
"		\"key3\":\n"
"		{\n"
"			\"key33\": true\n"
"		}\n"
"	},\n"
"	\"key4\": +123e-1\n"
"}";

int main()
{
	try
	{
		Json json(jsonFile.data(), jsonFile.length());
		LOGP(json.toString());
		LOGP("===========");

		// You may need to check map["..."] != nullptr
		Json::PairMap map = json.buildMap();
		LOGP("key1 = " << map["key1"]->toString());
		LOGP("key2 = " << map["key2"]->getArray()->get(0).getArray()->get(0).toString());
		LOGP("obj2 = " <<
			map["obj2"]->getObject()->buildMap()["key3"]->
				getObject()->buildMap()["key33"]->toString());
		LOGP("key4 = " << map["key4"]->toString());
	}
	catch (Exception &ex)
	{
		LOGP(ex.getMessage());
	}
	return 0;
}
