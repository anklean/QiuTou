
// This file is generated by src/Tools/generateTemaplates/templateClassPyExport.py out of the XML file
// Every change you make here get lost at the next full rebuild!
#ifndef PART_TOPOSHAPEPY_H
#define PART_TOPOSHAPEPY_H

#include <App/ComplexGeoDataPy.h>
#include <Mod/Part/App/TopoShape.h>
#include <string>

namespace Part
{

//===========================================================================
// TopoShapePy - Python wrapper
//===========================================================================

/** The python export class for TopoShape
 */
class PartExport TopoShapePy : public Data::ComplexGeoDataPy
{
public:
    static PyTypeObject   Type;
    static PyMethodDef    Methods[];
    static PyGetSetDef    GetterSetter[];
    static PyParentObject Parents[];
    virtual PyTypeObject *GetType(void) {return &Type;};
    virtual PyParentObject *GetParents(void) {return Parents;}

public:
    TopoShapePy(TopoShape *pcObject, PyTypeObject *T = &Type);
    static PyObject *PyMake(struct _typeobject *, PyObject *, PyObject *);
    virtual int PyInit(PyObject* args, PyObject*k);
    ~TopoShapePy();

    typedef TopoShape* PointerType ;

    virtual PyObject *_repr(void);        // the representation
    std::string representation(void) const;

    /** @name callbacks and implementers for the python object methods */
    //@{
    /// callback for the __getstate__() method
    static PyObject * staticCallback___getstate__ (PyObject *self, PyObject *args);
    /// implementer for the __getstate__() method
    PyObject*  __getstate__(PyObject *args);
    /// callback for the __setstate__() method
    static PyObject * staticCallback___setstate__ (PyObject *self, PyObject *args);
    /// implementer for the __setstate__() method
    PyObject*  __setstate__(PyObject *args);
    /// callback for the read() method
    static PyObject * staticCallback_read (PyObject *self, PyObject *args);
    /// implementer for the read() method
    PyObject*  read(PyObject *args);
    /// callback for the writeInventor() method
    static PyObject * staticCallback_writeInventor (PyObject *self, PyObject *args);
    /// implementer for the writeInventor() method
    PyObject*  writeInventor(PyObject *args);
    /// callback for the exportIges() method
    static PyObject * staticCallback_exportIges (PyObject *self, PyObject *args);
    /// implementer for the exportIges() method
    PyObject*  exportIges(PyObject *args);
    /// callback for the exportStep() method
    static PyObject * staticCallback_exportStep (PyObject *self, PyObject *args);
    /// implementer for the exportStep() method
    PyObject*  exportStep(PyObject *args);
    /// callback for the exportBrep() method
    static PyObject * staticCallback_exportBrep (PyObject *self, PyObject *args);
    /// implementer for the exportBrep() method
    PyObject*  exportBrep(PyObject *args);
    /// callback for the exportBrepToString() method
    static PyObject * staticCallback_exportBrepToString (PyObject *self, PyObject *args);
    /// implementer for the exportBrepToString() method
    PyObject*  exportBrepToString(PyObject *args);
    /// callback for the dumpToString() method
    static PyObject * staticCallback_dumpToString (PyObject *self, PyObject *args);
    /// implementer for the dumpToString() method
    PyObject*  dumpToString(PyObject *args);
    /// callback for the exportStl() method
    static PyObject * staticCallback_exportStl (PyObject *self, PyObject *args);
    /// implementer for the exportStl() method
    PyObject*  exportStl(PyObject *args);
    /// callback for the importBrep() method
    static PyObject * staticCallback_importBrep (PyObject *self, PyObject *args);
    /// implementer for the importBrep() method
    PyObject*  importBrep(PyObject *args);
    /// callback for the importBrepFromString() method
    static PyObject * staticCallback_importBrepFromString (PyObject *self, PyObject *args);
    /// implementer for the importBrepFromString() method
    PyObject*  importBrepFromString(PyObject *args);
    /// callback for the extrude() method
    static PyObject * staticCallback_extrude (PyObject *self, PyObject *args);
    /// implementer for the extrude() method
    PyObject*  extrude(PyObject *args);
    /// callback for the revolve() method
    static PyObject * staticCallback_revolve (PyObject *self, PyObject *args);
    /// implementer for the revolve() method
    PyObject*  revolve(PyObject *args);
    /// callback for the check() method
    static PyObject * staticCallback_check (PyObject *self, PyObject *args);
    /// implementer for the check() method
    PyObject*  check(PyObject *args);
    /// callback for the fuse() method
    static PyObject * staticCallback_fuse (PyObject *self, PyObject *args);
    /// implementer for the fuse() method
    PyObject*  fuse(PyObject *args);
    /// callback for the multiFuse() method
    static PyObject * staticCallback_multiFuse (PyObject *self, PyObject *args);
    /// implementer for the multiFuse() method
    PyObject*  multiFuse(PyObject *args);
    /// callback for the oldFuse() method
    static PyObject * staticCallback_oldFuse (PyObject *self, PyObject *args);
    /// implementer for the oldFuse() method
    PyObject*  oldFuse(PyObject *args);
    /// callback for the common() method
    static PyObject * staticCallback_common (PyObject *self, PyObject *args);
    /// implementer for the common() method
    PyObject*  common(PyObject *args);
    /// callback for the section() method
    static PyObject * staticCallback_section (PyObject *self, PyObject *args);
    /// implementer for the section() method
    PyObject*  section(PyObject *args);
    /// callback for the slices() method
    static PyObject * staticCallback_slices (PyObject *self, PyObject *args);
    /// implementer for the slices() method
    PyObject*  slices(PyObject *args);
    /// callback for the slice() method
    static PyObject * staticCallback_slice (PyObject *self, PyObject *args);
    /// implementer for the slice() method
    PyObject*  slice(PyObject *args);
    /// callback for the cut() method
    static PyObject * staticCallback_cut (PyObject *self, PyObject *args);
    /// implementer for the cut() method
    PyObject*  cut(PyObject *args);
    /// callback for the sewShape() method
    static PyObject * staticCallback_sewShape (PyObject *self, PyObject *args);
    /// implementer for the sewShape() method
    PyObject*  sewShape(PyObject *args);
    /// callback for the childShapes() method
    static PyObject * staticCallback_childShapes (PyObject *self, PyObject *args);
    /// implementer for the childShapes() method
    PyObject*  childShapes(PyObject *args);
    /// callback for the removeInternalWires() method
    static PyObject * staticCallback_removeInternalWires (PyObject *self, PyObject *args);
    /// implementer for the removeInternalWires() method
    PyObject*  removeInternalWires(PyObject *args);
    /// callback for the mirror() method
    static PyObject * staticCallback_mirror (PyObject *self, PyObject *args);
    /// implementer for the mirror() method
    PyObject*  mirror(PyObject *args);
    /// callback for the transformGeometry() method
    static PyObject * staticCallback_transformGeometry (PyObject *self, PyObject *args);
    /// implementer for the transformGeometry() method
    PyObject*  transformGeometry(PyObject *args);
    /// callback for the transformShape() method
    static PyObject * staticCallback_transformShape (PyObject *self, PyObject *args);
    /// implementer for the transformShape() method
    PyObject*  transformShape(PyObject *args);
    /// callback for the translate() method
    static PyObject * staticCallback_translate (PyObject *self, PyObject *args);
    /// implementer for the translate() method
    PyObject*  translate(PyObject *args);
    /// callback for the rotate() method
    static PyObject * staticCallback_rotate (PyObject *self, PyObject *args);
    /// implementer for the rotate() method
    PyObject*  rotate(PyObject *args);
    /// callback for the scale() method
    static PyObject * staticCallback_scale (PyObject *self, PyObject *args);
    /// implementer for the scale() method
    PyObject*  scale(PyObject *args);
    /// callback for the makeFillet() method
    static PyObject * staticCallback_makeFillet (PyObject *self, PyObject *args);
    /// implementer for the makeFillet() method
    PyObject*  makeFillet(PyObject *args);
    /// callback for the makeChamfer() method
    static PyObject * staticCallback_makeChamfer (PyObject *self, PyObject *args);
    /// implementer for the makeChamfer() method
    PyObject*  makeChamfer(PyObject *args);
    /// callback for the makeThickness() method
    static PyObject * staticCallback_makeThickness (PyObject *self, PyObject *args);
    /// implementer for the makeThickness() method
    PyObject*  makeThickness(PyObject *args);
    /// callback for the makeOffsetShape() method
    static PyObject * staticCallback_makeOffsetShape (PyObject *self, PyObject *args, PyObject *kwd);
    /// implementer for the makeOffsetShape() method
    PyObject*  makeOffsetShape(PyObject *args, PyObject *kwd);
    /// callback for the reverse() method
    static PyObject * staticCallback_reverse (PyObject *self, PyObject *args);
    /// implementer for the reverse() method
    PyObject*  reverse(PyObject *args);
    /// callback for the complement() method
    static PyObject * staticCallback_complement (PyObject *self, PyObject *args);
    /// implementer for the complement() method
    PyObject*  complement(PyObject *args);
    /// callback for the nullify() method
    static PyObject * staticCallback_nullify (PyObject *self, PyObject *args);
    /// implementer for the nullify() method
    PyObject*  nullify(PyObject *args);
    /// callback for the isClosed() method
    static PyObject * staticCallback_isClosed (PyObject *self, PyObject *args);
    /// implementer for the isClosed() method
    PyObject*  isClosed(PyObject *args);
    /// callback for the isPartner() method
    static PyObject * staticCallback_isPartner (PyObject *self, PyObject *args);
    /// implementer for the isPartner() method
    PyObject*  isPartner(PyObject *args);
    /// callback for the isSame() method
    static PyObject * staticCallback_isSame (PyObject *self, PyObject *args);
    /// implementer for the isSame() method
    PyObject*  isSame(PyObject *args);
    /// callback for the isEqual() method
    static PyObject * staticCallback_isEqual (PyObject *self, PyObject *args);
    /// implementer for the isEqual() method
    PyObject*  isEqual(PyObject *args);
    /// callback for the isNull() method
    static PyObject * staticCallback_isNull (PyObject *self, PyObject *args);
    /// implementer for the isNull() method
    PyObject*  isNull(PyObject *args);
    /// callback for the isValid() method
    static PyObject * staticCallback_isValid (PyObject *self, PyObject *args);
    /// implementer for the isValid() method
    PyObject*  isValid(PyObject *args);
    /// callback for the fix() method
    static PyObject * staticCallback_fix (PyObject *self, PyObject *args);
    /// implementer for the fix() method
    PyObject*  fix(PyObject *args);
    /// callback for the hashCode() method
    static PyObject * staticCallback_hashCode (PyObject *self, PyObject *args);
    /// implementer for the hashCode() method
    PyObject*  hashCode(PyObject *args);
    /// callback for the tessellate() method
    static PyObject * staticCallback_tessellate (PyObject *self, PyObject *args);
    /// implementer for the tessellate() method
    PyObject*  tessellate(PyObject *args);
    /// callback for the project() method
    static PyObject * staticCallback_project (PyObject *self, PyObject *args);
    /// implementer for the project() method
    PyObject*  project(PyObject *args);
    /// callback for the makeParallelProjection() method
    static PyObject * staticCallback_makeParallelProjection (PyObject *self, PyObject *args);
    /// implementer for the makeParallelProjection() method
    PyObject*  makeParallelProjection(PyObject *args);
    /// callback for the makePerspectiveProjection() method
    static PyObject * staticCallback_makePerspectiveProjection (PyObject *self, PyObject *args);
    /// implementer for the makePerspectiveProjection() method
    PyObject*  makePerspectiveProjection(PyObject *args);
    /// callback for the makeShapeFromMesh() method
    static PyObject * staticCallback_makeShapeFromMesh (PyObject *self, PyObject *args);
    /// implementer for the makeShapeFromMesh() method
    PyObject*  makeShapeFromMesh(PyObject *args);
    /// callback for the toNurbs() method
    static PyObject * staticCallback_toNurbs (PyObject *self, PyObject *args);
    /// implementer for the toNurbs() method
    PyObject*  toNurbs(PyObject *args);
    /// callback for the copy() method
    static PyObject * staticCallback_copy (PyObject *self, PyObject *args);
    /// implementer for the copy() method
    PyObject*  copy(PyObject *args);
    /// callback for the cleaned() method
    static PyObject * staticCallback_cleaned (PyObject *self, PyObject *args);
    /// implementer for the cleaned() method
    PyObject*  cleaned(PyObject *args);
    /// callback for the replaceShape() method
    static PyObject * staticCallback_replaceShape (PyObject *self, PyObject *args);
    /// implementer for the replaceShape() method
    PyObject*  replaceShape(PyObject *args);
    /// callback for the removeShape() method
    static PyObject * staticCallback_removeShape (PyObject *self, PyObject *args);
    /// implementer for the removeShape() method
    PyObject*  removeShape(PyObject *args);
    /// callback for the isInside() method
    static PyObject * staticCallback_isInside (PyObject *self, PyObject *args);
    /// implementer for the isInside() method
    PyObject*  isInside(PyObject *args);
    /// callback for the removeSplitter() method
    static PyObject * staticCallback_removeSplitter (PyObject *self, PyObject *args);
    /// implementer for the removeSplitter() method
    PyObject*  removeSplitter(PyObject *args);
    /// callback for the proximity() method
    static PyObject * staticCallback_proximity (PyObject *self, PyObject *args);
    /// implementer for the proximity() method
    PyObject*  proximity(PyObject *args);
    /// callback for the distToShape() method
    static PyObject * staticCallback_distToShape (PyObject *self, PyObject *args);
    /// implementer for the distToShape() method
    PyObject*  distToShape(PyObject *args);
    /// callback for the getElement() method
    static PyObject * staticCallback_getElement (PyObject *self, PyObject *args);
    /// implementer for the getElement() method
    PyObject*  getElement(PyObject *args);
    //@}


    /** @name callbacks and implementers for the python object attributes */
    //@{
    ///getter callback for the ShapeType attribute
    static PyObject * staticCallback_getShapeType (PyObject *self, void *closure);
    /// getter for the ShapeType attribute
    Py::String getShapeType(void) const;
    /// setter callback for the ShapeType attribute
    static int staticCallback_setShapeType (PyObject *self, PyObject *value, void *closure);
    // no setter method,  ShapeType is read only!
    ///getter callback for the Orientation attribute
    static PyObject * staticCallback_getOrientation (PyObject *self, void *closure);
    /// getter for the Orientation attribute
    Py::String getOrientation(void) const;
    /// setter callback for the Orientation attribute
    static int staticCallback_setOrientation (PyObject *self, PyObject *value, void *closure);
    /// setter for the Orientation attribute
    void setOrientation(Py::String arg);
    ///getter callback for the Faces attribute
    static PyObject * staticCallback_getFaces (PyObject *self, void *closure);
    /// getter for the Faces attribute
    Py::List getFaces(void) const;
    /// setter callback for the Faces attribute
    static int staticCallback_setFaces (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Faces is read only!
    ///getter callback for the Vertexes attribute
    static PyObject * staticCallback_getVertexes (PyObject *self, void *closure);
    /// getter for the Vertexes attribute
    Py::List getVertexes(void) const;
    /// setter callback for the Vertexes attribute
    static int staticCallback_setVertexes (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Vertexes is read only!
    ///getter callback for the Shells attribute
    static PyObject * staticCallback_getShells (PyObject *self, void *closure);
    /// getter for the Shells attribute
    Py::List getShells(void) const;
    /// setter callback for the Shells attribute
    static int staticCallback_setShells (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Shells is read only!
    ///getter callback for the Solids attribute
    static PyObject * staticCallback_getSolids (PyObject *self, void *closure);
    /// getter for the Solids attribute
    Py::List getSolids(void) const;
    /// setter callback for the Solids attribute
    static int staticCallback_setSolids (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Solids is read only!
    ///getter callback for the CompSolids attribute
    static PyObject * staticCallback_getCompSolids (PyObject *self, void *closure);
    /// getter for the CompSolids attribute
    Py::List getCompSolids(void) const;
    /// setter callback for the CompSolids attribute
    static int staticCallback_setCompSolids (PyObject *self, PyObject *value, void *closure);
    // no setter method,  CompSolids is read only!
    ///getter callback for the Edges attribute
    static PyObject * staticCallback_getEdges (PyObject *self, void *closure);
    /// getter for the Edges attribute
    Py::List getEdges(void) const;
    /// setter callback for the Edges attribute
    static int staticCallback_setEdges (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Edges is read only!
    ///getter callback for the Wires attribute
    static PyObject * staticCallback_getWires (PyObject *self, void *closure);
    /// getter for the Wires attribute
    Py::List getWires(void) const;
    /// setter callback for the Wires attribute
    static int staticCallback_setWires (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Wires is read only!
    ///getter callback for the Compounds attribute
    static PyObject * staticCallback_getCompounds (PyObject *self, void *closure);
    /// getter for the Compounds attribute
    Py::List getCompounds(void) const;
    /// setter callback for the Compounds attribute
    static int staticCallback_setCompounds (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Compounds is read only!
    ///getter callback for the Length attribute
    static PyObject * staticCallback_getLength (PyObject *self, void *closure);
    /// getter for the Length attribute
    Py::Float getLength(void) const;
    /// setter callback for the Length attribute
    static int staticCallback_setLength (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Length is read only!
    ///getter callback for the Area attribute
    static PyObject * staticCallback_getArea (PyObject *self, void *closure);
    /// getter for the Area attribute
    Py::Float getArea(void) const;
    /// setter callback for the Area attribute
    static int staticCallback_setArea (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Area is read only!
    ///getter callback for the Volume attribute
    static PyObject * staticCallback_getVolume (PyObject *self, void *closure);
    /// getter for the Volume attribute
    Py::Float getVolume(void) const;
    /// setter callback for the Volume attribute
    static int staticCallback_setVolume (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Volume is read only!
    //@}

    /// getter method for special attributes (e.g. dynamic ones)
    PyObject *getCustomAttributes(const char* attr) const;
    /// setter for special attributes (e.g. dynamic ones)
    int setCustomAttributes(const char* attr, PyObject *obj);
    PyObject *_getattr(char *attr);              // __getattr__ function
    int _setattr(char *attr, PyObject *value);        // __setattr__ function

    /// getter for the object handled by this class
    TopoShape *getTopoShapePtr(void) const;

    /** @name additional declarations and methods for the wrapper class */
    //@{

    //@}
};

#define PARENTSPartTopoShapePy &TopoShapePy::Type,PARENTSDataComplexGeoDataPy

}  //namespace Part

#endif  // PART_TOPOSHAPEPY_H


