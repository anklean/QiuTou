#ifndef CUSTOM_PATH_H
#define CUSTOM_PATH_H

#include "PreCompiled.h"
#include <App/PropertyStandard.h>
#include <App/PropertyUnits.h>

#include <TopoDS_Face.hxx>
#include "App/DocumentObject.h"
#include "TopoDS_Edge.hxx"
#include "Mod\Part\App\PartFeature.h"
#include "Base\Vector3D.h"
#include <vector>
#include "App/PropertyLinks.h"
#include "BRepAdaptor_Surface.hxx"

namespace Custom
{
	struct CustomExport NCStepInfo
	{
		int type; //0 ���� 1 ���� 2 �˵�
		std::string gcode;
		double speed;//������
		Base::Vector3d Point;
		Base::Vector3d Normal;
	};

	class CustomExport PathObject : public Part::Feature
	{
		PROPERTY_HEADER(Custom::PathObject);

	public:
		typedef std::vector<NCStepInfo> PathPointList;
	public:
		PathObject();

		App::PropertyLinkSub Curve;//18����
		App::PropertyLinkSub Face;//����
		App::PropertyString fileName;

		App::PropertyEnumeration PathType;//�ӹ�����  0:�����ر߽�����
		App::PropertyEnumeration UVType;//�߽����� 0:U ; 1:V

		App::PropertyString ToolSetNumber;//���߱��

		//App::PropertyBool ToolSetFlag;//����ǰ�Ե�
		App::PropertySpeed Speed;//ת��

		App::PropertyLength SafeHeight;//��ȫ�߶�
		App::PropertyStringList BeforeAdditionGCode;//·��ִ��ǰ����G����
		App::PropertyString  StepAdditionGCode;//·��ִ��ʱ����G����
		App::PropertyStringList AfterAdditionGCode;//·��ִ�к󸽼�G����

		App::PropertyLength Tolerance;   //U�����Tolerance == ���ߵİ뾶
		//App::PropertyLength VTolerance;  //V�����Tolerance

		//App::PropertyVector ToolDirection;//���ߵķ���
		/** @name methods override feature */
		//@{
		/// recalculate the feature
		App::DocumentObjectExecReturn *execute(void);
		short mustExecute() const;
		void onChanged(const App::Property* prop);
		/// returns the type name of the view provider
		const char* getViewProviderName(void) const {
			return "CustomGui::ViewProviderCustomPath";
		}
		//@}

		PathPointList getPointList() const { return m_PointList; }
		void setPointList(PathPointList& val) { m_PointList = val; }
	private:
		void buildCylinderSurfacePath(const TopoDS_Face& face);
		TopoDS_Shape buildPath(const TopoDS_Face& face);
		void buildPath(TopoDS_Edge& aEdge, TopoDS_Face& aFace);
		Standard_Boolean getNormal(BRepAdaptor_Surface& theSurface, Standard_Real& theParamU, Standard_Real& theParamV, gp_Pnt& thePoint, gp_Dir& theNormal);
	private:
		PathPointList  m_PointList;

		static const char* PathTypeEnums[];
		static const char* UVTypeEnums[];
	};

} //namespace Custom


#endif // CUSTOM_PATH_H

