
// This file is generated by src/Tools/generateTemaplates/templateClassPyExport.py out of the XML file
// Every change you make here get lost at the next full rebuild!
#ifndef PART_TOPOSHAPESOLIDPY_H
#define PART_TOPOSHAPESOLIDPY_H

#include <Mod/Part/App/TopoShapePy.h>
#include <Mod/Part/App/TopoShape.h>
#include <string>

namespace Part
{

//===========================================================================
// TopoShapeSolidPy - Python wrapper
//===========================================================================

/** The python export class for TopoShape
 */
class PartExport TopoShapeSolidPy : public Part::TopoShapePy
{
public:
    static PyTypeObject   Type;
    static PyMethodDef    Methods[];
    static PyGetSetDef    GetterSetter[];
    static PyParentObject Parents[];
    virtual PyTypeObject *GetType(void) {return &Type;};
    virtual PyParentObject *GetParents(void) {return Parents;}

public:
    TopoShapeSolidPy(TopoShape *pcObject, PyTypeObject *T = &Type);
    static PyObject *PyMake(struct _typeobject *, PyObject *, PyObject *);
    virtual int PyInit(PyObject* args, PyObject*k);
    ~TopoShapeSolidPy();

    typedef TopoShape* PointerType ;

    virtual PyObject *_repr(void);        // the representation
    std::string representation(void) const;

    /** @name callbacks and implementers for the python object methods */
    //@{
    /// callback for the getMomentOfInertia() method
    static PyObject * staticCallback_getMomentOfInertia (PyObject *self, PyObject *args);
    /// implementer for the getMomentOfInertia() method
    PyObject*  getMomentOfInertia(PyObject *args);
    /// callback for the getRadiusOfGyration() method
    static PyObject * staticCallback_getRadiusOfGyration (PyObject *self, PyObject *args);
    /// implementer for the getRadiusOfGyration() method
    PyObject*  getRadiusOfGyration(PyObject *args);
    /// callback for the offsetFaces() method
    static PyObject * staticCallback_offsetFaces (PyObject *self, PyObject *args);
    /// implementer for the offsetFaces() method
    PyObject*  offsetFaces(PyObject *args);
    //@}


    /** @name callbacks and implementers for the python object attributes */
    //@{
    ///getter callback for the Mass attribute
    static PyObject * staticCallback_getMass (PyObject *self, void *closure);
    /// getter for the Mass attribute
    Py::Object getMass(void) const;
    /// setter callback for the Mass attribute
    static int staticCallback_setMass (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Mass is read only!
    ///getter callback for the CenterOfMass attribute
    static PyObject * staticCallback_getCenterOfMass (PyObject *self, void *closure);
    /// getter for the CenterOfMass attribute
    Py::Object getCenterOfMass(void) const;
    /// setter callback for the CenterOfMass attribute
    static int staticCallback_setCenterOfMass (PyObject *self, PyObject *value, void *closure);
    // no setter method,  CenterOfMass is read only!
    ///getter callback for the MatrixOfInertia attribute
    static PyObject * staticCallback_getMatrixOfInertia (PyObject *self, void *closure);
    /// getter for the MatrixOfInertia attribute
    Py::Object getMatrixOfInertia(void) const;
    /// setter callback for the MatrixOfInertia attribute
    static int staticCallback_setMatrixOfInertia (PyObject *self, PyObject *value, void *closure);
    // no setter method,  MatrixOfInertia is read only!
    ///getter callback for the StaticMoments attribute
    static PyObject * staticCallback_getStaticMoments (PyObject *self, void *closure);
    /// getter for the StaticMoments attribute
    Py::Object getStaticMoments(void) const;
    /// setter callback for the StaticMoments attribute
    static int staticCallback_setStaticMoments (PyObject *self, PyObject *value, void *closure);
    // no setter method,  StaticMoments is read only!
    ///getter callback for the PrincipalProperties attribute
    static PyObject * staticCallback_getPrincipalProperties (PyObject *self, void *closure);
    /// getter for the PrincipalProperties attribute
    Py::Dict getPrincipalProperties(void) const;
    /// setter callback for the PrincipalProperties attribute
    static int staticCallback_setPrincipalProperties (PyObject *self, PyObject *value, void *closure);
    // no setter method,  PrincipalProperties is read only!
    ///getter callback for the OuterShell attribute
    static PyObject * staticCallback_getOuterShell (PyObject *self, void *closure);
    /// getter for the OuterShell attribute
    Py::Object getOuterShell(void) const;
    /// setter callback for the OuterShell attribute
    static int staticCallback_setOuterShell (PyObject *self, PyObject *value, void *closure);
    // no setter method,  OuterShell is read only!
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

#define PARENTSPartTopoShapeSolidPy &TopoShapeSolidPy::Type,PARENTSPartTopoShapePy

}  //namespace Part

#endif  // PART_TOPOSHAPESOLIDPY_H

