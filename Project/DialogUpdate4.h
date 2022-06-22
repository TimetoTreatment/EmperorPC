#pragma once
#include <QDialog>
#include <QTableWidget>
#include <QMessageBox>
#include "ui_DialogUpdate4.h"
#include "Database.h"
#include "Utility.h"


class DialogUpdate4 : public QDialog
{
	Q_OBJECT

public:

	DialogUpdate4(QWidget* parent = nullptr);

	Database* database = nullptr;
	QueryResult* resultBuyer = nullptr;
	QueryResult* resultSeller = nullptr;
	QTableWidget* tableWidgetBuyer = nullptr;
	QTableWidget* tableWidgetSeller = nullptr;

	std::vector<bool> changedBuyerRow;
	bool enabled = false;


private:

	Ui::DialogUpdate4 ui;

	void UpdateChanged(QTableWidgetItem* item);
	void UpdateSelected(int row);


private slots:

	void on_comboBox_currentIndexChanged(int index);
	void on_buttonBox_clicked(QAbstractButton* button);
	void on_tableWidgetBuyer_itemChanged(QTableWidgetItem* item);
	void on_tableWidgetSeller_itemChanged(QTableWidgetItem* item);
	void on_tableWidgetBuyer_cellClicked(int row, int col);
	void on_tableWidgetSeller_cellClicked(int row, int col);
};
