#pragma once
#include <QDialog>
#include <QTableWidget>
#include <QString>
#include "ui_DialogDelete.h"


class DialogDelete : public QDialog
{
	Q_OBJECT

public:

	DialogDelete(QWidget* parent = Q_NULLPTR);

	QTableWidget* tableWidget;
	QString target;


private:

	Ui::DialogDelete ui;


private slots:

	void on_tableWidget_currentCellChanged(int row);
	void on_buttonBox_accepted();
};

