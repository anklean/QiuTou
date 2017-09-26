#include "TopoDS_Shape.hxx"
#include <vector>
#include "PathObject.h"

void BuildPath(Part::TopoShape& shape, std::string subs, std::vector<Custom::NCStepInfo>& pathPointList);
int getNormal2(gp_Pnt& pnt, gp_Lin& center_line, gp_Dir& normal);