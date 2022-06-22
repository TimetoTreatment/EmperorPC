#include "DialogDelete.h"


DialogDelete::DialogDelete(QWidget* parent)
{
	ui.setupUi(this);

	tableWidget = ui.tableWidget;

	ui.lineEdit->setFixedSize(75, ui.lineEdit->size().height());

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}


void DialogDelete::on_tableWidget_currentCellChanged(int row)
{
	ui.lineEdit->setText(tableWidget->item(row, 0)->text());
}


void DialogDelete::on_buttonBox_accepted()
{
	target = ui.lineEdit->text();
}