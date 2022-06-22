#pragma once
#include <QDialog>
#include <QTableWidget>
#include <QMessageBox>
#include "ui_DialogInsert4.h"
#include "Utility.h"


class DialogInsert4 : public QDialog
{
	Q_OBJECT

public:

	DialogInsert4(QWidget* parent = Q_NULLPTR);

	QTableWidget* tableWidgetSeller;
	QTableWidget* tableWidgetOrder;

	std::string sellerID;
	std::string orderID;
	std::string lowestBid;
	std::string price;
	std::string computerPrice;


private:

	Ui::DialogInsert4 ui;


private slots:

	void on_tableWidgetOrder_currentCellChanged(int row, int col);
	void on_tableWidgetSeller_currentCellChanged(int row, int col);
	void on_spinBoxPrice_valueChanged(int value);
};

