#pragma once
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include "QueryResult.h"

using namespace std;


class Database
{
private:
	const string defaultServerName = "DB01";
	const string defaultUserName = "Emperor";
	const string defaultPassword = "1234";

	SQLHENV hEnv = nullptr;
	SQLHDBC hDbc = nullptr;

	bool isConnected = false;

public:
	Database() {}

	bool IsConnected()
	{
		return isConnected;
	}

	bool DBConnect()
	{
		return DBConnect(defaultServerName, defaultUserName, defaultPassword);
	}

	bool DBConnect(string serverName, string userName, string password);
	void DBDisconnect();

	QueryResult Query(string query);
};