/***************************************************************************
*   Copyright (c) 2005 Werner Mayer <wmayer[at]users.sourceforge.net>     *
*                                                                         *
*   This file is Custom of the FreeCAD CAx development system.              *
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License as published by the Free Software Foundation; either          *
*   version 2 of the License, or (at your option) any later version.      *
*                                                                         *
*   This library  is distributed in the hope that it will be useful,      *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A CustomICULAR PURPOSE.  See the         *
*   GNU Library General Public License for more details.                  *
*                                                                         *
*   You should have received a copy of the GNU Library General Public     *
*   License along with this library; see the file COPYING.LIB. If not,    *
*   write to the Free Software Foundation, Inc., 59 Temple Place,         *
*   Suite 330, Boston, MA  02111-1307, USA                                *
*                                                                         *
***************************************************************************/


#include "PreCompiled.h"

#ifndef _PreComp_
# include <qobject.h>
#endif

#include "Workbench.h"
#include <Gui/MenuManager.h>
#include <Gui/ToolBarManager.h>
#include "Gui/DockWindowManager.h"

using namespace CustomGui;

#if 0 // needed for Qt's lupdate utility
qApp->translate("Workbench", "&Custom");
qApp->translate("Workbench", "&Simple");
qApp->translate("Workbench", "&Parametric");
qApp->translate("Workbench", "Solids");
qApp->translate("Workbench", "Custom tools");
qApp->translate("Workbench", "Boolean");
#endif

/// @namespace CustomGui @class Workbench
TYPESYSTEM_SOURCE(CustomGui::Workbench, Gui::StdWorkbench)

Workbench::Workbench()
{
}

Workbench::~Workbench()
{
}

Gui::MenuItem* Workbench::setupMenuBar() const
{
	// Setup the default menu bar
	Gui::MenuItem* menuBar = new Gui::MenuItem;

	// File
	Gui::MenuItem* file = new Gui::MenuItem(menuBar);
	file->setCommand("&File");
	*file << "Std_New" << "Std_Open" << "Separator" << "Std_CloseActiveWindow"
		<< "Std_CloseAllWindows" << "Separator" << "Std_Save" /*<< "Std_SaveAs"
		<< "Std_Revert" << "Separator" << "Std_Import" << "Std_Export"
		<< "Std_MergeProjects" << "Std_ProjectInfo"
		<< "Separator" << "Std_Print" << "Std_PrintPreview" << "Std_PrintPdf"
		<< "Separator" << "Std_RecentFiles"*/ << "Separator" << "Std_Quit";

	// Edit
	/*Gui::MenuItem* edit = new Gui::MenuItem(menuBar);
	edit->setCommand("&Edit");
	*edit << "Std_Undo" << "Std_Redo" << "Separator" << "Std_Cut" << "Std_Copy"
		<< "Std_Paste" << "Std_DuplicateSelection" << "Separator"
		<< "Std_Refresh" << "Std_BoxSelection" << "Std_SelectAll" << "Std_Delete"
		<< "Separator" << "Std_Placement" << "Std_TransformManip"<< "Std_Alignment"
		<< "Std_Edit" << "Separator" << "Std_DlgPreferences";*/ 

	// Standard views
	Gui::MenuItem* stdviews = new Gui::MenuItem;
	stdviews->setCommand("Standard views");
	*stdviews << "Std_ViewFitAll" << "Std_ViewFitSelection" /*<< "Std_DrawStyle" */<< "Std_ViewAxo"
		<< "Separator" << "Std_ViewFront" << "Std_ViewTop"
		<< "Std_ViewRight" << "Separator" << "Std_ViewRear"
		<< "Std_ViewBottom" << "Std_ViewLeft"
		<< "Separator" << "Std_ViewRotateLeft" << "Std_ViewRotateRight";

	// zoom
	Gui::MenuItem* zoom = new Gui::MenuItem;
	zoom->setCommand("&Zoom");
	*zoom << "Std_ViewZoomIn" << "Std_ViewZoomOut" << "Separator" << "Std_ViewBoxZoom";

	// View
	Gui::MenuItem* view = new Gui::MenuItem(menuBar);
	view->setCommand("&View");
	*view /*<< "Std_ViewCreate" << "Std_OrthographicCamera" << "Std_PerspectiveCamera" << "Separator"*/
		<< stdviews /*<< "Std_FreezeViews" << "Std_DrawStyle" << "Separator"*/ /*<< view3d*/ << zoom
		/*<< "Std_ViewDockUndockFullscreen" << "Std_AxisCross" << "Std_ToggleClipPlane"
		<< "Std_TextureMapping"*/
#ifdef BUILD_VR
		<< "Std_ViewVR"
#endif 
		/*<< "Separator" << visu
		<< "Std_ToggleVisibility" << "Std_ToggleNavigation"*/
		/*<< "Std_SetAppearance" << "Std_RandomColor"*/ << "Separator"
		/*<< "Std_Workbench" << "Std_ToolBarMenu" << "Std_DockViewMenu"*/ << "Separator"
		<< "Std_ViewStatusBar";

	// Windows
	Gui::MenuItem* wnd = new Gui::MenuItem(menuBar);
	wnd->setCommand("&Windows");
	*wnd << "Std_ActivateNextWindow" << "Std_ActivatePrevWindow" << "Separator"
		/*<< "Std_TileWindows" << "Std_CascadeWindows"*/
		<< "Std_ArrangeIcons" << "Separator" << "Std_WindowsMenu" << "Std_Windows";

	// Separator
	Gui::MenuItem* sep = new Gui::MenuItem(menuBar);
	sep->setCommand("Separator");

	// Help
	Gui::MenuItem* help = new Gui::MenuItem(menuBar);
	help->setCommand("&Help");
	*help /*<< "Std_OnlineHelp" << "Std_FreeCADWebsite"
		<< "Std_FreeCADUserHub" << "Std_FreeCADPowerUserHub"
		<< "Std_PythonHelp" << "Std_FreeCADForum"
		<< "Std_FreeCADFAQ"*/ << "Std_About" /*<< "Std_WhatsThis"*/;

	return menuBar;

}

Gui::ToolBarItem* Workbench::setupToolBars() const
{
	Gui::ToolBarItem* root = new Gui::ToolBarItem;
	//Gui::ToolBarItem* root = StdWorkbench::setupToolBars();

	/*Gui::ToolBarItem* solids = new Gui::ToolBarItem(root);
	solids->setCommand("Solids");
	*solids << "Custom_Box" << "Custom_Cylinder" << "Custom_Sphere" << "Custom_Cone"
	<< "Custom_Torus" << "Custom_Primitives" << "Custom_Builder";

	Gui::ToolBarItem* tool = new Gui::ToolBarItem(root);
	tool->setCommand("Custom tools");
	*tool << "Custom_Extrude" << "Custom_Revolve" << "Custom_Mirror" << "Custom_Fillet"
	<< "Custom_Chamfer" << "Custom_RuledSurface" << "Custom_Loft" << "Custom_Sweep"
	<< "Custom_Offset" << "Custom_Thickness";

	Gui::ToolBarItem* boolop = new Gui::ToolBarItem(root);
	boolop->setCommand("Boolean");
	*boolop << "Custom_Boolean" << "Custom_Cut" << "Custom_Fuse" << "Custom_Common"
	<< "Custom_CompJoinFeatures" << "Custom_CheckGeometry" << "Custom_Section"
	<< "Custom_CrossSections";

	Gui::ToolBarItem* measure = new Gui::ToolBarItem(root);
	measure->setCommand("Measure");
	*measure << "Custom_Measure_Linear" << "Custom_Measure_Angular"  << "Separator" << "Custom_Measure_Clear_All" << "Custom_Measure_Toggle_All"
	<< "Custom_Measure_Toggle_3d" << "Custom_Measure_Toggle_Delta";*/

	Gui::ToolBarItem* model = new Gui::ToolBarItem(root);
	model->setCommand("NC");
	*model << "Part_Import" << "Custom_CreateBallCutter"/*;

	Gui::ToolBarItem* NC = new Gui::ToolBarItem(root);
	NC->setCommand("NC");
	*NC */<< "Custom_CreateTools"/*<< "Custom_BuildPath"*/<< "Custom_BuildNCFile"
		/*<< "Separator"	<< "Custom_StartPlayPath" */;

	Gui::ToolBarItem* view = new Gui::ToolBarItem(root);
	view->setCommand("view");
	*view << "Std_ViewFitAll" << "Std_ViewFitSelection"/* << "Std_DrawStyle" */<< "Std_ViewAxo"
		<< "Separator" << "Std_ViewFront" << "Std_ViewTop"
		<< "Std_ViewRight" << "Separator" << "Std_ViewRear"
		<< "Std_ViewBottom" << "Std_ViewLeft"
		<< "Separator" << "Std_ViewRotateLeft" << "Std_ViewRotateRight" << "Std_ViewZoomIn" << "Std_ViewZoomOut";
	return root;
}

Gui::ToolBarItem* Workbench::setupCommandBars() const
{
	// Custom tools
	Gui::ToolBarItem* root = new Gui::ToolBarItem;
	return root;
}

Gui::DockWindowItems* Workbench::setupDockWindows() const
{
	Gui::DockWindowItems* root = new Gui::DockWindowItems();
	//root->addDockWidget("Std_ToolBox", Qt::RightDockWidgetArea, true, false);
	root->addDockWidget("Std_ParamView", Qt::RightDockWidgetArea, true, true);
	//root->addDockWidget("Std_HelpView", Qt::RightDockWidgetArea, true, false);
	root->addDockWidget("Std_TreeView", Qt::LeftDockWidgetArea, true, false);
	root->addDockWidget("Std_PropertyView", Qt::LeftDockWidgetArea, true, false);
	//root->addDockWidget("Std_SelectionView", Qt::LeftDockWidgetArea, false, false);
	root->addDockWidget("Std_CombiView", Qt::LeftDockWidgetArea, false, false);
	//root->addDockWidget("Std_ReportView", Qt::BottomDockWidgetArea, true, true);
	root->addDockWidget("Std_PythonView", Qt::BottomDockWidgetArea, false, true);
	return root;
}