#include "Database.h"


bool Database::DBConnect(string serverName, string userName, string password)
{
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
	{
		isConnected = false;
		return false;
	}

	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		isConnected = false;
		return false;
	}

	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
	{
		isConnected = false;
		return false;
	}

	SQLRETURN Ret = SQLConnect(hDbc, (SQLCHAR*)serverName.c_str(), SQL_NTS, (SQLCHAR*)userName.c_str(), SQL_NTS, (SQLCHAR*)password.c_str(), SQL_NTS);

	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
	{
		isConnected = false;
		return false;
	}

	isConnected = true;
	return true;
}


void Database::DBDisconnect()
{
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

	isConnected = false;
}


QueryResult Database::Query(string query)
{
	SQLHSTMT hStmt;
	QueryResult result;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		SQLRETURN sqlReturnCode = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

		if (sqlReturnCode == SQL_ERROR || sqlReturnCode == SQL_NO_DATA_FOUND)
		{
			char message[SQL_MAX_MESSAGE_LENGTH + 1];
			SQLError(hEnv, hDbc, hStmt, NULL, NULL, (SQLCHAR*)message, SQL_MAX_MESSAGE_LENGTH + 1, NULL);
			result.AddRecord(vector<string>{message});
		}
		else
		{
			SQLSMALLINT maxCol = 0;

			SQLNumResultCols(hStmt, &maxCol);

			if (maxCol > 0)
			{
				vector<char[51]> data(maxCol);
				vector<SQLLEN> dataLength(maxCol);

				SQLCHAR columnName[30][30];
				SQLSMALLINT columnNameSize[30];
				SQLSMALLINT columnDataType[30];
				SQLULEN columnDataSize[30];
				SQLSMALLINT columnDataDigits[30];
				SQLSMALLINT columnDataNullable[30];

				for (int col = 0; col < maxCol; col++)
				{
					SQLBindCol(hStmt, col + 1, SQL_C_CHAR, data[col], sizeof(data[col]), &dataLength[col]);
					SQLDescribeCol(hStmt, col + 1,
						columnName[col], sizeof(columnName[col]),
						&columnNameSize[col],
						&columnDataType[col],
						&columnDataSize[col],
						&columnDataDigits[col],
						&columnDataNullable[col]
					);

					result.AddDescription((char*)columnName[col], (int)columnDataType[col]);
					result.AddHeaderItem((char*)columnName[col]);
				}

				while (SQLFetch(hStmt) != SQL_NO_DATA)
				{
					vector<string> record;

					for (int col = 0; col < maxCol; col++)
					{
						if (dataLength[col] == SQL_NULL_DATA)
							record.emplace_back("-");
						else
							record.emplace_back(data[col]);
					}

					result.AddRecord(record);
				}
			}
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	return result;
}
