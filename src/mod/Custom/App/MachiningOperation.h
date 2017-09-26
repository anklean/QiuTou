#pragma once

#ifndef MACHININGOPERATION_H
#define MACHININGOPERATION_H

#include <App/PropertyStandard.h>
#include <App/PropertyUnits.h>
#include <App/PropertyLinks.h>
#include <Mod/Part/App/PartFeature.h>

namespace Custom
{

	class MachiningOperation : public Part::Feature
	{
		PROPERTY_HEADER(Custom::MachiningOperation);

	public:
		MachiningOperation();

		App::PropertyLink Solid; 
		App::PropertyLength Deviation;
		App::PropertyLength AngularDeflection;
		

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


		void OffsetFace(TopoDS_Face& aSurface, std::vector<Handle(Geom_Curve)>& geoms);

		void toPolyline(TopoDS_Edge& aEdge, std::vector<gp_Pnt>& pnts, double LinDeflection, double AngDeflection);
	private:
	};

} //namespace Custom


#endif // CUSTOM_BALLCUTTER_H
