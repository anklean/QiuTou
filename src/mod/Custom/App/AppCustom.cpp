/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *   for detail see the LICENCE text file.                                 *
 *   Jürgen Riegel 2002                                                    *
 *                                                                         *
 ***************************************************************************/

#include "PreCompiled.h"
#ifndef _PreComp_
# include <Python.h>
# include <Interface_Static.hxx>
# include <IGESControl_Controller.hxx>
# include <STEPControl_Controller.hxx>
# include <OSD.hxx>
# include <sstream>
#endif

#include <Base/Console.h>
#include <Base/Interpreter.h>
#include <Base/Parameter.h>

#include <App/Application.h>
#include "BallCutter.h"
#include "PathObject.h"
#include "NCOutputter.h"

extern struct PyMethodDef Custom_methods[];
//using namespace Custom;

PyDoc_STRVAR(module_custom_doc,
"This is a module working with shapes.");

extern "C" {
void CustomExport initCustom()
{
	// load needed modules
	try {
		Base::Interpreter().runString("import Part");
	}
	catch (const Base::Exception& e) {
		PyErr_SetString(PyExc_ImportError, e.what());
		return;
	}
    // Add Types to module
    PyObject* customModule = Py_InitModule3("Custom", Custom_methods, module_custom_doc);   /* mod name, table ptr */
    Base::Console().Log("Loading Custom module... done\n");

    Custom::BallCutter::init();
	Custom::PathObject::init();
	Custom::NCOutPutter::init();

}

} // extern "C"
