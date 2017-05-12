#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tinyxml.h"

int main(int argc, char *argv[])
{
	if ( argc < 2 ) {
		printf("usage:%s xmlfile\n", argv[0]);
		return 0;
	}
	TiXmlDocument doc;

	TiXmlDeclaration* dec = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc.LinkEndChild(dec);

	TiXmlComment* comment = new TiXmlComment("i am a test comment");
	doc.LinkEndChild(comment);

	TiXmlElement* root = new TiXmlElement("fruitPrice");
	doc.LinkEndChild(root);
	root->SetAttribute("id", "1");

	TiXmlElement* banana = new TiXmlElement("banana");
	root->LinkEndChild(banana);
	banana->SetAttribute("color", "yellow");
	{
		TiXmlElement* name = new TiXmlElement("name");
		name->LinkEndChild(new TiXmlText("banana"));
		TiXmlElement* price = new TiXmlElement("price");
		price->LinkEndChild(new TiXmlText("4元/斤"));
		banana->LinkEndChild(name);
		banana->LinkEndChild(price);
	}

	TiXmlElement* apple = new TiXmlElement("apple");
	root->LinkEndChild(apple);
	apple->SetAttribute("color", "red");
	{
		TiXmlElement* name = new TiXmlElement("apple");
		name->LinkEndChild(new TiXmlText("apple"));
		TiXmlElement* price = new TiXmlElement("price");
		price->LinkEndChild(new TiXmlText("5元/斤"));
		apple->LinkEndChild(name);
		apple->LinkEndChild(price);
	}

	doc.SaveFile(argv[1]);
	return 0;
}

