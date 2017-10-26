#include "ParamDockWindow.h"

#include "Gui\Document.h"
#include "Base\Parameter.h"
#include "App\Application.h"
#include "qaction.h"
#include "qobject.h"
#include "qmessagebox.h"

using namespace Gui;

ParamDockWindow::ParamDockWindow(Gui::Document* pcDocument, QWidget *parent)
: DockWindow(pcDocument,parent)
{
	paramPage = new BallCutterDialog(this);
	
	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(paramPage);
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
}

ParamDockWindow::ParamDockWindow(QWidget *parent /*= 0*/)
{
	paramPage = new BallCutterDialog(this);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(paramPage);
	
}

ParamDockWindow::~ParamDockWindow(void)
{
}

#include "GeneratedFiles/moc_ParamDockWindow.cpp"