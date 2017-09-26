#pragma once
#include "PreCompiled.h"
#include "Gui\dockwindow.h"
#include "Gui\Document.h"
#include "CreateBallCutter.h"

class ParamDockWindow : public Gui::DockWindow
{
	Q_OBJECT
public:
	ParamDockWindow(Gui::Document*  pcDocument, QWidget *parent);
	ParamDockWindow(QWidget *parent = 0);
	~ParamDockWindow(void);

private:
	CreateBallCutter* paramPage;
};