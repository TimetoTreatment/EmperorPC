#include "QueryResult.h"


void QueryResult::AddDescription(std::string name, int type)
{
	colDescription.emplace_back(Description(name, type));
}


void QueryResult::AddHeaderItem(std::string name)
{
	header.emplace_back(name);
	colMaxLength.emplace_back(name.size());
}


void QueryResult::AddRecord(std::vector<std::string> record)
{
	for (size_t col = 0; col < record.size(); col++)
	{
		record[col].erase(std::find_if(record[col].rbegin(), record[col].rend(), [](auto ch) {
			return ch != ' ';
			}).base(), record[col].end());

		if (colMaxLength.empty())
			colMaxLength.emplace_back(record[col].size());

		else if (colMaxLength[col] < record[col].size())
			colMaxLength[col] = record[col].size();
	}

	records.emplace_back(record);
}


std::vector<std::vector<std::string>> QueryResult::ToTable()
{
	std::vector<std::vector<std::string>> table;
	std::vector<std::string> record;
	std::string item;

	for (int i = 0; i < header.size(); i++)
	{
		item = header[i];
		item.resize(colMaxLength[i], ' ');
		record.emplace_back(item);
	}

	table.emplace_back(record);

	for (size_t row = 0; row < records.size(); row++)
	{
		record.clear();

		for (size_t col = 0; col < records[0].size(); col++)
		{
			item = records[row][col];
			item.resize(colMaxLength[col], ' ');

			record.emplace_back(item);
		}

		table.emplace_back(record);
	}

	return table;
}


void QueryResult::Clear()
{
	colMaxLength.clear();
	header.clear();
	records.clear();
}