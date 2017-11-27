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

		App::PropertyLength A; //��ͷֱ��
		App::PropertyAngle B;//������
		App::PropertyLength C; //ɰ��ֱ��C
		App::PropertyAngle D; //����ʼ��
		App::PropertyAngle E; //��������
		App::PropertyLength F; //��ȫ�߶�
		App::PropertyLength G; //�������
		App::PropertyAngle H; //ǰ��
		App::PropertyLength I;//ë��ͷ������
		App::PropertyInteger J; //����
		App::PropertyInteger K;//ÿ������
		App::PropertyInteger L;//������

		App::PropertyLength CylinderLength; //���泤��
		App::PropertyLength CylinderDiameter;//����ֱ��
		App::PropertyLength CylinderChamferA;//���浹�ǳ���A
		App::PropertyLength CylinderChamferB;//���浹�ǳ���B
		
		App::PropertyLength GrindingWheelThick; //ɰ�ֵĺ��
		App::PropertyAngle GrindingWheelAngle;//ɰ�ֵĽǶ�

		App::PropertyBool CheckSpine;//�л�������
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
