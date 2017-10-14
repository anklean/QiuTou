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


#include "ViewProviderNC.h"
#include "SoFCShapeObject.h"

#include <Mod/Part/App/PartFeature.h>
#include <Mod/Part/App/PrimitiveFeature.h>
#include "../App/PathObject.h"
#include "Inventor/nodes/SoPickStyle.h"
#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoCylinder.h"
#include "Inventor/nodes/SoTransform.h"
#include "../App/NCOutputter.h"
//#include "CutterPlayer.h"

using namespace CustomGui;

PROPERTY_SOURCE(CustomGui::ViewProviderNC, PartGui::ViewProviderPart)


ViewProviderNC::ViewProviderNC()
{
	lineset = new SoIndexedLineSet;
	lineset->ref();
	pCoords = new SoCoordinate3;
	pCoords->ref();
	pColor = new SoBaseColor();
	pColor->ref();

	//player = 0;
	cutterOnPointIndex = 0;
}

ViewProviderNC::~ViewProviderNC()
{
	pCoords->unref();
	lineset->unref();
	pColor->unref();
// 	if (player != 0)
// 	{
// 		delete player;
// 		player = 0;
// 	}
}

void ViewProviderNC::onChanged(const App::Property* prop)
{
	if (prop == &LineColor) {
		const App::Color& c = LineColor.getValue();
		pColor->rgb.setValue(c.r, c.g, c.b);
	}
	else {
		PartGui::ViewProviderPart::onChanged(prop);
	}
}


void CustomGui::ViewProviderNC::attach(App::DocumentObject *pcFeat)
{
	PartGui::ViewProviderPart::attach(pcFeat);

	SoPickStyle* ps = new SoPickStyle();
	ps->style = SoPickStyle::SHAPE;

	SoSeparator *lineSep = new SoSeparator();
	SoDrawStyle* style = new SoDrawStyle();
	style->lineWidth = 2.0f;

	lineSep->addChild(ps);
	lineSep->addChild(style);
	lineSep->addChild(pColor);
	lineSep->addChild(pCoords);
	lineSep->addChild(lineset);

	pcRoot->addChild(lineSep);

	SoSeparator* sep = new SoSeparator;
	switchCutter = new SoSwitch;

	cutterTransform = new SoTransform();
	cutterTransform->ref();
	switchCutter->addChild(cutterTransform);
	SoCylinder* cyliner = new SoCylinder;
	cyliner->radius = 0.02;
	cyliner->height = 0.5;
	switchCutter->addChild(cyliner);	

	switchCutter->whichChild = SO_SWITCH_NONE;
	sep->addChild(switchCutter);
	pcRoot->addChild(sep);
	
}

void CustomGui::ViewProviderNC::updateData(const App::Property* prop)
{
	if (prop->getTypeId() == Part::PropertyPartShape::getClassTypeId()) {
		// get the shape to show
		const TopoDS_Shape &cShape = static_cast<const Part::PropertyPartShape*>(prop)->getValue();

		// calculate the visual only if visible
		if (Visibility.getValue())
			updateVisual(cShape);
		else
			VisualTouched = true;
	}
	PartGui::ViewProviderPart::updateData(prop);
}

void CustomGui::ViewProviderNC::updateVisual(const TopoDS_Shape & prop)
{
	Custom::NCOutPutter* pPathObj = dynamic_cast<Custom::NCOutPutter*>(pcObject);
	std::vector<Custom::PathObject::PathPointList> nclist = pPathObj->getNclist();

	std::vector<SbVec3f> vertex_list;
	std::vector<int32_t> index_list;
	
	int nIndex = 0;
	for (int i = 0; i < nclist.size(); i++)
	{
		Custom::PathObject::PathPointList ptlist = nclist[i];
		for (int j = 0; j < ptlist.size(); j++)
		{
			Base::Vector3d v = ptlist[j].Point;
			Base::Vector3d n = ptlist[j].Normal;
			
#if _DEBUG
			vertex_list.push_back(SbVec3f(v.x, v.y, v.z));
			index_list.push_back(nIndex++);

			Base::Vector3d v2 = v + n*0.5;
			vertex_list.push_back(SbVec3f(v2.x, v2.y, v2.z));
			index_list.push_back(nIndex++);

			index_list.push_back(-1);
#else
			vertex_list.push_back(SbVec3f(v.x, v.y, v.z));
			index_list.push_back(nIndex++);
#endif
		}
#if _DEBUG
#else
		index_list.push_back(-1);
#endif
	}
	if (vertex_list.size() > 0)
	{
		pCoords->point.setNum(vertex_list.size());
		pCoords->point.setValues(0, vertex_list.size(), (const SbVec3f*)(&(vertex_list[0])));
	}

	if (index_list.size() > 0)
	{
		lineset->coordIndex.setNum(index_list.size());
		lineset->coordIndex.setValues(0, index_list.size(), (const int32_t*)(&(index_list[0])));
	}
}
//
//void CustomGui::ViewProviderNC::StartCutter()
//{	
//	if (player==0)
//		player = new CutterPlayer(this);
//	if (player != 0)
//	{
//		player->Start();
//		Custom::PathObject* pPathObj = dynamic_cast<Custom::PathObject*>(pcObject);
//		pathPointList = pPathObj->getPointList();
//		
//		switchCutter->whichChild = SO_SWITCH_ALL;
//	}
//}
//
//void CustomGui::ViewProviderNC::stopCutter()
//{
//	if (player != 0)
//	{
//		player->Stop();
//	}
//	switchCutter->whichChild = SO_SWITCH_NONE;
//}