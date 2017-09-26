#ifndef PART_FEATUREOFFSETEDGE_H
#define PART_FEATUREOFFSETEDGE_H

#include <App/PropertyStandard.h>
#include <App/PropertyUnits.h>
#include <Mod/Part/App/PartFeature.h>

namespace Part{
	class FeatureOffsetEdge : public Part::Feature
	{
		PROPERTY_HEADER(Part::FeatureOffsetEdge);
	public:
		FeatureOffsetEdge();
		~FeatureOffsetEdge();
		
		App::PropertyLinkSub TheFace; //Ãæ
		App::PropertyLinkSub TheEdge;//±ß
		App::PropertyDistance Distance;//Æ«ÒÆµÄ¾àÀë
		App::PropertyInteger Num;//Æ«ÒÆ´ÎÊý

		/** @name methods override feature */
		//@{
		/// recalculate the feature
		App::DocumentObjectExecReturn *execute(void);
		short mustExecute() const;
		const char* getViewProviderName(void) const {
			return "PartGui::ViewProviderPrism";
		}
		//@}

	};
}

#endif //PART_FEATUREOFFSETEDGE_H