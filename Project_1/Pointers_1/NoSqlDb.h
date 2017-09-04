///////////////////////////////////////////////////////////////////
// NoSqlDb.h - stores and retrieves elements                    //
// Ver 2.2                                                       //
// Application: Project 1                                        //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Jim Fawcett, CST 4-187, 443-3948                 //
//              jfawcett@twcny.rr.com                            //
// Co Author:   Prannoy Singh                                    //
///////////////////////////////////////////////////////////////////
/*
* Element class represents a data record in our NoSql database
* NoSqlDb class is a key/value pair in-memory database
* stores and retrieves elements
* Queries capabilities are added
* Persist class to store database into an XML File
* Package operations:
* -------------------
* This package defines the methods needed to
* - display record in our NoSqlDb structure
* - Element class that persists strings
* - NoSqlDb class that stores and retrieves elements
* - Queries are added
* - Addition, modification, deletion of data in key/value pair
*
* Required Files:
* ---------------
*   Help.h
*   toXml.h
*   PersistHelp.h, PersistHelp.cpp,
*   Convert.h, Convert.cpp,
*   CppProperties.h, CppProperties.cpp,
*   XmlDocument.h, XmlDocument.cpp, XmlElement.h, XmlElement.cpp,
*  ---- required to build XmlDocument from file or string ----
*   XmlParser.h, XmlParser.cpp,
*   XmlElementParts.h, XmlElementParts.cpp,
*   ITokenizer.h, Tokenizer.h, Tokenizer.cpp,
*
* Build Instructions:
* -------------------
* - Uses XmlDocument, so build XmlDocument project as static library
*   after undefining its test stub
* - Uses NoSqlDb, so build XmlDocument project as static library
*   after undefining its test stub
* - Uses CppProperties(1), so build XmlDocument project as static library
*   after undefining its test stub
* - Uses Convert, so build XmlDocument project as static library
*   after undefining its test stub
*/

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include<unordered_set>
#include "CppProperties.h"
#include <regex>
#include <time.h>
#include <thread>
#include "Convert.h"
#include "CppProperties.h"
#include "Help.h"
#include "StrHelper.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../XmlDocument/XmlElementParts/itokcollection.h"
#include "../XmlDocument/XmlElementParts/Tokenizer.h"
#include "../XmlDocument/XmlElementParts/xmlElementParts.h"
#include "../XmlDocument/XmlParser/XmlParser.h"
#include <fstream>
#include "Query.h"
#include <ctime>
using namespace std;

// Element class represents a data record in our NoSql database

template<typename Data>
class Element
{

public:
	using Name = std::string;
	using Category = std::string;
	using Description = std::string;
	using TimeDate = std::string;
	using Children = std::string;

	Property<Name> name;	    	           // metadata
	Property<Category> category;		       // metadata
	Property<Description> description;		   // metadata
	Property<TimeDate> timeDate;			   // metadata
	Property<Data> data;                       // data
	vector<string> children;
	vector<string> & getVector();
	string getDateTime();
	std::string show();
};

// function of Element classs, return child node
template<typename Data>
vector<string> & Element<Data>::getVector()
{
	return children;
}
// function of Element class, to get time date
template<typename Data>
string Element<Data>::getDateTime()
{
	time_t tim;
	time(&tim);
	return ctime(&tim);
}

// function of Element class, to display data
template<typename Data>
std::string Element<Data>::show()			
{
	// show children when you've implemented them

	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	//out << "\n    " << std::setw(8) << "address" << " : " << this;
	out << "\n    " << std::setw(8) << "name" << " : " << name;
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "description" << " : " << description;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "children" << " : ";
	for (auto it = children.begin(); it != children.end(); ++it)
		out << setw(8) << *it;
	out << "\n    " << std::setw(8) << "data" << " : " << data;
	out << "\n";
	return out.str();
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements
// - Queries capabilities are added

template<typename Data>
class NoSqlDb
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;									// vector of keys

	Keys keys();
	bool save(Key key, Element<Data> elem);
	Element<Data> value(Key key);				
	Element<Data> delValue(Key key);			
	Element<Data> keyValue(Key key);								
	Element<Data> modify(Key key, Element<Data> elem);
	unordered_map<Key, Element<Data>> & getMapAccess();
	
	vector<std::string> getItemPattern(std::string pattern);
	vector<std::string> getKeyPattern(std::string pattern);
	Element<Data> getValue(Key key);
	vector<std::string> getCategoryPattern(std::string pattern);
	vector<std::string> getDataPattern(std::string pattern);
	vector<std::string> getTimeDateInterval(std::string time1, std::string time2);
	vector<string>  NoSqlDb<Data>::Intersect(vector<string> keys1, vector<string> keys2);
	vector<string> NoSqlDb<Data>::Union(vector<string> keys1, vector<string> keys2);

	vector<string> NoSqlDb<Data>::getChild(Key key);
	size_t count();				
	std::string getDateTime();
	void persistData(NoSqlDb<Data>& db);
	std::string saveToXml(NoSqlDb<Data>& db);
	NoSqlDb<Data> restore();
	
private:
	using Item = std::pair<Key, Element<Data>>;

	std::unordered_map<Key, Element<Data>> store;		
	};

// get date time
template<typename Data>
std::string NoSqlDb<Data>::getDateTime()
{
	time_t tim;
	time(&tim);
	return ctime(&tim);
}

template<typename Data>
unordered_map<std::string, Element<Data>> & NoSqlDb<Data>::getMapAccess()
{
	return store;

}

//QUERY 6, returns keys that contain a specified string in their template data 
template<typename Data>
vector<std::string>  NoSqlDb<Data>::getDataPattern(std::string pattern)
{
	vector<std::string> input_keys = keys();
	vector<std::string> output_keys;

	regex e(pattern);

	Element<Data> elem;

	for (auto it = input_keys.begin(); it != input_keys.end(); ++it)
	{
		elem = getValue(*it);

		std::string str = elem.data;
		if (regex_match(str, e))
			output_keys.push_back(*it);
	}

	return output_keys;  
}

// returns data for a specific key
template<typename Data>
Element<Data> NoSqlDb<Data>::getValue(Key key)
{
	unordered_map<Key, Element<Data>> store = getMapAccess();

	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}

// Query 5, returns keys that contain a specified string in their category name
template<typename Data>
vector<std::string>  NoSqlDb<Data>::getCategoryPattern(std::string pattern)
{
	vector<string> input_keys = keys();
	vector<string> output_keys;

	regex e(pattern);

	Element<Data> elem;

	for (auto it = input_keys.begin(); it != input_keys.end(); ++it)
	{
		elem = getValue(*it);

		std::string str = elem.category;
		if (regex_match(str, e))
			output_keys.push_back(*it);
	}

	return output_keys;  // default no keys

}
// Query 4, returns keys that contain a specified string in their item name
template<typename Data>
vector<std::string>  NoSqlDb<Data>::getItemPattern(std::string pattern)
{
	vector<std::string> input_keys = keys();
	vector<std::string> output_keys;

	regex p(pattern);

	Element<Data> elem;

	for (auto it = input_keys.begin(); it != input_keys.end(); ++it)
	{
		elem = getValue(*it);

		string str = elem.name;
		if (regex_match(str, p))
			output_keys.push_back(*it);
	}

	return output_keys;  // default no keys

}
// Query 3, returns set of all keys matching a specified pattern which defaults to all keys.
template<typename Data>
vector<std::string> NoSqlDb<Data>::getKeyPattern(std::string pattern)
{
	vector<string> input_keys = keys();
	vector<string> output_keys;

	regex p(pattern);

	for (auto it = input_keys.begin(); it != input_keys.end(); ++it)
	{
		if (regex_match(*it, p))
			output_keys.push_back(*it);
	}

	return output_keys;  // default no keys
}


template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()	// keys()
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}
// to save data in database
template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)	
{
	if (store.find(key) != store.end())
		return false;
	store[key] = elem;
	return true;
}
// to modify data in database
template<typename Data>
Element<Data> NoSqlDb<Data>::modify(Key key, Element<Data> elem)	
{
	store[key] = elem;
	//return true;
	return Element<Data>();
}
// to return value for a particular key
template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)				// value
{
	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}
// return key value pair for a given key
template<typename Data>
Element<Data> NoSqlDb<Data>::keyValue(Key key)				// key value pair
{
	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}

// delete key value pair from the database
template<typename Data>
Element<Data> NoSqlDb<Data>::delValue(Key key)				
{
	
	if (store.find(key) != store.end())
		{
			store.erase(key);
		}
		return Element<Data>();
}
// to count no of key value pairs
template<typename Data>
size_t NoSqlDb<Data>::count()								
{
	return store.size();
}


// Query 7, keys that contain values written within a specified time-date interval
template<typename Data>
vector<std::string>  NoSqlDb<Data>::getTimeDateInterval(std::string time1, std::string time2)
{
	vector<std::string> input_keys = keys();
	vector<std::string> output_keys;

	Element<Data> elem;

	for (auto it = input_keys.begin(); it != input_keys.end(); ++it)
	{
		elem = getValue(*it);

		std::string str = elem.timeDate;

		if (str > time1 || str < time2)
			output_keys.push_back(*it);
	}

	return output_keys;  // default no keys
}


// Requirement 8, gives intersection of two keys
template<typename Data>
vector<string>  NoSqlDb<Data>::Intersect(vector<string> keys1, vector<string> keys2)
{
	vector<string> keys3;

	sort(keys1.begin(), keys1.end());
	sort(keys2.begin(), keys2.end());

	set_intersection(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), back_inserter(keys3));

	return keys3;
}

// Requirement 9, gives union of two keys
template<typename Data>
vector<string> NoSqlDb<Data>::Union(vector<string> keys1, vector<string> keys2)
{
	vector<string> keys4;

	sort(keys1.begin(), keys1.end());
	sort(keys2.begin(), keys2.end());

	set_union(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), back_inserter(keys4));

	return keys4;

}


// saves data in XML
template<typename Data>
std::string NoSqlDb<Data>::saveToXml(NoSqlDb<Data>& db)
{

	string xml;

	XmlDocument doc;

	SPtr pRoot = makeTaggedElement("db");
	doc.docElement() = pRoot;

	vector<string>::const_iterator vec_itr;

	unordered_map<Key, Element<Data>>::iterator map_itr;


	for (map_itr = db.getMapAccess().begin(); map_itr != db.getMapAccess().end(); ++map_itr)
	{
		int count_child_keys = 0;
	
		SPtr pRec = makeTaggedElement("Record");
		pRoot->addChild(pRec);

		SPtr pKeyElem = makeTaggedElement("Key");
		pRec->addChild(pKeyElem);
		SPtr pTextElem = makeTextElement(map_itr->first);
		pKeyElem->addChild(pTextElem);

		SPtr pNameElem = makeTaggedElement("Name");
		pRec->addChild(pNameElem);
		pTextElem = makeTextElement(map_itr->second.name);
		pNameElem->addChild(pTextElem);

		SPtr pCategoryElem = makeTaggedElement("Category");
		pRec->addChild(pCategoryElem);
		pTextElem = makeTextElement(map_itr->second.category);
		pCategoryElem->addChild(pTextElem);

		SPtr pDescriptionElem = makeTaggedElement("Description");
		pRec->addChild(pDescriptionElem);
		pTextElem = makeTextElement(map_itr->second.description);
		pDescriptionElem->addChild(pTextElem);

		SPtr pTimeDateElem = makeTaggedElement("timeDate");
		pRec->addChild(pTimeDateElem);
		pTextElem = makeTextElement(map_itr->second.timeDate);
		pTimeDateElem->addChild(pTextElem);
	    SPtr pChildrenElem = makeTaggedElement("Children");
		SPtr pDataElem = makeTaggedElement("Data");
		pRec->addChild(pDataElem);
		pTextElem = makeTextElement(map_itr->second.data);
		pDataElem->addChild(pTextElem);
	
	
	}

	xml = doc.toString();

	ofstream fp_out;

	fp_out.open("PersistedDb.xml");

	fp_out << xml << endl;

	return xml;

}

// reads a XML file and displays it
std::string textFileToString1(const std::string& fileName)
{
	std::ifstream in("packageStructure.xml");
	if (!in.good())
		throw(std::exception(("can't open file " + fileName).c_str()));
	std::ostringstream out;
	out << in.rdbuf();
	return std::move(out.str());
}

// returns child for a specific key
template<typename Data>
vector<string> NoSqlDb<Data>::getChild(Key key)
{
	
	Element<Data> elem;

	if (store.find(key) != store.end())
	{
		elem = store[key];
	}

	return elem.getVector();
}

// reads data from XML file and displays it as String
template<typename Data>
NoSqlDb<Data> NoSqlDb<Data>::restore()
{
	std::string path = "XmlfileStrdata.xml";//

	NoSqlDb<Data> db;
	Element<Data> elem;

	
		XmlDocument doc(path, XmlDocument::file);

		cout << endl;

		std::vector<SPtr> record_desc = doc.descendents("record").select();

		std::vector<SPtr> key_desc = doc.descendents("key").select();
		std::string key_value;

		std::vector<SPtr> name_desc = doc.descendents("name").select();
		std::string name_value;

		std::vector<SPtr> category_desc = doc.descendents("category").select();
		std::string category_value;

		std::vector<SPtr> description_desc = doc.descendents("description").select();
		std::string description_value;

		std::vector<SPtr> timeDate_desc = doc.descendents("timeDate").select();
		std::string timeDate_value;

		std::vector<SPtr> children_desc = doc.descendents("children").select();
		std::string children_value;

	    std::vector<SPtr> childkey_desc = doc.descendents("childkey").select();
		std::string childkey_value;

		std::vector<SPtr> numchildkeys_desc = doc.descendents("numchildkeys").select();
		std::string numchildkeys_value;

		std::vector<SPtr> data_desc = doc.descendents("data").select();
		std::string data_value;

		int recordSize = record_desc.size();

		static int j = 0;

		for (int i = 0; i < recordSize; i++)
		{
			key_value = key_desc[i]->children()[0]->value();
			key_value = trim(key_value);

			name_value = name_desc[i]->children()[0]->value();
			name_value = trim(name_value);
			elem.name = name_value;

			category_value = category_desc[i]->children()[0]->value();
			category_value = trim(category_value);
			elem.category = category_value;

			description_value = description_desc[i]->children()[0]->value();
			description_value = trim(description_value);
			elem.description = description_value;

			timeDate_value = timeDate_desc[i]->children()[0]->value();
			timeDate_value = trim(timeDate_value);
			elem.timeDate = timeDate_value;

			numchildkeys_value = numchildkeys_desc[i]->children()[0]->value();
			int numchildkeys = Convert<int>::fromString(numchildkeys_value);

			for (int k = 0; k < numchildkeys; k++)
			{
				childkey_value = childkey_desc[j]->children()[0]->value();
				childkey_value = trim(childkey_value);
				elem.getVector().push_back(childkey_value);
				j++;
			}

			data_value = data_desc[i]->children()[0]->value();
			data_value = trim(data_value);
			elem.data = data_value;

			db.save(key_value, elem);

			elem.getVector().clear();
		}

	return db;
}

/////////////////////////////////////////////////////////////////////
// Persist class to store data in an XML file



template<typename Data>
class persist:public NoSqlDb<Data>  
{
public:
	string toXml(NoSqlDb<Data>& db);
	void toXmlfile(const string& xml);
};


template<typename Data>
string persist<Data>::toXml(NoSqlDb<Data>& db)
{
	string xml;

	XmlDocument doc;

	SPtr pRoot = makeTaggedElement("db");
	doc.docElement() = pRoot;

	vector<string>::const_iterator vec_itr;

	unordered_map<Key, Element<Data>>::iterator map_itr;


	for (map_itr = db.getMapAccess().begin(); map_itr != db.getMapAccess().end(); ++map_itr)
	{
		int count_child_keys = 0;

		SPtr pRecord = makeTaggedElement("DataBase");
		pRoot->addChild(pRecord);

		SPtr pKeyElem = makeTaggedElement("key");
		pRecord->addChild(pKeyElem);
		SPtr pTextElem = makeTextElement(map_itr->first);
		pKeyElem->addChild(pTextElem);

		SPtr pNameElem = makeTaggedElement("name");
		pRecord->addChild(pNameElem);
		pTextElem = makeTextElement(map_itr->second.name);
		pNameElem->addChild(pTextElem);

		SPtr pCategoryElem = makeTaggedElement("category");
		pRecord->addChild(pCategoryElem);
		pTextElem = makeTextElement(map_itr->second.category);
		pCategoryElem->addChild(pTextElem);

		SPtr pDescriptionElem = makeTaggedElement("description");
		pRecord->addChild(pDescriptionElem);
		pTextElem = makeTextElement(map_itr->second.description);
		pDescriptionElem->addChild(pTextElem);

		SPtr pTimeDateElem = makeTaggedElement("timeDate");
		pRecord->addChild(pTimeDateElem);
		pTextElem = makeTextElement(map_itr->second.timeDate);
		pTimeDateElem->addChild(pTextElem);

		SPtr pChildElem = makeTaggedElement("children");
		pRecord->addChild(pChildElem);


		for (vec_itr = map_itr->second.getVector().begin(); vec_itr != map_itr->second.getVector().end(); ++vec_itr)
		{
			//SPtr pChildKeyElem = makeTaggedElement("childkey");
			//pChildElem->addChild(pChildKeyElem);
			pChildElem->addChild(pTextElem);
			pTextElem = makeTextElement(*vec_itr);
			//pChildKeyElem->addChild(pTextElem);
			count_child_keys++;
		}

		//SPtr pChildKeyNumElem = makeTaggedElement("numchildkeys");
		//pRecord->addChild(pChildKeyNumElem);
		//std::string ChildKeyNumString = Convert<int>::toString(count_child_keys);
		//pTextElem = makeTextElement(ChildKeyNumString);
		//pChildKeyNumElem->addChild(pTextElem);

		SPtr pDataElem = makeTaggedElement("data");
		pRecord->addChild(pDataElem);
		pTextElem = makeTextElement(map_itr->second.data);
		pDataElem->addChild(pTextElem);

	}

	xml = doc.toString();

	return xml;
}

//store data in XML file 
template<typename Data>
void persist<Data>::toXmlfile(const string& xml)
{
	ofstream fp_out;

	fp_out.open("PersistDB.xml");

	fp_out << xml << endl;

}