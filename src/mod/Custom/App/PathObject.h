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
		int type; //0 进刀 1 切削 2 退刀
		std::string gcode;
		double speed;//进给率
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

		App::PropertyLinkSub Curve;//18度线
		App::PropertyLinkSub Face;//球面
		App::PropertyString fileName;

		App::PropertyEnumeration PathType;//加工类型  0:面上沿边界往返
		App::PropertyEnumeration UVType;//边界类型 0:U ; 1:V

		App::PropertyString ToolSetNumber;//刀具编号

		//App::PropertyBool ToolSetFlag;//进刀前对刀
		App::PropertySpeed Speed;//转速

		App::PropertyLength SafeHeight;//安全高度
		App::PropertyStringList BeforeAdditionGCode;//路径执行前附加G代码
		App::PropertyString  StepAdditionGCode;//路径执行时附加G代码
		App::PropertyStringList AfterAdditionGCode;//路径执行后附加G代码

		App::PropertyLength Tolerance;   //U方向的Tolerance == 刀具的半径
		//App::PropertyLength VTolerance;  //V方向的Tolerance

		//App::PropertyVector ToolDirection;//刀具的方向
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

