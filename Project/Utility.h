#pragma once
#include <QString>
#include <fstream>
#include <sstream>


class Utility
{
public:

	static QString Korean(const char* str);
	static std::string Convert(QString str);
	static QString Convert(std::string str);
	static std::string FileToString(std::string path);
};
