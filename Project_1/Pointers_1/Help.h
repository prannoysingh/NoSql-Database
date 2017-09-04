#pragma once

/////////////////////////////////////////////////////////////////////////////////
// Help.h: Contains class with string Properties                                                                      //
// Application: Project 1                            //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013                 //
// Author:   Prannoy Singh                                                     //
////////////////////////////////////////////////////////////////////////////////


// Help.h
// Contains class with <std::string> properties

#include "CppProperties.h"
#include <string>

template<typename Data>
class X
{
public:
	Property<std::string> name;
	Property<std::string> category;
	Property<std::string> description;
	Property<std::string> timeDate;
	std::unordered_set<std::string> children;
	Property<Data> data;
 };
