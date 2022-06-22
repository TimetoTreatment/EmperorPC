#include "MainForm.h"

MainForm::MainForm(QWidget* parent)
{
	ui.setupUi(this);

	statusBar = ui.statusBar;

	QActionGroup* manuUiGroup = new QActionGroup(this);
	QButtonGroup* schemaGroup = new QButtonGroup(this);

	manuUiGroup->addAction(ui.actionCLI);
	manuUiGroup->addAction(ui.actionGUI);

	schemaGroup->addButton(ui.radioButtonCheck1Buyer);
	schemaGroup->addButton(ui.radioButtonCheck1Seller);
	schemaGroup->addButton(ui.radioButtonCheck1PurchaseOrder);
	schemaGroup->addButton(ui.radioButtonCheck1Bid);
	schemaGroup->addButton(ui.radioButtonCheck1ASService);
	schemaGroup->addButton(ui.radioButtonCheck2Computer);
	schemaGroup->addButton(ui.radioButtonCheck2HasSSD);
	schemaGroup->addButton(ui.radioButtonCheck2HasHDD);
	schemaGroup->addButton(ui.radioButtonCheck2RAMPackaging);
	schemaGroup->addButton(ui.radioButtonCheck3CPU);
	schemaGroup->addButton(ui.radioButtonCheck3RAM);
	schemaGroup->addButton(ui.radioButtonCheck3VGA);
	schemaGroup->addButton(ui.radioButtonCheck3PSU);
	schemaGroup->addButton(ui.radioButtonCheck3SSD);
	schemaGroup->addButton(ui.radioButtonCheck3HDD);

	tableWidgets[0] = ui.tableWidget1SchemaCheck;
	tableWidgets[1] = ui.tableWidget2Select;
	tableWidgets[2] = ui.tableWidget3Insert;
	tableWidgets[3] = ui.tableWidget4Update;
	tableWidgets[4] = ui.tableWidget5Delete;

	ui.groupBoxMenu->setFixedSize(273, 455);
	ui.groupBoxMenu->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));

	ui.toolBoxMenu->setCurrentIndex(5);
	ui.stackedWidgetOutput->setCurrentIndex(ui.toolBoxMenu->currentIndex());

	ui.toolBoxMenu->setEnabled(false);
	ui.textEditInput->setEnabled(false);
	ui.textEditOutput->setEnabled(false);
	ui.textEditOutput->setFont(QFont("Courier New"));

	database = new Database();
	isTriggeredByUIAction = false;
	isTriggeredByUITab = false;

	PrintStatusBar("연결된 데이터베이스가 없습니다.");
}


void MainForm::DatabaseConnect()
{
	bool isConnected = false;

	if (ui.actionDefault_Authentication->isChecked())
		isConnected = database->DBConnect();
	else
		isConnected = database->DBConnect(serverName.toStdString(), userName.toStdString(), password.toStdString());

	if (isConnected)
	{
		ui.actionConnect->setChecked(true);
		ui.toolBoxMenu->setEnabled(true);
		ui.textEditInput->setEnabled(true);
		ui.textEditInput->clear();
		ui.textEditOutput->setEnabled(true);
		PrintStatusBar("데이터베이스에 연결되었습니다.");
	}
	else
	{
		ui.toolBoxMenu->setEnabled(false);
		ui.textEditInput->setEnabled(false);
		ui.textEditOutput->setEnabled(false);
		PrintStatusBar("데이터베이스에 연결할 수 없습니다.");
	}
}


void MainForm::NewAuthentication()
{
	DialogNewAuthentication dialogNewAuthentication;

	if (dialogNewAuthentication.exec() != QDialog::DialogCode::Accepted)
		return;

	serverName = dialogNewAuthentication.serverName;
	databaseName = dialogNewAuthentication.databaseName;
	userName = dialogNewAuthentication.userName;
	password = dialogNewAuthentication.password;

	ui.actionUserAuthentication->setText(userName);
	ui.actionUserAuthentication->setVisible(true);
	ui.actionUserAuthentication->setChecked(true);
	ui.actionDefault_Authentication->setChecked(false);

	ui.actionConnect->setEnabled(true);
}


void MainForm::CLIModeManager()
{
	QString statement = ui.textEditInput->toPlainText();

	if (statement.isEmpty() || statement.back() != '\n')
		return;

	statement = statement.trimmed();

	if (statement.back() != ';' || statement.size() <= 1)
		return;

	statement = statement.mid(statement.lastIndexOf(';', statement.size() - 2) + 1);

	QueryResult queryResult = database->Query(statement.toStdString());
	vector<vector<string>> table = queryResult.ToTable();
	QString output = "";

	for (auto record = table.begin(); record != table.end(); record++)
	{
		for (auto item = record->begin(); item != record->end(); item++)
			output += item->c_str() + QString("  ");

		output += '\n';
	}

	ui.textEditOutput->setText(output);
}


void MainForm::PrintStatusBar(string text)
{
	statusBar->setText(Utility::Korean(text.c_str()));
}


void MainForm::PrintMessageBox(string text)
{
	QMessageBox msgBox;

	msgBox.setText(Utility::Korean(text.c_str()) + "     ");
	msgBox.setWindowTitle("Notice");
	msgBox.exec();
}


QueryResult MainForm::ReadSchema(string query, QTableWidget* tableWidget)
{
	QueryResult result = database->Query(query);

	PrintTable(result.Header(), result.Records(), tableWidget);

	return result;
}


void MainForm::PrintTable(vector<string> header, vector<vector<string>> records, QTableWidget* tableWidget)
{
	auto isDouble = [](QString str)
	{
		if ('0' <= str[0] && str[0] <= '9' && str[1] == '.' && str[2] >= '0' && str[2] <= '9')
			return true;

		return false;
	};

	int maxRow = records.size();
	int maxCol = header.size();

	tableWidget->clear();
	tableWidget->setRowCount(maxRow);
	tableWidget->setColumnCount(maxCol);

	for (int i = 0; i < maxCol; i++)
	{
		QTableWidgetItem* item = new QTableWidgetItem();

		item->setText(QString::fromStdString(header[i]));
		tableWidget->setHorizontalHeaderItem(i, item);
	}

	for (int row = 0; row < maxRow; row++)
	{
		for (int col = 0; col < maxCol; col++)
		{
			QTableWidgetItem* item = new QTableWidgetItem();
			QString value = QString::fromStdString(records[row][col]);

			if (value.size() >= 3 && isDouble(value))
			{
				value = QString::fromStdString(to_string(value.toDouble() + 0.1));
				value = value.mid(0, 3);
			}

			item->setText(value);
			tableWidget->setItem(row, col, item);
		}
	}

	tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
}


void MainForm::EmphasizeRow(string ID, QTableWidget* tableWidget)
{
	for (int row = 0; row < tableWidget->rowCount(); row++)
	{
		if (tableWidget->item(row, 0)->text().toStdString() == ID)
		{
			for (int col = 0; col < tableWidget->columnCount(); col++)
			{
				tableWidget->item(row, col)->setBackground(QBrush(QColor(100, 100, 100)));
				tableWidget->item(row, col)->setForeground(QBrush(QColor(255, 255, 255)));
			}

			break;
		}
	}
}


void MainForm::EmphasizeRow(int row, QTableWidget* tableWidget)
{
	for (int col = 0; col < tableWidget->columnCount(); col++)
	{
		tableWidget->item(row, col)->setBackground(QBrush(QColor(100, 100, 100)));
		tableWidget->item(row, col)->setForeground(QBrush(QColor(255, 255, 255)));
	}
}


void MainForm::on_actionCLI_changed()
{
	if (isTriggeredByUITab)
	{
		isTriggeredByUIAction = false;
		isTriggeredByUITab = false;
		return;
	}

	isTriggeredByUITab = false;
	isTriggeredByUIAction = true;

	if (ui.actionCLI->isChecked())
		ui.tabWidgetUI->setCurrentIndex(0);
	else
		ui.tabWidgetUI->setCurrentIndex(1);
}


void MainForm::on_tabWidgetUI_currentChanged(int index)
{
	if (isTriggeredByUIAction)
	{
		isTriggeredByUIAction = false;
		isTriggeredByUITab = false;
		return;
	}

	isTriggeredByUITab = true;
	isTriggeredByUIAction = false;

	if (index == 1)
		ui.actionGUI->setChecked(true);
	else
		ui.actionCLI->setChecked(true);
}


void MainForm::on_actionEnable_All_triggered()
{
	if (!database->IsConnected())
	{
		ui.actionDefault_Authentication->setChecked(true);
		DatabaseConnect();
	}

	ui.actionEnable_All->setEnabled(false);
	ui.actionEnable_All->setChecked(true);
	ui.actionConnect->setEnabled(true);
	ui.actionInitialize_Table->setEnabled(true);

	ui.toolBoxMenu->setEnabled(true);
	ui.textEditInput->setEnabled(true);
	ui.textEditInput->clear();
	ui.textEditOutput->setEnabled(true);

	PrintStatusBar("Debug Mode");
}


void MainForm::on_actionDefault_Authentication_triggered()
{
	if (ui.actionDefault_Authentication->isChecked())
	{
		ui.actionUserAuthentication->setChecked(false);
		ui.actionConnect->setEnabled(true);
	}
	else
		ui.actionConnect->setEnabled(false);
}


void MainForm::on_actionInitialize_Table_triggered()
{
	database->Query(Utility::FileToString("Resource/SQL/DropAll.sql"));
	database->Query(Utility::FileToString("Resource/SQL/Create.sql"));

	PrintMessageBox("모든 테이블이 초기화 되었습니다.");
}


void MainForm::on_toolBoxMenu_currentChanged(int index)
{
	ui.stackedWidgetOutput->setCurrentIndex(index);
	currentOutput = tableWidgets[index];
}


void MainForm::on_pushButtonSelect1Run_clicked()
{
	string query =
		"SELECT \
			C.ID, C.PURPOSE AS 'Purpose', CPU.MANUFACTURER AS 'CPU', CPU.NAME AS 'CPU Model', \
			RAM.NAME AS 'RAM Model', RAM.CAPACITY AS 'RAM (GB)', VGA.NAME AS 'Graphics Card',\
			POWERSUPPLY.NAME AS 'Power Supply Unit', SSDSummary.CAPACITY AS 'SSD (GB)', \
			HDDSummary.CAPACITY AS 'HDD (GB)', C.PRICE AS 'Price (KRW)'\
		FROM\
			CPU, RAM, POWERSUPPLY, COMPUTER AS C\
			LEFT JOIN VGA ON C.VGAID = VGA.ID\
			LEFT JOIN (\
				SELECT COMPUTERID, SUM(SSD.CAPACITY * QUANTITY) AS CAPACITY\
				FROM (\
					SELECT HAS_SSD.COMPUTERID, CAPACITY, QUANTITY\
					FROM HAS_SSD, SSD\
					WHERE HAS_SSD.SSDID = SSD.ID\
				) AS SSD\
				GROUP BY SSD.COMPUTERID\
			) AS SSDSummary ON C.ID = SSDSummary.COMPUTERID\
			LEFT JOIN (\
				SELECT COMPUTERID, SUM(HDD.CAPACITY * QUANTITY) AS CAPACITY\
				FROM (\
					SELECT HAS_HDD.COMPUTERID, CAPACITY, QUANTITY\
					FROM HAS_HDD, HDD\
					WHERE HAS_HDD.HDDID = HDD.ID\
				) AS HDD\
				GROUP BY HDD.COMPUTERID\
			) AS HDDSummary ON C.ID = HDDSummary.COMPUTERID\
		WHERE\
			C.CPUID = CPU.ID AND\
			C.RAMID = RAM.ID AND\
			C.POWERID = POWERSUPPLY.ID";

	ReadSchema(query, currentOutput);
}

void MainForm::on_pushButtonSelect2Run_clicked()
{
	string budget = ui.lineEdit2Budget->text().toStdString();
	string query =
		"SELECT \
			ID,\
			PURPOSE AS Purpose,\
			PRICE AS Price,\
			UPPERCOMPATIBILITY AS Compatibility,\
			CPUID AS RAM,\
			VGAID AS VGA,\
			POWERID AS PSU \
		FROM COMPUTER \
		WHERE PRICE <= " + budget;

	ReadSchema(query, currentOutput);
}
void MainForm::on_lineEdit2Budget_textChanged(QString budgetText)
{
	if (budgetText.size() > 0 && !('0' <= budgetText.back() && budgetText.back() <= '9'))
		ui.lineEdit2Budget->setText(budgetText.remove(budgetText.size() - 1, 1));
}

void MainForm::on_pushButtonSelect3Run_clicked()
{
	string query =
		"SELECT \
			COMPUTER.ID AS 'COMPUTER ID', \
			SSDINFO.CAPACITY AS 'SSD Capacity', \
			HDDINFO.CAPACITY AS 'HDD Capacity' \
		FROM COMPUTER\
			LEFT JOIN (\
				SELECT COMPUTERID, SUM(SSD.CAPACITY * QUANTITY) AS CAPACITY\
				FROM (\
					SELECT COMPUTERID, CAPACITY, QUANTITY\
					FROM HAS_SSD, SSD\
					WHERE HAS_SSD.SSDID = SSD.ID\
				) AS SSD\
				GROUP BY SSD.COMPUTERID\
			) AS SSDINFO ON SSDINFO.COMPUTERID = COMPUTER.ID\
			LEFT JOIN (\
				SELECT COMPUTERID, SUM(HDD.CAPACITY * QUANTITY) AS CAPACITY\
				FROM (\
					SELECT COMPUTERID, CAPACITY, QUANTITY\
					FROM HAS_HDD, HDD\
					WHERE HAS_HDD.HDDID = HDD.ID\
				) AS HDD\
				GROUP BY HDD.COMPUTERID\
			) AS HDDINFO ON HDDINFO.COMPUTERID = COMPUTER.ID ";

	int minCapacity = ui.spinBoxSelect3Capacity->value();
	string type = ui.comboBoxSelect3Type->currentText().toStdString();
	string condition = "WHERE SSDINFO.CAPACITY >= " + to_string(minCapacity);

	if (type == "SSD + HDD")
		condition += " OR SSDINFO.CAPACITY + HDDINFO.CAPACITY >= " + to_string(minCapacity);

	ReadSchema(query + condition, currentOutput);
}

void MainForm::on_pushButtonSelect4Run_clicked()
{
	string query =
		"SELECT \
			B.NAME AS 'Buyer Name',\
			P.ID AS 'Order ID',\
			C.ID AS 'Computer ID',\
			C.PRICE AS 'Price',\
			P.QUANTITY AS 'Quantity',\
			C.PRICE* P.QUANTITY AS 'Total Price'\
		FROM\
			PURCHASEORDER AS P,\
			COMPUTER AS C,\
			BUYER AS B\
		WHERE\
			P.BUYERID = B.ID AND\
			P.COMPUTERID = C.ID";

	ReadSchema(query, currentOutput);
}

void MainForm::on_pushButtonSelect5Run_clicked()
{
	string query =
		"SELECT \
			PURCHASEORDER.ID AS 'Order ID',\
			COMPUTER.ID AS 'Computer ID',\
			PURCHASEORDER.REMAININGDAYS AS 'Remaining Days',\
			BUYER.NAME AS 'Buyer Name',\
			BUYER.PHONE AS 'Buyer Phone No.',\
			PURCHASEORDER.TOTALCOST AS 'Total Price',\
			BIDTABLE.SELLERNAME AS 'Seller Name',\
			BIDTABLE.SELLERPHONE AS 'Seller Phone No.',\
			LOWESTBID AS 'Lowest Bid'\
		FROM\
			COMPUTER,\
			BUYER,\
			PURCHASEORDER LEFT JOIN(\
				SELECT SELLER.NAME AS SELLERNAME, PHONE AS SELLERPHONE, LOWESTBID, ORDERID\
				FROM(\
					SELECT SELLERID, LOWESTBID, BID.ORDERID AS ORDERID\
					FROM (\
						SELECT BID.ORDERID, MIN(BID.PRICE) AS LOWESTBID\
						FROM BID\
						GROUP BY BID.ORDERID\
					) AS BIDTABLE LEFT JOIN BID ON BID.ORDERID = BIDTABLE.ORDERID AND BID.PRICE = BIDTABLE.LOWESTBID\
				) AS BIDTABLE LEFT JOIN SELLER ON BIDTABLE.SELLERID = SELLER.ID\
			) AS BIDTABLE ON PURCHASEORDER.ID = BIDTABLE.ORDERID\
		WHERE\
			PURCHASEORDER.BUYERID = BUYER.ID AND\
			PURCHASEORDER.COMPUTERID = COMPUTER.ID\
		ORDER BY PURCHASEORDER.ID";

	ReadSchema(query, currentOutput);
}

void MainForm::on_pushButtonSelect6Run_clicked()
{
	string query =
		"SELECT \
			BUYER.NAME AS 'Buyer Name',\
			BUYER.PHONE AS 'Phone Number',\
			BUYER.ADDRESS AS 'Address',\
			ASSERVICE.SERVICENAME AS 'After-Service Name',\
			ASSERVICE.REMAININGDAYS AS 'Remaining Days'\
		FROM \
			ASSERVICE,\
			BUYER \
		WHERE ASSERVICE.BUYERID = BUYER.ID";

	ReadSchema(query, currentOutput);
}


void MainForm::on_pushButtonSelect7Run_clicked()
{
	QueryResult result;
	string query =
		"SELECT \
			COMPUTER.ID AS ID,\
			COMPUTER.PURPOSE AS Purpose,\
			COMPUTER.UPPERCOMPATIBILITY AS Compatibility,\
			COMPUTER.CPUID AS CPU,\
			COMPUTER.RAMID AS RAM,\
			COMPUTER.VGAID AS VGA,\
			COMPUTER.POWERID AS 'Power',\
			COMPUTER.PRICE AS 'Prev Price',\
			NEWPRICE AS 'New Price' \
		FROM COMPUTER LEFT JOIN (\
			SELECT ID, SUM(CPU + RAM + ISNULL(VGA, 0) + PSU + SSD + ISNULL(HDD, 0)) AS NEWPRICE \
			FROM ( \
				SELECT \
					C.ID, CPU.PRICE AS CPU,\
					RAM.PRICE AS RAM,\
					VGA.PRICE AS VGA,\
					POWERSUPPLY.PRICE AS PSU,\
					SSDSummary.PRICE AS SSD,\
					HDDSummary.PRICE AS HDD \
				FROM \
				CPU, RAM, POWERSUPPLY, COMPUTER AS C \
				LEFT JOIN VGA ON C.VGAID = VGA.ID \
				LEFT JOIN ( \
					SELECT COMPUTERID, SUM(SSD.PRICE * QUANTITY) AS PRICE \
					FROM ( \
						SELECT HAS_SSD.COMPUTERID, PRICE, QUANTITY \
						FROM HAS_SSD, SSD \
						WHERE HAS_SSD.SSDID = SSD.ID \
					) AS SSD \
					GROUP BY SSD.COMPUTERID \
				) AS SSDSummary ON C.ID = SSDSummary.COMPUTERID \
				LEFT JOIN( \
					SELECT COMPUTERID, SUM(HDD.PRICE * QUANTITY) AS PRICE \
					FROM( \
						SELECT HAS_HDD.COMPUTERID, PRICE, QUANTITY \
						FROM HAS_HDD, HDD \
						WHERE HAS_HDD.HDDID = HDD.ID \
					) AS HDD \
					GROUP BY HDD.COMPUTERID \
				) AS HDDSummary ON C.ID = HDDSummary.COMPUTERID \
				WHERE \
				C.CPUID = CPU.ID AND \
				C.RAMID = RAM.ID AND \
				C.POWERID = POWERSUPPLY.ID \
			) AS PRICESUMLIST \
			GROUP BY ID \
		) AS NEWPRICELIST ON COMPUTER.ID = NEWPRICELIST.ID";

	result = ReadSchema(query, currentOutput);

	for (int row = 0; row < result.Records().size(); row++)
	{
		if (result.Records()[row][7] != result.Records()[row][8])
			EmphasizeRow(row, currentOutput);
	}
}


void MainForm::on_pushButtonInsert1Run_clicked()
{
	DialogInsert1 dialog;
	QueryResult resultComputer;
	QueryResult resultCPU;
	QueryResult resultRAM;
	QueryResult resultVGA;
	QueryResult resultPSU;
	QueryResult resultSSD;
	QueryResult resultHDD;
	string query;
	QueryResult resultDebug;
	QueryResult computerList;

	resultComputer = database->Query("SELECT ID FROM COMPUTER");
	resultCPU = database->Query("SELECT ID, NAME, PRICE, INTEGRATEDGRAPHICS FROM CPU");
	resultRAM = database->Query("SELECT ID, NAME, PRICE FROM RAM");
	resultVGA = database->Query("SELECT ID, NAME, PRICE FROM VGA");
	resultPSU = database->Query("SELECT ID, NAME, PRICE FROM POWERSUPPLY");
	resultSSD = database->Query("SELECT ID, NAME, PRICE FROM SSD");
	resultHDD = database->Query("SELECT ID, NAME, PRICE FROM HDD");

	for (int row = 0; row < resultCPU.Records().size(); row++)
		dialog.comboBoxCPU->addItem(Utility::Convert(resultCPU.Records()[row][1]));

	for (int row = 0; row < resultRAM.Records().size(); row++)
		dialog.comboBoxRAM->addItem(Utility::Convert(resultRAM.Records()[row][1]));

	for (int row = 0; row < resultVGA.Records().size(); row++)
		dialog.comboBoxVGA->addItem(Utility::Convert(resultVGA.Records()[row][1]));

	for (int row = 0; row < resultPSU.Records().size(); row++)
		dialog.comboBoxPSU->addItem(Utility::Convert(resultPSU.Records()[row][1]));

	for (int row = 0; row < resultSSD.Records().size(); row++)
		dialog.comboBoxSSD->addItem(Utility::Convert(resultSSD.Records()[row][1]));

	for (int row = 0; row < resultHDD.Records().size(); row++)
		dialog.comboBoxHDD->addItem(Utility::Convert(resultHDD.Records()[row][1]));

	dialog.listComputer = &resultComputer;
	dialog.listCPU = &resultCPU;
	dialog.listRAM = &resultRAM;
	dialog.listVGA = &resultVGA;
	dialog.listPSU = &resultPSU;
	dialog.listSSD = &resultSSD;
	dialog.listHDD = &resultHDD;

	if (dialog.exec() != QDialog::Accepted)
		return;

	string computerQuery = "INSERT INTO COMPUTER VALUES (";

	computerQuery += dialog.computerID + ", ";
	computerQuery += '\'' + dialog.purpose + "\', ";
	computerQuery += to_string(dialog.computerPrice) + ", ";
	computerQuery += "NULL, ";
	computerQuery += dialog.cpuID + ", ";
	computerQuery += dialog.ramID + ", ";
	computerQuery += dialog.vgaID + ", ";
	computerQuery += dialog.psuID + ")";

	resultDebug = database->Query(computerQuery);

	string ssdQuery = "INSERT INTO HAS_SSD VALUES (";

	ssdQuery += dialog.computerID + ", ";
	ssdQuery += dialog.ssdID + ", ";
	ssdQuery += to_string(dialog.ssdQuantity) + ")";

	resultDebug = database->Query(ssdQuery);

	string hddQuery = "INSERT INTO HAS_HDD VALUES (";

	hddQuery += dialog.computerID + ", ";
	hddQuery += dialog.hddID + ", ";
	hddQuery += to_string(dialog.hddQuantity) + ")";

	resultDebug = database->Query(hddQuery);

	ReadSchema("SELECT * FROM COMPUTER", currentOutput);
	EmphasizeRow(dialog.computerID, currentOutput);
}


void MainForm::on_pushButtonInsert2Run_clicked()
{
	DialogInsert2 dialog;
	QueryResult result;
	string query;

	ReadSchema("SELECT * FROM BUYER", dialog.tableWidgetBuyer);
	ReadSchema("SELECT * FROM SELLER", dialog.tableWidgetSeller);

	if (dialog.exec() != QDialog::Accepted)
		return;

	string type = dialog.type;
	string ID = dialog.userID;
	string name = dialog.name;
	string phone = dialog.phone;
	string address = dialog.address;

	if (phone == "\'\'")
		phone = "NULL";

	if (address == "\'\'")
		address = "NULL";

	query = "INSERT INTO " + type + " VALUES (";
	query += ID;
	query += ", " + name;
	query += ", " + phone;

	if (type == "Buyer")
		query += ", " + address;

	query += ")";
	result = database->Query(query);

	ReadSchema("SELECT * FROM " + type, currentOutput);
	EmphasizeRow(ID, currentOutput);
}


void MainForm::on_pushButtonInsert3Run_clicked()
{
	DialogInsert3 dialog;
	QueryResult result;
	string query;

	ReadSchema("SELECT * FROM BUYER", dialog.tableWidget);

	if (dialog.exec() != QDialog::Accepted)
		return;

	string ID = dialog.userID;
	string name = dialog.name;
	string period = dialog.period;

	result = database->Query("SELECT * FROM ASSERVICE WHERE BUYERID = " + ID + "AND SERVICENAME = " + name);

	if (result.Records().size() != 0 && result.Records()[0][0] == ID)
	{
		QMessageBox msgBox;

		msgBox.setText(Utility::Korean("이미 가입한 상품입니다.   "));
		msgBox.setWindowTitle("Notice");
		msgBox.exec();

		ReadSchema("SELECT * FROM ASSERVICE", currentOutput);

		for (int row = 0; row < currentOutput->rowCount(); row++)
		{
			if (currentOutput->item(row, 0)->text().toStdString() == ID && currentOutput->item(row, 1)->text().toStdString() == name)
			{
				for (int col = 0; col < currentOutput->columnCount(); col++)
				{
					currentOutput->item(row, col)->setBackground(QBrush(QColor(100, 100, 100)));
					currentOutput->item(row, col)->setForeground(QBrush(QColor(255, 255, 255)));
				}

				break;
			}
		}

		return;
	}

	query = "INSERT INTO ASSERVICE VALUES (";
	query += ID;
	query += ", " + name;
	query += ", " + period + ")";

	result = database->Query(query);

	ReadSchema("SELECT * FROM ASSERVICE", currentOutput);

	name.erase(0, 1);
	name.erase(name.size() - 1);

	for (int row = 0; row < currentOutput->rowCount(); row++)
	{
		if (currentOutput->item(row, 0)->text().toStdString() == ID && currentOutput->item(row, 1)->text().toStdString() == name)
		{
			for (int col = 0; col < currentOutput->columnCount(); col++)
			{
				currentOutput->item(row, col)->setBackground(QBrush(QColor(100, 100, 100)));
				currentOutput->item(row, col)->setForeground(QBrush(QColor(255, 255, 255)));
			}

			break;
		}
	}
}


void MainForm::on_pushButtonInsert4Run_clicked()
{
	DialogInsert4 dialog;
	QueryResult result;
	string query =
		"SELECT \
				PURCHASEORDER.ID AS 'Order ID',\
				PURCHASEORDER.REMAININGDAYS AS 'Remaining Days',\
				COMPUTER.ID AS 'Computer ID',\
				PURCHASEORDER.TOTALCOST AS 'Computer Price',\
				LOWESTBID AS 'Lowest Bid'\
			FROM\
				COMPUTER,\
				BUYER,\
				PURCHASEORDER LEFT JOIN(\
					SELECT SELLER.NAME AS SELLERNAME, PHONE AS SELLERPHONE, LOWESTBID, ORDERID\
					FROM(\
						SELECT SELLERID, LOWESTBID, BID.ORDERID AS ORDERID\
						FROM (\
							SELECT BID.ORDERID, MIN(BID.PRICE) AS LOWESTBID\
							FROM BID\
							GROUP BY BID.ORDERID\
						) AS BIDTABLE LEFT JOIN BID ON BID.ORDERID = BIDTABLE.ORDERID AND BID.PRICE = BIDTABLE.LOWESTBID\
					) AS BIDTABLE LEFT JOIN SELLER ON BIDTABLE.SELLERID = SELLER.ID\
				) AS BIDTABLE ON PURCHASEORDER.ID = BIDTABLE.ORDERID\
			WHERE\
				PURCHASEORDER.BUYERID = BUYER.ID AND\
				PURCHASEORDER.COMPUTERID = COMPUTER.ID\
			ORDER BY PURCHASEORDER.ID";

	ReadSchema(query, dialog.tableWidgetOrder);
	ReadSchema("SELECT * FROM SELLER", dialog.tableWidgetSeller);

	if (dialog.exec() != QDialog::Accepted)
		return;

	string bidID;
	string sellerID = dialog.sellerID;
	string orderID = dialog.orderID;
	string price = dialog.price;

	result = database->Query("SELECT ID FROM BID");

	for (;;)
	{
		bool isUnique = true;

		bidID = to_string(Random::Integer(1000, 9999));

		for (int row = 0; row < result.Records().size(); row++)
		{
			if (bidID == result.Records()[row][0])
			{
				isUnique = false;
				break;
			}
		}

		if (isUnique)
			break;
	}

	query = "INSERT INTO BID VALUES (" + bidID;
	query += ", " + sellerID;
	query += "," + orderID;
	query += ", " + price + ")";

	result = database->Query(query);

	ReadSchema("SELECT * FROM BID", currentOutput);
	EmphasizeRow(bidID, currentOutput);
}


void MainForm::on_pushButtonUpdate1Run_clicked()
{
	QueryResult resultComputerPrice;
	QueryResult resultComputerPriceNew;
	QueryResult resultDebug;
	string query =
		"SELECT ID, SUM(CPU + RAM + ISNULL(VGA,0) + PSU + SSD + ISNULL(HDD,0)) \
			FROM ( \
				SELECT \
					C.ID, CPU.PRICE AS CPU, \
					RAM.PRICE AS RAM, \
					VGA.PRICE AS VGA, \
					POWERSUPPLY.PRICE AS PSU, \
					SSDSummary.PRICE AS SSD, \
					HDDSummary.PRICE AS HDD \
				FROM \
					CPU, RAM, POWERSUPPLY, COMPUTER AS C \
					LEFT JOIN VGA ON C.VGAID = VGA.ID \
					LEFT JOIN( \
						SELECT COMPUTERID, SUM(SSD.PRICE * QUANTITY) AS PRICE \
						FROM ( \
							SELECT HAS_SSD.COMPUTERID, PRICE, QUANTITY \
							FROM HAS_SSD, SSD \
							WHERE HAS_SSD.SSDID = SSD.ID \
						) AS SSD \
						GROUP BY SSD.COMPUTERID \
					) AS SSDSummary ON C.ID = SSDSummary.COMPUTERID \
					LEFT JOIN( \
						SELECT COMPUTERID, SUM(HDD.PRICE * QUANTITY) AS PRICE \
						FROM ( \
							SELECT HAS_HDD.COMPUTERID, PRICE, QUANTITY \
							FROM HAS_HDD, HDD \
							WHERE HAS_HDD.HDDID = HDD.ID \
						) AS HDD \
						GROUP BY HDD.COMPUTERID \
					) AS HDDSummary ON C.ID = HDDSummary.COMPUTERID \
				WHERE \
					C.CPUID = CPU.ID AND \
					C.RAMID = RAM.ID AND \
					C.POWERID = POWERSUPPLY.ID \
			) AS PRICESUMLIST \
			GROUP BY ID";

	resultComputerPrice = database->Query("SELECT ID, PRICE FROM COMPUTER");
	resultComputerPriceNew = database->Query(query);

	ReadSchema("SELECT * FROM COMPUTER", currentOutput);

	for (int row = 0; row < resultComputerPriceNew.Records().size(); row++)
	{
		string updateQuery =
			"UPDATE COMPUTER \
				SET COMPUTER.PRICE = " + resultComputerPriceNew.Records()[row][1] +
			"WHERE COMPUTER.ID = " + resultComputerPriceNew.Records()[row][0];

		database->Query(updateQuery);
	}

	query =
		"UPDATE PURCHASEORDER \
			SET PURCHASEORDER.TOTALCOST = PRICESUM.PRICE \
			FROM ( \
				SELECT \
					PURCHASEORDER.ID, \
					PURCHASEORDER.QUANTITY * COMPUTER.PRICE AS PRICE \
					FROM PURCHASEORDER, COMPUTER \
				WHERE PURCHASEORDER.COMPUTERID = COMPUTER.ID \
			) AS PRICESUM \
			WHERE PURCHASEORDER.ID = PRICESUM.ID";

	resultDebug = database->Query(query);

	ReadSchema("SELECT * FROM COMPUTER", currentOutput);

	for (int row = 0; row < resultComputerPrice.Records().size(); row++)
	{
		if (resultComputerPrice.Records()[row][1] != resultComputerPriceNew.Records()[row][1])
		{
			for (int col = 0; col < currentOutput->columnCount(); col++)
			{
				currentOutput->item(row, col)->setBackground(QBrush(QColor(100, 100, 100)));
				currentOutput->item(row, col)->setForeground(QBrush(QColor(255, 255, 255)));
			}
		}
	}
}


void MainForm::on_pushButtonUpdate2Run_clicked()
{
	string type = ui.comboBoxUpdate2Type->currentText().toStdString();
	string targetID = currentOutput->item(ui.comboBoxUpdate2Model->currentIndex(), 0)->text().toStdString();
	string query =
		"UPDATE " + type + " \
			SET PRICE = " + to_string(ui.spinBoxUpdate2Price->value()) + " \
			WHERE ID = " + targetID;

	QueryResult resultDebug = database->Query(query);

	ReadSchema("SELECT * FROM " + type, currentOutput);
	EmphasizeRow(targetID, currentOutput);
}


void MainForm::on_comboBoxUpdate2Type_currentTextChanged(QString text)
{
	QueryResult result;

	ui.comboBoxUpdate2Model->clear();

	ReadSchema("SELECT * FROM " + text.toStdString(), currentOutput);
	result = database->Query("SELECT NAME FROM " + text.toStdString());

	for (int row = 0; row < result.Records().size(); row++)
		ui.comboBoxUpdate2Model->addItem(Utility::Convert(result.Records()[row][0]));
}


void MainForm::on_comboBoxUpdate2Model_currentIndexChanged(int index)
{
	if (index < 0)
		return;

	QueryResult result = database->Query("SELECT PRICE FROM " + ui.comboBoxUpdate2Type->currentText().toStdString());

	ui.spinBoxUpdate2Price->setValue(stoi(result.Records()[index][0]));

	for (int row = 0; row < currentOutput->rowCount(); row++)
	{
		for (int col = 0; col < currentOutput->columnCount(); col++)
		{
			if (row == index)
				currentOutput->item(row, col)->setSelected(true);
			else
				currentOutput->item(row, col)->setSelected(false);
		}
	}
}


void MainForm::on_tableWidget4Update_currentCellChanged(int row)
{
	if (ui.toolBoxMenu->currentIndex() != 3 || ui.tabUpdate->currentIndex() != 1)
		return;

	ui.comboBoxUpdate2Model->setCurrentIndex(row);
}


void MainForm::on_tabUpdate_currentChanged(int index)
{
	if (index != 1 || ui.comboBoxUpdate2Type->currentIndex() == -1)
		return;

	on_comboBoxUpdate2Type_currentTextChanged(ui.comboBoxUpdate2Type->currentText());
}


void MainForm::on_pushButtonUpdate3Run_clicked()
{
	QueryResult resultLowestBid;
	QueryResult resultOrder;
	string query;
	int elapsedDate;

	query =
		"SELECT \
				PURCHASEORDER.ID AS 'Order ID',\
				COMPUTER.ID AS 'Computer ID',\
				PURCHASEORDER.REMAININGDAYS AS 'Remaining Days',\
				BUYER.NAME AS 'Buyer Name',\
				BUYER.PHONE AS 'Buyer Phone No.',\
				PURCHASEORDER.TOTALCOST AS 'Total Price',\
				BIDTABLE.SELLERNAME AS 'Seller Name',\
				BIDTABLE.SELLERPHONE AS 'Seller Phone No.',\
				LOWESTBID AS 'Lowest Bid'\
			FROM\
				COMPUTER,\
				BUYER,\
				PURCHASEORDER LEFT JOIN(\
					SELECT SELLER.NAME AS SELLERNAME, PHONE AS SELLERPHONE, LOWESTBID, ORDERID\
					FROM(\
						SELECT SELLERID, LOWESTBID, BID.ORDERID AS ORDERID\
						FROM (\
							SELECT BID.ORDERID, MIN(BID.PRICE) AS LOWESTBID\
							FROM BID\
							GROUP BY BID.ORDERID\
						) AS BIDTABLE LEFT JOIN BID ON BID.ORDERID = BIDTABLE.ORDERID AND BID.PRICE = BIDTABLE.LOWESTBID\
					) AS BIDTABLE LEFT JOIN SELLER ON BIDTABLE.SELLERID = SELLER.ID\
				) AS BIDTABLE ON PURCHASEORDER.ID = BIDTABLE.ORDERID\
			WHERE\
				PURCHASEORDER.BUYERID = BUYER.ID AND\
				PURCHASEORDER.COMPUTERID = COMPUTER.ID\
			ORDER BY PURCHASEORDER.ID";

	resultLowestBid = ReadSchema(query, currentOutput);
	resultOrder = database->Query("SELECT ID, REMAININGDAYS FROM PURCHASEORDER");
	elapsedDate = ui.spinBoxUpdate3->value();

	for (int row = 0; row < resultOrder.Records().size(); row++)
	{
		int newRemainingDays = stoi(resultOrder.Records()[row][1]) - elapsedDate;

		if (newRemainingDays < 0)
			newRemainingDays = 0;

		currentOutput->item(row, 2)->setText(QString::number(newRemainingDays));

		if (newRemainingDays == 0)
		{
			query =
				"DELETE FROM BID \
					WHERE ORDERID = " + resultOrder.Records()[row][0];

			database->Query(query);

			query =
				"DELETE FROM PURCHASEORDER \
					WHERE ID = " + resultOrder.Records()[row][0];

			database->Query(query);

			for (int col = 0; col < currentOutput->columnCount(); col++)
			{
				currentOutput->item(row, col)->setBackground(QBrush(QColor(100, 100, 100)));
				currentOutput->item(row, col)->setForeground(QBrush(QColor(255, 255, 255)));
			}
		}
		else
		{
			query =
				"UPDATE PURCHASEORDER \
					SET REMAININGDAYS = " + to_string(newRemainingDays) + " \
					WHERE ID = " + resultOrder.Records()[row][0];

			database->Query(query);
		}
	}
}


void MainForm::on_pushButtonUpdate4Run_clicked()
{
	DialogUpdate4 dialog;
	QueryResult resultBuyer;
	QueryResult resultSeller;
	QueryResult resultBuyerNew;
	QueryResult resultSellerNew;
	string query;

	resultBuyer = ReadSchema("SELECT * FROM BUYER", dialog.tableWidgetBuyer);
	resultSeller = ReadSchema("SELECT * FROM SELLER", dialog.tableWidgetSeller);

	dialog.database = database;
	dialog.resultBuyer = &resultBuyer;
	dialog.resultSeller = &resultSeller;
	dialog.changedBuyerRow.resize(resultBuyer.Records().size(), false);
	dialog.enabled = true;

	dialog.exec();

	resultBuyerNew = ReadSchema("SELECT * FROM BUYER", currentOutput);
	// resultSellerNew = ReadSchema("SELECT * FROM SELLER",currentOutput);

	for (int index = 0; index < resultBuyerNew.Records().size(); index++)
	{
		if (dialog.changedBuyerRow[index])
			EmphasizeRow(index, currentOutput);
	}
}


void MainForm::on_pushButtonDelete1Run_clicked()
{
	DialogDelete dialog;
	QTableWidget* tableWidget = dialog.tableWidget;
	QueryResult result;
	string query =
		"SELECT * \
		FROM COMPUTER \
		WHERE NOT EXISTS ( \
			SELECT * \
			FROM PURCHASEORDER \
			WHERE COMPUTER.ID = PURCHASEORDER.COMPUTERID)";

	ReadSchema(query, tableWidget);

	if (dialog.exec() != QDialog::Accepted)
		return;

	bool isExist = false;
	string targetID = dialog.target.toStdString();

	for (int row = 0; row < tableWidget->rowCount(); row++)
	{
		if (tableWidget->item(row, 0)->text() == Utility::Convert(targetID))
		{
			isExist = true;
			break;
		}
	}

	if (!isExist)
	{
		PrintMessageBox(targetID + "번 컴퓨터를 찾을 수 없습니다.");
		return;
	}

	ReadSchema("SELECT * FROM COMPUTER", currentOutput);

	targetID = dialog.target.toStdString();
	query =
		"UPDATE COMPUTER \
		SET UPPERCOMPATIBILITY = NULL \
		WHERE UPPERCOMPATIBILITY = " + targetID +
		" \
		DELETE FROM COMPUTER \
		WHERE COMPUTER.ID = " + targetID;

	result = database->Query(query);

	EmphasizeRow(targetID, currentOutput);
	PrintMessageBox(targetID + "번 컴퓨터가 삭제되었습니다.");
}


void MainForm::on_pushButtonDelete2Run_clicked()
{
	DialogDelete dialog;
	QTableWidget* tableWidget = dialog.tableWidget;
	QueryResult result;
	string type;
	string typeKorean;
	string query;

	type = ui.comboBoxDelete2->currentText().toStdString();

	if (type == "Buyer")
	{
		typeKorean = "구매자";
		query =
			"SELECT * \
			FROM BUYER \
			WHERE NOT EXISTS ( \
				SELECT * \
				FROM PURCHASEORDER \
				WHERE BUYER.ID = PURCHASEORDER.BUYERID)";
	}
	else
	{
		typeKorean = "판매자";
		query =
			"SELECT * \
			FROM SELLER \
			WHERE NOT EXISTS ( \
				SELECT * \
				FROM BID \
				WHERE SELLER.ID = BID.SELLERID)";
	}


	ReadSchema(query, tableWidget);

	if (dialog.exec() != QDialog::Accepted)
		return;

	bool isExist = false;
	string targetID = dialog.target.toStdString();

	for (int row = 0; row < tableWidget->rowCount(); row++)
	{
		if (tableWidget->item(row, 0)->text() == Utility::Convert(targetID))
		{
			isExist = true;
			break;
		}
	}

	if (!isExist)
	{
		PrintMessageBox(targetID + "번 " + typeKorean + "를 찾을 수 없습니다.");
		return;
	}

	targetID = dialog.target.toStdString();

	if (type == "Buyer")
	{
		query =
			"DELETE FROM BUYER \
			WHERE BUYER.ID = " + targetID;

		ReadSchema("SELECT * FROM BUYER", currentOutput);
	}
	else
	{
		query =
			"DELETE FROM SELLER \
			WHERE SELLER.ID = " + targetID;

		ReadSchema("SELECT * FROM SELLER", currentOutput);
	}

	result = database->Query(query);

	EmphasizeRow(targetID, currentOutput);
	PrintMessageBox(targetID + "번 " + typeKorean + "가 삭제되었습니다.");
}


void MainForm::on_pushButtonDelete3Run_clicked()
{
	DialogDelete dialog;
	QTableWidget* tableWidget = dialog.tableWidget;
	QueryResult result;
	string query =
		"SELECT * \
		FROM PURCHASEORDER \
		WHERE NOT EXISTS ( \
			SELECT * \
			FROM BID \
			WHERE PURCHASEORDER.ID = BID.ORDERID)";

	ReadSchema(query, tableWidget);

	if (dialog.exec() != QDialog::Accepted)
		return;

	bool isExist = false;
	string targetID = dialog.target.toStdString();

	for (int row = 0; row < tableWidget->rowCount(); row++)
	{
		if (tableWidget->item(row, 0)->text() == Utility::Convert(targetID))
		{
			isExist = true;
			break;
		}
	}

	if (!isExist)
	{
		PrintMessageBox(targetID + "번 주문서를 찾을 수 없습니다.");
		return;
	}

	ReadSchema("SELECT * FROM PURCHASEORDER", currentOutput);

	targetID = dialog.target.toStdString();
	query =
		"DELETE FROM PURCHASEORDER \
		WHERE ID = " + targetID;

	result = database->Query(query);

	EmphasizeRow(targetID, currentOutput);
	PrintMessageBox(targetID + "번 주문서가 삭제되었습니다.");
}


void MainForm::on_pushButtonDelete4Run_clicked()
{
	DialogDelete dialog;
	QTableWidget* tableWidget = dialog.tableWidget;
	QueryResult result;
	string type;
	string typeKorean;
	string query;

	type = ui.comboBoxDelete4->currentText().toStdString();

	if (type == "SSD" || type == "HDD")
	{
		query =
			"SELECT * \
			FROM " + type + " \
			WHERE NOT EXISTS ( \
				SELECT * \
				FROM HAS_" + type + "\
				WHERE HAS_" + type + "." + type + "ID = " + type + ".ID)";
	}
	else
	{
		query =
			"SELECT * \
			FROM " + type + " \
			WHERE NOT EXISTS ( \
				SELECT * \
				FROM COMPUTER ";

		if (type == "PowerSupply")
			query += "WHERE COMPUTER.POWERID = " + type + ".ID)";
		else
			query += "WHERE COMPUTER." + type + "ID = " + type + ".ID)";
	}

	result = ReadSchema(query, tableWidget);

	if (dialog.exec() != QDialog::Accepted)
		return;

	bool isExist = false;
	string targetID = dialog.target.toStdString();

	for (int row = 0; row < tableWidget->rowCount(); row++)
	{
		if (tableWidget->item(row, 0)->text() == Utility::Convert(targetID))
		{
			isExist = true;
			break;
		}
	}

	if (!isExist)
	{
		PrintMessageBox(targetID + "번 " + type + "을(를) 찾을 수 없습니다.");
		return;
	}

	targetID = dialog.target.toStdString();
	query =
		"DELETE FROM " + type + " \
		WHERE " + type + ".ID = " + targetID;

	result = ReadSchema("SELECT * FROM " + type, currentOutput);
	result = database->Query(query);

	EmphasizeRow(targetID, currentOutput);
	PrintMessageBox(targetID + "번 " + type + "이(가) 삭제되었습니다.");
}