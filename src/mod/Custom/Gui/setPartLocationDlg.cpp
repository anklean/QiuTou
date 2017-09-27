#include "SetPartLocationDlg.h"

SetPartLocationDlg::SetPartLocationDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(onCancle()));
}

SetPartLocationDlg::~SetPartLocationDlg()
{

}

void SetPartLocationDlg::onOK()
{
	this->accept();
}

void SetPartLocationDlg::onCancle()
{
	this->reject();
}

Base::Vector3d SetPartLocationDlg::getLocation()
{
	double dx = ui.doubleSpinBox->value();
	double dy = ui.doubleSpinBox_2->value();
	double dz = ui.doubleSpinBox_3->value();

	Base::Vector3d pos(dx, dy, dz);
	return pos;
}

#include "GeneratedFiles/moc_SetPartLocationDlg.cpp"