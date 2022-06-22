#include "Utility.h"


QString Utility::Korean(const char* str)
{
	return QString::fromLocal8Bit(str);
}

std::string Utility::Convert(QString str)
{
	return str.toStdString();
}

QString Utility::Convert(std::string str)
{
	return QString::fromStdString(str);
}

std::string Utility::FileToString(std::string path)
{
	std::stringstream result;
	std::ifstream file(path);

	result << file.rdbuf();
	file.close();

	return result.str();
}