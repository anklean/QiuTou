/***************************************************************************
 *   Copyright (c) 2008 Werner Mayer <wmayer[at]users.sourceforge.net>     *
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
# include <cmath>
# include <Bnd_Box.hxx>
# include <BRepBndLib.hxx>
# include <gp_Pln.hxx>
# include <gp_Trsf.hxx>
# include <BRep_Tool.hxx>
# include <BRepAdaptor_Surface.hxx>
# include <BRepCheck_Analyzer.hxx>
# include <BRepOffsetAPI_MakeOffset.hxx>
# include <BRepBuilderAPI_Copy.hxx>
# include <BRepBuilderAPI_MakeFace.hxx>
# include <BRepBuilderAPI_MakeWire.hxx>
# include <BRepBuilderAPI_Transform.hxx>
# include <BRepOffsetAPI_ThruSections.hxx>
# include <BRepPrimAPI_MakePrism.hxx>
# include <Geom_Plane.hxx>
# include <IntTools_FClass2d.hxx>
# include <Precision.hxx>
# include <ShapeAnalysis.hxx>
# include <ShapeAnalysis_Surface.hxx>
# include <ShapeFix_Shape.hxx>
# include <ShapeFix_Wire.hxx>
# include <TopoDS.hxx>
# include <TopoDS_Iterator.hxx>
# include <TopExp.hxx>
# include <TopExp_Explorer.hxx>
# include <TopTools_IndexedMapOfShape.hxx>
#endif


#include "BallCutter.h"
#include <Base/Tools.h>
#include <Base/Exception.h>
#include "math.h"
#include "App/Application.h"
#include "App/Document.h"
#include "PrimitiveFeature.h"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "Base/Console.h"
#include "BRepPrimAPI_MakeRevol.hxx"
#include "BRepAlgoAPI_Common.hxx"
#include "BRepAlgoAPI_BooleanOperation.hxx"
#include "BRepAlgoAPI_Fuse.hxx"
#include "gp_Circ.hxx"
#include "BRepAlgoAPI_Cut.hxx"


using namespace Part;


PROPERTY_SOURCE(Part::BallCutter, Part::Feature)

BallCutter::BallCutter()
{
	ADD_PROPERTY(number, (6));
	ADD_PROPERTY(angle, (18));
	ADD_PROPERTY(sketchAngle, (30));
	ADD_PROPERTY(sketchLength, (0.3));

	ADD_PROPERTY(radius, (0.8));
	ADD_PROPERTY(ballHeight, (1.2));
	ADD_PROPERTY(pathStep, (0.3));

	ADD_PROPERTY(cylinderDiameter, (0.8));
	ADD_PROPERTY(cylinderLength, (1.1));
	ADD_PROPERTY(chamferEdgeA, (0.1));
	ADD_PROPERTY(chamferEdgeB, (0.05));

	ADD_PROPERTY(headerHeight, (0.1));
	ADD_PROPERTY(headerAngle, (54));

}

short BallCutter::mustExecute() const
{
	if (number.isTouched() ||
		angle.isTouched() ||
		sketchAngle.isTouched() ||
		sketchLength.isTouched() ||
		radius.isTouched() ||
		ballHeight.isTouched())
        return 1;
    return 0;
}

App::DocumentObjectExecReturn* BallCutter::makeLine(Base::Vector3d& startPt, Base::Vector3d& endPt, TopoDS_Edge& edge)
{
	//Part::GeomLineSegment line;
	//line.setPoints(startPt, endPt);
	//lineList.push_back(line);

	App::Document* doc = App::GetApplication().getActiveDocument();
	if (doc)
	{
		App::DocumentObject* pobj = doc->addObject("Part::Line", "line");
		if (pobj)
		{
			Part::Line* pLine = dynamic_cast<Part::Line*>(pobj);
			pLine->X1.setValue(startPt.x);
			pLine->Y1.setValue(startPt.y);
			pLine->Z1.setValue(startPt.z);
			pLine->X2.setValue(endPt.x);
			pLine->Y2.setValue(endPt.y);
			pLine->Z2.setValue(endPt.z);
		}
	}

	return 0;
}

TopoDS_Edge BallCutter::makeEdge_Line(const Base::Vector3d& startPt, const Base::Vector3d& endPt)
{
	TopoDS_Edge edge;
	gp_Pnt point1;
	point1.SetX(startPt.x);
	point1.SetY(startPt.y);
	point1.SetZ(startPt.z);

	gp_Pnt point2;
	point2.SetX(endPt.x);
	point2.SetY(endPt.y);
	point2.SetZ(endPt.z);

	BRepBuilderAPI_MakeEdge mkEdge(point1, point2);
	if (mkEdge.IsDone())
	{
		edge = mkEdge.Edge();
	}

	return edge;
}

void BallCutter::makePloyline(std::vector<Base::Vector3d> points,TopoDS_Wire& wrie)
{
	// create polygon
	BRepBuilderAPI_MakePolygon mkPoly;

	int size = points.size();
	for (int i = 0; i < size; i++) 
	{
		Base::Vector3d v = points[i];
		mkPoly.Add(gp_Pnt(v.x, v.y, v.z));
	}

	if (mkPoly.IsDone())
	{
		mkPoly.Close();
		wrie = mkPoly.Wire();
	}
}

void BallCutter::makeMainBoby(TopoDS_Shape& body)
{
  double cyDiameter = cylinderDiameter.getValue();
  double cyLength = cylinderLength.getValue();

  double ballH = ballHeight.getValue();
  double ballradius = radius.getValue();

  double chamferA = chamferEdgeA.getValue();
  double chamferB = chamferEdgeB.getValue();
  
  Base::Vector3d pt0(0,0,0);
  Base::Vector3d pt1(pt0.x + cyDiameter / 2, pt0.y, pt0.z);
  Base::Vector3d pt2(pt1.x, pt0.y, pt1.z - cyLength + chamferA);
  Base::Vector3d pt3(pt1.x + chamferB, pt2.y, pt1.z - cyLength);
  Base::Vector3d pt4(pt0.x, pt0.y, pt0.z - cyLength);

  double h = sqrt(ballradius * ballradius - (ballH - ballradius) * (ballH - ballradius));

  std::vector<Base::Vector3d> points;
  points.push_back(pt0);
  points.push_back(pt1);
  points.push_back(pt2);
  points.push_back(pt3);
  points.push_back(pt4);

  TopoDS_Wire w;
  makePloyline(points, w);
  
  gp_Pnt pnt(0, 0, 0);
  gp_Dir dir(0, 0, 1);
  gp_Ax1 axis(pnt, dir);

  body = this->doRevolution(w,axis);
}

void BallCutter::makeMainSketch(double h, double L, double angleIncre, double sketchY, TopoDS_Wire& wire)
{
	double z = cylinderLength.getValue() + ballHeight.getValue() - radius.getValue();
	Base::Vector3d basePt(0, 0, -z);
	Base::Vector3d pt0(basePt.x, basePt.y, basePt.z + sketchY);

	std::vector<Base::Vector3d> points;
	// 创建外圈点
	int num = number.getValue();
	for (int i = 0; i < num; i++)
	{
		double ag = 2 * M_PI / num * i + angleIncre;
		Base::Vector3d pt = Base::Vector3d(pt0.x + h * sin(ag), pt0.y + h * cos(ag), pt0.z);
		points.push_back(pt);
	}

	// 创建内圈点
	if (points.size() == 0)
	{
		return;
	}
	Base::Vector3d ptStart = points[0];
	double sktAngle = sketchAngle.getValue() * M_PI / 180;
	Base::Vector3d ptTemp = Base::Vector3d(ptStart.x + L * sin(sktAngle),
		ptStart.y - L * cos(sktAngle), ptStart.z);
	double ag = atan(ptTemp.y / ptTemp.x);
	double r = ptTemp.y / sin(ag);

	for (int i = 0; i < num; i++)
	{
		double agStep = 2 * M_PI / num * i;
		Base::Vector3d pt = Base::Vector3d(pt0.x + r * cos(ag - agStep),
			pt0.y + r * sin(ag - agStep), pt0.z);
		points.push_back(pt);
	}

	int size = (int)points.size() / 2;

	std::vector<Base::Vector3d> pointList;

	for (int i = 0; i < size; i++)
	{
		Base::Vector3d pt = points[i];
		Base::Vector3d ptNext = points[i + num];

		pointList.push_back(pt);
		pointList.push_back(ptNext);
	}

	//BRepBuilderAPI_MakeWire mkWrie;
	//for (int i = 0; i < size; i++)
	//{
	//	Base::Vector3d pt = points[i];
	//	Base::Vector3d ptNext;
	//	Base::Vector3d ptInSide = points[i + num];
	//	if (i != size - 1)
	//	{			
	//		ptNext = points[i + 1];
	//	}
	//	else
	//	{
	//		ptNext = points[0];
	//	}

	//	double stp = pathStep.getValue();
	//	Base::Vector3d vec = (ptInSide - pt).Normalize();

	//	int count = (int)(L / pathStep.getValue());
	//	for (int i = 0; i < count; i++)
	//	{
	//		Base::Vector3d ptTemp = pt + (vec * i * stp);
	//		pointList.push_back(ptTemp);
	//	}
	//	double dist = Distance(ptInSide, ptNext);
	//	count = (int)(dist / stp);
	//	Base::Vector3d vec2 = (ptNext - ptInSide).Normalize();

	//	for (int i = 0; i < count; i++)
	//	{
	//		Base::Vector3d ptTemp = ptInSide + (vec2 * i * stp);
	//		pointList.push_back(ptTemp);
	//	}
	//}

	makePloyline(pointList, wire);

// 	for (int i = 0; i < pointList.size(); i++)
// 	{
// 		Base::Vector3d pt = pointList[i];
// 		Base::Vector3d ptNext;
// 		if (i != pointList.size() -1)
// 		{
// 			ptNext = pointList[i + 1];
// 		}
// 		else
// 		{
// 			ptNext = pointList[0];
// 		}
// 		TopoDS_Edge edge1;
// 		makeLine(pt, ptNext, edge1);
// 	}
}
App::DocumentObjectExecReturn *BallCutter::execute(void)
{
	TopoDS_Shape BaseShape;
	makeMainBoby(BaseShape);

	//this->Shape.setValue(ss);
	//App::Document* doc = App::GetApplication().getActiveDocument();
	//if (doc)
	//{
	//	std::vector<App::DocumentObject*> lines = doc->getObjectsOfType(Part::Line::getClassTypeId());
	//	for (std::vector<App::DocumentObject*>::iterator it = lines.begin();
	//		it != lines.end(); ++it)
	//	{
	//		doc->remObject((*it)->getNameInDocument());
	//	}		
	//}
	//makeMainBoby(1.1, 0.8, 0.1, 0.05, 1.2, 1.6)
	double r = radius.getValue();
	double bh = ballHeight.getValue();
	double h = sqrt(r * r - (bh - r) * (bh - r));
	double ag = asin((bh - r) / r);
	double L = sketchLength.getValue();

	Standard_Boolean isSolid = Standard_True;
	Standard_Boolean isRuled = Standard_False;
	Standard_Boolean isClosed = Standard_False;

	BRepOffsetAPI_ThruSections aGenerator(isSolid, isRuled);

	double angleIncre = 0.;
	int n = 10;
	for (int i = 0; i < n; i++)
	{
		double agNew = ag - (4 * M_PI / 8 + ag)*i / n;
		double hNew = r * cos(agNew);

		double sketchY = r * sin(agNew);
		angleIncre += 18 * M_PI / 180 / n;

		double b = L - L / n * i;
		TopoDS_Wire wire;
		makeMainSketch(hNew, b, angleIncre, sketchY, wire);

		aGenerator.AddWire(wire);
	}
	
	Standard_Boolean anIsCheck = Standard_True;
	aGenerator.CheckCompatibility(anIsCheck);   // use BRepFill_CompatibleWires on profiles. force #edges, orientation, "origin" to match.
	aGenerator.Build();
	if (!aGenerator.IsDone())
		Standard_Failure::Raise("Failed to create loft face");

	//Base::Console().Message("DEBUG: TopoShape::makeLoft returns.\n");
	TopoDS_Shape ToolShape = aGenerator.Shape();

	TopoDS_Shape headerBody;
	this->makeHeaderBoby(headerBody);

	TopoDS_Shape headerCutBody;
	this->makeHeaderCutBoby(headerCutBody);

	TopoDS_Shape temp = this->doBoolean_Fuse(BaseShape, ToolShape);
	TopoDS_Shape temp2 = this->doBoolean_Cut(temp, headerBody);
	//TopoDS_Shape temp3 = this->doBoolean_Cut(headerBody, headerCutBody);
	//TopoDS_Shape ret = this->doBoolean_Fuse(temp2, temp3);

	this->Shape.setValue(temp2);

	
	return App::DocumentObject::StdReturn;
}


void Part::BallCutter::onChanged(const App::Property* prop)
{
	if (prop == &number ||
		prop == &angle ||
		prop == &sketchAngle ||
		prop == &sketchLength ||
		prop == &radius ||
		prop == &ballHeight)
	{
		try
		{
			App::DocumentObjectExecReturn *r =this->recompute();
			delete r;
		}
		catch (...)
		{
			
		}
	}

	Part::Feature::onChanged(prop);
}

void Part::BallCutter::makeHeaderBoby(TopoDS_Shape& body)
{
	double height = headerHeight.getValue();
	double angle = headerAngle.getValue() / 180 * M_PI;
	double r = radius.getValue();

	double z = cylinderLength.getValue() + ballHeight.getValue() - height;
	Base::Vector3d ptBase(0, 0, -z);

	Base::Vector3d pt0(ptBase.x, ptBase.y, ptBase.z);
	double h = sqrt(r*r - (r - height)*(r - height));
	Base::Vector3d pt1(pt0.x + h, pt0.y, pt0.z);
	Base::Vector3d pt2(pt0.x, pt0.y, pt0.z - height);

	TopoDS_Edge edge0 = makeEdge_Line(pt0, pt1);

	gp_Circ circle;
	circle.SetRadius(r);
	
	gp_Dir dir(0, 1, 0);
	gp_Ax1 axis(gp_Pnt(0, 0, 0), dir);
	circle.SetAxis(axis);
	gp_Pnt pnt(ptBase.x, ptBase.y, ptBase.z - height + r);
	circle.SetLocation(pnt);

	double Angle0 = asin(h / r);

	BRepBuilderAPI_MakeEdge clMakeEdge(circle, M_PI / 2 - Angle0, M_PI / 2);

	TopoDS_Edge edge1 = clMakeEdge.Edge();

	TopoDS_Edge edge2 = makeEdge_Line(pt2, pt0);

	TopoDS_Wire www;
	BRepBuilderAPI_MakeWire mkWire;
	mkWire.Add(edge0);
	mkWire.Add(edge1);
	mkWire.Add(edge2);

	www = mkWire.Wire();

	gp_Pnt pnt1(pt0.x, pt0.y, pt0.z);
	gp_Dir dir1(0, 0, 1);
	gp_Ax1 axis1(pnt1, dir1);
	body = this->doRevolution(www, axis1);


}

TopoDS_Shape Part::BallCutter::doRevolution(TopoDS_Wire wire, gp_Ax1 axis,double angle)
{
	Standard_Boolean convertFailed = false;
	Standard_Boolean isSolid = Standard_True;

	TopoDS_Face f;
	TopoDS_Edge e;

	TopoDS_Shape base;
	if (isSolid)
	{
		if (!convertFailed)
		{
			BRepBuilderAPI_MakeFace mkFace(wire);
			if (mkFace.IsDone()) {
				f = mkFace.Face();
				base = f;
			}
			else {
				convertFailed = true;
			}
		}
	}

	if (convertFailed) {
		Base::Console().Message("TopoShape::revolve could not make Solid from Wire/Edge.\n");
	}

	BRepPrimAPI_MakeRevol mkRevol(base, axis, angle);
	return mkRevol.Shape();
}

TopoDS_Shape Part::BallCutter::doExtrusion(TopoDS_Wire wire, Base::Vector3d Dir, double start, double end)
{
	TopoDS_Shape ext;
	double len = end - start;
	gp_Vec vec(Dir.x * len, Dir.y * len, Dir.z * len);

	bool makeSolid = true;

	try {
		BRepBuilderAPI_MakeFace mkFace(wire);
		if (mkFace.IsDone()) {
			ext = mkFace.Face();
		}
	}
	catch (...) {
	}
				
	BRepPrimAPI_MakePrism mkPrism(ext, vec);
	ext = mkPrism.Shape();

	return ext;
}

TopoDS_Shape Part::BallCutter::doBoolean_Fuse(TopoDS_Shape BaseShape, TopoDS_Shape ToolShape)
{
	TopoDS_Shape resShape;
	try {
		std::auto_ptr<BRepAlgoAPI_BooleanOperation> mkBool(new BRepAlgoAPI_Fuse(BaseShape, ToolShape));
		if (mkBool->IsDone()) {			
			resShape = mkBool->Shape();
		}
	}
	catch (...) {
		
	}

	return resShape;
}

TopoDS_Shape Part::BallCutter::doBoolean_Cut(TopoDS_Shape BaseShape, TopoDS_Shape ToolShape)
{
	TopoDS_Shape resShape;
	try {
		std::auto_ptr<BRepAlgoAPI_BooleanOperation> mkBool(new BRepAlgoAPI_Cut(BaseShape, ToolShape));
		if (mkBool->IsDone()) {
			resShape = mkBool->Shape();
		}
	}
	catch (...) {

	}

	return resShape;
}

void Part::BallCutter::makeHeaderCutBoby(TopoDS_Shape& body)
{
	double height = headerHeight.getValue();
	double angle = headerAngle.getValue() / 180 * M_PI;
	double r = radius.getValue();

	double z = cylinderLength.getValue() + ballHeight.getValue() - height;
	Base::Vector3d ptBase(0, 0, -z);

	Base::Vector3d pt0(ptBase.x, ptBase.y, ptBase.z);
	Base::Vector3d tPt0(pt0.x, pt0.y - r, pt0.z - height);
	Base::Vector3d tPt1(pt0.x + height * sin(angle / 2), tPt0.y, pt0.z);
	Base::Vector3d tPt2(tPt1.x * (-1), tPt1.y, tPt1.z);

	Base::Vector3d tPt3(pt0.x + 2 * r, tPt0.y, pt0.z);
	Base::Vector3d tPt4(tPt3.x * (-1), tPt3.y, tPt3.z);

	Base::Vector3d tPt5(pt0.x + 2 * r, tPt0.y, pt0.z - 2 * height);
	Base::Vector3d tPt6(tPt5.x * (-1), tPt5.y, tPt5.z);

	std::vector<Base::Vector3d> points;
	points.push_back(tPt6);
	points.push_back(tPt4);
	points.push_back(tPt2);
	points.push_back(tPt0);
	points.push_back(tPt1);
	points.push_back(tPt3);
	points.push_back(tPt5);

	TopoDS_Wire w;
	this->makePloyline(points, w);

	body = this->doExtrusion(w, Base::Vector3d(0, 1, 0), 0, 2 * r);
}
