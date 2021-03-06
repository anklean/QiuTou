
// This file is generated by src/Tools/generateTemaplates/templateClassPyExport.py out of the XML file
// Every change you make here get lost at the next full rebuild!
#ifndef PART_OFFSETCURVEPY_H
#define PART_OFFSETCURVEPY_H

#include <Mod/Part/App/GeometryCurvePy.h>
#include <Mod/Part/App/Geometry.h>
#include <string>

namespace Part
{

//===========================================================================
// OffsetCurvePy - Python wrapper
//===========================================================================

/** The python export class for GeomOffsetCurve
 */
class PartExport OffsetCurvePy : public Part::GeometryCurvePy
{
public:
    static PyTypeObject   Type;
    static PyMethodDef    Methods[];
    static PyGetSetDef    GetterSetter[];
    static PyParentObject Parents[];
    virtual PyTypeObject *GetType(void) {return &Type;};
    virtual PyParentObject *GetParents(void) {return Parents;}

public:
    OffsetCurvePy(GeomOffsetCurve *pcObject, PyTypeObject *T = &Type);
    static PyObject *PyMake(struct _typeobject *, PyObject *, PyObject *);
    virtual int PyInit(PyObject* args, PyObject*k);
    ~OffsetCurvePy();

    typedef GeomOffsetCurve* PointerType ;

    virtual PyObject *_repr(void);        // the representation
    std::string representation(void) const;

    /** @name callbacks and implementers for the python object methods */
    //@{
    //@}


    /** @name callbacks and implementers for the python object attributes */
    //@{
    ///getter callback for the OffsetValue attribute
    static PyObject * staticCallback_getOffsetValue (PyObject *self, void *closure);
    /// getter for the OffsetValue attribute
    Py::Float getOffsetValue(void) const;
    /// setter callback for the OffsetValue attribute
    static int staticCallback_setOffsetValue (PyObject *self, PyObject *value, void *closure);
    /// setter for the OffsetValue attribute
    void setOffsetValue(Py::Float arg);
    ///getter callback for the OffsetDirection attribute
    static PyObject * staticCallback_getOffsetDirection (PyObject *self, void *closure);
    /// getter for the OffsetDirection attribute
    Py::Object getOffsetDirection(void) const;
    /// setter callback for the OffsetDirection attribute
    static int staticCallback_setOffsetDirection (PyObject *self, PyObject *value, void *closure);
    /// setter for the OffsetDirection attribute
    void setOffsetDirection(Py::Object arg);
    ///getter callback for the BasisCurve attribute
    static PyObject * staticCallback_getBasisCurve (PyObject *self, void *closure);
    /// getter for the BasisCurve attribute
    Py::Object getBasisCurve(void) const;
    /// setter callback for the BasisCurve attribute
    static int staticCallback_setBasisCurve (PyObject *self, PyObject *value, void *closure);
    /// setter for the BasisCurve attribute
    void setBasisCurve(Py::Object arg);
    //@}

    /// getter method for special attributes (e.g. dynamic ones)
    PyObject *getCustomAttributes(const char* attr) const;
    /// setter for special attributes (e.g. dynamic ones)
    int setCustomAttributes(const char* attr, PyObject *obj);
    PyObject *_getattr(char *attr);              // __getattr__ function
    int _setattr(char *attr, PyObject *value);        // __setattr__ function

    /// getter for the object handled by this class
    GeomOffsetCurve *getGeomOffsetCurvePtr(void) const;

    /** @name additional declarations and methods for the wrapper class */
    //@{

    //@}
};

#define PARENTSPartOffsetCurvePy &OffsetCurvePy::Type,PARENTSPartGeometryCurvePy

}  //namespace Part

#endif  // PART_OFFSETCURVEPY_H


