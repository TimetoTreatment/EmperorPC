#pragma once
#include <QDialog>
#include <QTableWidget>
#include <QMessageBox>
#include "ui_DialogInsert2.h"
#include "Utility.h"


class DialogInsert2 : public QDialog
{
	Q_OBJECT

public:

	DialogInsert2(QWidget* parent = Q_NULLPTR);

	QTableWidget* tableWidgetBuyer;
	QTableWidget* tableWidgetSeller;

	std::string type;
	std::string userID;
	std::string name;
	std::string phone;
	std::string address;


private:

	Ui::DialogInsert2 ui;


private slots:

	void on_comboBox_currentIndexChanged(int index);
	void Check();
};

