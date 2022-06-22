#include "DialogInsert2.h"


DialogInsert2::DialogInsert2(QWidget* parent)
{
	ui.setupUi(this);

	tableWidgetBuyer = ui.tableWidgetBuyer;
	tableWidgetSeller = ui.tableWidgetSeller;

	ui.stackedWidget->setCurrentIndex(0);
	ui.comboBox->setCurrentIndex(0);

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &DialogInsert2::Check);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}


void DialogInsert2::on_comboBox_currentIndexChanged(int index)
{
	ui.stackedWidget->setCurrentIndex(index);

	if (index == 0)
		ui.lineEditAddress->setEnabled(true);
	else
		ui.lineEditAddress->setEnabled(false);
}


void DialogInsert2::Check()
{
	type = ui.comboBox->currentText().toStdString();
	userID = ui.lineEditID->text().toStdString();
	name = ui.lineEditName->text().toStdString();
	phone = ui.lineEditPhone->text().toStdString();
	address = ui.lineEditAddress->text().toStdString();

	if (ui.lineEditPhone->text().isEmpty())
		phone = "NULL";

	if (ui.lineEditAddress->text().isEmpty())
		address = "NULL";

	if (userID.size() == 0)
	{
		QMessageBox msgBox;

		msgBox.setText(Utility::Korean("ID를 입력해야 합니다.   "));
		msgBox.setWindowTitle("Notice");
		msgBox.exec();

		return;
	}

	if (name == "-" || name == "NULL" || name.size() == 0)
	{
		QMessageBox msgBox;

		msgBox.setText(Utility::Korean("이름을 입력해야 합니다.   "));
		msgBox.setWindowTitle("Notice");
		msgBox.exec();

		return;
	}

	bool isUnique = true;
	QTableWidget* currentWidget = (ui.comboBox->currentText() == "Buyer") ? tableWidgetBuyer : tableWidgetSeller;

	for (int row = 0; row < currentWidget->rowCount(); row++)
	{
		if (userID == currentWidget->item(row, 0)->text().toStdString())
		{
			isUnique = false;
			break;
		}
	}

	if (isUnique == false)
	{
		QMessageBox msgBox;

		msgBox.setText(Utility::Korean("ID는 고유한 값이어야 합니다.   "));
		msgBox.setWindowTitle("Notice");
		msgBox.exec();

		return;
	}

	name = "\'" + ui.lineEditName->text().toStdString() + "\'";
	phone = "\'" + ui.lineEditPhone->text().toStdString() + "\'";
	address = "\'" + ui.lineEditAddress->text().toStdString() + "\'";

	done(QDialog::Accepted);
}