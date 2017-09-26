
// This file is generated by src/Tools/generateTemaplates/templateClassPyExport.py out of the .XML file
// Every change you make here get lost at the next full rebuild!
// This File is normaly build as an include in ArcOfEllipsePyImp.cpp! Its not intended to be in a project!

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <Base/PyObjectBase.h>
#include <Base/Console.h>
#include <Base/Exception.h>
#include <CXX/Objects.hxx>

using Base::streq;
using namespace Part;

/// Type structure of ArcOfEllipsePy
PyTypeObject ArcOfEllipsePy::Type = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                                /*ob_size*/
    "Part.GeomArcOfEllipse",     /*tp_name*/
    sizeof(ArcOfEllipsePy),                       /*tp_basicsize*/
    0,                                                /*tp_itemsize*/
    /* methods */
    PyDestructor,                                     /*tp_dealloc*/
    0,                                                /*tp_print*/
    __getattr,                                        /*tp_getattr*/
    __setattr,                                        /*tp_setattr*/
    0,                                                /*tp_compare*/
    __repr,                                           /*tp_repr*/
    0,                                                /*tp_as_number*/
    0,                                                /*tp_as_sequence*/
    0,                                                /*tp_as_mapping*/
    0,                                                /*tp_hash*/
    0,                                                /*tp_call */
    0,                                                /*tp_str  */
    0,                                                /*tp_getattro*/
    0,                                                /*tp_setattro*/
    /* --- Functions to access object as input/output buffer ---------*/
    0,                                                /* tp_as_buffer */
    /* --- Flags to define presence of optional/expanded features */
    Py_TPFLAGS_BASETYPE|Py_TPFLAGS_HAVE_CLASS,        /*tp_flags */
    "Describes a portion of an ellipse",           /*tp_doc */
    0,                                                /*tp_traverse */
    0,                                                /*tp_clear */
    0,                                                /*tp_richcompare */
    0,                                                /*tp_weaklistoffset */
    0,                                                /*tp_iter */
    0,                                                /*tp_iternext */
    Part::ArcOfEllipsePy::Methods,                     /*tp_methods */
    0,                                                /*tp_members */
    Part::ArcOfEllipsePy::GetterSetter,                     /*tp_getset */
    &Part::GeometryCurvePy::Type,                        /*tp_base */
    0,                                                /*tp_dict */
    0,                                                /*tp_descr_get */
    0,                                                /*tp_descr_set */
    0,                                                /*tp_dictoffset */
    __PyInit,                                         /*tp_init */
    0,                                                /*tp_alloc */
    Part::ArcOfEllipsePy::PyMake,/*tp_new */
    0,                                                /*tp_free   Low-level free-memory routine */
    0,                                                /*tp_is_gc  For PyObject_IS_GC */
    0,                                                /*tp_bases */
    0,                                                /*tp_mro    method resolution order */
    0,                                                /*tp_cache */
    0,                                                /*tp_subclasses */
    0,                                                /*tp_weaklist */
    0                                                 /*tp_del */
};

/// Methods structure of ArcOfEllipsePy
PyMethodDef ArcOfEllipsePy::Methods[] = {
    {NULL, NULL, 0, NULL}		/* Sentinel */
};



/// Attribute structure of ArcOfEllipsePy
PyGetSetDef ArcOfEllipsePy::GetterSetter[] = {
    {"MajorRadius",
        (getter) staticCallback_getMajorRadius,
        (setter) staticCallback_setMajorRadius, 
        "The major radius of the ellipse.",
        NULL
    },
    {"MinorRadius",
        (getter) staticCallback_getMinorRadius,
        (setter) staticCallback_setMinorRadius, 
        "The minor radius of the ellipse.",
        NULL
    },
    {"AngleXU",
        (getter) staticCallback_getAngleXU,
        (setter) staticCallback_setAngleXU, 
        "The angle between the X axis and the major axis of the ellipse.",
        NULL
    },
    {"Center",
        (getter) staticCallback_getCenter,
        (setter) staticCallback_setCenter, 
        "Center of the ellipse.",
        NULL
    },
    {"Axis",
        (getter) staticCallback_getAxis,
        (setter) staticCallback_setAxis, 
        "The axis direction of the ellipse",
        NULL
    },
    {"Ellipse",
        (getter) staticCallback_getEllipse,
        (setter) staticCallback_setEllipse, 
        "The internal ellipse representation",
        NULL
    },
    {NULL, NULL, NULL, NULL, NULL}		/* Sentinel */
};

// MajorRadius() callback and implementer
// PyObject*  ArcOfEllipsePy::MajorRadius(PyObject *args){};
// has to be implemented in ArcOfEllipsePyImp.cpp
PyObject * ArcOfEllipsePy::staticCallback_getMajorRadius (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((ArcOfEllipsePy*)self)->getMajorRadius());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'MajorRadius' of object 'GeomArcOfEllipse'");
        return NULL;
    }
}

int ArcOfEllipsePy::staticCallback_setMajorRadius (PyObject *self, PyObject *value, void * /*closure*/)
{    
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return -1;
    }
    if (((PyObjectBase*) self)->isConst()){
        PyErr_SetString(PyExc_ReferenceError, "This object is immutable, you can not set any attribute or call a method");
        return -1;
    }

    try {
        ((ArcOfEllipsePy*)self)->setMajorRadius(Py::Float(PyNumber_Float(value),true));
        return 0;
    } catch (const Py::Exception&) {
        // The exception text is already set
        return -1;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while writing attribute 'MajorRadius' of object 'GeomArcOfEllipse'");
        return -1;
    }
}

// MinorRadius() callback and implementer
// PyObject*  ArcOfEllipsePy::MinorRadius(PyObject *args){};
// has to be implemented in ArcOfEllipsePyImp.cpp
PyObject * ArcOfEllipsePy::staticCallback_getMinorRadius (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((ArcOfEllipsePy*)self)->getMinorRadius());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'MinorRadius' of object 'GeomArcOfEllipse'");
        return NULL;
    }
}

int ArcOfEllipsePy::staticCallback_setMinorRadius (PyObject *self, PyObject *value, void * /*closure*/)
{    
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return -1;
    }
    if (((PyObjectBase*) self)->isConst()){
        PyErr_SetString(PyExc_ReferenceError, "This object is immutable, you can not set any attribute or call a method");
        return -1;
    }

    try {
        ((ArcOfEllipsePy*)self)->setMinorRadius(Py::Float(PyNumber_Float(value),true));
        return 0;
    } catch (const Py::Exception&) {
        // The exception text is already set
        return -1;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while writing attribute 'MinorRadius' of object 'GeomArcOfEllipse'");
        return -1;
    }
}

// AngleXU() callback and implementer
// PyObject*  ArcOfEllipsePy::AngleXU(PyObject *args){};
// has to be implemented in ArcOfEllipsePyImp.cpp
PyObject * ArcOfEllipsePy::staticCallback_getAngleXU (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((ArcOfEllipsePy*)self)->getAngleXU());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'AngleXU' of object 'GeomArcOfEllipse'");
        return NULL;
    }
}

int ArcOfEllipsePy::staticCallback_setAngleXU (PyObject *self, PyObject *value, void * /*closure*/)
{    
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return -1;
    }
    if (((PyObjectBase*) self)->isConst()){
        PyErr_SetString(PyExc_ReferenceError, "This object is immutable, you can not set any attribute or call a method");
        return -1;
    }

    try {
        ((ArcOfEllipsePy*)self)->setAngleXU(Py::Float(PyNumber_Float(value),true));
        return 0;
    } catch (const Py::Exception&) {
        // The exception text is already set
        return -1;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while writing attribute 'AngleXU' of object 'GeomArcOfEllipse'");
        return -1;
    }
}

// Center() callback and implementer
// PyObject*  ArcOfEllipsePy::Center(PyObject *args){};
// has to be implemented in ArcOfEllipsePyImp.cpp
PyObject * ArcOfEllipsePy::staticCallback_getCenter (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((ArcOfEllipsePy*)self)->getCenter());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'Center' of object 'GeomArcOfEllipse'");
        return NULL;
    }
}

int ArcOfEllipsePy::staticCallback_setCenter (PyObject *self, PyObject *value, void * /*closure*/)
{    
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return -1;
    }
    if (((PyObjectBase*) self)->isConst()){
        PyErr_SetString(PyExc_ReferenceError, "This object is immutable, you can not set any attribute or call a method");
        return -1;
    }

    try {
        ((ArcOfEllipsePy*)self)->setCenter(Py::Object(value,false));
        return 0;
    } catch (const Py::Exception&) {
        // The exception text is already set
        return -1;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while writing attribute 'Center' of object 'GeomArcOfEllipse'");
        return -1;
    }
}

// Axis() callback and implementer
// PyObject*  ArcOfEllipsePy::Axis(PyObject *args){};
// has to be implemented in ArcOfEllipsePyImp.cpp
PyObject * ArcOfEllipsePy::staticCallback_getAxis (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((ArcOfEllipsePy*)self)->getAxis());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'Axis' of object 'GeomArcOfEllipse'");
        return NULL;
    }
}

int ArcOfEllipsePy::staticCallback_setAxis (PyObject *self, PyObject *value, void * /*closure*/)
{    
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return -1;
    }
    if (((PyObjectBase*) self)->isConst()){
        PyErr_SetString(PyExc_ReferenceError, "This object is immutable, you can not set any attribute or call a method");
        return -1;
    }

    try {
        ((ArcOfEllipsePy*)self)->setAxis(Py::Object(value,false));
        return 0;
    } catch (const Py::Exception&) {
        // The exception text is already set
        return -1;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while writing attribute 'Axis' of object 'GeomArcOfEllipse'");
        return -1;
    }
}

// Ellipse() callback and implementer
// PyObject*  ArcOfEllipsePy::Ellipse(PyObject *args){};
// has to be implemented in ArcOfEllipsePyImp.cpp
PyObject * ArcOfEllipsePy::staticCallback_getEllipse (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((ArcOfEllipsePy*)self)->getEllipse());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'Ellipse' of object 'GeomArcOfEllipse'");
        return NULL;
    }
}

int ArcOfEllipsePy::staticCallback_setEllipse (PyObject *self, PyObject * /*value*/, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return -1;
    }

    PyErr_SetString(PyExc_AttributeError, "Attribute 'Ellipse' of object 'GeomArcOfEllipse' is read-only");
    return -1;
}



//--------------------------------------------------------------------------
// Parents structure
//--------------------------------------------------------------------------
PyParentObject ArcOfEllipsePy::Parents[] = { PARENTSPartArcOfEllipsePy };

//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------
ArcOfEllipsePy::ArcOfEllipsePy(GeomArcOfEllipse *pcObject, PyTypeObject *T)
    : GeometryCurvePy(reinterpret_cast<GeometryCurvePy::PointerType>(pcObject), T)
{
}


//--------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------
ArcOfEllipsePy::~ArcOfEllipsePy()                                // Everything handled in parent
{
}

//--------------------------------------------------------------------------
// ArcOfEllipsePy representation
//--------------------------------------------------------------------------
PyObject *ArcOfEllipsePy::_repr(void)
{
    return Py_BuildValue("s", representation().c_str());
}

//--------------------------------------------------------------------------
// ArcOfEllipsePy Attributes
//--------------------------------------------------------------------------
PyObject *ArcOfEllipsePy::_getattr(char *attr)				// __getattr__ function: note only need to handle new state
{
    try {
        // getter method for special Attributes (e.g. dynamic ones)
        PyObject *r = getCustomAttributes(attr);
        if(r) return r;
    }
#ifndef DONT_CATCH_CXX_EXCEPTIONS 
    catch(const Base::Exception& e) // catch the FreeCAD exceptions
    {
        std::string str;
        str += "FreeCAD exception thrown (";
        str += e.what();
        str += ")";
        e.ReportException();
        PyErr_SetString(Base::BaseExceptionFreeCADError,str.c_str());
        return NULL;
    }
    catch(const std::exception& e) // catch other c++ exceptions
    {
        std::string str;
        str += "FC++ exception thrown (";
        str += e.what();
        str += ")";
        Base::Console().Error(str.c_str());
        PyErr_SetString(Base::BaseExceptionFreeCADError,str.c_str());
        return NULL;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return NULL;
    }
    catch(...)  // catch the rest!
    {
        PyErr_SetString(Base::BaseExceptionFreeCADError,"Unknown C++ exception");
        return NULL;
    }
#else  // DONT_CATCH_CXX_EXCEPTIONS  
    catch(const Base::Exception& e) // catch the FreeCAD exceptions
    {
        std::string str;
        str += "FreeCAD exception thrown (";
        str += e.what();
        str += ")";
        e.ReportException();
        PyErr_SetString(Base::BaseExceptionFreeCADError,str.c_str());
        return NULL;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return NULL;
    }
#endif  // DONT_CATCH_CXX_EXCEPTIONS

    PyObject *rvalue = Py_FindMethod(Methods, this, attr);
    if (rvalue == NULL)
    {
        PyErr_Clear();
        return GeometryCurvePy::_getattr(attr);
    }
    else
    {
        return rvalue;
    }
}

int ArcOfEllipsePy::_setattr(char *attr, PyObject *value) 	// __setattr__ function: note only need to handle new state
{
    try {
        // setter for  special Attributes (e.g. dynamic ones)
        int r = setCustomAttributes(attr, value);
        if(r==1) return 0;
    }
#ifndef DONT_CATCH_CXX_EXCEPTIONS 
    catch(const Base::Exception& e) // catch the FreeCAD exceptions
    {
        std::string str;
        str += "FreeCAD exception thrown (";
        str += e.what();
        str += ")";
        e.ReportException();
        PyErr_SetString(Base::BaseExceptionFreeCADError,str.c_str());
        return -1;
    }
    catch(const std::exception& e) // catch other c++ exceptions
    {
        std::string str;
        str += "FC++ exception thrown (";
        str += e.what();
        str += ")";
        Base::Console().Error(str.c_str());
        PyErr_SetString(Base::BaseExceptionFreeCADError,str.c_str());
        return -1;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return -1;
    }
    catch(...)  // catch the rest!
    {
        PyErr_SetString(Base::BaseExceptionFreeCADError,"Unknown C++ exception");
        return -1;
    }
#else  // DONT_CATCH_CXX_EXCEPTIONS  
    catch(const Base::Exception& e) // catch the FreeCAD exceptions
    {
        std::string str;
        str += "FreeCAD exception thrown (";
        str += e.what();
        str += ")";
        e.ReportException();
        PyErr_SetString(Base::BaseExceptionFreeCADError,str.c_str());
        return -1;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return -1;
    }
#endif  // DONT_CATCH_CXX_EXCEPTIONS

    return GeometryCurvePy::_setattr(attr, value);
}

GeomArcOfEllipse *ArcOfEllipsePy::getGeomArcOfEllipsePtr(void) const
{
    return static_cast<GeomArcOfEllipse *>(_pcTwinPointer);
}

#if 0
/* From here on come the methods you have to implement, but NOT in this module. Implement in ArcOfEllipsePyImp.cpp! This prototypes 
 * are just for convenience when you add a new method.
 */

PyObject *ArcOfEllipsePy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // create a new instance of ArcOfEllipsePy and the Twin object 
    return new ArcOfEllipsePy(new GeomArcOfEllipse);
}

// constructor method
int ArcOfEllipsePy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
std::string ArcOfEllipsePy::representation(void) const
{
    return std::string("<GeomArcOfEllipse object>");
}



Py::Float ArcOfEllipsePy::getMajorRadius(void) const
{
    //return Py::Float();
    throw Py::AttributeError("Not yet implemented");
}

void  ArcOfEllipsePy::setMajorRadius(Py::Float arg)
{
    throw Py::AttributeError("Not yet implemented");
}

Py::Float ArcOfEllipsePy::getMinorRadius(void) const
{
    //return Py::Float();
    throw Py::AttributeError("Not yet implemented");
}

void  ArcOfEllipsePy::setMinorRadius(Py::Float arg)
{
    throw Py::AttributeError("Not yet implemented");
}

Py::Float ArcOfEllipsePy::getAngleXU(void) const
{
    //return Py::Float();
    throw Py::AttributeError("Not yet implemented");
}

void  ArcOfEllipsePy::setAngleXU(Py::Float arg)
{
    throw Py::AttributeError("Not yet implemented");
}

Py::Object ArcOfEllipsePy::getCenter(void) const
{
    //return Py::Object();
    throw Py::AttributeError("Not yet implemented");
}

void  ArcOfEllipsePy::setCenter(Py::Object arg)
{
    throw Py::AttributeError("Not yet implemented");
}

Py::Object ArcOfEllipsePy::getAxis(void) const
{
    //return Py::Object();
    throw Py::AttributeError("Not yet implemented");
}

void  ArcOfEllipsePy::setAxis(Py::Object arg)
{
    throw Py::AttributeError("Not yet implemented");
}

Py::Object ArcOfEllipsePy::getEllipse(void) const
{
    //return Py::Object();
    throw Py::AttributeError("Not yet implemented");
}

PyObject *ArcOfEllipsePy::getCustomAttributes(const char* attr) const
{
    return 0;
}

int ArcOfEllipsePy::setCustomAttributes(const char* attr, PyObject *obj)
{
    return 0; 
}
#endif



