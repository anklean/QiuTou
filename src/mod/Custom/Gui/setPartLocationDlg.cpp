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

#include "GeneratedFiles/moc_SetPartLocationDlg.cpp"