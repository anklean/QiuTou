#ifndef PART_BALLCUTTER_H
#define PART_BALLCUTTER_H

#include <App/PropertyStandard.h>
#include <App/PropertyUnits.h>

#include <TopoDS_Face.hxx>
#include "Geometry.h"
#include "App/DocumentObject.h"
#include "TopoDS_Edge.hxx"
#include "PartFeature.h"

namespace Part
{

	class BallCutter : public Part::Feature
	{
		PROPERTY_HEADER(Part::BallCutter);

	public:
		BallCutter();

		App::PropertyInteger number; // 刀刃数量
		App::PropertyAngle angle; // 螺旋角

		App::PropertyAngle sketchAngle; // 截面夹角
		App::PropertyLength sketchLength; // 截面边长

		App::PropertyLength radius; // 球头半径
		App::PropertyLength ballHeight; // 球头总高

		App::PropertyLength pathStep; // 刀路歩距

		App::PropertyLength cylinderDiameter; // 圆柱部分直径
		App::PropertyLength cylinderLength; // 圆柱部分直径

		App::PropertyLength chamferEdgeA;  //倒角边A
		App::PropertyLength chamferEdgeB;  //倒角边B

		App::PropertyLength headerHeight;  // 顶部刀刃高度
		App::PropertyAngle headerAngle;  // 顶部刀刃角度

		/** @name methods override feature */
		//@{
		/// recalculate the feature
		App::DocumentObjectExecReturn *execute(void);
		short mustExecute() const;
		void onChanged(const App::Property* prop);
		/// returns the type name of the view provider
		const char* getViewProviderName(void) const {
			return "PartGui::ViewProviderPart";
		}
		//@}

	private:
		App::DocumentObjectExecReturn* makeLine(Base::Vector3d& startPt, Base::Vector3d& endPt, TopoDS_Edge& edge);
		void makeMainSketch(double h, double L, double angleIncre, double sketchY, TopoDS_Wire& wire);
		void makePloyline(std::vector<Base::Vector3d> points, TopoDS_Wire& wire);
		TopoDS_Edge makeEdge_Line(const Base::Vector3d& startPt, const Base::Vector3d& endPt);

		void makeMainBoby(TopoDS_Shape& body);
		void makeHeaderBoby(TopoDS_Shape& body);
		void makeHeaderCutBoby(TopoDS_Shape& body);


		TopoDS_Shape doRevolution(TopoDS_Wire wire, gp_Ax1 axis, double angle = 2 * M_PI);
		TopoDS_Shape doExtrusion(TopoDS_Wire wire, Base::Vector3d Dir, double start, double end);

		TopoDS_Shape doBoolean_Fuse(TopoDS_Shape BaseShape, TopoDS_Shape ToolShape);
		TopoDS_Shape doBoolean_Cut(TopoDS_Shape BaseShape, TopoDS_Shape ToolShape);
	private:
		std::vector<TopoDS_Edge>  lineList;
		TopTools_ListOfShape profiles;
	};

} //namespace Part


#endif // PART_BALLCUTTER_H
