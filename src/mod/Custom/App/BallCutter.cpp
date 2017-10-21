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
#include "BRepAlgoAPI_Section.hxx"
#include <Handle_Geom_TrimmedCurve.hxx>
#include <Handle_Geom_Circle.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_Circle.hxx>
#include "GC_MakeArcOfCircle.hxx"
#include <ProjLib_ProjectOnSurface.hxx>
#include "BRepAdaptor_HSurface.hxx"
#include "BRepAdaptor_HCurve.hxx"
#include "Geom_BSplineCurve.hxx"
#include "TopoDS_Face.hxx"
#include "BRepOffsetAPI_MakePipeShell.hxx"
#include "TopTools_ListIteratorOfListOfShape.hxx"
#include "../../Part/App/TopoShape.h"
#include "ProjLib_Sphere.hxx"
#include "GeomProjLib.hxx"
#include "GC_MakeLine.hxx"
#include "GC_MakeSegment.hxx"
#include "Geom_SphericalSurface.hxx"
#include "BRepProj_Projection.hxx"
#include "BRepBuilderAPI_Transform.hxx"
#include "BRepOffsetAPI_MakePipe.hxx"
#include "BRepAlgo_BooleanOperation.hxx"
#include "BRepAlgo_Cut.hxx"

using namespace Custom;


PROPERTY_SOURCE(Custom::BallCutter, Part::Feature)

BallCutter::BallCutter()
{
	ADD_PROPERTY(number, (6));
	ADD_PROPERTY(angle, (11));
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
	//Custom::GeomLineSegment line;
	//line.setPoints(startPt, endPt);
	//lineList.push_back(line);

	App::Document* doc = App::GetApplication().getActiveDocument();
	if (doc)
	{
		App::DocumentObject* pobj = doc->addObject("Custom::Line", "line");
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

void BallCutter::makeMainBoby(TopoDS_Shape& body, double ang)
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
  //Base::Vector3d pt4(pt0.x, pt0.y, pt0.z - cyLength);
  Base::Vector3d pt4(pt0.x, pt0.y, pt0.z - cyLength - ballH);

  double h = sqrt(ballradius * ballradius - (ballH - ballradius) * (ballH - ballradius));

  Base::Vector3d pt5(pt0.x + h, pt0.y, pt3.z);

  BRepBuilderAPI_MakeWire mkWire; 

  // 绘制线段
  TopoDS_Edge line1 = makeEdge_Line(pt0, pt1);
  mkWire.Add(line1);

  TopoDS_Edge line2 = makeEdge_Line(pt1, pt2);
  mkWire.Add(line2);
  TopoDS_Edge line3 = makeEdge_Line(pt2, pt3);
  mkWire.Add(line3);
  TopoDS_Edge line4 = makeEdge_Line(pt3, pt5);
  mkWire.Add(line4);

  // 绘制 圆球 线
  Base::Vector3d ptCenter(pt0.x, pt0.y, pt0.z - cyLength - ballH + ballradius);

  double endAngel = acos((ballH - ballradius) / ballradius);
  double test = endAngel * 180 / M_PI;
  
  gp_Pnt pt(ptCenter.x, ptCenter.y, ptCenter.z);
  gp_Ax2 ax2(pt,gp_Dir(0,1,0),gp_Dir(0,0,1));
  gp_Circ circ(ax2,ballradius);

  GC_MakeArcOfCircle mkArc(circ, endAngel, M_PI,true);
  BRepBuilderAPI_MakeEdge mkEdge(mkArc.Value());
  mkWire.Add(mkEdge.Edge());

  TopoDS_Edge line5 = makeEdge_Line(pt4, pt0);
  mkWire.Add(line5);

  TopoDS_Wire wrie;
  if (mkWire.IsDone())
  {
	  wrie = mkWire.Wire();
  }

  // 开始做旋转
  gp_Pnt pnt(0, 0, 0);
  gp_Dir dir(0, 0, 1);
  gp_Ax1 axis(pnt, dir);
  body = this->doRevolution(wrie, axis, ang);

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
	makeMainBoby(BaseShape, -1 * M_PI*2);

	TopoDS_Shape newBaseShape = cutHeader(BaseShape);
	//this->Shape.setValue(BaseShape);
	//return App::DocumentObject::StdReturn;

	TopoDS_Shape CutBody;
	TopoDS_Edge yindaoxian;
	this->makeCutterBody(newBaseShape, CutBody, yindaoxian);

	//TopoDS_Shape s = doBoolean_Cut(BaseShape, CutBody);
	
// 	TopoDS_Compound comp;
// 	BRep_Builder builder;
// 	builder.MakeCompound(comp);
// 	builder.Add(comp,BaseShape);
// 	builder.Add(comp, yindaoxian);

	//this->Shape.setValue(yindaoxian);

	return App::DocumentObject::StdReturn;


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
	int n = 1;
	for (int i = 0; i < n; i++)
	{
		double agNew = ag - (4 * M_PI / 8 + ag)*i / n;
		double hNew = r * cos(agNew);

		double sketchY = r * sin(agNew);
		angleIncre += 18 * M_PI / 180 / n;

		double b = L - L / n * i;
		TopoDS_Wire wire;
		makeMainSketch(hNew, b, angleIncre, sketchY, wire);

		
		//return;
		
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

	//TopoDS_Shape CutBody;
	//this->makeCutterBody(h,L,sCutBody);
	//TopoDS_Shape temp = this->doBoolean_Fuse(BaseShape, ToolShape);
	//TopoDS_Shape temp2 = this->doBoolean_Cut(temp, headerBody);
	//TopoDS_Shape temp3 = this->doBoolean_Cut(headerBody, headerCutBody);
	//TopoDS_Shape ret = this->doBoolean_Fuse(temp2, temp3);

	//this->Shape.setValue(temp2);

	
	return App::DocumentObject::StdReturn;
}


void BallCutter::onChanged(const App::Property* prop)
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

void BallCutter::makeHeaderBoby(TopoDS_Shape& body)
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

TopoDS_Shape BallCutter::doRevolution(TopoDS_Wire wire, gp_Ax1 axis,double angle)
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

TopoDS_Shape Custom::BallCutter::doExtrusion(TopoDS_Wire wire, Base::Vector3d Dir, double start, double end)
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
	BRepPrimAPI_MakePrism mkPrism(ext, vec);
	ext = mkPrism.Shape();
	}
	catch (...) {
	}
				

	return ext;
}

TopoDS_Shape Custom::BallCutter::doBoolean_Fuse(TopoDS_Shape BaseShape, TopoDS_Shape ToolShape)
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

TopoDS_Shape  Custom::BallCutter::doBoolean_Cut(const TopoDS_Shape& BaseShape, const TopoDS_Shape& ToolShape)
{
	TopoDS_Shape r;
	try {
		std::auto_ptr<BRepAlgo_BooleanOperation> mkBool(new BRepAlgo_Cut(BaseShape, ToolShape));
		if (mkBool->IsDone()) {
			r= mkBool->Shape();
		}
	}
	catch (...) {

	}

	return r;
}

void Custom::BallCutter::makeHeaderCutBoby(TopoDS_Shape& body)
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

void Custom::BallCutter::makeCutterBody(TopoDS_Shape& baseShape, TopoDS_Shape& CutBody , TopoDS_Edge& yindaoxian)
{
	double cyDiameter = cylinderDiameter.getValue();
	double cyLength = cylinderLength.getValue();

	double ballH = ballHeight.getValue();
	double ballradius = radius.getValue();

	int num = number.getValue();
	double ag = 2 * M_PI / num;
	Base::Vector3d ptBase(0, 0, 0);

	double dSketchAngel = sketchAngle.getValue() * M_PI / 180;
	double dSketchLength = sketchLength.getValue();
	//////////////////////////////////////////////////////////////////////////
	// 绘制引导截面
	//////////////////////////////////////////////////////////////////////////
	double h = sqrt(ballradius * ballradius - (ballH - ballradius) * (ballH - ballradius));
	
	// 外圆上的两个点
	Base::Vector3d pt1 = Base::Vector3d(ptBase.x, ptBase.y - h, ptBase.z - cyLength);
	Base::Vector3d pt2 = Base::Vector3d(ptBase.x + h * sin(ag), ptBase.y - h * cos(ag), pt1.z);
	// 内部的一个点
	Base::Vector3d pt3 = Base::Vector3d(pt1.x + dSketchLength * sin(dSketchAngel),
		pt1.y + dSketchLength * cos(dSketchAngel), pt1.z);

	Base::Vector3d dir0 = (pt1 - pt3).Normalize();
	Base::Vector3d pt4 = pt1 + dir0 * dSketchLength * 2;

	Base::Vector3d dir1 = (pt2 - pt3).Normalize();
	Base::Vector3d pt5 = pt2 + dir1 * dSketchLength * 2;

	Base::Vector3d pt6 = pt5 + dir0 * dSketchLength * 3;

	std::vector<Base::Vector3d> ptList;
	//ptList.push_back(pt2);
	ptList.push_back(pt3);
	//ptList.push_back(pt1);
	ptList.push_back(pt4);
	ptList.push_back(pt6);
	ptList.push_back(pt5);

	TopoDS_Wire wire;
 	makePloyline(ptList, wire);


	TopoDS_Compound comp;
	BRep_Builder builder;
	builder.MakeCompound(comp);
	builder.Add(comp, wire);

	
	//////////////////////////////////////////////////////////////////////////
	// 绘制18度直线
	//////////////////////////////////////////////////////////////////////////
	Base::Vector3d pt0 = Base::Vector3d(ptBase.x, ptBase.y, ptBase.z - cyLength);
	Base::Vector3d ptLineStart = pt0;

	double dDiameter = radius.getValue() * 2;
	double dAngle = angle.getValue() * M_PI / 180;
	Base::Vector3d ptLineEnd(pt0.x + dDiameter * sin(dAngle), pt0.y, pt0.z - dDiameter * cos(dAngle));

	Base::Vector3d v = ptLineEnd - ptLineStart;

	gp_Pnt ptS(pt0.x, pt0.y, pt0.z);
	gp_Pnt ptE(ptLineEnd.x, ptLineEnd.y, ptLineEnd.z);
	gp_Dir ptDir(v.x,v.y,v.z);

	//TopoDS_Edge line = BRepBuilderAPI_MakeEdge(lin);
// 
// 	Base::Vector3d ptLineEndUp(ptLineEnd.x, ptLineEnd.y + dDiameter, ptLineEnd.z);
// 	Base::Vector3d ptLineStartUp(ptLineStart.x, ptLineStart.y + dDiameter, ptLineStart.z);
// 
// 	TopoDS_Edge line = makeEdge_Line(ptLineStart, ptLineEnd);

	//////////////////////////////////////////////////////////////////////////
	//  使用投影 创建引导线
	//////////////////////////////////////////////////////////////////////////	
	std::vector<TopoDS_Wire> list18ProjLines;
	TopoDS_Face f;
	TopExp_Explorer Ex;
	for (Ex.Init(baseShape, TopAbs_FACE); Ex.More(); Ex.Next()) {
		f = TopoDS::Face(Ex.Current());
		BRepAdaptor_Surface surfaceAdaptor(f);
	/*	Handle(BRepAdaptor_HSurface) hSurfaceAdaptor = new BRepAdaptor_HSurface(surfaceAdaptor);*/
		if (surfaceAdaptor.GetType() == GeomAbs_Sphere)
		{	
			gp_Lin L(ptS, ptDir);
			Handle(Geom_TrimmedCurve) hGeomLine = GC_MakeSegment(ptS , ptE).Value();

			TopoDS_Edge e = BRepBuilderAPI_MakeEdge(hGeomLine);
			TopoDS_Wire w = BRepBuilderAPI_MakeWire(e);

			BRepProj_Projection proj(w, f, gp_Dir(0, 1, 0));


			if (proj.IsDone()) {
				while (proj.More()) {
					list18ProjLines.push_back(proj.Current());
					proj.Next();
				}
			}

			break;
		}
	}

	std::vector<TopoDS_Edge> list18ProjEdges;
	for (int i = 0; i < list18ProjLines.size(); i++)
	{
		TopoDS_Wire w = list18ProjLines[i];
		for (Ex.Init(w, TopAbs_EDGE); Ex.More(); Ex.Next()) {
			TopoDS_Edge e = TopoDS::Edge(Ex.Current());
			list18ProjEdges.push_back(e);
		}
	}

	std::vector<TopoDS_Wire> spineWires;
	for (int i = 0; i < list18ProjEdges.size(); i++)
	{
		BRepBuilderAPI_MakeWire makeSpine(list18ProjEdges[i]);
		spineWires.push_back(makeSpine.Wire());
	}

	//gp_Trsf T;
	//T.SetRotation(gp_Ax1(gp_Pnt(0., 0., 0.), gp_Vec(0., 0., 1.)),M_PI);
	//BRepBuilderAPI_Transform theTrsf(T);
	//TopoDS_Wire newShape;
	//theTrsf.Perform(projLine, Standard_False);
	//newShape = TopoDS::Wire(theTrsf.Shape());
	//builder.Add(comp, newShape);


	//return;
	//this->Shape.setValue(curver.);

	//////////////////////////////////////////////////////////////////////////
	// 绘制18度直线在球面上的投影线
	//////////////////////////////////////////////////////////////////////////
#if 0
	Base::Vector3d ptCenter(ptBase.x, ptBase.y, ptBase.z - cyLength - ballH + ballradius);
	double b = (ballH - ballradius)*sin(dAngle);
	Base::Vector3d ptCenterNew(ptCenter.x - b*cos(dAngle), ptCenter.y, ptCenter.z + b*sin(dAngle));
	double r = sqrt(ballradius*ballradius - b*b);
	Base::Vector3d ptOther(ptCenterNew.x - r*sin(dAngle), ptCenterNew.y, ptCenterNew.z - r*cos(dAngle));
	

	Base::Vector3d dir1 = (ptCenter - ptCenterNew).Normalize();
	Base::Vector3d dir2 = (ptOther - ptCenterNew).Normalize();

	gp_Pnt pt(ptCenterNew.x, ptCenterNew.y, ptCenterNew.z);
	gp_Ax2 ax2(pt, gp_Dir(dir1.x, dir1.y, dir1.z), gp_Dir(dir2.x, dir2.y, dir2.z));
	gp_Circ circ(ax2, r);

	double hh = (ballH - ballradius) / cos(dAngle);
	double angleNew = asin(hh / r);

	GC_MakeArcOfCircle mkArc(circ, -M_PI / 2 - angleNew, 0, true);
	BRepBuilderAPI_MakeEdge mkEdge(mkArc.Value());

	TopoDS_Edge yindaoxian = mkEdge.Edge();

	//this->Shape.setValue(yindaoxian);
	//return;
#endif

#if 0
	//////////////////////////////////////////////////////////////////////////
	// 扫描生成模型
	//////////////////////////////////////////////////////////////////////////
	TopTools_ListOfShape profiles;

	profiles.Append(wire);

	BRepBuilderAPI_MakeWire mkWire(projLine);
	TopoDS_Shape path = mkWire.Wire();

	Standard_Boolean isSolid =  Standard_True;
	Standard_Boolean isFrenet = Standard_True;
	BRepBuilderAPI_TransitionMode transMode;
	transMode = BRepBuilderAPI_RightCorner;

	BRepOffsetAPI_MakePipeShell mkPipeShell(TopoDS::Wire(path));
	mkPipeShell.SetMode(isFrenet);
	mkPipeShell.SetTransitionMode(transMode);
	TopTools_ListIteratorOfListOfShape iter;
	for (iter.Initialize(profiles); iter.More(); iter.Next()) {
		mkPipeShell.Add(TopoDS_Shape(iter.Value()));
	}

	if (!mkPipeShell.IsReady())
		Standard_Failure::Raise("shape is not ready to build");
	mkPipeShell.Build();
	if (isSolid)
		mkPipeShell.MakeSolid();
#endif

	TopoDS_Shape pipeShape[2];
	double ang[2] = { M_PI / 3.0, M_PI / 3.0+M_PI  };
	for (int i = 0; i < 2; i++)
	{
		gp_Trsf TT;
		TT.SetRotation(gp_Ax1(gp_Pnt(0., 0., 0.), gp_Vec(0., 0., -1.)), ang[i]);
		BRepBuilderAPI_Transform theTrsfNew(TT);
		TopoDS_Wire newWire;
		theTrsfNew.Perform(wire, Standard_True);
		newWire = TopoDS::Wire(theTrsfNew.Shape());

		pipeShape[i] = this->doSweep(newWire, spineWires[i]);
	}



	std::vector<TopoDS_Shape> pipeShapes;
	pipeShapes.push_back(pipeShape[0]);
	//pipeShapes.push_back(pipeShape[1]);

	for (int i = 1; i < 6; i++)
	{
		gp_Trsf T;
		T.SetRotation(gp_Ax1(gp_Pnt(0., 0., 0.), gp_Vec(0., 0., -1.)), i*2*M_PI / 6.0);
		BRepBuilderAPI_Transform theTrsf(T);
		theTrsf.Perform(pipeShape[0], Standard_True);
		pipeShapes.push_back(theTrsf.Shape());
	
	}
	//this->Shape.setValue(comp1);
	//return;

	//Part::TopoShape compPipe(pipeShape1);
	//TopoDS_Shape S=compPipe.multiFuse(pipeShapes);
// 	this->Shape.setValue(S);
// 	return;
	//builder.Add(comp, temp);


	std::vector<Base::Vector3d> ptListWWW;
	ptListWWW.push_back(pt2);
	ptListWWW.push_back(pt3);
	ptListWWW.push_back(pt1);
	ptListWWW.push_back(pt6);
	ptListWWW.push_back(pt5);

	TopoDS_Wire wireWWW;
	makePloyline(ptListWWW, wireWWW);
	
// 	TopoDS_Shape pipeShape2 = this->doSweep(newWire, newShape);
	//builder.Add(comp, pipeShape2);
	//TopoDS_Shape temp = this->doBoolean_Fuse(pipeShape2, pipeShape1);

	//TopoDS_Shape temp2 = this->doBoolean_Cut(baseShape, temp);
// 
// 	TopoDS_Shape temp = this->doBoolean_Cut(baseShape, pipeShape2);
// 
// 	TopoDS_Shape temp2 = this->doBoolean_Cut(temp, pipeShape1);
// 	this->Shape.setValue(temp2);


// 	Part::TopoShape baseshape_tt(baseShape);
// 	TopoDS_Shape R = baseshape_tt.cut(pipeShape1);
// 	this->Shape.setValue(R);
// 	return;
	TopoDS_Shape B = baseShape;
	for (int i = 0; i <pipeShapes.size(); i++)
	{
		if (i != 3)
		{
			TopoDS_Shape T = pipeShapes[i];
			std::auto_ptr<BRepAlgo_BooleanOperation> mkBool(new BRepAlgo_Cut(B, T));
			if (mkBool->IsDone()) {
				B = mkBool->Shape();
			}
		}
	}
// 单独处理第三个	
	std::auto_ptr<BRepAlgo_BooleanOperation> mkBool(new BRepAlgo_Cut(B, pipeShape[1]));
	if (mkBool->IsDone()) {
		B = mkBool->Shape();
	}

// 	TopoDS_Compound comp1;
// 	BRep_Builder builder1;
// 	builder1.MakeCompound(comp1);
// 	builder1.Add(comp1, pipeShape[1]);
// 	builder1.Add(comp1, B);

	this->Shape.setValue(B);

// 	std::auto_ptr<BRepAlgo_BooleanOperation> mkBool(new BRepAlgo_Cut(baseShape, S));
// 	if (mkBool->IsDone()) {
// // 		TopoDS_Compound comp1;
// // 		BRep_Builder builder1;
// // 		builder1.MakeCompound(comp1);
// // 		builder1.Add(comp1, mkBool->Shape());
// 		this->Shape.setValue(mkBool->Shape());
// 	}

	
	//this->Shape.setValue(mkPipeShell.Shape());
	return ;
}

TopoDS_Shape Custom::BallCutter::doBoolean_Section(TopoDS_Shape BaseShape, TopoDS_Shape ToolShape)
{
	TopoDS_Shape resShape;
	try {
		std::auto_ptr<BRepAlgoAPI_BooleanOperation> mkBool(new BRepAlgoAPI_Section(BaseShape, ToolShape));
		if (mkBool->IsDone()) {
			resShape = mkBool->Shape();
		}
	}
	catch (...) {

	}

	return resShape;
}

TopoDS_Shape Custom::BallCutter::doSweep(TopoDS_Wire wire, TopoDS_Wire path)
{
	TopTools_ListOfShape profiles;

	profiles.Append(wire);

	Standard_Boolean isSolid = Standard_True;
	Standard_Boolean isFrenet = Standard_False;
	BRepBuilderAPI_TransitionMode transMode;
	transMode = BRepBuilderAPI_RoundCorner;

	BRepOffsetAPI_MakePipeShell mkPipeShell(path);
	//mkPipeShell.SetMode(gp_Dir(0., 0., 1.));
	mkPipeShell.SetMode(gp_Ax2(gp_Pnt(0,0,0), gp_Dir(0,1,0)));
	//mkPipeShell.SetMode(isFrenet);
	mkPipeShell.SetTransitionMode(transMode);
	TopTools_ListIteratorOfListOfShape iter;
	for (iter.Initialize(profiles); iter.More(); iter.Next()) {
		mkPipeShell.Add(TopoDS_Shape(iter.Value()));
	}

	if (!mkPipeShell.IsReady())
		Standard_Failure::Raise("shape is not ready to build");
	mkPipeShell.Build();
	if (isSolid)
		mkPipeShell.MakeSolid();

	return mkPipeShell.Shape();
}


TopoDS_Shape Custom::BallCutter::doSweep2(TopoDS_Wire wire, TopoDS_Wire path)
{
	TopTools_ListOfShape profiles;

	profiles.Append(wire);

	Standard_Boolean isSolid = Standard_True;
	Standard_Boolean isFrenet = Standard_False;
	BRepBuilderAPI_TransitionMode transMode;
	transMode = BRepBuilderAPI_Transformed;

	BRepOffsetAPI_MakePipeShell mkPipeShell(path);
	//mkPipeShell.SetMode(gp_Dir(0., 0., 1.));
	//mkPipeShell.SetMode(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0)));
	mkPipeShell.SetMode(isFrenet);
	mkPipeShell.SetTransitionMode(transMode);
	TopTools_ListIteratorOfListOfShape iter;
	for (iter.Initialize(profiles); iter.More(); iter.Next()) {
		mkPipeShell.Add(TopoDS_Shape(iter.Value()));
	}

	if (!mkPipeShell.IsReady())
		Standard_Failure::Raise("shape is not ready to build");
	mkPipeShell.Build();
	if (isSolid)
		mkPipeShell.MakeSolid();

	return mkPipeShell.Shape();
}

TopoDS_Shape Custom::BallCutter::cutHeader(TopoDS_Shape& baseShape)
{
	Base::Vector3d ptBase(0, 0, 0);
	double cyLength1 = cylinderLength.getValue();
	double dH = ballHeight.getValue();
	double draidus = radius.getValue();

	double dAg = angle.getValue() * M_PI / 180;

	double tempH = dH - draidus;
	double tempL = sqrt(draidus* draidus - tempH*sin(dAg)*tempH*sin(dAg)) + tempH*cos(dAg);
	double dZ = cyLength1 + tempL*cos(dAg);

	Base::Vector3d pt11(ptBase.x + 2 * draidus, ptBase.y + 2 * draidus, ptBase.z - dZ + 0.05);
	Base::Vector3d pt21(ptBase.x - 2 * draidus, ptBase.y + 2 * draidus, pt11.z);
	Base::Vector3d pt31(ptBase.x - 2 * draidus, ptBase.y - 2 * draidus, pt11.z);
	Base::Vector3d pt41(ptBase.x + 2 * draidus, ptBase.y - 2 * draidus, pt11.z);


	std::vector<Base::Vector3d> ptList11;
	ptList11.push_back(pt11);
	ptList11.push_back(pt21);
	ptList11.push_back(pt31);
	ptList11.push_back(pt41);

	TopoDS_Wire wire11;
	makePloyline(ptList11, wire11);

	TopoDS_Shape ext = this->doExtrusion(wire11, Base::Vector3d(0, 0, -1), 0, draidus);

	TopoDS_Shape ss = this->doBoolean_Cut(baseShape, ext);

	return ss;
}
