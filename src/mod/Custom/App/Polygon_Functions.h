#pragma once

#include <vector>
#include "gp_Pnt2d.hxx"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/geometry/index/rtree.hpp>

#include <cmath>
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>
#include "TopoDS_Face.hxx"


typedef  boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> BoostPoint;
typedef  boost::geometry::model::box<BoostPoint> box;
typedef  boost::geometry::model::polygon<BoostPoint, false, false> BoostPolygon; // ccw, open polygon

BoostPolygon vectexUVToPolygon(const TopoDS_Face& f);
bool isInPolygon(gp_Pnt2d pnt, BoostPolygon& poly);
bool isConvex(BoostPolygon& poly);

