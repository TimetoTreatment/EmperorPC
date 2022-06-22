#include "DialogInsert4.h"


DialogInsert4::DialogInsert4(QWidget* parent)
{
	ui.setupUi(this);

	tableWidgetSeller = ui.tableWidgetSeller;
	tableWidgetOrder = ui.tableWidgetOrder;

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}


void DialogInsert4::on_tableWidgetOrder_currentCellChanged(int row, int col)
{
	ui.lineEditOrderID->setText(ui.tableWidgetOrder->item(row, 0)->text());
	ui.lineEditLowestBid->setText(ui.tableWidgetOrder->item(row, 4)->text());

	int priceValue;

	if (ui.tableWidgetOrder->item(row, 4)->text() != "-")
		priceValue = ui.tableWidgetOrder->item(row, 4)->text().toInt() - 10;
	else
		priceValue = ui.tableWidgetOrder->item(row, 3)->text().toInt();

	orderID = ui.tableWidgetOrder->item(row, 0)->text().toStdString();
	lowestBid = ui.tableWidgetOrder->item(row, 4)->text().toStdString();
	ui.spinBoxPrice->setValue(priceValue);
	price = std::to_string(ui.spinBoxPrice->value());
	computerPrice = ui.tableWidgetOrder->item(row, 3)->text().toStdString();

	if (!orderID.empty() && !sellerID.empty())
		ui.groupBox->setEnabled(true);
}


void DialogInsert4::on_tableWidgetSeller_currentCellChanged(int row, int col)
{
	ui.lineEditSellerID->setText(ui.tableWidgetSeller->item(row, 0)->text());

	sellerID = ui.tableWidgetSeller->item(row, 0)->text().toStdString();

	if (!orderID.empty() && !sellerID.empty())
		ui.groupBox->setEnabled(true);
}


void DialogInsert4::on_spinBoxPrice_valueChanged(int value)
{
	if (lowestBid == "-")
	{
		if (value > stoi(computerPrice))
		{
			value = stoi(computerPrice);
			ui.spinBoxPrice->setValue(value);
		}
	}

	else
	{
		if (value >= stoi(lowestBid))
		{
			value = stoi(lowestBid) - 10;
			ui.spinBoxPrice->setValue(value);
		}
	}

	price = std::to_string(value);
}