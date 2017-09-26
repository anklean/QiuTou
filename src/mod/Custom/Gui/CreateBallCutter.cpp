#include "CreateBallCutter.h"

CreateBallCutter::CreateBallCutter(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(onCancle()));
}

CreateBallCutter::~CreateBallCutter()
{

}

void CreateBallCutter::onOK()
{
	//this->accept();
}

void CreateBallCutter::onCancle()
{
	//this->reject();
}

#include "GeneratedFiles/moc_CreateBallCutter.cpp"