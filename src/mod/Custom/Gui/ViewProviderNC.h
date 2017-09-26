#ifndef CUSTOMGUI_VIEWPROVIDERNC_H
#define CUSTOMGUI_VIEWPROVIDERNC_H

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

class CustomGuiExport ViewProviderNC : public PartGui::ViewProviderPart
{
	PROPERTY_HEADER(CustomGui::ViewProviderNC);

public:
    /// constructor
	ViewProviderNC();
    /// destructor
	virtual ~ViewProviderNC();

	virtual void attach(App::DocumentObject *);
	virtual void updateData(const App::Property*);
	//void StartCutter();	
	//void runCutter();
	//void stopCutter();
protected:
	void updateVisual(const TopoDS_Shape &);
	void onChanged(const App::Property* prop);

	SoBaseColor* pColor;
	SoCoordinate3* pCoords;
	SoIndexedLineSet* lineset;

	SoSwitch * switchCutter;
	SoTransform* cutterTransform;

	//SoCoordinate3     * coords;
	Custom::PathObject::PathPointList pathPointList;
	int cutterOnPointIndex;

	//CutterPlayer* player;

	//friend class CutterPlayer;
};

} // namespace CustomGui


#endif // CUSTOMGUI_VIEWPROVIDERCUSTOMPATH_H

