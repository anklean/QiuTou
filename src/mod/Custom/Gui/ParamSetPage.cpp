#include "ParamSetPage.h"

ParamSetPage::ParamSetPage(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(onCancle()));
}

ParamSetPage::~ParamSetPage()
{

}

void ParamSetPage::onOK()
{
	this->accept();
}

void ParamSetPage::onCancle()
{
	this->reject();
}

#include "GeneratedFiles/moc_ParamSetPage.cpp"