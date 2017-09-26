#include "BRepOffset_OffsetEdgeOnFace.h"
#include <vector>
#include "BRepAdaptor_Surface.hxx"
#include "BRepAdaptor_HSurface.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "GeomAdaptor_Curve.hxx"
#include "Geom_BSplineCurve.hxx"
#include "GeomAPI_ProjectPointOnSurf.hxx"
#include "Quantity_Parameter.hxx"
#include "GeomAPI_PointsToBSpline.hxx"
#include "TColgp_Array1OfPnt.hxx"
#include "BRepMesh_GeomTool.hxx"
#include "Bnd_Box.hxx"
#include "BRepBndLib.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"
#include "GeomLProp_CLProps.hxx"
#include "GC_MakeLine.hxx"
#include "GC_MakeSegment.hxx"
#include "BRepIntCurveSurface_Inter.hxx"
#include "BRepAdaptor_Curve.hxx"
#include "IntCurveSurface_IntersectionPoint.hxx"


struct OffPnt
{
	gp_Pnt Point;//点坐标
	gp_Pnt PointOnSurface;//点坐标
	gp_Dir Normal;//点在面上的法线方向
	gp_Dir Tangent;//点在线上的切线方向
	gp_Vec OffsetDir;//法线方向和切线方向的正交方向

	double para;//点在线上的参数
	double u, v;//点在面上的参数
};
struct OffRow
{
	std::vector<OffPnt> pntlist;
	Handle(Geom_BSplineCurve) BSpline;
};
struct BRepOffset_OffsetEdgeOnFaceP
{
	//for input
	BRepAdaptor_Surface srfAdapter;
	Handle(BRepAdaptor_HSurface) hSrfAapter;

	BRepAdaptor_Curve crvAdapter;
	BRepMesh_GeomTool* crvMeshTools;

	TopoDS_Face Face;
	TopoDS_Edge Edge;
	double offdis;
	int offnum;

	//for result
	std::vector<OffRow> Result;
};

BRepOffset_OffsetEdgeOnFace::BRepOffset_OffsetEdgeOnFace()
{
	d = new BRepOffset_OffsetEdgeOnFaceP;
}
BRepOffset_OffsetEdgeOnFace::BRepOffset_OffsetEdgeOnFace(const TopoDS_Face& F, const TopoDS_Edge& E, const Standard_Real Offset, const Standard_Integer Num)
{
	d = new BRepOffset_OffsetEdgeOnFaceP;
	d->Face = F;
	d->Edge = E;
	d->offdis = Offset;
	d->offnum = Num;
	d->srfAdapter.Initialize(F);
	d->crvAdapter.Initialize(E);
	d->hSrfAapter = new BRepAdaptor_HSurface(d->srfAdapter);
	
	myDone = Perform();
}

BRepOffset_OffsetEdgeOnFace::~BRepOffset_OffsetEdgeOnFace()
{
}

void ProjectPointOnSurface(OffPnt& aPnt, BRepOffset_OffsetEdgeOnFaceP* d)
{
	// 将第一条线投影到曲面上去
	GeomAPI_ProjectPointOnSurf  ppos(aPnt.Point, d->srfAdapter.Surface().Surface());

#if 1
	int pnt_num = ppos.NbPoints();
	if (pnt_num > 1)
	{
		aPnt.PointOnSurface = ppos.NearestPoint();
		ppos.LowerDistanceParameters(aPnt.u, aPnt.v);
	}
// 	std::vector<gp_Pnt> ppnts;
// 	std::vector<double> ppntdist;
// 	std::vector<std::pair<double, double>> ppntuvs;
// 	for (int kk = 1; kk < pnt_num; kk++)
// 	{
// 		ppnts.push_back(ppos.Point(kk));
// 		ppntdist.push_back(ppos.Distance(kk));
// 		double u = 0, v = 0;
// 		ppos.Parameters(kk, u, v);
// 		ppntuvs.push_back(std::pair<double, double>(u, v));
// 
// 
// 	}
#else
	//ppos.LowerDistanceParameters(aPnt.u, aPnt.v);
	//BRepMesh_GeomTool::Normal(d->hSrfAapter, aPnt.u, aPnt.v, aPnt.PointOnSurface, aPnt.Normal);
#endif

}

bool BRepOffset_OffsetEdgeOnFace::Perform()
{
	// 1.calculating the deflection value
	Bnd_Box bounds;
	BRepBndLib::Add(d->Face, bounds);
	bounds.SetGap(0.0);
	Standard_Real xMin, yMin, zMin, xMax, yMax, zMax;
	bounds.Get(xMin, yMin, zMin, xMax, yMax, zMax);
	Standard_Real deflection = ((xMax - xMin) + (yMax - yMin) + (zMax - zMin)) / 300.0 *
		1;
	Standard_Real AngDeflectionRads = 1 / 180.0 * M_PI;

	//2. polyline the edge
	Standard_Real first = d->crvAdapter.FirstParameter();
	Standard_Real last = d->crvAdapter.LastParameter();
	d->crvMeshTools = new BRepMesh_GeomTool(d->crvAdapter, first, last, deflection, AngDeflectionRads);

	
	OffRow aRow;
	int num = d->crvMeshTools->NbPoints();
	for (int i = 1; i < num + 1; i++)
	{
		OffPnt aPnt ;

		 Standard_Real    theIsoParam = 0;
		//Standard_Real         theParam;
		//gp_Pnt             thePoint;
		gp_Pnt2d             theUV;
		d->crvMeshTools->Value(i, theIsoParam, aPnt.para, aPnt.Point, theUV);

		ProjectPointOnSurface(aPnt, d);

		aRow.pntlist.push_back(aPnt);
	}

	//3.do offset
	
	d->Result.push_back(aRow);
	for (int i = 1; i < 20; i++)
	{
		doOffset();
	}
	//4. result
// 	if (res.size() > 0)
// 		{
// 			for (int i = 0; i < res.size(); i++)
// 			{
// 				BRepBuilderAPI_MakePolygon aPol;
// 				std::vector<gp_Pnt > o = res[i];
// 				for (int j = 0; j < o.size(); j++)
// 				{
// 					aPol.Add(o[j]);
// 					//	aPol.Add(gp_Pnt(ppth[j].X / 10000.0, ppth[j].Y / 10000.0, ppth[j].Z / 10000.0));
// 				}
// 
// 				TopoDS_Wire aWire = aPol.Wire();
// 				elist.push_back(aWire);
// 			}
// 		}

	return true;
}

bool BRepOffset_OffsetEdgeOnFace::IsDone()
{
	return myDone;
}

int BRepOffset_OffsetEdgeOnFace::NbEdges()
{
	if (myDone = true)
	{
		return d->Result.size();
	}
	else return 0;
}

void BRepOffset_OffsetEdgeOnFace::Value(int nIndex, std::vector<gp_Pnt>& pntlist)
{
	if (nIndex < d->Result.size() && nIndex >=0)
	{
		OffRow row=	d->Result[nIndex];
		for (int i = 0; i < row.pntlist.size(); i++)
		{
			pntlist.push_back(row.pntlist[i].PointOnSurface);
		}
	}
}

void BRepOffset_OffsetEdgeOnFace::Value(int nIndex, std::vector<std::pair<gp_Pnt, gp_Dir>>& pntlist)
{
	if (nIndex < d->Result.size() && nIndex >=0)
	{
		OffRow row = d->Result[nIndex];
		for (int i = 0; i < row.pntlist.size(); i++)
		{
			pntlist.push_back(std::pair<gp_Pnt, gp_Dir>(row.pntlist[i].PointOnSurface, row.pntlist[i].Normal));
		}
	}
}

//判断两点构成的线段和面的关系
TopAbs_State isIntersectWithFace(GeomAdaptor_Curve& adaptCrv, gp_Pnt& crossPoint, BRepOffset_OffsetEdgeOnFaceP* d)
{
	BRepIntCurveSurface_Inter intersection;
	
	intersection.Init(d->Face, adaptCrv, 0.001);
	bool bMore = intersection.More();
	if (bMore)
	{
		if (intersection.State() != TopAbs_UNKNOWN)
		{
			crossPoint = intersection.Point().Pnt();
		}

		return intersection.State();
	}
	else
	{
		return TopAbs_UNKNOWN;
	}

}

void trimStart(std::vector<OffPnt>& pntlist ,BRepOffset_OffsetEdgeOnFaceP* d)
{
	if (pntlist.size() < 2)
		return;

	//首先处理开始的地方
	//1.判断第一个点和第二个点是否与面的边界相交

	Handle(Geom_TrimmedCurve) geom = GC_MakeSegment(pntlist[0].Point, pntlist[1].Point);
	GeomAdaptor_Curve crvAdapter(geom);
	//GC_MakeLine mkline(pntlist[0].PointOnSurface, pntlist[1].PointOnSurface);

		gp_Pnt crossPoint;
		TopAbs_State state = isIntersectWithFace(crvAdapter, crossPoint, d);
		if (state == TopAbs_ON)
		{
			//相交
			//将交点作为第一个点
			pntlist[0].Point = crossPoint;
			ProjectPointOnSurface(pntlist[0], d);
		}
		else if (state == TopAbs_IN)//在曲面的内部
		{
			// 			//求线和面的交点，作为第一个点
			// 			gp_XYZ xyz = pntlist[1].Point.XYZ() - pntlist[0].Point.XYZ();
			// 			gp_Dir dir(xyz.Normalized());
			// 			gp_Lin lin(pntlist[0].Point, dir);
			// 			intersection.Init(f, lin, 0.01);
			// 			pntlist[0].Point = intersection.Point().Pnt();
			OffPnt ppnt;
			ppnt.Point = crossPoint;
			ProjectPointOnSurface(ppnt, d);
			pntlist.insert(pntlist.begin(), ppnt);
		}
		else if (state == TopAbs_OUT)
		{
			//都在曲面的外边，
			//继续判断第二个点和第三个点
		}
		else
		{
			//TopAbs_UNKNOWN
		}
	
}


void trimEnd(std::vector<OffPnt>& pntlist, BRepOffset_OffsetEdgeOnFaceP* d)
{
	if (pntlist.size() < 2)
		return;
	
	//首先处理最后的地方
	//1.判断倒数第一个点和倒数第二个点是否与面的边界相交
	{
		int last1 = pntlist.size()-1;
		int last2 = last1 - 1;

		Handle(Geom_TrimmedCurve) geom = GC_MakeSegment(pntlist[last2].PointOnSurface, pntlist[last1].PointOnSurface);
		GeomAdaptor_Curve crvAdapter(geom);

		gp_Pnt crossPoint;
		TopAbs_State state = isIntersectWithFace(crvAdapter, crossPoint, d);
		if (state == TopAbs_ON)//相交
		{
			pntlist[last1].PointOnSurface = crossPoint;
			//ProjectPointOnSurface(pntlist[last1], d);
		}
		else if (state == TopAbs_IN)//在曲面的内部
		{
			// 			//求线和面的交点，作为第一个点
			// 			gp_XYZ xyz = pntlist[1].Point.XYZ() - pntlist[0].Point.XYZ();
			// 			gp_Dir dir(xyz.Normalized());
			// 			gp_Lin lin(pntlist[0].Point, dir);
			// 			intersection.Init(f, lin, 0.01);
			// 			pntlist[0].Point = intersection.Point().Pnt();
// 			OffPnt ppnt;
// 			ppnt.PointOnSurface = crossPoint;
// 
// 			//ProjectPointOnSurface(ppnt, d);
// 			pntlist.push_back( ppnt);
			//都在曲面的外边，//判断倒数第二个点和倒数第三个点
			pntlist.erase(pntlist.end() - 1);
			trimEnd(pntlist, d);
		}
		else if (state == TopAbs_OUT)
		{
			//都在曲面的外边，//判断倒数第二个点和倒数第三个点
			pntlist.erase(pntlist.end() - 1);
			trimEnd(pntlist, d);
		}
		else
		{
			pntlist.erase(pntlist.end() - 1);
			trimEnd(pntlist, d);
		}
	}
}

void BRepOffset_OffsetEdgeOnFace::doOffset()
{
	if (d->Result.size() >= 1)
	{
		std::vector<OffRow>::iterator it = d->Result.end() - 1;
		//将这些点连成曲线

		TColgp_Array1OfPnt pnts(1, (*it).pntlist.size());
		for (int i = 0; i < (*it).pntlist.size(); i++)
		{
			pnts.SetValue(i + 1, (*it).pntlist[i].PointOnSurface);
		}

		try{
			GeomAPI_PointsToBSpline  pnt2Spline(pnts);
			(*it).BSpline = pnt2Spline.Curve();
		}
		catch (...)
		{
			return ;
		}

		GeomAdaptor_Curve crv_adapt((*it).BSpline);
		OffRow row;
		for (int i = 0; i < (*it).pntlist.size(); i++)
		{
			OffPnt& srcPnt = (*it).pntlist[i];
			OffPnt destPnt;
			
			GeomAPI_ProjectPointOnCurve ppoc(srcPnt.PointOnSurface, (*it).BSpline);
			int pnt_num2 = ppoc.NbPoints();		
			ppoc.Parameter(1, srcPnt.para);

			GeomLProp_CLProps prop(crv_adapt.Curve(), srcPnt.para, 1, Precision::Confusion());
			if (prop.IsTangentDefined()) {
				prop.Tangent(srcPnt.Tangent);
			}

			srcPnt.OffsetDir = srcPnt.Normal.Crossed(srcPnt.Tangent);
			gp_XYZ xyz = srcPnt.PointOnSurface.XYZ() + srcPnt.OffsetDir.XYZ()*(d->offdis);

			destPnt.Point = gp_Pnt(xyz);

			ProjectPointOnSurface(destPnt, d);

			row.pntlist.push_back(destPnt);
		}

		//处理开始和结束的点
		{
		//	trimStart(row.pntlist, d);
		//	trimEnd(row.pntlist, d);
		}
// 		for (int i = 0; i < row.pntlist.size(); i++)
// 		{
// 			gp_Pnt pp = row.pntlist[i].Point;
// 			GeomAPI_ProjectPointOnSurf  ppos2(pp, d->srfAdapter.Surface().Surface());
// 			row.pntlist[i].Point = ppos2.NearestPoint();
// 		}
		
		d->Result.push_back(row);
	}
}
