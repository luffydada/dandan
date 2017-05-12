#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tinyxml.h"

void printAttr(TiXmlElement* pElem)
{
	if ( pElem ) {
		TiXmlAttribute* pAttr = pElem->FirstAttribute();
		while ( pAttr ) {
			printf("attr name:%s,value:%s\n", pAttr->Name(), pAttr->Value());
			pAttr = pAttr->Next();
		}
	}
}

void printNode(TiXmlHandle handle)
{
	TiXmlElement* pElem = handle.FirstChildElement().Element();
	while ( pElem ) {
		printf("find element,name:%s,text:%s\n", pElem->Value(), pElem->GetText());
		printAttr(pElem);
		printNode(pElem);
		pElem = pElem->NextSiblingElement();
	}
}

int main(int argc, char *argv[])
{
	if ( argc < 2 ) {
		printf("usage:%s xmlfile\n", argv[0]);
		return 0;
	}
	TiXmlDocument doc(argv[1]);
	if ( !doc.LoadFile() ) {
		printf("xml loadfile failed\n");
		return 0;
	}
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem = hDoc.FirstChildElement().Element();
	if ( !pElem ) {
		printf("xml don't have root element\n");
		return 0;
	}
	printf("find root element,name:%s\n", pElem->Value());

	TiXmlHandle hRoot(pElem);
	printNode(hRoot);
	return 0;
}

