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
		App::PropertyLinkSubList PathList;
		App::PropertyStringList BeforeGcode;
		App::PropertyStringList AfterGcode;

		App::PropertyLength SafeHeight;//安全高度
		App::PropertyLength Speed;//速度
		
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
		void getAngle(Base::Vector3d nrm, double& a, double &b, double & c,Base::Placement p);
		void printHeader(std::ofstream& ofs, int& nlines);
		void printFooter(std::ofstream& ofs, int& nlines);
		void printFeed(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths , Base::Placement p);
		void printTract(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement p);
		void printAPath(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement p);
	private:
		std::vector<Custom::PathObject::PathPointList> m_nclist;
	};

} //namespace Custom


#endif // CUSTOM_BALLCUTTER_H
