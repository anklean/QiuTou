
// This file is generated by src/Tools/generateTemaplates/templateClassPyExport.py out of the XML file
// Every change you make here get lost at the next full rebuild!
#ifndef APP_PROPERTYCONTAINERPY_H
#define APP_PROPERTYCONTAINERPY_H

#include <Base/PersistencePy.h>
#include <App/PropertyContainer.h>
#include <string>

namespace App
{

//===========================================================================
// PropertyContainerPy - Python wrapper
//===========================================================================

/** The python export class for PropertyContainer
 */
class AppExport PropertyContainerPy : public Base::PersistencePy
{
public:
    static PyTypeObject   Type;
    static PyMethodDef    Methods[];
    static PyGetSetDef    GetterSetter[];
    static PyParentObject Parents[];
    virtual PyTypeObject *GetType(void) {return &Type;};
    virtual PyParentObject *GetParents(void) {return Parents;}

public:
    PropertyContainerPy(PropertyContainer *pcObject, PyTypeObject *T = &Type);
    static PyObject *PyMake(struct _typeobject *, PyObject *, PyObject *);
    virtual int PyInit(PyObject* args, PyObject*k);
    ~PropertyContainerPy();

    typedef PropertyContainer* PointerType ;

    virtual PyObject *_repr(void);        // the representation
    std::string representation(void) const;

    /** @name callbacks and implementers for the python object methods */
    //@{
    /// callback for the getPropertyByName() method
    static PyObject * staticCallback_getPropertyByName (PyObject *self, PyObject *args);
    /// implementer for the getPropertyByName() method
    PyObject*  getPropertyByName(PyObject *args);
    /// callback for the getTypeOfProperty() method
    static PyObject * staticCallback_getTypeOfProperty (PyObject *self, PyObject *args);
    /// implementer for the getTypeOfProperty() method
    PyObject*  getTypeOfProperty(PyObject *args);
    /// callback for the getTypeIdOfProperty() method
    static PyObject * staticCallback_getTypeIdOfProperty (PyObject *self, PyObject *args);
    /// implementer for the getTypeIdOfProperty() method
    PyObject*  getTypeIdOfProperty(PyObject *args);
    /// callback for the setEditorMode() method
    static PyObject * staticCallback_setEditorMode (PyObject *self, PyObject *args);
    /// implementer for the setEditorMode() method
    PyObject*  setEditorMode(PyObject *args);
    /// callback for the getEditorMode() method
    static PyObject * staticCallback_getEditorMode (PyObject *self, PyObject *args);
    /// implementer for the getEditorMode() method
    PyObject*  getEditorMode(PyObject *args);
    /// callback for the getGroupOfProperty() method
    static PyObject * staticCallback_getGroupOfProperty (PyObject *self, PyObject *args);
    /// implementer for the getGroupOfProperty() method
    PyObject*  getGroupOfProperty(PyObject *args);
    /// callback for the getDocumentationOfProperty() method
    static PyObject * staticCallback_getDocumentationOfProperty (PyObject *self, PyObject *args);
    /// implementer for the getDocumentationOfProperty() method
    PyObject*  getDocumentationOfProperty(PyObject *args);
    //@}


    /** @name callbacks and implementers for the python object attributes */
    //@{
    ///getter callback for the PropertiesList attribute
    static PyObject * staticCallback_getPropertiesList (PyObject *self, void *closure);
    /// getter for the PropertiesList attribute
    Py::List getPropertiesList(void) const;
    /// setter callback for the PropertiesList attribute
    static int staticCallback_setPropertiesList (PyObject *self, PyObject *value, void *closure);
    // no setter method,  PropertiesList is read only!
    //@}

    /// getter method for special attributes (e.g. dynamic ones)
    PyObject *getCustomAttributes(const char* attr) const;
    /// setter for special attributes (e.g. dynamic ones)
    int setCustomAttributes(const char* attr, PyObject *obj);
    PyObject *_getattr(char *attr);              // __getattr__ function
    int _setattr(char *attr, PyObject *value);        // __setattr__ function

    /// getter for the object handled by this class
    PropertyContainer *getPropertyContainerPtr(void) const;

    /** @name additional declarations and methods for the wrapper class */
    //@{

    //@}
};

#define PARENTSAppPropertyContainerPy &PropertyContainerPy::Type,PARENTSBasePersistencePy

}  //namespace App

#endif  // APP_PROPERTYCONTAINERPY_H


