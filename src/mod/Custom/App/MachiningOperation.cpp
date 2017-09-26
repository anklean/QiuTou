

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

#include "MachiningOperation.h"
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
#include "Geom_BSplineSurface.hxx"
#include "BRepMesh_GeomTool.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "TopoShape.h"


using namespace Custom;


PROPERTY_SOURCE(Custom::MachiningOperation, Part::Feature)

MachiningOperation::MachiningOperation()
{
	ADD_PROPERTY(Solid, (0));
	ADD_PROPERTY(Deviation, (0.001));
	ADD_PROPERTY(AngularDeflection, (28.5));

}

short MachiningOperation::mustExecute() const
{
	if (Solid.isTouched() ||
		Deviation.isTouched() || 
		AngularDeflection.isTouched()
		)
		return 1;
	return 0;
}

App::DocumentObjectExecReturn *MachiningOperation::execute(void)
{	
	App::DocumentObject* solid = Solid.getValue();
	if (solid ==0)
	{
		return new App::DocumentObjectExecReturn("Solid is null");
	}

	Part::Feature* feature = dynamic_cast<Part::Feature*>(solid);
	if (feature == 0)
	{
		return new App::DocumentObjectExecReturn("Solid is not Part::Feature");
	}

	TopoDS_Shape topoShape = feature->Shape.getValue();

	//求取整个造型的BOX，计算合适的Tolerance
	Bnd_Box bounds;
	BRepBndLib::Add(topoShape, bounds);
	bounds.SetGap(0.0);
	Standard_Real xMin, yMin, zMin, xMax, yMax, zMax;
	bounds.Get(xMin, yMin, zMin, xMax, yMax, zMax);
	Standard_Real deflection = ((xMax - xMin) + (yMax - yMin) + (zMax - zMin)) / 300.0 *Deviation.getValue();
	Standard_Real AngDeflectionRads = AngularDeflection.getValue() / 180.0 * M_PI;


	//1.查找造型中所有的面
	std::vector<TopoDS_Face> facelist;
	TopExp_Explorer xp;
	xp.Init(topoShape, TopAbs_FACE);
	for (; xp.More(); xp.Next()) {
		TopoDS_Face aSurface = TopoDS::Face(xp.Current());
		BRepAdaptor_Surface ada(aSurface);
		if(ada.GetType() == GeomAbs_BSplineSurface)
			facelist.push_back(aSurface);
	}

	//2.对面进行偏移处理
	std::vector<std::vector<Handle(Geom_Curve)>> crvs;
	for (int i = 0; i < facelist.size(); i++)
	{
		std::vector<Handle(Geom_Curve)> geoms;
		OffsetFace(facelist[i], geoms);
		crvs.push_back(geoms);
	}
// 
// 	bool first = true;
// 	Part::TopoShape S;
// 	for (int i = 0; i < crvs.size(); i++)
// 	{
// 		for (int j = 0; j < crvs[i].size(); j++)
// 		{
// 			BRepBuilderAPI_MakeEdge mk(crvs[i][j]);
// 			
// 			if (first)
// 			{
// 				S = mk.Edge();
// 				first = false;
// 			}
// 			else
// 			{
// 				S.fuse(mk.Edge());
// 			}
// 		}
// 	}
// 	this->Shape.setValue(S);


	//3.偏移出来的线进行离散，形成点列
	std::vector<std::vector<std::vector<gp_Pnt>>> pnts;
	for (int i = 0; i < crvs.size(); i++)
	{
		std::vector<std::vector<gp_Pnt>> ppnts;
		for (int j = 0; j < crvs[i].size(); j++)
		{
			Handle(Geom_Curve) h = crvs[i][j];
			 
			TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(h);
			std::vector<gp_Pnt> pnts;
			toPolyline(aEdge, pnts, deflection, AngDeflectionRads);
			ppnts.push_back(pnts);
		}
		pnts.push_back(ppnts);
	}

	//将所有的点合成一个图形
	bool first = true;
	Part::TopoShape S;
	for (int i = 0; i < pnts.size(); i++)
	{
		BRepBuilderAPI_MakePolygon mk;
		for (int j = 0; j < pnts[i].size(); j++)
		{
			if (pnts[i][j].size() < 10)
				continue;

			for (int k = 0; k < pnts[i][j].size(); k++)
			{				
				mk.Add(pnts[i][j][k]);				
			}
		}
		if (first)
		{
			S = mk.Wire();
			first = false;
		}
		else
		{
			S.fuse(mk.Wire());
		}
	}
	this->Shape.setValue(S);


	return App::DocumentObject::StdReturn;
}


 
void MachiningOperation::toPolyline(TopoDS_Edge& aEdge, std::vector<gp_Pnt>& pnts, double LinDeflection, double AngDeflection)
{
	BRepAdaptor_Curve ada(aEdge);
	Standard_Real      theFirstParam = ada.FirstParameter();
	Standard_Real      theLastParam = ada.LastParameter();

	BRepMesh_GeomTool gt(ada, theFirstParam, theLastParam, LinDeflection, AngDeflection);
	int nPoints = gt.NbPoints();
	for (int i = 1; i < nPoints; i++)
	{
		const Standard_Real    theIsoParam = 0;
		Standard_Real         theParam = 0;
		gp_Pnt                thePoint;
		gp_Pnt2d              theUV;
		gt.Value(i, theIsoParam, theParam, thePoint, theUV);
		pnts.push_back(thePoint);
	}
}

void MachiningOperation::OffsetFace(TopoDS_Face& aSurface, std::vector<Handle(Geom_Curve)>& geoms)
{
	BRepAdaptor_Surface topofaceToSurface(aSurface);
	Handle(Geom_BSplineSurface)  hBSpline = topofaceToSurface.BSpline();

	double tol=Deviation.getValue();
	int num = 1 / tol;
	for (int i = 1; i < num; i++)
	{
		Handle(Geom_Curve) h = hBSpline->VIso(i* tol);
		geoms.push_back(h);
	}
}

void MachiningOperation::onChanged(const App::Property* prop)
{
	if (prop == &Solid ||
		prop == &Deviation ||
		prop == &AngularDeflection)
	{
		try
		{
			App::DocumentObjectExecReturn *r = this->recompute();
			delete r;
		}
		catch (...)
		{

		}
	}

	Part::Feature::onChanged(prop);
}
