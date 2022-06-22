#include "DialogUpdate4.h"


DialogUpdate4::DialogUpdate4(QWidget* parent)
{
	ui.setupUi(this);

	tableWidgetBuyer = ui.tableWidgetBuyer;
	tableWidgetSeller = ui.tableWidgetSeller;

	ui.stackedWidget->setCurrentIndex(0);
	ui.comboBox->setCurrentIndex(0);
}


void DialogUpdate4::UpdateChanged(QTableWidgetItem* item)
{
	int row = item->row();
	int col = item->column();

	if (col == 0)
	{
		QMessageBox msgBox;

		msgBox.setText(Utility::Korean("사용자 ID는 변경할 수 없습니다.     "));
		msgBox.setWindowTitle("Notice");
		msgBox.exec();

		return;
	}

	if (col == 1 && item->text().size() == 0)
	{
		QMessageBox msgBox;

		msgBox.setText(Utility::Korean("사용자 이름은 공백일 수 없습니다.     "));
		msgBox.setWindowTitle("Notice");
		msgBox.exec();

		return;
	}

	std::string query;
	std::string type = ui.comboBox->currentText().toStdString();
	std::string ID = (type == "Buyer") ?
		ui.tableWidgetBuyer->item(row, 0)->text().toStdString() :
		ui.tableWidgetSeller->item(row, 0)->text().toStdString();

	query = "UPDATE " + type + " SET ";

	switch (item->column())
	{
	case 1:
		query += "NAME = \'" + item->text().toStdString() + "\' ";
		query += "WHERE ID = " + ID;
		break;

	case 2:
		if (item->text() == "NULL" || item->text() == "-" || item->text().size() == 0)
			query += "PHONE = NULL ";
		else
			query += "PHONE = \'" + item->text().toStdString() + "\' ";

		query += "WHERE ID = " + ID;
		break;

	case 3:
		if (item->text() == "NULL" || item->text() == "-" || item->text().size() == 0)
			query += "ADDRESS = NULL ";
		else
			query += "ADDRESS = \'" + item->text().toStdString() + "\' ";

		query += "WHERE ID = " + ID;
		break;
	}

	database->Query(query);

	if (type == "Buyer")
		changedBuyerRow[row] = true;
}


void DialogUpdate4::UpdateSelected(int row)
{
	std::string type = ui.comboBox->currentText().toStdString();

	if (type == "Buyer")
	{
		ui.lineEditID->setText(tableWidgetBuyer->item(row, 0)->text());
		ui.lineEditName->setText(tableWidgetBuyer->item(row, 1)->text());
		ui.lineEditPhone->setText(tableWidgetBuyer->item(row, 2)->text());
		ui.lineEditAddress->setText(tableWidgetBuyer->item(row, 3)->text());
	}
	else
	{
		ui.lineEditID->setText(tableWidgetSeller->item(row, 0)->text());
		ui.lineEditName->setText(tableWidgetSeller->item(row, 1)->text());
		ui.lineEditPhone->setText(tableWidgetSeller->item(row, 2)->text());
		ui.lineEditAddress->clear();
	}
}


void DialogUpdate4::on_comboBox_currentIndexChanged(int index)
{
	ui.stackedWidget->setCurrentIndex(index);

	if (index == 0)
		ui.lineEditAddress->setEnabled(true);
	else
		ui.lineEditAddress->setEnabled(false);
}


void DialogUpdate4::on_buttonBox_clicked(QAbstractButton* button)
{
	enabled = false;
	done(QDialog::DialogCode::Accepted);
}


void DialogUpdate4::on_tableWidgetBuyer_itemChanged(QTableWidgetItem* item)
{
	if (enabled == false)
		return;

	UpdateChanged(item);
}


void DialogUpdate4::on_tableWidgetSeller_itemChanged(QTableWidgetItem* item)
{
	if (enabled == false)
		return;

	UpdateChanged(item);
}


void DialogUpdate4::on_tableWidgetBuyer_cellClicked(int row, int col)
{
	if (enabled == false)
		return;

	UpdateSelected(row);
}


void DialogUpdate4::on_tableWidgetSeller_cellClicked(int row, int col)
{
	if (enabled == false)
		return;

	UpdateSelected(row);
}
