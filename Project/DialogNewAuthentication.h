#pragma once
#include <QDialog>
#include "ui_DialogNewAuthentication.h"


class DialogNewAuthentication : public QDialog
{
	Q_OBJECT

public:

	DialogNewAuthentication(QWidget* parent = Q_NULLPTR);

	QString serverName;
	QString databaseName;
	QString userName;
	QString password;


private:

	Ui::DialogNewAuthentication ui;


private slots:

	void on_buttonBox_accepted();
};

