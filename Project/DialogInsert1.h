#pragma once
#include <QDialog>
#include <QString>
#include <QMessageBox>
#include "ui_DialogInsert1.h"
#include "Random.h"
#include "QueryResult.h"
#include "Utility.h"


class DialogInsert1 : public QDialog
{
	Q_OBJECT

public:

	DialogInsert1(QWidget* parent = Q_NULLPTR);

	QString target;
	QComboBox* comboBoxCPU;
	QComboBox* comboBoxRAM;
	QComboBox* comboBoxVGA;
	QComboBox* comboBoxPSU;
	QComboBox* comboBoxSSD;
	QComboBox* comboBoxHDD;
	QueryResult* listComputer = nullptr;
	QueryResult* listCPU = nullptr;
	QueryResult* listRAM = nullptr;
	QueryResult* listVGA = nullptr;
	QueryResult* listPSU = nullptr;
	QueryResult* listSSD = nullptr;
	QueryResult* listHDD = nullptr;
	std::string computerID;
	std::string purpose;
	int computerPrice;
	std::string cpuID;
	std::string ramID;
	std::string vgaID;
	std::string psuID;
	std::string ssdID;
	std::string hddID;
	int ssdQuantity;
	int hddQuantity;


private:

	Ui::DialogInsert1 ui;

	void ComputerPriceCalculate();


private slots:

	void on_comboBoxPurpose_currentTextChanged(QString text);
	void on_comboBoxCPU_currentIndexChanged(int index);
	void on_comboBoxRAM_currentIndexChanged(int index);
	void on_comboBoxVGA_currentIndexChanged(int index);
	void on_comboBoxPSU_currentIndexChanged(int index);
	void on_comboBoxSSD_currentIndexChanged(int index);
	void on_comboBoxHDD_currentIndexChanged(int index);
	void on_spinBoxSSD_valueChanged(int quantity);
	void on_spinBoxHDD_valueChanged(int quantity);
	void on_toolButtonRandom_clicked();
	void CheckID();
};

