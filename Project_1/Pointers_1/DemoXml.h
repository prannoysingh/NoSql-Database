#pragma once
/////////////////////////////////////////////////////////////////////
// DemoXml.h - Demo how to use XmlDocument to perist Widgets       //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

#include "CppProperties.h"
#include <string>

class Widget
{
public:
  Property<std::string> name;
  Property<double> pi = 3.415927;
};