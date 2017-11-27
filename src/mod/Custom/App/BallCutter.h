#ifndef CUSTOM_BALLCUTTER_H
#define CUSTOM_BALLCUTTER_H

#include "PreCompiled.h"
#include <App/PropertyStandard.h>
#include <App/PropertyUnits.h>

#include <TopoDS_Face.hxx>
#include "Mod\Part\App\Geometry.h"
#include "App/DocumentObject.h"
#include "TopoDS_Edge.hxx"
#include "Mod\Part\App\PartFeature.h"

namespace Custom
{
	class CustomExport BallCutter : public Part::Feature
	{
		PROPERTY_HEADER(Custom::BallCutter);

	public:
		BallCutter();

		App::PropertyLength A; //球头直径
		App::PropertyAngle B;//螺旋角
		App::PropertyLength C; //砂轮直径C
		App::PropertyAngle D; //弧开始角
		App::PropertyAngle E; //弧结束角
		App::PropertyLength F; //安全高度
		App::PropertyLength G; //下切深度
		App::PropertyAngle H; //前角
		App::PropertyLength I;//毛坯头部余量
		App::PropertyInteger J; //组数
		App::PropertyInteger K;//每组刃数
		App::PropertyInteger L;//总刃数

		App::PropertyLength CylinderLength; //柱面长度
		App::PropertyLength CylinderDiameter;//柱面直径
		App::PropertyLength CylinderChamferA;//柱面倒角长度A
		App::PropertyLength CylinderChamferB;//柱面倒角长度B
		
		App::PropertyLength GrindingWheelThick; //砂轮的厚度
		App::PropertyAngle GrindingWheelAngle;//砂轮的角度

		App::PropertyBool CheckSpine;//切换引导线
		/** @name methods override feature */
		//@{
		/// recalculate the feature
		App::DocumentObjectExecReturn *execute(void);
		short mustExecute() const;
		void onChanged(const App::Property* prop);
	
		/// returns the type name of the view provider
		const char* getViewProviderName(void) const {
			return "CustomGui::ViewProviderBallCutter";
		}
		//@}

		TopoDS_Compound getSpineEdge() const { return m_SpineComps; }
		void setSpineEdge(TopoDS_Compound val) { m_SpineComps = val; }
	private:
		App::DocumentObjectExecReturn* makeLine(Base::Vector3d& startPt, Base::Vector3d& endPt, TopoDS_Edge& edge);
		void makeMainSketch(double h, double L, double angleIncre, double sketchY, TopoDS_Wire& wire);
		void makePloyline(std::vector<Base::Vector3d> points, TopoDS_Wire& wire);
		TopoDS_Edge makeEdge_Line(const Base::Vector3d& startPt, const Base::Vector3d& endPt);

		void makeMainBoby(TopoDS_Shape& body,double ang);
		void makeHeaderBoby(TopoDS_Shape& body);
		TopoDS_Shape cutHeader(TopoDS_Shape& baseShape);
		void makeHeaderCutBoby(TopoDS_Shape& body);

		TopoDS_Shape makeCutterBody(TopoDS_Shape& baseShape, TopoDS_Shape& CutBody, TopoDS_Edge& yindaoxian);
		TopoDS_Shape doRevolution(TopoDS_Wire wire, gp_Ax1 axis, double angle = 2 * M_PI);
		TopoDS_Shape doExtrusion(TopoDS_Wire wire, Base::Vector3d Dir, double start, double end);

		TopoDS_Shape doSweep(TopoDS_Wire wire, TopoDS_Wire path);

		TopoDS_Shape doBoolean_Fuse(TopoDS_Shape BaseShape, TopoDS_Shape ToolShape);
		TopoDS_Shape  doBoolean_Cut(const TopoDS_Shape&  BaseShape, const TopoDS_Shape&  ToolShape);
		TopoDS_Shape doBoolean_Section(TopoDS_Shape BaseShape, TopoDS_Shape ToolShape);
		TopoDS_Shape doSweep2(TopoDS_Wire wire, TopoDS_Wire path);

		double getBallHeight();
		TopoDS_Shape validateFace(const TopoDS_Shape& shape);
	private:
		std::vector<TopoDS_Edge>  lineList;
		TopTools_ListOfShape profiles;

		TopoDS_Edge m_SpineEdge;
		TopoDS_Compound m_SpineComps;
	};

} //namespace Custom


#endif // CUSTOM_BALLCUTTER_H
