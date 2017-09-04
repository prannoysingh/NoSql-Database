/////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database                 //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////


#include "NoSqlDb.h"

using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
using namespace std;
using namespace XmlProcessing;
using SPtr = std::shared_ptr<AbstractXmlElement>;

int main()
{
	NoSqlDb<StrData> db;

	
	std::cout << "\n\n \t\t REQUIREMENT 2  \n";
	std::cout << "Implemented a class template providing key/value in memory database." << std::endl;

	Element<StrData> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	elem1.description = "Mother";
	elem1.timeDate = "01/29/2017";
	elem1.getVector().push_back("elem2");
	elem1.data = "elem1's StrData";
	db.save(elem1.name, elem1);
	std::cout << "Element 1 added \n";

	std::cout << "\n\n \t\t REQUIREMENT 3  \n";//************************************************************************
	std::cout << "ADDITION AND DELETION OF KEY/VALUE PAIRS \n";

	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	elem2.description = "description 2";
	elem2.timeDate = "01/29/2017";
	elem2.getVector().push_back("elem1");
	elem2.getVector().push_back("elem3");
	elem2.data = "elem2's StrData";
	db.save(elem2.name, elem2);
	std::cout << "Element 2 added  \n";

	Element<StrData> elem3;
	elem3.name = "elem3";
	elem3.category = "test";
	elem3.description = "description 3";
	elem3.timeDate = "01/29/2017";
	elem3.getVector().push_back("elem2");
	elem3.data = "elem3's StrData";
	db.save(elem3.name, elem3);
	std::cout << "Element 3 added \n";

	Element<StrData> elem4;
	elem4.name = "elem4";
	elem4.category = "test";
	elem4.description = "description 4";
	elem4.timeDate = "01/29/2017";
	elem1.getVector().push_back("elem1");
	elem4.data = "elem4's StrData";
	db.save(elem4.name, elem4);
	std::cout << "Element 4 added\n";

	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";

	std::cout << "\n  size of db = " << db.count() << "\n";
	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show() << "\t";
	}
	std::cout << "\n\n";

	std::cout << "DELETING elem3 from the database \n";
	db.delValue("elem3");
	//***
	std::cout << "\nDisplaying all the elements again after deletion \n";

	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show() << "\t";
	}

	std::cout << "\n \t\t REQUIREMENT 4 \n";//******************************************************************************
	std::cout << "\n Replacing an existing value instance with new instance \n";
	//**************************************
	std::cout << "\n Adding a new Element \n";
	Element<StrData> elem5;
	elem5.name = "elem5";
	elem5.category = "test";
	elem5.description = "description 5";
	elem5.timeDate = "01/29/2017";
	elem5.getVector().push_back("elem3");
	elem5.data = "elem5's StrData";
	db.save(elem5.name, elem5);
	std::cout << "\n elem5 added \n";

	std::cout << elem5.show();
	std::cout << "\n Replacing elem4's value instance with elem5's value instance \n";
	db.modify(elem4.name, elem5);
	std::cout << db.value(elem4.name).show();

	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";

	std::cout << "\n  size of db = " << db.count() << "\n";
	//Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show() << "\t";
	}
	std::cout << "\n\n";

	//Element<StrData> temp = db.value(elem1.name);

	std::cout << "\n \n Editing Text metadata: elem1's value modified \n";
	Element<StrData> temp1;
	temp1 = db.value(elem1.name);
	temp1.category = "New_Category";
	temp1.data = "New_Data";
	temp1.description = "New_Description";
	temp1.timeDate = "2/3/2017";
	db.modify(elem1.name, temp1);
	std::cout << " \n Displaying data after modification \n";

	keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show() << "\t";
	}
	std::cout << "\n\n";

	std::cout << "\t\t	REQUIREMENT 5 \n";

	persist<string> pd;

	cout << "\nDatabase contents are persisted to an XML File\n";//*****************************************************************
	string xml = pd.toXml(db);
	pd.toXmlfile(xml);

	std::cout << " contents pesisted successfully to PersistDB.xml \n";
	cout << "loading data from XML file \n";

	NoSqlDb<string> db2;

	db2 = db2.restore();

	std::cout << "\n  size of db = " << db2.count() << "\n";

	Keys keys2 = db2.keys();					// calls keys in db and get vector of keys
	for (Key key : keys2)					// iterate thru vector of keys
	{
		cout << "\n  " << key << ":";		// print key
		cout << db2.value(key).show();		// print its values
	}



	std::cout << "\n\t\t REQUIREMENT 6 AT END OF THE PROJECT \n";

	std::cout << "\n\t\t REQUIREMENT 7 \n";

	std::cout << "\n\nQUERY 1. \n";
	std::string a = "elem1";
	std::cout << db.keyValue(a).show() << "\n";
	std::cout << "QUERY 2 \n\n";
	vector<string> children = db.getChild(elem1.name);
	for (auto it = children.begin(); it != children.end(); ++it)
		std::cout << setw(8) << *it;

	std::cout << "\n\n QUERY 3 \n\n";
	std::vector<std::string> vec1;

	vec1 = db.getKeyPattern("(elem)(.*)");

	for (auto it = vec1.begin(); it != vec1.end(); ++it)
	{
		std::cout << setw(8) << *it;
	}

	std::cout << "\n\n QUERY 4 \n\n ";
	std::vector<std::string> vec2;

	vec2 = db.getItemPattern("(elem)(.*)");

	for (auto it = vec2.begin(); it != vec2.end(); ++it)
	{
		std::cout << setw(8) << *it;
	}

	std::cout << "\n\n QUERY 5 \n\n";
	std::vector<std::string> vec3;
	vec3 = db.getCategoryPattern("(tes)(.*)");

	for (auto it = vec3.begin(); it != vec3.end(); ++it)
	{
		std::cout << setw(8) << *it;
	}


	std::cout << "\n\n QUERY 6 \n\n";
	std::vector<string> vec4;

	vec4 = db.getDataPattern("(ele)(.*)");

	for (auto it = vec4.begin(); it != vec4.end(); ++it)
	{
		std::cout << setw(8) << *it;
	}

	std::cout << "\n\n QUERY 7 \n\n";
	std::vector<string> vec5;

	vec5 = db.getTimeDateInterval("Sun Feb 3 16:25:49 2017", db.getDateTime());

	for (auto it = vec5.begin(); it != vec5.end(); ++it)
	{
		std::cout << setw(8) << *it;
	}

	std::cout << std::endl;

	std::cout << "\n\n \t\tREQUIREMENT 8 \n";
	std::cout << "\nThe intersection is \t";
	std::vector<string> vec6;

	vec6 = db.Intersect(vec3, vec4);
	for (auto it = vec6.begin(); it != vec6.end(); ++it)
	{
		std::cout << setw(8) << *it;
	}

	std::cout << "\n\n\t\t REQUIREMENT 9 \n";
	std::cout << "\n The Union is";
	std::vector<string> vec7;

	vec7 = db.Union(vec4, vec5);
	for (auto it = vec7.begin(); it != vec7.end(); ++it)
	{
		std::cout << setw(8) << *it;
	}

	std::cout << "\n\n\t\t REQUIREMENT 10 \n";
	std::cout << "The Projects Package Structure is loaded in packageStructure.xml file";
	std::cout << "\n\n RETRIVING DATA FROM packageStructure.xml \n\n";
	std::cout << textFileToString1("packageStructure.xml");


	std::cout << "\n\n \t\tREQUIREMENT 6 \n";
	db.persistData(db);

}
