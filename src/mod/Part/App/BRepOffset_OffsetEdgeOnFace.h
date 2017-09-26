#pragma once
#include "TopoDS_Edge.hxx"
#include "TopoDS_Face.hxx"
#include "gp_pnt.hxx"
#include <vector>
#include "PreCompiled.h"

struct BRepOffset_OffsetEdgeOnFaceP;

class PartExport BRepOffset_OffsetEdgeOnFace
{
public:
	BRepOffset_OffsetEdgeOnFace();
	BRepOffset_OffsetEdgeOnFace(
		const TopoDS_Face& F,
		const TopoDS_Edge& E,
		const Standard_Real Offset,
		const Standard_Integer Num);

	~BRepOffset_OffsetEdgeOnFace();

	bool Perform();
	bool IsDone();
	
	int NbEdges();
	void Value(int nIndex, std::vector<gp_Pnt>&);
	void Value(int nIndex, std::vector<std::pair<gp_Pnt,gp_Dir>>&);


protected:
	void doOffset();

	BRepOffset_OffsetEdgeOnFaceP* d;
	bool myDone;
};

