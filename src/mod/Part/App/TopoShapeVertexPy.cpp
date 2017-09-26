
// This file is generated by src/Tools/generateTemaplates/templateClassPyExport.py out of the .XML file
// Every change you make here get lost at the next full rebuild!
// This File is normaly build as an include in TopoShapeVertexPyImp.cpp! Its not intended to be in a project!

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <Base/PyObjectBase.h>
#include <Base/Console.h>
#include <Base/Exception.h>
#include <CXX/Objects.hxx>

using Base::streq;
using namespace Part;

/// Type structure of TopoShapeVertexPy
PyTypeObject TopoShapeVertexPy::Type = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                                /*ob_size*/
    "Part.TopoShape",     /*tp_name*/
    sizeof(TopoShapeVertexPy),                       /*tp_basicsize*/
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
    "TopoShapeVertex is the OpenCasCade topological vertex wrapper",           /*tp_doc */
    0,                                                /*tp_traverse */
    0,                                                /*tp_clear */
    0,                                                /*tp_richcompare */
    0,                                                /*tp_weaklistoffset */
    0,                                                /*tp_iter */
    0,                                                /*tp_iternext */
    Part::TopoShapeVertexPy::Methods,                     /*tp_methods */
    0,                                                /*tp_members */
    Part::TopoShapeVertexPy::GetterSetter,                     /*tp_getset */
    &Part::TopoShapePy::Type,                        /*tp_base */
    0,                                                /*tp_dict */
    0,                                                /*tp_descr_get */
    0,                                                /*tp_descr_set */
    0,                                                /*tp_dictoffset */
    __PyInit,                                         /*tp_init */
    0,                                                /*tp_alloc */
    Part::TopoShapeVertexPy::PyMake,/*tp_new */
    0,                                                /*tp_free   Low-level free-memory routine */
    0,                                                /*tp_is_gc  For PyObject_IS_GC */
    0,                                                /*tp_bases */
    0,                                                /*tp_mro    method resolution order */
    0,                                                /*tp_cache */
    0,                                                /*tp_subclasses */
    0,                                                /*tp_weaklist */
    0                                                 /*tp_del */
};

/// Methods structure of TopoShapeVertexPy
PyMethodDef TopoShapeVertexPy::Methods[] = {
    {"setTolerance",
        (PyCFunction) staticCallback_setTolerance,
        METH_VARARGS,
        "Set the tolerance for the vertex."
    },
    {NULL, NULL, 0, NULL}		/* Sentinel */
};



/// Attribute structure of TopoShapeVertexPy
PyGetSetDef TopoShapeVertexPy::GetterSetter[] = {
    {"X",
        (getter) staticCallback_getX,
        (setter) staticCallback_setX, 
        "X component of this Vertex.",
        NULL
    },
    {"Y",
        (getter) staticCallback_getY,
        (setter) staticCallback_setY, 
        "Y component of this Vertex.",
        NULL
    },
    {"Z",
        (getter) staticCallback_getZ,
        (setter) staticCallback_setZ, 
        "Z component of this Vertex.",
        NULL
    },
    {"Point",
        (getter) staticCallback_getPoint,
        (setter) staticCallback_setPoint, 
        "Position of this Vertex as a Vector",
        NULL
    },
    {"Tolerance",
        (getter) staticCallback_getTolerance,
        (setter) staticCallback_setTolerance, 
        "Set or get the tolerance of the vertex",
        NULL
    },
    {NULL, NULL, NULL, NULL, NULL}		/* Sentinel */
};

// setTolerance() callback and implementer
// PyObject*  TopoShapeVertexPy::setTolerance(PyObject *args){};
// has to be implemented in TopoShapeVertexPyImp.cpp
PyObject * TopoShapeVertexPy::staticCallback_setTolerance (PyObject *self, PyObject *args)
{
    // test if twin object not allready deleted
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    // test if object is set Const
    if (((PyObjectBase*) self)->isConst()){
        PyErr_SetString(PyExc_ReferenceError, "This object is immutable, you can not set any attribute or call a non const method");
        return NULL;
    }

    try { // catches all exceptions coming up from c++ and generate a python exception
        PyObject* ret = ((TopoShapeVertexPy*)self)->setTolerance(args);
        if (ret != 0)
            ((TopoShapeVertexPy*)self)->startNotify();
        return ret;
    }
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
    catch(const boost::filesystem::filesystem_error& e) // catch boost filesystem exception
    {
        std::string str;
        str += "File system exception thrown (";
        //str += e.who();
        //str += ", ";
        str += e.what();
        str += ")\n";
        Base::Console().Error(str.c_str());
        PyErr_SetString(Base::BaseExceptionFreeCADError,str.c_str());
        return NULL;
    }
    catch(const Py::Exception&)
    {
        // The exception text is already set
        return NULL;
    }
    catch(const char* e) // catch simple string exceptions
    {
        Base::Console().Error(e);
        PyErr_SetString(Base::BaseExceptionFreeCADError,e);
        return NULL;
    }
    // in debug not all exceptions will be catched to get the attention of the developer!
#ifndef DONT_CATCH_CXX_EXCEPTIONS 
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
    catch(...)  // catch the rest!
    {
        PyErr_SetString(Base::BaseExceptionFreeCADError,"Unknown C++ exception");
        return NULL;
    }
#endif
}

// X() callback and implementer
// PyObject*  TopoShapeVertexPy::X(PyObject *args){};
// has to be implemented in TopoShapeVertexPyImp.cpp
PyObject * TopoShapeVertexPy::staticCallback_getX (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((TopoShapeVertexPy*)self)->getX());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'X' of object 'TopoShape'");
        return NULL;
    }
}

int TopoShapeVertexPy::staticCallback_setX (PyObject *self, PyObject * /*value*/, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return -1;
    }

    PyErr_SetString(PyExc_AttributeError, "Attribute 'X' of object 'TopoShape' is read-only");
    return -1;
}

// Y() callback and implementer
// PyObject*  TopoShapeVertexPy::Y(PyObject *args){};
// has to be implemented in TopoShapeVertexPyImp.cpp
PyObject * TopoShapeVertexPy::staticCallback_getY (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((TopoShapeVertexPy*)self)->getY());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'Y' of object 'TopoShape'");
        return NULL;
    }
}

int TopoShapeVertexPy::staticCallback_setY (PyObject *self, PyObject * /*value*/, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return -1;
    }

    PyErr_SetString(PyExc_AttributeError, "Attribute 'Y' of object 'TopoShape' is read-only");
    return -1;
}

// Z() callback and implementer
// PyObject*  TopoShapeVertexPy::Z(PyObject *args){};
// has to be implemented in TopoShapeVertexPyImp.cpp
PyObject * TopoShapeVertexPy::staticCallback_getZ (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((TopoShapeVertexPy*)self)->getZ());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'Z' of object 'TopoShape'");
        return NULL;
    }
}

int TopoShapeVertexPy::staticCallback_setZ (PyObject *self, PyObject * /*value*/, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return -1;
    }

    PyErr_SetString(PyExc_AttributeError, "Attribute 'Z' of object 'TopoShape' is read-only");
    return -1;
}

// Point() callback and implementer
// PyObject*  TopoShapeVertexPy::Point(PyObject *args){};
// has to be implemented in TopoShapeVertexPyImp.cpp
PyObject * TopoShapeVertexPy::staticCallback_getPoint (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((TopoShapeVertexPy*)self)->getPoint());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'Point' of object 'TopoShape'");
        return NULL;
    }
}

int TopoShapeVertexPy::staticCallback_setPoint (PyObject *self, PyObject * /*value*/, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return -1;
    }

    PyErr_SetString(PyExc_AttributeError, "Attribute 'Point' of object 'TopoShape' is read-only");
    return -1;
}

// Tolerance() callback and implementer
// PyObject*  TopoShapeVertexPy::Tolerance(PyObject *args){};
// has to be implemented in TopoShapeVertexPyImp.cpp
PyObject * TopoShapeVertexPy::staticCallback_getTolerance (PyObject *self, void * /*closure*/)
{
    if (!((PyObjectBase*) self)->isValid()){
        PyErr_SetString(PyExc_ReferenceError, "This object is already deleted most likely through closing a document. This reference is no longer valid!");
        return NULL;
    }

    try {
        return Py::new_reference_to(((TopoShapeVertexPy*)self)->getTolerance());
    } catch (const Py::Exception&) {
        // The exception text is already set
        return NULL;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while reading attribute 'Tolerance' of object 'TopoShape'");
        return NULL;
    }
}

int TopoShapeVertexPy::staticCallback_setTolerance (PyObject *self, PyObject *value, void * /*closure*/)
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
        ((TopoShapeVertexPy*)self)->setTolerance(Py::Float(PyNumber_Float(value),true));
        return 0;
    } catch (const Py::Exception&) {
        // The exception text is already set
        return -1;
    } catch (...) {
        PyErr_SetString(Base::BaseExceptionFreeCADError, "Unknown exception while writing attribute 'Tolerance' of object 'TopoShape'");
        return -1;
    }
}



//--------------------------------------------------------------------------
// Parents structure
//--------------------------------------------------------------------------
PyParentObject TopoShapeVertexPy::Parents[] = { PARENTSPartTopoShapeVertexPy };

//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------
TopoShapeVertexPy::TopoShapeVertexPy(TopoShape *pcObject, PyTypeObject *T)
    : TopoShapePy(reinterpret_cast<TopoShapePy::PointerType>(pcObject), T)
{
}


//--------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------
TopoShapeVertexPy::~TopoShapeVertexPy()                                // Everything handled in parent
{
}

//--------------------------------------------------------------------------
// TopoShapeVertexPy representation
//--------------------------------------------------------------------------
PyObject *TopoShapeVertexPy::_repr(void)
{
    return Py_BuildValue("s", representation().c_str());
}

//--------------------------------------------------------------------------
// TopoShapeVertexPy Attributes
//--------------------------------------------------------------------------
PyObject *TopoShapeVertexPy::_getattr(char *attr)				// __getattr__ function: note only need to handle new state
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
        return TopoShapePy::_getattr(attr);
    }
    else
    {
        return rvalue;
    }
}

int TopoShapeVertexPy::_setattr(char *attr, PyObject *value) 	// __setattr__ function: note only need to handle new state
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

    return TopoShapePy::_setattr(attr, value);
}

TopoShape *TopoShapeVertexPy::getTopoShapePtr(void) const
{
    return static_cast<TopoShape *>(_pcTwinPointer);
}

#if 0
/* From here on come the methods you have to implement, but NOT in this module. Implement in TopoShapeVertexPyImp.cpp! This prototypes 
 * are just for convenience when you add a new method.
 */

PyObject *TopoShapeVertexPy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // create a new instance of TopoShapeVertexPy and the Twin object 
    return new TopoShapeVertexPy(new TopoShape);
}

// constructor method
int TopoShapeVertexPy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
std::string TopoShapeVertexPy::representation(void) const
{
    return std::string("<TopoShape object>");
}

PyObject* TopoShapeVertexPy::setTolerance(PyObject *args)
{
    PyErr_SetString(PyExc_NotImplementedError, "Not yet implemented");
    return 0;
}



Py::Float TopoShapeVertexPy::getX(void) const
{
    //return Py::Float();
    throw Py::AttributeError("Not yet implemented");
}

Py::Float TopoShapeVertexPy::getY(void) const
{
    //return Py::Float();
    throw Py::AttributeError("Not yet implemented");
}

Py::Float TopoShapeVertexPy::getZ(void) const
{
    //return Py::Float();
    throw Py::AttributeError("Not yet implemented");
}

Py::Object TopoShapeVertexPy::getPoint(void) const
{
    //return Py::Object();
    throw Py::AttributeError("Not yet implemented");
}

Py::Float TopoShapeVertexPy::getTolerance(void) const
{
    //return Py::Float();
    throw Py::AttributeError("Not yet implemented");
}

void  TopoShapeVertexPy::setTolerance(Py::Float arg)
{
    throw Py::AttributeError("Not yet implemented");
}

PyObject *TopoShapeVertexPy::getCustomAttributes(const char* attr) const
{
    return 0;
}

int TopoShapeVertexPy::setCustomAttributes(const char* attr, PyObject *obj)
{
    return 0; 
}
#endif



