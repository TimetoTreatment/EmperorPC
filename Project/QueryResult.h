#pragma once
#include <string>
#include <vector>


class QueryResult
{
private:
	struct Description
	{
		std::string name;
		int type;
		// int nameSze;
		// int dataSize;
		// int dataDigits;
		// int dataNullable;

		Description(std::string _name, int _type)
		{
			name = _name;
			type = _type;
		}
	};

	std::vector<size_t> colMaxLength;
	std::vector<Description> colDescription;
	std::vector<std::string> header;
	std::vector<std::vector<std::string>> records;

public:
	enum DataType
	{
		String = 1,
		Int = 4,
		Float = 6
	};

	std::vector<size_t> ColMaxLength() { return colMaxLength; }
	std::vector<std::string> Header() { return header; }
	std::vector<std::vector<std::string>> Records() { return records; }

	void AddDescription(std::string name, int type);
	void AddHeaderItem(std::string name);
	void AddRecord(std::vector<std::string> record);

	std::vector<std::vector<std::string>> ToTable();

	void Clear();
};