#ifndef CUSTOM_NCOutPutter_H
#define CUSTOM_NCOutPutter_H

#include "PreCompiled.h"
#include <App/PropertyStandard.h>
#include <App/PropertyUnits.h>

#include <TopoDS_Face.hxx>
#include "App/DocumentObject.h"
#include "TopoDS_Edge.hxx"
#include "Mod\Part\App\PartFeature.h"
#include "App/PropertyFile.h"
#include "PathObject.h"
#include <iosfwd>

namespace Custom
{

	class NCOutPutter : public Part::Feature
	{
		PROPERTY_HEADER(Custom::NCOutPutter);

	public:
		NCOutPutter();

		App::PropertyString OutputFile;		
		App::PropertyLinkList PathList;
		//App::PropertyLinkSubList PathList;
		App::PropertyStringList BeforeGcode;
		App::PropertyStringList AfterGcode;

		App::PropertyLength SafeHeight;//安全高度
		App::PropertyLength Speed;//速度
		App::PropertyLength DeltaX;//工件坐标与机床坐标的X差值
		App::PropertyString ToolNum;

		/** @name methods override feature */
		//@{
		/// recalculate the feature
		App::DocumentObjectExecReturn *execute(void);
		short mustExecute() const;
		void onChanged(const App::Property* prop);
		/// returns the type name of the view provider
		const char* getViewProviderName(void) const {
			return "CustomGui::ViewProviderNC";
		}
		std::vector<Custom::PathObject::PathPointList> getNclist() const { return m_nclist; }
		void setNclist(std::vector<Custom::PathObject::PathPointList>& val) { m_nclist = val; }
		//@}
	
	protected:
		double getA(Base::Vector3d nrm, Base::Placement place);
		double getA(Base::Vector3d nrm, Base::Placement place, double B);
		double getA(Base::Vector3d pnt1, Base::Vector3d pnt2, Base::Placement place, double B);
		double getB(Base::Vector3d nrm, Base::Placement place);
		Base::Vector3d adjustPointByTools(Base::Vector3d& point, Base::Placement& place, double B);
		Base::Vector3d adjustPointByTools(Custom::PathObject::PathPointList& pathPointList, int i, Base::Placement& place, double B);
		double getToolDiameter();
		void getAngle(Base::Vector3d nrm, double& a, double &b, double & c, Base::Placement p);
		void printHeader(std::ofstream& ofs, int& nlines);
		void printFooter(std::ofstream& ofs, int& nlines);
		void printFeed(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths , Base::Placement p);
		void printTract(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement p);
		void printAPath(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement p);
	private:
		std::vector<Custom::PathObject::PathPointList> m_nclist;

		Base::Vector3d m_curPtCenter;
	};

} //namespace Custom


#endif // CUSTOM_BALLCUTTER_H
