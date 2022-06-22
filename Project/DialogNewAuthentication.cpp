#include "DialogNewAuthentication.h"

DialogNewAuthentication::DialogNewAuthentication(QWidget* parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void DialogNewAuthentication::on_buttonBox_accepted()
{
	serverName = ui.lineEdit1Server->text();
	databaseName = ui.lineEdit2Database->text();
	userName = ui.lineEdit3User->text();
	password = ui.lineEdit4Password->text();
}