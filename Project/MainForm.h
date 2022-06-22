#pragma once
#include <QtWidgets/QMainWindow>
#include <QActionGroup>
#include <QButtonGroup>
#include <QMessageBox>
#include <QComboBox>

#include <fstream>
#include <sstream>
#include "Database.h"
#include "Utility.h"
#include "Random.h"

#include "ui_MainForm.h"
#include "DialogNewAuthentication.h"
#include "DialogDelete.h"
#include "DialogInsert1.h"
#include "DialogInsert2.h"
#include "DialogInsert3.h"
#include "DialogInsert4.h"
#include "DialogUpdate4.h"

#define qout qDebug()

using namespace std;


class MainForm : public QMainWindow
{
	Q_OBJECT

public:

	MainForm(QWidget* parent = nullptr);


private:

	Ui::MainForm ui;

	QLabel* statusBar;
	QTableWidget* tableWidgets[6];
	QTableWidget* currentOutput;

	Database* database = nullptr;
	QString serverName;
	QString databaseName;
	QString userName;
	QString password;

	bool isTriggeredByUIAction = false;
	bool isTriggeredByUITab = false;

	void DatabaseConnect();
	void NewAuthentication();

	void CLIModeManager();

	void PrintStatusBar(string text);
	void PrintMessageBox(string text);

	QueryResult ReadSchema(string query, QTableWidget* tableWidget);
	void PrintTable(vector<string> header, vector<vector<string>> records, QTableWidget* tableWidget);

	void EmphasizeRow(int row, QTableWidget* tableWidget);
	void EmphasizeRow(string ID, QTableWidget* tableWidget);


private slots:

	void on_actionCLI_changed();
	void on_tabWidgetUI_currentChanged(int index);

	void on_actionEnable_All_triggered();
	void on_actionInitialize_Table_triggered();
	void on_actionDefault_Authentication_triggered();
	void on_toolBoxMenu_currentChanged(int index);

	void on_actionNew_Authentication_triggered() { NewAuthentication(); }
	void on_pushButtonCLI_clicked() { ui.tabWidgetUI->setCurrentIndex(1); }
	void on_actionConnect_triggered() { DatabaseConnect(); }
	void on_textEditInput_textChanged() { CLIModeManager(); }

	void on_radioButtonCheck1Buyer_clicked() { ReadSchema("SELECT * FROM BUYER", currentOutput); }
	void on_radioButtonCheck1Seller_clicked() { ReadSchema("SELECT * FROM SELLER", currentOutput); }
	void on_radioButtonCheck1PurchaseOrder_clicked() { ReadSchema("SELECT * FROM PURCHASEORDER", currentOutput); }
	void on_radioButtonCheck1Bid_clicked() { ReadSchema("SELECT * FROM BID", currentOutput); }
	void on_radioButtonCheck1ASService_clicked() { ReadSchema("SELECT * FROM ASSERVICE", currentOutput); }

	void on_radioButtonCheck2Computer_clicked() { ReadSchema("SELECT * FROM COMPUTER", currentOutput); }
	void on_radioButtonCheck2HasSSD_clicked() { ReadSchema("SELECT * FROM HAS_SSD", currentOutput); }
	void on_radioButtonCheck2HasHDD_clicked() { ReadSchema("SELECT * FROM HAS_HDD", currentOutput); }
	void on_radioButtonCheck2RAMPackaging_clicked() { ReadSchema("SELECT * FROM RAM_PACKAGING", currentOutput); }

	void on_radioButtonCheck3CPU_clicked() { ReadSchema("SELECT * FROM CPU", currentOutput); }
	void on_radioButtonCheck3RAM_clicked() { ReadSchema("SELECT * FROM RAM", currentOutput); }
	void on_radioButtonCheck3VGA_clicked() { ReadSchema("SELECT * FROM VGA", currentOutput); }
	void on_radioButtonCheck3PSU_clicked() { ReadSchema("SELECT * FROM POWERSUPPLY", currentOutput); }
	void on_radioButtonCheck3SSD_clicked() { ReadSchema("SELECT * FROM SSD", currentOutput); }
	void on_radioButtonCheck3HDD_clicked() { ReadSchema("SELECT * FROM HDD", currentOutput); }

	void on_pushButtonSelect1Run_clicked();
	void on_pushButtonSelect2Run_clicked();
	void on_lineEdit2Budget_textChanged(QString budgetText);
	void on_pushButtonSelect3Run_clicked();
	void on_pushButtonSelect4Run_clicked();
	void on_pushButtonSelect5Run_clicked();
	void on_pushButtonSelect6Run_clicked();
	void on_pushButtonSelect7Run_clicked();

	void on_pushButtonInsert1Run_clicked();
	void on_pushButtonInsert2Run_clicked();
	void on_pushButtonInsert3Run_clicked();
	void on_pushButtonInsert4Run_clicked();

	void on_pushButtonUpdate1Run_clicked();
	void on_pushButtonUpdate2Run_clicked();
	void on_comboBoxUpdate2Type_currentTextChanged(QString text);
	void on_comboBoxUpdate2Model_currentIndexChanged(int index);
	void on_tableWidget4Update_currentCellChanged(int row);
	void on_tabUpdate_currentChanged(int index);
	void on_pushButtonUpdate3Run_clicked();
	void on_pushButtonUpdate4Run_clicked();

	void on_pushButtonDelete1Run_clicked();
	void on_pushButtonDelete2Run_clicked();
	void on_pushButtonDelete3Run_clicked();
	void on_pushButtonDelete4Run_clicked();
};
