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

		App::PropertyInteger number; // ��������
		App::PropertyAngle angle; // ������

		App::PropertyAngle sketchAngle; // ����н�
		App::PropertyLength sketchLength; // ����߳�

		App::PropertyLength radius; // ��ͷ�뾶
		App::PropertyLength ballHeight; // ��ͷ�ܸ�

		App::PropertyLength pathStep; // ��·�i��

		App::PropertyLength cylinderDiameter; // Բ������ֱ��
		App::PropertyLength cylinderLength; // Բ������ֱ��

		App::PropertyLength chamferEdgeA;  //���Ǳ�A
		App::PropertyLength chamferEdgeB;  //���Ǳ�B

		App::PropertyLength headerHeight;  // �������и߶�
		App::PropertyAngle headerAngle;  // �������нǶ�

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
