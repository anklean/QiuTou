#include "PreCompiled.h"
#include "FeatureOffsetEdge.h"

#include "BRepPrimAPI_MakePrism.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Edge.hxx"
#include "BRepOffsetAPI_MakeOffset.hxx"
#include "..\..\..\..\include\oce\BRepBuilderAPI_MakeWire.hxx"
#include "..\..\..\..\include\oce\BRepOffsetAPI_MakeOffsetShape.hxx"
#include "..\..\..\..\include\oce\TopExp_Explorer.hxx"
#include "IntTools_FaceFace.hxx"
#include "..\..\..\..\include\oce\IntTools_Curve.hxx"
#include "Geometry.h"
#include "..\..\..\..\include\oce\BRepBuilderAPI_MakeEdge.hxx"
using namespace Part;

PROPERTY_SOURCE(Part::FeatureOffsetEdge, Part::Feature)

FeatureOffsetEdge::FeatureOffsetEdge()
{
	ADD_PROPERTY_TYPE(TheFace, (0), "OffsetEdge", App::Prop_None, "Face");
	ADD_PROPERTY_TYPE(TheEdge, (0), "OffsetEdge", App::Prop_None, "Edge on Face");
	ADD_PROPERTY_TYPE(Distance, (long(1)), "OffsetEdge", App::Prop_None, "offset distance");
	ADD_PROPERTY_TYPE(Num, (long(1)), "OffsetEdge", App::Prop_None, " offset num");
}


FeatureOffsetEdge::~FeatureOffsetEdge()
{
}


short FeatureOffsetEdge::mustExecute() const
{
	if (TheFace.isTouched())
		return 1;
	if (TheEdge.isTouched())
		return 1;
	if (Distance.isTouched())
		return 1;
	if (Num.isTouched())
		return 1;
	return 0;
}

App::DocumentObjectExecReturn *FeatureOffsetEdge::execute(void)
{
	App::DocumentObject* sourceF = TheFace.getValue();
	if (!(sourceF && sourceF->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId())))
		return new App::DocumentObjectExecReturn("No source shape linked.");
	const TopoShape& shapeF = static_cast<Part::Feature*>(sourceF)->Shape.getShape();
	if (shapeF.isNull())
		return new App::DocumentObjectExecReturn("Face shape is empty.");

	App::DocumentObject* sourceE = TheEdge.getValue();
	if (!(sourceE && sourceE->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId())))
		return new App::DocumentObjectExecReturn("No source shape linked.");
	const TopoShape& shapeE = static_cast<Part::Feature*>(sourceE)->Shape.getShape();
	if (shapeE.isNull())
		return new App::DocumentObjectExecReturn("Edge shape is empty.");	

	TopoDS_Face f;// = TopoDS::Face(shapeF._Shape);
	int countSolids = 0;
	TopExp_Explorer xp;
	xp.Init(shapeF._Shape, TopAbs_FACE);
	for (; xp.More(); xp.Next()) {
		countSolids++;
		f = TopoDS::Face(xp.Current());
	}

	//1.尝试直接调MakeOffset
// 	double offset = Distance.getValue();
// 	double tol = 0.001;
// 	/*BRepOffsetAPI_MakeOffsetShape*/
// 	BRepOffset_MakeOffset mkOffset;
// 	mkOffset.Initialize(shapeE._Shape, offset, tol, BRepOffset_Mode(BRepOffset_Skin),
// 	/*	intersection ? Standard_True : */Standard_False,
// 	/*	selfInter ? Standard_True : */Standard_False,
// 	GeomAbs_JoinType(GeomAbs_Arc));
// 
// 	mkOffset.SetOffsetOnFace(f,0.02);
// 	mkOffset.MakeOffsetShape();
// 	if (!mkOffset.IsDone())
// 		Standard_Failure::Raise("BRepOffsetAPI_MakeOffsetShape not done");
// 	const TopoDS_Shape& res = mkOffset.Shape();
// 	TopoDS_Shape S = shapeE.makeOffsetShape( 0.2, 0.001,		false,  false,		 0,  0,  false) ;
// 	this->Shape.setValue(S);

	//2 第 2种偏移的方式
	//使用线，做成平面，然后平面和原来的面求交线，交线即时想要的偏移线
 	gp_Vec vec(0.0, 1.0, 0.0);
	
	TopoShape shapePlane;
	shapePlane._Shape = shapeE.makePrism(vec);


	IntTools_SequenceOfCurves r;

	for (int i = 0; i < 60; i++)
	{
		Base::Matrix4D mtx;
		mtx.move(0, -0.2*i, 0);
		TopoDS_Shape sP = shapePlane.transformGShape(mtx);
		TopoDS_Face fP = TopoDS::Face(sP);
		
		IntTools_FaceFace iff;
		iff.SetParameters(false, true, true, 0.001);
		iff.Perform(f, fP);
		if (iff.IsDone())
		{
			iff.PrepareLines3D(true);

			IntTools_SequenceOfPntOn2Faces ff = iff.Points();

			IntTools_SequenceOfCurves lines=iff.Lines();
			r.Append(lines);
		}
	}

	TopoShape result;
	for (int i = 1; i < r.Length(); i++)
	{
		const IntTools_Curve& C = r.Value(i);
		const Handle(Geom_Curve)& crv = C.Curve();
	
		TopoDS_Edge e = BRepBuilderAPI_MakeEdge(crv);
		if (i==1) {
			result._Shape = e;
		}
		else {
			result._Shape = result.fuse(e);
		}
	}
	this->Shape.setValue(result);
	

// 	int countSolids = 0;
// 	TopExp_Explorer xp;
// 	xp.Init(shape._Shape, TopAbs_SOLID);
// 	for (; xp.More(); xp.Next()) {
// 		countSolids++;
// 	}
// 	if (countSolids != 1)
// 		return new App::DocumentObjectExecReturn("Source shape is not a solid.");
// 
// 	TopTools_ListOfShape closingFaces;
// 	const std::vector<std::string>& subStrings = Faces.getSubValues();
// 	for (std::vector<std::string>::const_iterator it = subStrings.begin(); it != subStrings.end(); ++it) {
// 		TopoDS_Face face = TopoDS::Face(shape.getSubShape(it->c_str()));
// 		closingFaces.Append(face);
// 	}
// 
// 	double thickness = Value.getValue();
// 	double tol = Precision::Confusion();
// 	bool inter = Intersection.getValue();
// 	bool self = SelfIntersection.getValue();
// 	short mode = (short)Mode.getValue();
// 	short join = (short)Join.getValue();
// 
// 	if (fabs(thickness) > 2 * tol)
// 		this->Shape.setValue(shape.makeThickSolid(closingFaces, thickness, tol, inter, self, mode, join));
// 	else
// 		this->Shape.setValue(shape);
	return App::DocumentObject::StdReturn;
}

