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


#include "PathObject.h"
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
#include "STEPControl_Reader.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "BRepMesh_GeomTool.hxx"
#include "Handle_STEPControl_Controller.hxx"
#include "CSLib.hxx"
#include "Base/Vector3D.h"
#include "Base/Matrix.h"
#include "Geom_Curve.hxx"
#include "GeomLProp_CLProps.hxx"
#include "Geom_BSplineSurface.hxx"
#include "TColgp_Array1OfPnt.hxx"
#include "GeomAPI_PointsToBSpline.hxx"
#include "Polygon_Functions.h"
#include "BRepAdaptor_HSurface.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "BRepProj_Projection.hxx"
#include "GeomAPI_ProjectPointOnSurf.hxx"
#include "GC_MakeSegment.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"
#include "Geom_Line.hxx"
#include "GC_MakeLine.hxx"

using namespace Custom;

#define _UTF8(X) (const char*)(QString::fromLocal8Bit(X).toAscii())

const char* Custom::PathObject::UVTypeEnums[] = {"U" , "V" , NULL};
const char* Custom::PathObject::PathTypeEnums[] = { _UTF8( "面上沿边界往返"), _UTF8("沿边界单向") ,NULL };

PROPERTY_SOURCE(Custom::PathObject, Part::Feature)

PathObject::PathObject()
{
	const char* strPara = _UTF8("参数");

	ADD_PROPERTY(fileName, ("d:\\ccc22.step"));
	

	ADD_PROPERTY_TYPE(Face, (0), strPara, App::Prop_None, _UTF8("要加工的面"));
	ADD_PROPERTY_TYPE(Curve, (0), strPara, App::Prop_None, _UTF8("轨迹线"));
	ADD_PROPERTY_TYPE(PathType, ((long)0), strPara, App::Prop_None, _UTF8("路径的类型"));
	PathType.setEnums(PathTypeEnums);

	ADD_PROPERTY_TYPE(UVType, ((long)0), strPara, App::Prop_None, _UTF8("边界的类型"));
	UVType.setEnums(UVTypeEnums);

	ADD_PROPERTY_TYPE(ToolSetNumber, (""), strPara, App::Prop_None, _UTF8("刀具的编号"));
	//ADD_PROPERTY_TYPE(ToolDirection, (Base::Vector3d(0,1,0)), strPara, App::Prop_None, _UTF8("安全高度"));

	ADD_PROPERTY_TYPE(Speed, (1000), strPara, App::Prop_None, _UTF8( "转速"));

	ADD_PROPERTY_TYPE(SafeHeight, (15), strPara, App::Prop_None, _UTF8("安全高度"));
	ADD_PROPERTY_TYPE(BeforeAdditionGCode, (""), strPara, App::Prop_None, _UTF8("路径执行前附加G代码"));
	ADD_PROPERTY_TYPE(StepAdditionGCode, (""), strPara, App::Prop_None, _UTF8("路径执行时附加G代码"));
	ADD_PROPERTY_TYPE(AfterAdditionGCode, (""), strPara, App::Prop_None, _UTF8("路径执行后附加G代码"));

	//ADD_PROPERTY_TYPE(UTolerance, (0.2), strPara, App::Prop_None, _UTF8("U方向的Tolerance"));//==刀具半径
	ADD_PROPERTY_TYPE(Tolerance, (0.02), strPara, App::Prop_None, _UTF8("Tolerance"));

}

short PathObject::mustExecute() const
{
	if (UVType.isTouched() ||
		Face.isTouched() ||Curve.isTouched()||
		ToolSetNumber.isTouched() /*||*/
		/*SafeHeight.isTouched()*/)
		return 1;

    return 0;
}

void PathObject::onChanged(const App::Property* prop)
{	
	if (isRestoring() == true)
	{
		return;
	}
	if (prop == &UVType || prop == &Face || prop == &Curve
		|| prop == &ToolSetNumber
		/*|| prop==&SafeHeight*/)
	{
		
			App::DocumentObjectExecReturn *r = this->recompute();
			delete r;
		
	}

	Part::Feature::onChanged(prop);
}

App::DocumentObjectExecReturn *PathObject::execute(void)
{
	App::DocumentObject * pCurve = Curve.getValue();
	std::vector<std::string> subCurve = Curve.getSubValues();
	if (pCurve == 0)
	{
		return new App::DocumentObjectExecReturn("obj is null");
	}
	Part::Feature * pCurveFea = dynamic_cast<Part::Feature *>(pCurve);
	Part::TopoShape topCurveShape = pCurveFea->Shape.getShape();

	App::DocumentObject * pObj = Face.getValue();
	std::vector<std::string> subFaces = Face.getSubValues();
	if (pObj == 0)
	{
		return new App::DocumentObjectExecReturn("obj is null");
	}
	Part::Feature * pFea = dynamic_cast<Part::Feature *>(pObj);
	Part::TopoShape topShape = pFea->Shape.getShape();


	TopoDS_Shape S = topCurveShape.getSubShape(subCurve[0].c_str());
	TopoDS_Edge aEdge;
	TopExp_Explorer Ex;
	for (Ex.Init(S, TopAbs_EDGE); Ex.More(); Ex.Next())
	{
		aEdge = TopoDS::Edge(Ex.Current());
	}

	TopoDS_Shape F = topShape.getSubShape(subFaces[0].c_str());
	TopoDS_Face aFace;

	for (Ex.Init(F, TopAbs_FACE); Ex.More(); Ex.Next())
	{
		aFace = TopoDS::Face(Ex.Current());
	}

	buildPath(aEdge, aFace);
	
	TopoDS_Shape result;
	this->Shape.setValue(result);
	return App::DocumentObject::StdReturn;
}


Standard_Boolean Custom::PathObject::getNormal(BRepAdaptor_Surface& theSurface, Standard_Real& theParamU, Standard_Real& theParamV, gp_Pnt& thePoint, gp_Dir& theNormal)
{
	Standard_Boolean isOK = Standard_True;
	gp_Vec aD1U, aD1V;

	theSurface.D1(theParamU, theParamV, thePoint, aD1U, aD1V);

	CSLib_DerivativeStatus aStatus;
	CSLib::Normal(aD1U, aD1V, Precision::Angular(), aStatus, theNormal);
	if (aStatus != CSLib_Done)
	{
		gp_Vec aD2U, aD2V, aD2UV;
		theSurface.D2(theParamU, theParamV, thePoint, aD1U, aD1V, aD2U, aD2V, aD2UV);
		CSLib_NormalStatus aNormalStatus;
		CSLib::Normal(aD1U, aD1V, aD2U, aD2V, aD2UV, Precision::Angular(),
			isOK, aNormalStatus, theNormal);
	}

	if (!isOK)
		return Standard_False;

	const TopoDS_Face& aFace = ((BRepAdaptor_Surface*)&(theSurface.Surface()))->Face();
	TopAbs_Orientation aOri = aFace.Orientation();
	if (aOri == TopAbs_REVERSED)
		theNormal.Reverse();

	return Standard_True;
}

int getNormal2(gp_Pnt& pnt, gp_Sphere& sphere, gp_Dir& normal)
{
	gp_Pnt center = sphere.Location();
	gp_Dir dir = sphere.Position().Direction();

	//gp_Lin lin  (center, dir);
	//Handle(Geom_TrimmedCurve) hGeomLine = GC_MakeSegment(lin, -10,10);

	Handle(Geom_Line) hGeomLine = GC_MakeLine(center,dir);

	GeomAPI_ProjectPointOnCurve aPPC(pnt, hGeomLine);

	Standard_Integer aNumSolutions = aPPC.NbPoints();
	if (aNumSolutions > 0)
	{
		gp_Pnt aNearestPoint = aPPC.NearestPoint();
		gp_XYZ xyz = aNearestPoint.XYZ() - pnt.XYZ();
		normal = gp_Dir(xyz);
		return 0;
	}
	return -1;
}

void Custom::PathObject::buildPath(TopoDS_Edge& aEdge, TopoDS_Face& aFace)
{
	double deflection = 0.01;
	double AngDeflectionRads = 0.1;

	BRepAdaptor_Curve curveAdaptor(aEdge);
	Standard_Real first = curveAdaptor.FirstParameter();
	Standard_Real last = curveAdaptor.LastParameter();

	TopAbs_Orientation edgeOrientation = aEdge.Orientation();
	if (edgeOrientation == TopAbs_REVERSED)
	{
		//反向
	}

	BRepAdaptor_Surface surfaceAdaptor(aFace);
	BRepMesh_GeomTool crvMeshTools(curveAdaptor, first, last, deflection, AngDeflectionRads);
	
	PathPointList paths;

	gp_Sphere spere = surfaceAdaptor.Sphere();

	int num = crvMeshTools.NbPoints();
	for (int i = 1; i < num + 1; i++)
	{
		Standard_Real         theIsoParam=0;
		Standard_Real         theParam;
		gp_Pnt             thePoint;
		gp_Pnt2d             theUV;
		crvMeshTools.Value(i, theIsoParam, theParam, thePoint, theUV);
		
		gp_Dir nrm;
		int nret = getNormal2(thePoint, spere, nrm);
		if (nret != 0)
		{
			//求取点在面上的UV值
			GeomAPI_ProjectPointOnSurf ppos(thePoint, surfaceAdaptor.Surface().Surface());
			if (ppos.IsDone() == false)
				continue;

			Standard_Real u = 0;
			Standard_Real v = 0;
			ppos.LowerDistanceParameters(u, v);

			gp_Pnt ppnt;
			//gp_Dir nrm;
			getNormal(surfaceAdaptor, u, v, ppnt, nrm);
		}	

		NCStepInfo si = { 0 };
		si.type = 0;
		si.gcode = "";
		si.speed = 0;
		si.Point = Base::Vector3d(thePoint.X(), thePoint.Y(), thePoint.Z());
		si.Normal = Base::Vector3d(nrm.X(), nrm.Y(), nrm.Z());
		paths.push_back(si);//从原点开始
	}

	//判断线的方向
	//如果和X方向反向，则让点的顺序反序
	std::reverse(paths.begin(), paths.end());

	m_PointList.insert(m_PointList.end(), paths.begin(), paths.end());
}