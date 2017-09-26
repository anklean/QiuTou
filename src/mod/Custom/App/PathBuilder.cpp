
#include "PathBuilder.h"
#include "GC_MakeLine.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"
#include "gp_Pnt.hxx"
#include "gp_XYZ.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "BRepMesh_GeomTool.hxx"
#include "TopoDS_Edge.hxx"
#include "TopoDS.hxx"
#include "TopExp_Explorer.hxx"
#include "gp_Lin.hxx"
#include "GCPnts_AbscissaPoint.hxx"


void BuildPath(Part::TopoShape& shape, std::string asub, std::vector<Custom::NCStepInfo>& pathPointList)
{
	TopoDS_Shape  topoFace = shape.getSubShape(asub.c_str());

	Base::Matrix4D mtx=shape.getTransform();
	mtx.transpose();
	
	Base::BoundBox3d bbox = shape.getBoundBox();
	Base::Vector3d center = bbox.CalcCenter();
	
	double d[3] = { 0 };
	d[0] = fabs(bbox.MinX - bbox.MaxX);
	d[1] = fabs(bbox.MinY - bbox.MaxY);
	d[2] = fabs(bbox.MinZ - bbox.MaxZ);
	double* dmax = std::max_element(d, d + 2);
	
	std::multimap<double, TopoDS_Edge> mapedges;
	std::vector<TopoDS_Edge> edgeListddd;
	std::vector<TopoDS_Edge>edgeList;
	TopoDS_Edge aEdge;
	TopExp_Explorer Ex;
	for (Ex.Init(topoFace, TopAbs_EDGE); Ex.More(); Ex.Next())
	{
		aEdge = TopoDS::Edge(Ex.Current());
		BRepAdaptor_Curve ad(aEdge);
		double len = GCPnts_AbscissaPoint::Length(ad);
		mapedges.insert(std::pair<double, TopoDS_Edge>(len, aEdge));
		if (ad.GetType() == GeomAbs_CurveType::GeomAbs_BSplineCurve)
		{
			if (len > (*dmax /2.0))
				edgeList.push_back(aEdge);
		}
		else if (ad.GetType() == GeomAbs_CurveType::GeomAbs_Line)
		{
			edgeListddd.push_back(aEdge);
		}
	}

	TopoDS_Edge theEdge;
	double mindis = DBL_MAX;
	double dis = 0;
	for (int i = 0; i < edgeList.size(); i++)
	{
		TopoDS_Edge aEdge = edgeList[i];
		BRepAdaptor_Curve curveAdaptor(aEdge);
		Standard_Real first = curveAdaptor.FirstParameter();
		Standard_Real last = curveAdaptor.LastParameter();
		gp_Pnt sp = curveAdaptor.Value(first);
		gp_Pnt ep = curveAdaptor.Value(last);

		Base::Vector3d vsp(sp.X(), sp.Y(), sp.Z());
		dis = (vsp - center).Length();
		if (dis < mindis)
		{
			mindis = dis;
			theEdge = aEdge;
		}
		Base::Vector3d vep(ep.X(), ep.Y(), ep.Z());
		dis = (vep - center).Length();
		if (dis < mindis)
		{
			mindis = dis;
			theEdge = aEdge;
		}
	}


	//theEdge是要加工的线
	double deflection = 0.01;
	double AngDeflectionRads = 0.1;
	BRepAdaptor_Curve curveAdaptor(theEdge);
	Standard_Real first = curveAdaptor.FirstParameter();
	Standard_Real last = curveAdaptor.LastParameter();
	gp_Pnt sp = curveAdaptor.Value(first);
	gp_Pnt ep = curveAdaptor.Value(last);
	
	Base::Vector3d rBase, rNormal;
	gp_Lin center_line;

	d[0] = fabs(sp.X() - ep.X());
	d[1] = fabs(sp.Y() - ep.Y());
	d[2] = fabs(sp.Z() - ep.Z());
	int nFlag = -1;
	if (d[0] > d[1])
	{
		if (d[0] > d[2]) nFlag = 1;
		else
			nFlag = 3;
	}
	else
	{
		if (d[1] > d[2]) nFlag = 2;
		else nFlag = 3;
	}
	if (nFlag ==1) //X
	{
		bbox.CalcPlane(Base::BoundBox3d::LEFT, rBase, rNormal);
		center_line = gp_Lin(gp_Pnt(center.x, center.y, center.z), gp_Dir(1, 0, 0));
	}
	else if (nFlag == 2)//Y
	{
		bbox.CalcPlane(Base::BoundBox3d::BOTTOM, rBase, rNormal);
		center_line = gp_Lin(gp_Pnt(center.x, center.y, center.z), gp_Dir(0, 1, 0));
	}
	else if (nFlag == 3)//Z
	{
		bbox.CalcPlane(Base::BoundBox3d::BACK, rBase, rNormal);
		center_line = gp_Lin(gp_Pnt(center.x, center.y, center.z), gp_Dir(0, 0, 1));
	}
	
	BRepMesh_GeomTool crvMeshTools(curveAdaptor, first, last, deflection, AngDeflectionRads);
		
	int num = crvMeshTools.NbPoints();
	for (int i = 1; i < num + 1; i++)
	{
		Standard_Real         theIsoParam = 0;
		Standard_Real         theParam;
		gp_Pnt             thePoint;
		gp_Pnt2d             theUV;
		crvMeshTools.Value(i, theIsoParam, theParam, thePoint, theUV);

		gp_Dir nrm;
		int nret = getNormal2(thePoint, center_line, nrm);
		if (nret != 0)
		{
		}

		Custom::NCStepInfo si = { 0 };
		si.type = 0;
		si.gcode = "";
		si.speed = 0;
		si.Point = Base::Vector3d(thePoint.X(), thePoint.Y(), thePoint.Z());
		si.Normal = Base::Vector3d(nrm.X(), nrm.Y(), nrm.Z());
		pathPointList.push_back(si);//从原点开始
	}
}




int getNormal2(gp_Pnt& pnt, gp_Lin& center_line, gp_Dir& normal)
{
	Handle(Geom_Line) hGeomLine = GC_MakeLine(center_line);

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