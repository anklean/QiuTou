/***************************************************************************
 *   Copyright (c) 2004 Juergen Riegel <juergen.riegel@web.de>             *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
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
# include <Bnd_Box.hxx>
# include <Poly_Polygon3D.hxx>
# include <BRepBndLib.hxx>
# include <BRepMesh_IncrementalMesh.hxx>
# include <BRep_Tool.hxx>
# include <BRepTools.hxx>
# include <BRepAdaptor_Curve.hxx>
# include <BRepAdaptor_Surface.hxx>
# include <GeomAbs_CurveType.hxx>
# include <GeomAbs_SurfaceType.hxx>
# include <Geom_BezierCurve.hxx>
# include <Geom_BSplineCurve.hxx>
# include <Geom_BezierSurface.hxx>
# include <Geom_BSplineSurface.hxx>
# include <GeomAPI_ProjectPointOnSurf.hxx>
# include <GeomLProp_SLProps.hxx>
# include <gp_Trsf.hxx>
# include <Poly_Array1OfTriangle.hxx>
# include <Poly_Triangulation.hxx>
# include <TColgp_Array1OfPnt.hxx>
# include <TopoDS.hxx>
# include <TopoDS_Edge.hxx>
# include <TopoDS_Wire.hxx>
# include <TopoDS_Face.hxx>
# include <TopoDS_Shape.hxx>
# include <TopoDS_Iterator.hxx>
# include <TopExp_Explorer.hxx>
# include <TopExp.hxx>
# include <TopTools_IndexedMapOfShape.hxx>
# include <Poly_PolygonOnTriangulation.hxx>
# include <TColStd_Array1OfInteger.hxx>
# include <TopTools_ListOfShape.hxx>
# include <Inventor/SoPickedPoint.h>
# include <Inventor/events/SoMouseButtonEvent.h>
# include <Inventor/nodes/SoCoordinate3.h>
# include <Inventor/nodes/SoDrawStyle.h>
# include <Inventor/nodes/SoIndexedFaceSet.h>
# include <Inventor/nodes/SoLineSet.h>
# include <Inventor/nodes/SoLocateHighlight.h>
# include <Inventor/nodes/SoMaterial.h>
# include <Inventor/nodes/SoNormal.h>
# include <Inventor/nodes/SoNormalBinding.h>
# include <Inventor/nodes/SoPointSet.h>
# include <Inventor/nodes/SoPolygonOffset.h>
# include <Inventor/nodes/SoShapeHints.h>
# include <Inventor/nodes/SoSwitch.h>
# include <Inventor/nodes/SoGroup.h>
# include <Inventor/nodes/SoSphere.h>
# include <Inventor/nodes/SoScale.h>
# include <QWidget>
#endif

/// Here the FreeCAD includes sorted by Base,App,Gui......
#include <Base/Console.h>
#include <Base/Parameter.h>
#include <Base/Exception.h>
#include <App/Application.h>
#include <App/Document.h>
#include <Gui/Command.h>
#include <Gui/Selection.h>
#include <Gui/View3DInventorViewer.h>
#include "ViewProviderBallCutter.h"
#include "QtGui/QAction"

#include <Gui/Utilities.h>
#include <Gui/Control.h>
#include "BallCutterDialog.h"
#include "QtGui/QMenu"

using namespace CustomGui;

PROPERTY_SOURCE(CustomGui::ViewProviderBallCutter, PartGui::ViewProviderPart)


ViewProviderBallCutter::ViewProviderBallCutter()
{
}

ViewProviderBallCutter::~ViewProviderBallCutter()
{

}

void CustomGui::ViewProviderBallCutter::setupContextMenu(QMenu*menu, QObject* receiver, const char* member)
{
	QAction* act;
	act = menu->addAction(QObject::tr("Edit "), receiver, member);
	act->setData(QVariant((int)ViewProvider::Default));
//	ViewProviderPart::setupContextMenu(menu, receiver, member);
}

bool CustomGui::ViewProviderBallCutter::setEdit(int ModNum)
{
	if (ModNum == ViewProvider::Default) {
		// When double-clicking on the item for this pad the
		// object unsets and sets its edit mode without closing
		// the task panel
		Gui::TaskView::TaskDialog *dlg = Gui::Control().activeDialog();
		if (dlg!=0) {
			Gui::Control().showDialog(dlg);
			return false;
		}

		Gui::Selection().clearSelection();
		Gui::Control().showDialog(new CustomGui::TaskBallCutter(this->getObject()));
		return Gui::ViewProviderGeometryObject::setEdit(ModNum);
	}
	else {
		return Gui::ViewProviderGeometryObject::setEdit(ModNum);
	}
}

void CustomGui::ViewProviderBallCutter::unsetEdit(int ModNum)
{
	if (ModNum == ViewProvider::Default) {

		Gui::ViewProviderGeometryObject::unsetEdit(ModNum);
	}
}