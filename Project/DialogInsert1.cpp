#include "DialogInsert1.h"


DialogInsert1::DialogInsert1(QWidget* parent)
{
	ui.setupUi(this);

	comboBoxCPU = ui.comboBoxCPU;
	comboBoxRAM = ui.comboBoxRAM;
	comboBoxVGA = ui.comboBoxVGA;
	comboBoxPSU = ui.comboBoxPSU;
	comboBoxSSD = ui.comboBoxSSD;
	comboBoxHDD = ui.comboBoxHDD;

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &DialogInsert1::CheckID);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}


void DialogInsert1::ComputerPriceCalculate()
{
	int computerPrice = 0;

	computerPrice += ui.lineEditPriceCPU->text().toInt();
	computerPrice += ui.lineEditPriceRAM->text().toInt();
	computerPrice += ui.lineEditPriceVGA->text().toInt();
	computerPrice += ui.lineEditPricePSU->text().toInt();
	computerPrice += ui.lineEditPriceSSD->text().toInt();
	computerPrice += ui.lineEditPriceHDD->text().toInt();

	ui.lineEditComputerPrice->setText(QString::number(computerPrice));
}


void DialogInsert1::on_comboBoxPurpose_currentTextChanged(QString text)
{
	if (text == "Office")
	{
		comboBoxVGA->setCurrentIndex(0);
		ui.lineEditPriceVGA->setText(0);
		ui.comboBoxVGA->setEnabled(false);
	}
	else if (text == "Gaming" || text == "Workstation")
		ui.comboBoxVGA->setEnabled(true);

	ui.groupBoxParts->setEnabled(true);

	ui.lineEditPriceCPU->setText(Utility::Convert(listCPU->Records()[0][2]));
	ui.lineEditPriceRAM->setText(Utility::Convert(listRAM->Records()[0][2]));
	ui.lineEditPricePSU->setText(Utility::Convert(listPSU->Records()[0][2]));
	ui.lineEditPriceSSD->setText(Utility::Convert(listSSD->Records()[0][2]));
	ui.lineEditPriceHDD->setText(Utility::Convert(listHDD->Records()[0][2]));

	ComputerPriceCalculate();
}


void DialogInsert1::on_comboBoxCPU_currentIndexChanged(int index)
{
	if (listCPU != nullptr)
	{
		ui.lineEditPriceCPU->setText(Utility::Convert(listCPU->Records()[index][2]));
		ComputerPriceCalculate();
	}
}


void DialogInsert1::on_comboBoxRAM_currentIndexChanged(int index)
{
	if (listRAM != nullptr)
	{
		ui.lineEditPriceRAM->setText(Utility::Convert(listRAM->Records()[index][2]));
		ComputerPriceCalculate();
	}
}


void DialogInsert1::on_comboBoxVGA_currentIndexChanged(int index)
{
	if (listVGA != nullptr)
	{
		if (index == 0)
			ui.lineEditPriceVGA->setText(0);
		else
			ui.lineEditPriceVGA->setText(Utility::Convert(listVGA->Records()[index - 1][2]));

		ComputerPriceCalculate();
	}
}


void DialogInsert1::on_comboBoxPSU_currentIndexChanged(int index)
{
	if (listPSU != nullptr)
	{
		ui.lineEditPricePSU->setText(Utility::Convert(listPSU->Records()[index][2]));
		ComputerPriceCalculate();
	}

}


void DialogInsert1::on_comboBoxSSD_currentIndexChanged(int index)
{
	if (listSSD != nullptr)
	{
		int totalPrice = stoi(listSSD->Records()[index][2]) * ui.spinBoxSSD->value();
		ui.lineEditPriceSSD->setText(QString::number(totalPrice));
		ComputerPriceCalculate();
	}
}


void DialogInsert1::on_comboBoxHDD_currentIndexChanged(int index)
{
	if (listHDD != nullptr)
	{
		int totalPrice = stoi(listHDD->Records()[index][2]) * ui.spinBoxHDD->value();
		ui.lineEditPriceHDD->setText(QString::number(totalPrice));
		ComputerPriceCalculate();
	}
}


void DialogInsert1::on_spinBoxSSD_valueChanged(int quantity)
{
	int totalPrice = stoi(listSSD->Records()[comboBoxSSD->currentIndex()][2]) * quantity;
	ui.lineEditPriceSSD->setText(QString::number(totalPrice));
	ComputerPriceCalculate();
}


void DialogInsert1::on_spinBoxHDD_valueChanged(int quantity)
{
	int totalPrice = stoi(listHDD->Records()[comboBoxHDD->currentIndex()][2]) * quantity;
	ui.lineEditPriceHDD->setText(QString::number(totalPrice));
	ComputerPriceCalculate();
}


void DialogInsert1::on_toolButtonRandom_clicked()
{
	ui.lineEditComputerID->setText(QString::number(Random::Integer(1000, 9999)));
}


void DialogInsert1::CheckID()
{
	bool isUnique = true;

	for (int row = 0; row < listComputer->Records().size(); row++)
	{
		if (listComputer->Records()[row][0] == ui.lineEditComputerID->text().toStdString())
		{
			isUnique = false;
			break;
		}
	}

	if (isUnique == false)
	{
		QMessageBox msgBox;

		msgBox.setText(Utility::Korean("컴퓨터 ID는 고유한 값이어야 합니다.   "));
		msgBox.setWindowTitle("Notice");
		msgBox.exec();

		return;
	}

	if (listCPU->Records()[ui.comboBoxCPU->currentIndex()][3] == "-" && ui.comboBoxVGA->currentIndex() == 0)
	{
		QMessageBox msgBox;

		msgBox.setText(Utility::Korean("내장그래픽 칩셋이 포함된 CPU를 선택하거나, 외장 그래픽카드를 추가하십시오.   "));
		msgBox.setWindowTitle("Notice");
		msgBox.exec();

		return;
	}

	if (ui.comboBoxVGA->currentIndex() > 0)
		vgaID = listVGA->Records()[ui.comboBoxVGA->currentIndex() - 1][0];
	else
		vgaID = "NULL";

	computerID = ui.lineEditComputerID->text().toStdString();
	purpose = ui.comboBoxPurpose->currentText().toStdString();
	computerPrice = stoi(ui.lineEditComputerPrice->text().toStdString());
	cpuID = listCPU->Records()[ui.comboBoxCPU->currentIndex()][0];
	ramID = listRAM->Records()[ui.comboBoxRAM->currentIndex()][0];
	psuID = listPSU->Records()[ui.comboBoxPSU->currentIndex()][0];
	ssdID = listSSD->Records()[ui.comboBoxSSD->currentIndex()][0];
	hddID = listHDD->Records()[ui.comboBoxHDD->currentIndex()][0];
	ssdQuantity = ui.spinBoxSSD->value();
	hddQuantity = ui.spinBoxHDD->value();

	done(QDialog::Accepted);
}