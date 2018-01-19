/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *   for detail see the LICENCE text file.                                 *
 *   Juergen Riegel 2002                                                   *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"

#include <Base/Console.h>
#include <Base/Interpreter.h>

#include <Gui/Application.h>
#include <Gui/BitmapFactory.h>
#include <Gui/WidgetFactory.h>


#include "Workbench.h"

#include <Gui/Language/Translator.h>
#include "ViewProviderCustomPath.h"
#include "Gui/DockWindowManager.h"
#include "Gui/MainWindow.h"
#include "ViewProviderNC.h"
#include "ViewProviderBallCutter.h"


// use a different name to CreateCommand()
void CreateCustomCommands(void);

void loadCustomResource()
{
    // add resources and reloads the translators
    Q_INIT_RESOURCE(Custom);
    Gui::Translator::instance()->refresh();
}

/* registration table  */
static struct PyMethodDef CustomGui_methods[] = {
    {NULL, NULL}                   /* end of table marker */
};

extern "C" {
void CustomGuiExport initCustomGui()
{
    if (!Gui::Application::Instance) {
        PyErr_SetString(PyExc_ImportError, "Cannot load Gui module in console application.");
        return;
    }

    // load needed modules
    try {
        Base::Interpreter().runString("import Custom");
		Base::Interpreter().runString("import PartGui");
    }
    catch(const Base::Exception& e) {
        PyErr_SetString(PyExc_ImportError, e.what());
        return;
    }

    (void) Py_InitModule("CustomGui", CustomGui_methods);   /* mod name, table ptr */
    Base::Console().Log("Loading GUI of Custom module... done\n");

    CustomGui::Workbench                      ::init();
	CustomGui::ViewProviderCustomPath::init();
	CustomGui::ViewProviderNC::init();
	CustomGui::ViewProviderBallCutter::init();

    // instantiating the commands
    CreateCustomCommands();

    // add resources and reloads the translators
    loadCustomResource();

// 
// 	Gui::DockWindowManager* pDockMgr = Gui::DockWindowManager::instance();
// 	Gui::MainWindow* pMainWindow = Gui::getMainWindow();
// 	// Param view
// 	ParamDockWindow* pcParamView = new ParamDockWindow(0,pMainWindow);
// 	pcParamView->setObjectName(QString::fromAscii(QT_TRANSLATE_NOOP("QDockWidget", "Param View")));
// 	pcParamView->setMinimumWidth(200);
// 	pDockMgr->registerDockWindow("Std_ParamView", pcParamView);

}
} // extern "C"
