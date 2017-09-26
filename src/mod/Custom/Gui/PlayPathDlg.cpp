#include "PlayPathDlg.h"

PlayPathDlg::PlayPathDlg(QWidget *parent)
	: QDialog(parent)
{
	pViewProvider = NULL;
	ui.setupUi(this);
	connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(onPlay()));
	connect(ui.toolButton_2, SIGNAL(clicked()), this, SLOT(onSpeedUp()));
	connect(ui.toolButton_3, SIGNAL(clicked()), this, SLOT(onPause()));
	connect(ui.toolButton_4, SIGNAL(clicked()), this, SLOT(onStop()));
}

PlayPathDlg::~PlayPathDlg()
{

}

void PlayPathDlg::onPlay()
{
	if (pViewProvider)
	{
		pViewProvider->StartCutter();
	}
}

void PlayPathDlg::onSpeedUp()
{
	if (pViewProvider)
	{
		pViewProvider->StartCutter();
	}
}

void PlayPathDlg::onPause()
{

}

void PlayPathDlg::onStop()
{
	if (pViewProvider)
	{
		pViewProvider->stopCutter();
	}
}

void PlayPathDlg::setCurrentViewProvider(CustomGui::ViewProviderCustomPath* vp)
{
	pViewProvider = vp;
}

#include "GeneratedFiles/moc_PlayPathDlg.cpp"