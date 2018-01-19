/***************************************************************************
*   Copyright (c) J黵gen Riegel          (juergen.riegel@web.de) 2002     *
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

#include <Base/Console.h>
#include <Base/Exception.h>
#include <App/Document.h>
#include <App/DocumentObjectGroup.h>
#include <Gui/Action.h>
#include <Gui/Application.h>
#include <Gui/BitmapFactory.h>
#include <Gui/Command.h>
#include <Gui/Control.h>
#include <Gui/Document.h>
#include <Gui/FileDialog.h>
#include <Gui/MainWindow.h>
#include <Gui/Selection.h>
#include <Gui/View3DInventor.h>
#include <Gui/View3DInventorViewer.h>
#include <Gui/WaitCursor.h>
#include <QApplication>

#include "App/Application.h"
#include "Gui/MainWindow.h"
#include "ParamSetPage.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include "Standard_TypeDef.hxx"
#include "STEPControl_Reader.hxx"
#include "TopoDS_Shape.hxx"
#include "TopoDS_Face.hxx"
#include "TopExp_Explorer.hxx"
#include "BRepAdaptor_Surface.hxx"
#include "GeomAbs_SurfaceType.hxx"
#include "TopTools_IndexedMapOfShape.hxx"
#include "TopoDS_Edge.hxx"
#include "../App/BRepOffset_OffsetEdgeOnFace.h"
#include "TopExp.hxx"
#include "TopoDS.hxx"
#include "../App/PartFeature.h"
#include "../App/TopoShape.h"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "STEPControl_Writer.hxx"
#include "ViewProviderCustomPath.h"
#include "GrindingWheelDlg.h"
#include "CreateToolsDlg.h"
#include "CreatePathDlg.h"
#include "CreateNCfileDlg.h"
#include "PlayPathDlg.h"
#include <QMessageBox>
#include "Base/Tools.h"
#include "BallCutterDialog.h"
#include "setPartLocationDlg.h"

#include <Gui/Utilities.h>
#include <Gui/Control.h>
//#include "Placement.h"


//===========================================================================
// Custom_CreateBallCutter
//===========================================================================
DEF_STD_CMD_A(CmdCustomCreateBallCutter);

CmdCustomCreateBallCutter::CmdCustomCreateBallCutter()
	:Command("Custom_CreateBallCutter")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = QT_TR_NOOP("qiu tou fen qu");
	sToolTipText = QT_TR_NOOP("Create an qiu tou fen qu");
	sWhatsThis = "Custom_NewDoc";
	sStatusTip = sToolTipText;
	sPixmap = "qiutoufenqu";
}

void CmdCustomCreateBallCutter::activated(int iMsg)
{
	Gui::TaskView::TaskDialog* dlg = Gui::Control().activeDialog();
	if (!dlg)
	{
		dlg = new CustomGui::TaskBallCutter();
	}
	Gui::Control().showDialog(dlg);
}

bool CmdCustomCreateBallCutter::isActive(void)
{
	return (hasActiveDocument() && !Gui::Control().activeDialog());
}


//===========================================================================
// Custom_SetModelPosition
//===========================================================================
DEF_STD_CMD_A(CmdCustom_SetModelPosition);

CmdCustom_SetModelPosition::CmdCustom_SetModelPosition()
:Command("Custom_SetModelPosition")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = QT_TR_NOOP("part Position");
	sToolTipText = QT_TR_NOOP("set the model's position");
	sWhatsThis = "Custom_SetModelPostion";
	sStatusTip = sToolTipText;
	sPixmap = "position";
}

void CmdCustom_SetModelPosition::activated(int iMsg)
{
	Gui::MainWindow* pMainWindow = Gui::getMainWindow();
	SetPartLocationDlg* dlg = new SetPartLocationDlg(pMainWindow);

	if (dlg->exec() == QDialog::Accepted)
	{
		Base::Vector3d pos = dlg->getLocation();

		App::Document* pDoc = App::GetApplication().getActiveDocument();
		std::vector<App::DocumentObject*> objList = pDoc->getObjectsOfType(Part::Feature::getClassTypeId());

		std::vector<App::DocumentObject*>::iterator it;
		for (it = objList.begin(); it != objList.end(); ++it)
		{
			App::DocumentObject* pObj = (*it);
			if (pObj && pObj->isDerivedFrom(Part::Feature::getClassTypeId()))
			{
				Part::Feature* featureObj = dynamic_cast<Part::Feature*>(pObj);

				Base::Rotation rotation(Base::Vector3d(0, 1, 0), -M_PI / 2);
				Base::Placement place;
				place.setPosition(pos);
				place.setRotation(rotation);
				featureObj->Placement.setValue(place);
			}
		}
	}
	doCommand(Command::Gui, "Gui.SendMsgToActiveView(\"ViewFit\")");
	doCommand(Command::Gui, "Gui.activeDocument().activeView().viewFront()");
}

bool CmdCustom_SetModelPosition::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}

//===========================================================================
// Custom_SetCutterParameter
//===========================================================================
DEF_STD_CMD_A(CmdCustomSetCutterParameter);

CmdCustomSetCutterParameter::CmdCustomSetCutterParameter()
	:Command("Custom_SetCutterParameter")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = "set Parameter";
	sToolTipText = "Set the Parameters of the cutter";
	sWhatsThis = "Custom_SetCutterParameter";
	sStatusTip = sToolTipText;
	sPixmap = "Path-LoadTool";
}

void CmdCustomSetCutterParameter::activated(int iMsg)
{
	Gui::MainWindow* pMainWindow = Gui::getMainWindow();
	ParamSetPage* page = new ParamSetPage(pMainWindow);

	if (page->exec() == QDialog::Accepted)
	{

	}
}

bool CmdCustomSetCutterParameter::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}


//===========================================================================
// Custom_SetCutterParameter
//===========================================================================
DEF_STD_CMD_A(CmdCustomSetBlankPart);

CmdCustomSetBlankPart::CmdCustomSetBlankPart()
	:Command("Custom_SetBlankPart")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = "设置毛坯料";
	sToolTipText = "Set the Parameters of the cutter";
	sWhatsThis = "Custom_SetCutterParameter";
	sStatusTip = sToolTipText;
	sPixmap = "Path-LoadTool";
}

void CmdCustomSetBlankPart::activated(int iMsg)
{
	Gui::MainWindow* pMainWindow = Gui::getMainWindow();
	ParamSetPage* page = new ParamSetPage(pMainWindow);

	if (page->exec() == QDialog::Accepted)
	{

	}
}

bool CmdCustomSetBlankPart::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}

//===========================================================================
// Custom_BuildPath
//===========================================================================
DEF_STD_CMD_A(CmdCustomBuildPath);

CmdCustomBuildPath::CmdCustomBuildPath()
	:Command("Custom_BuildPath")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = "切削圆柱形区域";
	sToolTipText = "build the path of cutter";
	sWhatsThis = "Custom_BuildPath";
	sStatusTip = sToolTipText;
	sPixmap = "Path-Toolpath";
}

void CmdCustomBuildPath::activated(int iMsg)
{
	Gui::MainWindow* pMainWindow = Gui::getMainWindow();

	CreatePathData data;
	QStringList lstFaces;
	QStringList lstCurves;

	
	std::vector<Gui::SelectionObject>  selobjs = Gui::Selection().getSelectionEx();
	for (std::vector<Gui::SelectionObject>::iterator it = selobjs.begin(); it != selobjs.end(); it++)
	{
		std::string ss = (*it).getAsPropertyLinkSubString();

		std::string buf1 = "", buf2 = "";
		buf1 += "(App.ActiveDocument.";
		buf1 += (*it).getFeatName();
		buf1 += ",[\"";
		buf2 += "\"])";
		std::vector<std::string> names = (*it).getSubNames();
		for (int i = 0; i < names.size(); i++)
		{
			std::string s = names[i];
			if (s.find("Face") != s.npos)
			{
				s = buf1 + s + buf2;
				lstFaces.push_back(Base::Tools::fromStdString(s));
			}
			else if (s.find("Edge") != s.npos)
			{
				s = buf1 + s + buf2;
				lstCurves.push_back(Base::Tools::fromStdString(s));
			}
		}	
	}
	data.setFaces(lstFaces);
	data.setCurves(lstCurves);
	data.setTolerance(0.01);
	//data.setEdgeDirection(QString::fromAscii("V"));
	data.setToolName(QString::fromAscii("1"));

	CreatePathDlg* dlg = new CreatePathDlg(pMainWindow);
	dlg->setData(data);
	if (dlg->exec() == QDialog::Accepted)
	{
		CreatePathData data = dlg->getData();
			
			QString direction;
			QStringList faces =data.getFaces();
			QStringList curves = data.getCurves();
			if (faces.size() > 0 && curves.size() > 0)
			{
				QString str = QString::fromAscii(
					"o=App.ActiveDocument.addObject('Custom::PathObject','Path')\n"	
					"o.ToolSetNumber='%3'\n"
					"o.BeforeAdditionGCode='%4'\n"
					"o.AfterAdditionGCode='%5'\n"
					"o.Tolerance=%6\n"
					"o.Face=%1\n"
					"o.Curve=%2\n"
					)
					.arg(faces[0])
					.arg(curves[0])
					//.arg(data.getEdgeDirection())
					.arg(data.getToolName())
					.arg(data.getBeforeG())
					.arg(data.getAfterG())
					.arg(data.getTolerance())

					;
				try {
					if (!str.isEmpty())
					{
						openCommand("Create Path");
						doCommand(Doc, (const char*)str.toAscii());
						commitCommand();
					}
				}
				catch (const Base::Exception& e) {
					//Base::Console().Error("Cannot convert %s because %s.\n",
					//	(*it).getObject()->Label.getValue(), e.what());
				}
				;
			}
			else
			{
				QMessageBox::warning(0, QString::fromLocal8Bit("Warning"), QString::fromLocal8Bit("请先选中轨迹线和切削面"));
			}
			
		
	}
#if 0
	App::Document* pDoc = App::GetApplication().getActiveDocument();
	App::DocumentObject* pObj = pDoc->getObject("Ball");
	Part::Feature* pFea = (Part::Feature*)pObj;

	Part::TopoShape topShape = pFea->Shape.getShape();


	TopoDS_Face f;
	TopExp_Explorer Ex;
	std::vector<TopoDS_Shape> elist;
	int kkk = 0;
	for (Ex.Init(topShape._Shape, TopAbs_FACE); Ex.More(); Ex.Next()) {
		f = TopoDS::Face(Ex.Current());
		BRepAdaptor_Surface ad(f);
		//	try 
		{
			if (ad.GetType() == GeomAbs_SurfaceType::GeomAbs_Cylinder ||
				ad.GetType() == GeomAbs_SurfaceType::GeomAbs_Cone)
			{
				Standard_Real su = ad.FirstUParameter();
				Standard_Real sv = ad.FirstVParameter();
				Standard_Real eu = ad.LastUParameter();
				Standard_Real ev = ad.LastVParameter();

				Standard_Real mu = (eu - su) / 50.0;
				Standard_Real mv = (ev - sv) / 50.0;
				for (int i = 0; i < 50.0; i++)
				{
					BRepBuilderAPI_MakePolygon mkk;
					for (int j = 0; j < 50; j++)
					{
						gp_Pnt ppnt;
						ad.D0(su + mu*j, sv + mv*i, ppnt);
						mkk.Add(ppnt);
					}
					elist.push_back(mkk.Wire());
				}

			}
		}
	}
#endif

}

bool CmdCustomBuildPath::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}


//== == == == == == == == == == == == ==
// Custom_BuildPath
//===========================================================================
DEF_STD_CMD_A(CmdCustomBuildPath2);

CmdCustomBuildPath2::CmdCustomBuildPath2()
	:Command("Custom_BuildPath2")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = "切削球头面";
	sToolTipText = "build the path of cutter";
	sWhatsThis = "Custom_BuildPath";
	sStatusTip = sToolTipText;
	sPixmap = "Path-Toolpath";
}

void CmdCustomBuildPath2::activated(int iMsg)
{
}

bool CmdCustomBuildPath2::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}


//== == == == == == == == == == == == ==
// Custom_BuildPath
//===========================================================================
DEF_STD_CMD_A(CmdCustomBuildPath3);

CmdCustomBuildPath3::CmdCustomBuildPath3()
	:Command("Custom_BuildPath3")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = "切削尖端平面";
	sToolTipText = "build the path of cutter";
	sWhatsThis = "Custom_BuildPath";
	sStatusTip = sToolTipText;
	sPixmap = "Path-Toolpath";
}

void CmdCustomBuildPath3::activated(int iMsg)
{
}

bool CmdCustomBuildPath3::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}
//===========================================================================
// Custom_BuildNCFile
//===========================================================================
DEF_STD_CMD_A(CmdCustomBuildNCFile);

CmdCustomBuildNCFile::CmdCustomBuildNCFile()
	:Command("Custom_BuildNCFile")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = QT_TR_NOOP("build the file of nc");
	sToolTipText = QT_TR_NOOP("build the file of nc");
	sWhatsThis = "Custom_BuildNCFile";
	sStatusTip = sToolTipText;
	sPixmap = "Path-Comment";
}

void CmdCustomBuildNCFile::activated(int iMsg)
{
	Gui::MainWindow* pMainWindow = Gui::getMainWindow();
	

	CreateNCData data;
	QStringList lstFaces;
	std::vector<Gui::SelectionObject>  selobjs = Gui::Selection().getSelectionEx();
	for (std::vector<Gui::SelectionObject>::iterator it = selobjs.begin(); it != selobjs.end(); it++)
	{
		std::string buf1 = "";

		buf1 += (*it).getFeatName();
		lstFaces.push_back(Base::Tools::fromStdString(buf1));
// 		buf1 += ".";
// 		std::vector<std::string> names = (*it).getSubNames();
// 		for (int i = 0; i < names.size(); i++)
// 		{
// 			std::string s = names[i];
// 			if (s.find("Face") != s.npos)
// 			{
// 				s = buf1 + s;
// 				lstFaces.push_back(Base::Tools::fromStdString(s));
// 			}
// 		}
	}
	data.setPathList(lstFaces);
	//data.setMachineCS(Base::Placement());
	//data.setWorkCS(workcs);
	data.setSafeHeight(5.0);
	data.setSpeed(1000);
	data.setDeltaX(18);
	data.setSavePath(QString::fromAscii("d:\\ccc.anc"));

	QStringList sb;
	sb
		//<< QString::fromLocal8Bit("M06 T1")//; 设置使用刀具1
		<< QString::fromLocal8Bit("G90 G54")//;  G64几何数据的基本设定:使用绝对坐标, 使用工件坐标系1, 使用连续路径运行
		<< QString::fromLocal8Bit("M03 S5000")//; 设置主轴顺时针旋转 转速5000
		<< QString::fromLocal8Bit("M08")//; 冷却打开 
		;
		//<< QString::fromAscii("G00 Z150; Z轴运行至安全高度")//
		//<< QString::fromAscii("G00 X - 7.2 Y - 7.2; XY运行到起始点"); //
	data.setBeforeGCode(sb);
	//	G43 H1 Z150.

	QStringList ab;
	ab  
		<< QString::fromLocal8Bit("M05")//; 主轴停止
		<< QString::fromLocal8Bit("M09")//; 冷却液关闭
		<< QString::fromLocal8Bit("M30");//; 程序结束
	data.setAfterGCode(ab);
	
	CreateNCfileDlg* dlg = new CreateNCfileDlg(pMainWindow);
	dlg->setData(data);
	if (dlg->exec() == QDialog::Accepted)
	{
		CreateNCData data = dlg->getData();

		QStringList lstPaths = data.getPathList();
		QString	strPaths = QLatin1String("[");
		for (int i = 0; i < lstPaths.size(); i++)
		{
			QString ss = lstPaths[i];
			
// 			strPaths+=
// 				QString::fromLocal8Bit("(App.ActiveDocument.%1,\"%2\"),")
// 				.arg(ss.section(QLatin1Char('.'), 0, 0))
// 				.arg(ss.section(QLatin1Char('.'), 1, 1));

			strPaths +=
				QString::fromLocal8Bit("App.ActiveDocument.%1,")
				.arg(ss);
		}
		if (strPaths.endsWith(QLatin1Char(',')))
			strPaths.chop(1);	
		strPaths += QLatin1String("]");
	
		QStringList lstBG = data.getBeforeGCode();
		QString strBGS;
		for (int i = 0; i < lstBG.size(); i++)
			strBGS += QString::fromAscii("'") + lstBG[i] + QString::fromAscii("',");

		QStringList lstAG = data.getAfterGCode();
		QString strAGS;
		for (int i = 0; i < lstAG.size(); i++)
			strAGS += QString::fromAscii("'")+ lstAG[i] + QString::fromAscii("',");

		QString str = QString::fromAscii(
			"o=App.ActiveDocument.addObject('Custom::NCOutPutter','NCOutPutter')\n"
			"o.PathList=%1 \n"
			"o.OutputFile='%2'\n"
			"o.BeforeGcode=[%3]\n"
			"o.AfterGcode=[%4]\n"
			"o.SafeHeight=%5\n"
			"o.Speed=%6\n"
			"o.DeltaX=%7\n"
			)
			.arg(strPaths)
			.arg(data.getSavePath())
			.arg(strBGS)
			.arg(strAGS)
			.arg(data.getSafeHeight())
			.arg(data.getSpeed())
			.arg(data.getDeltaX())
			;

		try {
			if (!str.isEmpty())
				doCommand(Doc, (const char*)str.toAscii());
		}
		catch (const Base::Exception& e) {
			Base::Console().Warning("an error occured.%s", e.what());
		};
	}
	

}

bool CmdCustomBuildNCFile::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}

//===========================================================================
// Custom_BuildNCFile2
//===========================================================================
DEF_STD_CMD_A(CmdCustomBuildNCFile2);

CmdCustomBuildNCFile2::CmdCustomBuildNCFile2()
	:Command("Custom_BuildNCFile2")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = QT_TR_NOOP("build the file of nc");
	sToolTipText = QT_TR_NOOP("build the file of nc");
	sWhatsThis = "Custom_BuildNCFile";
	sStatusTip = sToolTipText;
	sPixmap = "Path-Comment";
}

void CmdCustomBuildNCFile2::activated(int iMsg)
{
	Gui::MainWindow* pMainWindow = Gui::getMainWindow();


	CreateNCData data;
	QStringList lstPaths;
	Base::Placement workcs;
	std::vector<Gui::SelectionObject>  selobjs = Gui::Selection().getSelectionEx();
	for (std::vector<Gui::SelectionObject>::iterator it = selobjs.begin(); it != selobjs.end(); it++)
	{
		QString ss = QString::fromAscii("App.ActiveDocument.%1").arg(QString::fromAscii((*it).getFeatName()));
		lstPaths.push_back(ss);
		App::DocumentObject* obj = it->getObject();
		if (obj->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId()))
		{
			Part::Feature* fea = dynamic_cast<Part::Feature*>(obj);
			if (fea)
				workcs = fea->Placement.getValue();
		}
	}
	data.setPathList(lstPaths);
	data.setMachineCS(Base::Placement());
	data.setWorkCS(workcs);
	data.setSafeHeight(1.0);
	data.setSpeed(1000);
	data.setSavePath(QString::fromAscii("d:\\ccc.anc"));

	QStringList sb;
	sb
		<< QString::fromLocal8Bit("M06 T1")//; 设置使用刀具1
		<< QString::fromLocal8Bit("M03 S5000")//; 设置主轴顺时针旋转 转速5000
		<< QString::fromLocal8Bit("M08")//; 冷却打开 
		<< QString::fromLocal8Bit("G90 G54 G64");//; 几何数据的基本设定:使用绝对坐标, 使用工件坐标系1, 使用连续路径运行
	//<< QString::fromAscii("G00 Z150; Z轴运行至安全高度")//
	//<< QString::fromAscii("G00 X - 7.2 Y - 7.2; XY运行到起始点"); //
	data.setBeforeGCode(sb);
	//	G43 H1 Z150.

	QStringList ab;
	ab
		<< QString::fromLocal8Bit("M05 M09")//; 主轴停止，冷却液关闭
		<< QString::fromLocal8Bit("M30");//; 程序结束
	data.setAfterGCode(ab);

	CreateNCfileDlg* dlg = new CreateNCfileDlg(pMainWindow);
	dlg->setData(data);
	if (dlg->exec() == QDialog::Accepted)
	{
		CreateNCData data = dlg->getData();

		QStringList lstPaths = data.getPathList();
		QString strPaths;
		for (int i = 0; i < lstPaths.size(); i++)
			strPaths += lstPaths[i] + QString::fromAscii(",");

		QStringList lstBG = data.getBeforeGCode();
		QString strBGS;
		for (int i = 0; i < lstBG.size(); i++)
			strBGS += QString::fromAscii("'") + lstBG[i] + QString::fromAscii("',");

		QStringList lstAG = data.getAfterGCode();
		QString strAGS;
		for (int i = 0; i < lstAG.size(); i++)
			strAGS += QString::fromAscii("'") + lstAG[i] + QString::fromAscii("',");

		QString str = QString::fromAscii(
			"o=App.ActiveDocument.addObject('Custom::NCOutPutter','NCOutPutter')\n"
			"o.PathList=[%1] \n"
			"o.OutputFile='%2'\n"
			"o.BeforeGcode=[%3]\n"
			"o.AfterGcode=[%4]\n"
			"o.SafeHeight=%5\n"
			"o.Speed=%6\n"
			)
			.arg(strPaths)
			.arg(data.getSavePath())
			.arg(strBGS)
			.arg(strAGS)
			.arg(data.getSafeHeight())
			.arg(data.getSpeed())

			;

		try {
			if (!str.isEmpty())
				doCommand(Doc, (const char*)str.toAscii());
		}
		catch (const Base::Exception& e) {
		};
	}


}

bool CmdCustomBuildNCFile2::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}
//////////////////////////////////////////////////////////////////////////

//== == == == == == == == == == == == ==
// Custom_BuildPath
//===========================================================================
DEF_STD_CMD_A(CmdCustomStartPlayPath);

CmdCustomStartPlayPath::CmdCustomStartPlayPath()
	:Command("Custom_StartPlayPath")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = "StartPlayPath";
	sToolTipText = "build the path of cutter";
	sWhatsThis = "Custom_BuildPath";
	sStatusTip = sToolTipText;
	sPixmap = "media-playback-start";
}
CustomGui::ViewProviderCustomPath* vpp = 0;
void CmdCustomStartPlayPath::activated(int iMsg)
{
	PlayPathDlg * dlg = new PlayPathDlg;
	
	
	std::vector<Gui::SelectionObject>  selobjs = Gui::Selection().getSelectionEx();
	std::string ss = "";
	bool bFirst = true;
	App::DocumentObject* pObj=0;
	for (std::vector<Gui::SelectionObject>::iterator it = selobjs.begin(); it != selobjs.end(); it++)
	{
		//只处理第一个先 
		pObj = it->getObject();
	}

	if (pObj)
	{
		Gui::ViewProvider *vp = Gui::Application::Instance->getViewProvider(pObj);
		if (vp->getTypeId().isDerivedFrom(CustomGui::ViewProviderCustomPath::getClassTypeId()))
		{
			 vpp = dynamic_cast<CustomGui::ViewProviderCustomPath*>(vp);
			 dlg->setCurrentViewProvider(vpp);
			 dlg->show();
		}
	}

}

bool CmdCustomStartPlayPath::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////

//== == == == == == == == == == == == ==
// Custom_BuildPath
//===========================================================================
DEF_STD_CMD_A(CmdCustomStopPlayPath);

CmdCustomStopPlayPath::CmdCustomStopPlayPath()
	:Command("Custom_StopPlayPath")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = "StopPlayPath";
	sToolTipText = "build the path of cutter";
	sWhatsThis = "Custom_BuildPath";
	sStatusTip = sToolTipText;
	sPixmap = "media-playback-stop";
}

void CmdCustomStopPlayPath::activated(int iMsg)
{
	if (vpp)
	{
		vpp->stopCutter();
	}
}

bool CmdCustomStopPlayPath::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}


//== == == == == == == == == == == == ==
// Custom_CreateTools
//===========================================================================
DEF_STD_CMD_A(CmdCustom_CreateTools);

CmdCustom_CreateTools::CmdCustom_CreateTools()
:Command("Custom_CreateTools")
{
	sAppModule = "Custom";
	sGroup = "Custom";
	sMenuText = QT_TR_NOOP("CreateTools");
	sToolTipText = QT_TR_NOOP("Tools list");
	sWhatsThis = "Custom_CreateTools";
	sStatusTip = sToolTipText;
	sPixmap = "Path-LoadTool";
}

void CmdCustom_CreateTools::activated(int iMsg)
{
	Gui::MainWindow* pMainWindow = Gui::getMainWindow();
	CreateToolsDlg* dlg = new CreateToolsDlg(pMainWindow);
	App::Document* pDoc = App::GetApplication().getActiveDocument();
	if (pDoc)
	{
		Tools tools = pDoc->ToolList.getValue();
		dlg->setData(tools);
	
		if (dlg->exec() == QDialog::Accepted)
		{
			Tools newTools = dlg->getData();
			pDoc->ToolList.setValue(newTools);
		}
	}

	
}

bool CmdCustom_CreateTools::isActive(void)
{
	if (getActiveGuiDocument())
		return true;
	else
		return false;
}


//////////////////////////////////////////////////////////////////////////
void CreateCustomCommands(void)
{
	Gui::CommandManager &rcCmdMgr = Gui::Application::Instance->commandManager();

	rcCmdMgr.addCommand(new CmdCustomCreateBallCutter());
	rcCmdMgr.addCommand(new CmdCustomSetCutterParameter());
	rcCmdMgr.addCommand(new CmdCustomSetBlankPart());
	rcCmdMgr.addCommand(new CmdCustomBuildPath());
	rcCmdMgr.addCommand(new CmdCustomBuildPath2());
	rcCmdMgr.addCommand(new CmdCustomBuildPath3());
	rcCmdMgr.addCommand(new CmdCustomBuildNCFile());
	rcCmdMgr.addCommand(new CmdCustomBuildNCFile2());
	rcCmdMgr.addCommand(new CmdCustomStartPlayPath());
	rcCmdMgr.addCommand(new CmdCustomStopPlayPath());
	rcCmdMgr.addCommand(new CmdCustom_CreateTools());
	rcCmdMgr.addCommand(new CmdCustom_SetModelPosition());
}
