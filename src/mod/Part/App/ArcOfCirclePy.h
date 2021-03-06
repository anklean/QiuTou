
// This file is generated by src/Tools/generateTemaplates/templateClassPyExport.py out of the XML file
// Every change you make here get lost at the next full rebuild!
#ifndef PART_ARCOFCIRCLEPY_H
#define PART_ARCOFCIRCLEPY_H

#include <Mod/Part/App/GeometryCurvePy.h>
#include <Mod/Part/App/Geometry.h>
#include <string>

namespace Part
{

//===========================================================================
// ArcOfCirclePy - Python wrapper
//===========================================================================

/** The python export class for GeomArcOfCircle
 */
class PartExport ArcOfCirclePy : public Part::GeometryCurvePy
{
public:
    static PyTypeObject   Type;
    static PyMethodDef    Methods[];
    static PyGetSetDef    GetterSetter[];
    static PyParentObject Parents[];
    virtual PyTypeObject *GetType(void) {return &Type;};
    virtual PyParentObject *GetParents(void) {return Parents;}

public:
    ArcOfCirclePy(GeomArcOfCircle *pcObject, PyTypeObject *T = &Type);
    static PyObject *PyMake(struct _typeobject *, PyObject *, PyObject *);
    virtual int PyInit(PyObject* args, PyObject*k);
    ~ArcOfCirclePy();

    typedef GeomArcOfCircle* PointerType ;

    virtual PyObject *_repr(void);        // the representation
    std::string representation(void) const;

    /** @name callbacks and implementers for the python object methods */
    //@{
    //@}


    /** @name callbacks and implementers for the python object attributes */
    //@{
    ///getter callback for the Radius attribute
    static PyObject * staticCallback_getRadius (PyObject *self, void *closure);
    /// getter for the Radius attribute
    Py::Float getRadius(void) const;
    /// setter callback for the Radius attribute
    static int staticCallback_setRadius (PyObject *self, PyObject *value, void *closure);
    /// setter for the Radius attribute
    void setRadius(Py::Float arg);
    ///getter callback for the Center attribute
    static PyObject * staticCallback_getCenter (PyObject *self, void *closure);
    /// getter for the Center attribute
    Py::Object getCenter(void) const;
    /// setter callback for the Center attribute
    static int staticCallback_setCenter (PyObject *self, PyObject *value, void *closure);
    /// setter for the Center attribute
    void setCenter(Py::Object arg);
    ///getter callback for the Axis attribute
    static PyObject * staticCallback_getAxis (PyObject *self, void *closure);
    /// getter for the Axis attribute
    Py::Object getAxis(void) const;
    /// setter callback for the Axis attribute
    static int staticCallback_setAxis (PyObject *self, PyObject *value, void *closure);
    /// setter for the Axis attribute
    void setAxis(Py::Object arg);
    ///getter callback for the Circle attribute
    static PyObject * staticCallback_getCircle (PyObject *self, void *closure);
    /// getter for the Circle attribute
    Py::Object getCircle(void) const;
    /// setter callback for the Circle attribute
    static int staticCallback_setCircle (PyObject *self, PyObject *value, void *closure);
    // no setter method,  Circle is read only!
    //@}

    /// getter method for special attributes (e.g. dynamic ones)
    PyObject *getCustomAttributes(const char* attr) const;
    /// setter for special attributes (e.g. dynamic ones)
    int setCustomAttributes(const char* attr, PyObject *obj);
    PyObject *_getattr(char *attr);              // __getattr__ function
    int _setattr(char *attr, PyObject *value);        // __setattr__ function

    /// getter for the object handled by this class
    GeomArcOfCircle *getGeomArcOfCirclePtr(void) const;

    /** @name additional declarations and methods for the wrapper class */
    //@{

    //@}
};

#define PARENTSPartArcOfCirclePy &ArcOfCirclePy::Type,PARENTSPartGeometryCurvePy

}  //namespace Part

#endif  // PART_ARCOFCIRCLEPY_H


