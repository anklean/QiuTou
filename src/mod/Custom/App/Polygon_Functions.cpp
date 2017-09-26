#include "Polygon_Functions.h"
#include "TopExp_Explorer.hxx"
#include "TopoDS_Vertex.hxx"
#include "TopoDS.hxx"
#include "BRep_Tool.hxx"


BoostPolygon toBoostPolygon(std::vector<gp_Pnt2d>& poly)
{
	BoostPolygon bpoly;
	for (int i = 0; i < poly.size(); i++)
	{
		gp_Pnt2d pnt = poly[i];
		bpoly.outer().push_back(BoostPoint(pnt.X(), pnt.Y()));
	}
	return bpoly;
}

BoostPolygon vectexUVToPolygon(const TopoDS_Face& f)
{
	TopoDS_Vertex v;
	TopExp_Explorer Ex;
	std::vector<gp_Pnt2d> pntlist;

	for (Ex.Init(f, TopAbs_VERTEX); Ex.More(); Ex.Next())
	{
		v = TopoDS::Vertex(Ex.Current());
		gp_Pnt2d pnt = BRep_Tool::Parameters(v, f);
		pntlist.push_back(pnt);
	}

	return toBoostPolygon(pntlist);
}

bool isInPolygon(gp_Pnt2d pnt, BoostPolygon& poly)
{
	BoostPoint bpnt(pnt.X(), pnt.Y());

	// Declare a stream and an SVG mapper
	std::ofstream svg("d:\\cccc.svg");
	boost::geometry::svg_mapper<BoostPoint> mapper(svg, 400, 400);

	// Add geometries such that all these geometries fit on the map
	mapper.add(bpnt);
	mapper.add(poly);

	// Draw the geometries on the SVG map, using a specific SVG style
	mapper.map(bpnt, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2", 5);
	mapper.map(poly, "fill-opacity:0.3;fill:rgb(51,51,153);stroke:rgb(51,51,153);stroke-width:2");
	
	//BoostPolygon bpoly = toBoostPolygon(poly);
	return boost::geometry::covered_by(bpnt, poly);
	//return boost::geometry::within(bpnt, poly);
}


//
// 判断一个多边形是不是凸多边形
//http://www.cnblogs.com/grandyang/p/6146986.html
//多边形的性质，我大天朝的初中几何就应该有所涉猎啦吧，忘了的去面壁。就是所有的顶点角都不大于180度。那么我们该如何快速验证这一个特点呢，学过计算机图形学或者是图像处理的课应该对计算法线normal并不陌生吧，计算的curve的法向量是非常重要的手段，一段连续曲线可以离散看成许多离散点组成，而相邻的三个点就是最基本的单位，我们可以算由三个点组成的一小段曲线的法线方向，而凸多边形的每个三个相邻点的法向量方向都应该相同，要么同正，要么同负。那么我们只要遍历每个点，然后取出其周围的两个点计算法线方向，然后跟之前的方向对比，如果不一样，直接返回false。这里我们要特别注意法向量为0的情况，如果某一个点的法向量算出来为0，那么正确的pre就会被覆盖为0，后面再遇到相反的法向就无法检测出来，所以我们对计算出来法向量为0的情况直接跳过即可，参见代码如下：
//
bool isConvex(BoostPolygon& poly)
{
	long long n = poly.outer().size(), pre = 0, cur = 0;

	for (int i = 0; i < n; ++i) {		
		BoostPoint pnt_i0 = poly.outer()[i];
		BoostPoint pnt_i1 = poly.outer()[(i + 1) % n];
		BoostPoint pnt_i2 = poly.outer()[(i + 2) % n];
		int dx1 = pnt_i1.get<0>() - pnt_i0.get<0>();
		int dx2 = pnt_i2.get<0>() - pnt_i0.get<0>();
		int dy1 = pnt_i1.get<1>() - pnt_i0.get<1>();
		int dy2 = pnt_i2.get<1>() - pnt_i0.get<1>();
		cur = dx1 * dy2 - dx2 * dy1;
		if (cur != 0) {
			if (cur * pre < 0) return false;
			else pre = cur;
		}
	}
	return true;
}