#pragma once

#include "headers.h"
#include "NNMacroSet.h"
#include "Library/TinyXML/tinyxml.h"
#include "Library/TinyXML/xpath_static.h"

class CXMLWriter
{
public:
	CXMLWriter(std::string filename);
	CXMLWriter(void);
	~CXMLWriter(void);
		
	static CXMLWriter* Create( std::string filename);

	void AddRoot(std::string rootname);
	void AddNode(std::string elementName, std::string parent);
	void AddText(std::string content, std::string element);
	void ExportXMLFile(void);

private:
	void initXML();


	std::string		m_Filename;	// ��µ� ���� �̸�
	TiXmlDocument	m_Document; 
	std::map<std::string, TiXmlNode*> m_XMLhierarchy;

};

