#ifndef CUSTOMGUI_VIEWPROVIDERBALLCUTTER_H
#define CUSTOMGUI_VIEWPROVIDERBALLCUTTER_H

#ifdef signals
#undef signals
#endif

#include "PreCompiled.h"
#include <Standard_math.hxx>
#include <Standard_Boolean.hxx>
#include <TopoDS_Shape.hxx>
#include "Mod/part/Gui/ViewProvider.h"
#include "Inventor/nodes/SoIndexedLineSet.h"
#include "Inventor/nodes/SoCoordinate3.h"
#include "Inventor/nodes/SoBaseColor.h"
#include "../App/PathObject.h"


class SoSeparator;
//class CutterPlayer;

namespace CustomGui {

class CustomGuiExport ViewProviderBallCutter: public PartGui::ViewProviderPart
{
	PROPERTY_HEADER(CustomGui::ViewProviderBallCutter);

public:
    /// constructor
	ViewProviderBallCutter();
    /// destructor
	virtual ~ViewProviderBallCutter();
	
	/** @name Edit methods */
	//@{
	void setupContextMenu(QMenu*, QObject*, const char*);
protected:
	bool setEdit(int ModNum);
	void unsetEdit(int ModNum);
	//@}

};

} // namespace CustomGui


#endif // CUSTOMGUI_VIEWPROVIDERCUSTOMPATH_H

