#include <stdio.h>
#include <string.h>
#include <fstream>
#include "json/json.h"

int main(int argc, char* argv[])
{
	Json::Value mainRoot;
	Json::Value contact;
	Json::Value singer;
	Json::Value arryRoot;
	Json::Value arryContact;
	Json::Value arrySinger;
	Json::Value item;
	Json::StyledWriter writer;

	contact["dictname"] = "contact";
//	root["count"] = 4;
//	root["fnt"] = 46;
	item["name"] = "eddy";
	item["id"] = 0;
	arryContact.append(item);
	item["name"] = "wendy";
	item["id"] = 1;
	arryContact.append(item);
	item["name"] = "alex";
	item["id"] = 2;
	arryContact.append(item);
	item["name"] = "areacy";
	item["id"] = 3;
	arryContact.append(item);
	contact["dictcontant"] = arryContact;
	arryRoot.append(contact);

	singer["dictname"] = "singers";
//	root["count"] = 4;
//	root["fnt"] = 46;
	item["name"] = "S.H.E";
	item["id"] = 0;
	arrySinger.append(item);
	item["name"] = "梁静茹";
	item["id"] = 1;
	arrySinger.append(item);
	item["name"] = "刘德华";
	item["id"] = 2;
	arrySinger.append(item);
	item["name"] = "张学友";
	item["id"] = 3;
	arrySinger.append(item);
	singer["dictcontant"] = arrySinger;
	arryRoot.append(singer);

	mainRoot["girm"] = arryRoot;

//	std::string str = mainRoot.toStyledString();
	std::string str = writer.write(mainRoot);
	std::ofstream out;
	out.open("test.json");
	out<<str;
	printf("json:%s\n", str.data());
	return 0;
}
