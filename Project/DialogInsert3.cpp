#include "DialogInsert3.h"


DialogInsert3::DialogInsert3(QWidget* parent)
{
	ui.setupUi(this);

	tableWidget = ui.tableWidget;

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &DialogInsert3::Check);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}


void DialogInsert3::Check()
{
	if (ui.lineEditName->text().size() == 0 || ui.lineEditPeriod->text().size() == 0)
	{
		QMessageBox msgBox;

		msgBox.setText(Utility::Korean("빈 칸을 모두 채워야 합니다.   "));
		msgBox.setWindowTitle("Notice");
		msgBox.exec();

		return;
	}

	userID = ui.lineEditID->text().toStdString();
	name = "\'" + ui.lineEditName->text().toStdString() + "\'";
	period = ui.lineEditPeriod->text().toStdString();

	done(QDialog::Accepted);
}

void DialogInsert3::on_tableWidget_currentCellChanged(int row, int col)
{
	ui.lineEditID->setText(tableWidget->item(row, 0)->text());
}