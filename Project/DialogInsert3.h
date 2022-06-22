#pragma once
#include <QDialog>
#include <QTableWidget>
#include <QMessageBox>
#include "ui_DialogInsert3.h"
#include "Utility.h"


class DialogInsert3 : public QDialog
{
	Q_OBJECT

public:

	DialogInsert3(QWidget* parent = Q_NULLPTR);

	QTableWidget* tableWidget;

	std::string userID;
	std::string name;
	std::string period;


private:

	Ui::DialogInsert3 ui;


private slots:

	void Check();
	void on_tableWidget_currentCellChanged(int row, int col);
};

