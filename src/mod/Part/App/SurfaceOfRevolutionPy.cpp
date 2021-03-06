
// This file is generated by src/Tools/generateTemaplates/templateClassPyExport.py out of the .XML file
// Every change you make here get lost at the next full rebuild!
// This File is normaly build as an include in SurfaceOfRevolutionPyImp.cpp! Its not intended to be in a project!

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <Base/PyObjectBase.h>
#include <Base/Console.h>
#include <Base/Exception.h>
#include <CXX/Objects.hxx>

using Base::streq;
using namespace Part;

/// Type structure of SurfaceOfRevolutionPy
PyTypeObject SurfaceOfRevolutionPy::Type = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                                /*ob_size*/
    "Part.GeomSurfaceOfRevolution",     /*tp_name*/
    sizeof(SurfaceOfRevolutionPy),                       /*tp_basicsize*/
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
    "Describes a surface of revolution",           /*tp_doc */
    0,                                                /*tp_traverse */
    0,                                                /*tp_clear */
    0,                                                /*tp_richcompare */
    0,                                                /*tp_weaklistoffset */
    0,                                                /*tp_iter */
    0,                                                /*tp_iternext */
    Part::SurfaceOfRevolutionPy::Methods,                     /*tp_methods */
    0,                                                /*tp_members */
    Part::SurfaceOfRevolutionPy::GetterSetter,                     /*tp_getset */
    &Part::GeometrySurfacePy::Type,                        /*tp_base */
    0,                                                /*tp_dict */
    0,                                                /*tp_descr_get */
    0,                                                /*tp_descr_set */
    0,                                                /*tp_dictoffset */
    __PyInit,                                         /*tp_init */
    0,                                                /*tp_alloc */
    Part::SurfaceOfRevolutionPy::PyMake,/*tp_new */
    0,                                                /*tp_free   Low-level free-memory routine */
    0,                                                /*tp_is_gc  For PyObject_IS_GC */
    0,                                                /*tp_bases */
    0,                                                /*tp_mro    method resolution order */
    0,                                                /*tp_cache */
    0,                                                /*tp_subclasses */
    0,                                                /*tp_weaklist */
    0                                                 /*tp_del */
};

/// Methods structure of SurfaceOfRevolutionPy
PyMethodDef SurfaceOfRevolutionPy::Methods[] = {
    {NULL, NULL, 0, NULL}		/* Sentinel */
};



/// Attribute structure of SurfaceOfRevolutionPy
PyGetSetDef SurfaceOfRevolutionPy::GetterSetter[] = {
    {"Location",
        (getter) staticCallback_getLocation,
        (setter) staticCallback_setLocation, 
        "\n					Sets or gets the location of revolution.\n				",
        NULL
    },
    {"Direction",
        (getter) staticCallback_getDirection,
        (setter) staticCallback_setDirection, 
        "\n					Sets or gets the direction of revolution.\n				",
        NULL
    },
    {"BasisCurve",
        (getter) staticCallback_getBasisCurve,
        (setter) staticCallback_setBasisCurve, 
        "\n					Sets or gets the basic curve.\n				",
        NULL
    },
    {NULL, NULL, NULL, NULL, NULL}		/* Sentinel */
};

// Location() callback and implementer
// PyObject*  SurfaceOfRevolutionPy::Location(PyObject *args){};
// has to be implemented in SurfaceOfRevolutionPyImp.cpp
PyObject * SurfaceOfRevolutionPy::staticCallback_getLocation (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((SurfaceOfRevolutionPy*)self)->getLocation());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'Location' of object 'GeomSurfaceOfRevolution'");
        return NULL;
    }
}

int SurfaceOfRevolutionPy::staticCallback_setLocation (PyObject *self, PyObject *value, void * /*closure*/)
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
        ((SurfaceOfRevolutionPy*)self)->setLocation(Py::Object(value,false));
        return 0;
    } catch (const Py::Exception&) {
        // The exception text is already set
        return -1;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while writing attribute 'Location' of object 'GeomSurfaceOfRevolution'");
        return -1;
    }
}

// Direction() callback and implementer
// PyObject*  SurfaceOfRevolutionPy::Direction(PyObject *args){};
// has to be implemented in SurfaceOfRevolutionPyImp.cpp
PyObject * SurfaceOfRevolutionPy::staticCallback_getDirection (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((SurfaceOfRevolutionPy*)self)->getDirection());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'Direction' of object 'GeomSurfaceOfRevolution'");
        return NULL;
    }
}

int SurfaceOfRevolutionPy::staticCallback_setDirection (PyObject *self, PyObject *value, void * /*closure*/)
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
        ((SurfaceOfRevolutionPy*)self)->setDirection(Py::Object(value,false));
        return 0;
    } catch (const Py::Exception&) {
        // The exception text is already set
        return -1;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while writing attribute 'Direction' of object 'GeomSurfaceOfRevolution'");
        return -1;
    }
}

// BasisCurve() callback and implementer
// PyObject*  SurfaceOfRevolutionPy::BasisCurve(PyObject *args){};
// has to be implemented in SurfaceOfRevolutionPyImp.cpp
PyObject * SurfaceOfRevolutionPy::staticCallback_getBasisCurve (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((SurfaceOfRevolutionPy*)self)->getBasisCurve());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'BasisCurve' of object 'GeomSurfaceOfRevolution'");
        return NULL;
    }
}

int SurfaceOfRevolutionPy::staticCallback_setBasisCurve (PyObject *self, PyObject *value, void * /*closure*/)
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
        ((SurfaceOfRevolutionPy*)self)->setBasisCurve(Py::Object(value,false));
        return 0;
    } catch (const Py::Exception&) {
        // The exception text is already set
        return -1;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while writing attribute 'BasisCurve' of object 'GeomSurfaceOfRevolution'");
        return -1;
    }
}



//--------------------------------------------------------------------------
// Parents structure
//--------------------------------------------------------------------------
PyParentObject SurfaceOfRevolutionPy::Parents[] = { PARENTSPartSurfaceOfRevolutionPy };

//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------
SurfaceOfRevolutionPy::SurfaceOfRevolutionPy(GeomSurfaceOfRevolution *pcObject, PyTypeObject *T)
    : GeometrySurfacePy(reinterpret_cast<GeometrySurfacePy::PointerType>(pcObject), T)
{
}


//--------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------
SurfaceOfRevolutionPy::~SurfaceOfRevolutionPy()                                // Everything handled in parent
{
}

//--------------------------------------------------------------------------
// SurfaceOfRevolutionPy representation
//--------------------------------------------------------------------------
PyObject *SurfaceOfRevolutionPy::_repr(void)
{
    return Py_BuildValue("s", representation().c_str());
}

//--------------------------------------------------------------------------
// SurfaceOfRevolutionPy Attributes
//--------------------------------------------------------------------------
PyObject *SurfaceOfRevolutionPy::_getattr(char *attr)				// __getattr__ function: note only need to handle new state
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
        return GeometrySurfacePy::_getattr(attr);
    }
    else
    {
        return rvalue;
    }
}

int SurfaceOfRevolutionPy::_setattr(char *attr, PyObject *value) 	// __setattr__ function: note only need to handle new state
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

    return GeometrySurfacePy::_setattr(attr, value);
}

GeomSurfaceOfRevolution *SurfaceOfRevolutionPy::getGeomSurfaceOfRevolutionPtr(void) const
{
    return static_cast<GeomSurfaceOfRevolution *>(_pcTwinPointer);
}

#if 0
/* From here on come the methods you have to implement, but NOT in this module. Implement in SurfaceOfRevolutionPyImp.cpp! This prototypes 
 * are just for convenience when you add a new method.
 */

PyObject *SurfaceOfRevolutionPy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // create a new instance of SurfaceOfRevolutionPy and the Twin object 
    return new SurfaceOfRevolutionPy(new GeomSurfaceOfRevolution);
}

// constructor method
int SurfaceOfRevolutionPy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
std::string SurfaceOfRevolutionPy::representation(void) const
{
    return std::string("<GeomSurfaceOfRevolution object>");
}



Py::Object SurfaceOfRevolutionPy::getLocation(void) const
{
    //return Py::Object();
    throw Py::AttributeError("Not yet implemented");
}

void  SurfaceOfRevolutionPy::setLocation(Py::Object arg)
{
    throw Py::AttributeError("Not yet implemented");
}

Py::Object SurfaceOfRevolutionPy::getDirection(void) const
{
    //return Py::Object();
    throw Py::AttributeError("Not yet implemented");
}

void  SurfaceOfRevolutionPy::setDirection(Py::Object arg)
{
    throw Py::AttributeError("Not yet implemented");
}

Py::Object SurfaceOfRevolutionPy::getBasisCurve(void) const
{
    //return Py::Object();
    throw Py::AttributeError("Not yet implemented");
}

void  SurfaceOfRevolutionPy::setBasisCurve(Py::Object arg)
{
    throw Py::AttributeError("Not yet implemented");
}

PyObject *SurfaceOfRevolutionPy::getCustomAttributes(const char* attr) const
{
    return 0;
}

int SurfaceOfRevolutionPy::setCustomAttributes(const char* attr, PyObject *obj)
{
    return 0; 
}
#endif



