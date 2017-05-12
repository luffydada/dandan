#include <stdio.h>
#include <string.h>
#include <fstream>
#include "json/json.h"

void parseJson(Json::Value& mainRoot)
{
	Json::Value::Members members = mainRoot.getMemberNames();
	for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)  {
		const std::string &key = *it;
		printf("key:%s,value type:%d\n", key.data(), mainRoot[key.data()].type());
		if ( Json::objectValue == mainRoot[key.data()].type() ) {
			parseJson(mainRoot[key.data()]);
		} else if ( Json::arrayValue == mainRoot[key.data()].type() ) {
			int count = mainRoot[key.data()].size();
			for ( int i=0; i<count; i++ ) {
				parseJson(mainRoot[key.data()][i]);
			}
		} else if ( Json::intValue == mainRoot[key.data()].type() ) {
			printf("value:%d\n", mainRoot[key.data()].asInt());
		} else if ( Json::uintValue == mainRoot[key.data()].type() ) {
			printf("value:%d\n", mainRoot[key.data()].asUInt());
		} else if ( Json::realValue == mainRoot[key.data()].type() ) {
			printf("value:%f\n", mainRoot[key.data()].asDouble());
		} else if ( Json::stringValue == mainRoot[key.data()].type() ) {
			printf("value:%s\n", mainRoot[key.data()].asCString());
		} else if ( Json::booleanValue == mainRoot[key.data()].type() ) {
			printf("value:%d\n", mainRoot[key.data()].asBool());
		}
	}
}

int main(int argc, char* argv[])
{
	if ( argc < 2 ) {
		printf("usage:%s <file.json>\n", argv[1]);
		return 0;
	}
	std::ifstream in;
	in.open("test.json");
	if ( !in.is_open() ) {
		printf("open test.json failed\n");
		return 0;
	}
	Json::Reader reader;
	Json::Value mainRoot;
	if ( !reader.parse(in, mainRoot, false) ) {
		printf("get json data failed\n");
		return 0;
	}
	parseJson(mainRoot);
	return 0;
}
