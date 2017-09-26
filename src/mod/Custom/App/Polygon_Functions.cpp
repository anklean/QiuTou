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
// �ж�һ��������ǲ���͹�����
//http://www.cnblogs.com/grandyang/p/6146986.html
//����ε����ʣ��Ҵ��쳯�ĳ��м��ξ�Ӧ�������������ɣ����˵�ȥ��ڡ��������еĶ���Ƕ�������180�ȡ���ô���Ǹ���ο�����֤��һ���ص��أ�ѧ�������ͼ��ѧ������ͼ����Ŀ�Ӧ�öԼ��㷨��normal����İ���ɣ������curve�ķ������Ƿǳ���Ҫ���ֶΣ�һ���������߿�����ɢ���������ɢ����ɣ������ڵ����������������ĵ�λ�����ǿ���������������ɵ�һС�����ߵķ��߷��򣬶�͹����ε�ÿ���������ڵ�ķ���������Ӧ����ͬ��Ҫôͬ����Ҫôͬ������ô����ֻҪ����ÿ���㣬Ȼ��ȡ������Χ����������㷨�߷���Ȼ���֮ǰ�ķ���Աȣ������һ����ֱ�ӷ���false����������Ҫ�ر�ע�ⷨ����Ϊ0����������ĳһ����ķ����������Ϊ0����ô��ȷ��pre�ͻᱻ����Ϊ0�������������෴�ķ�����޷����������������ǶԼ������������Ϊ0�����ֱ���������ɣ��μ��������£�
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