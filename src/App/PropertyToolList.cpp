#include "PreCompiled.h"

#ifndef _PreComp_
#   include <assert.h>
#endif

/// Here the FreeCAD includes sorted by Base,App,Gui......

#include <Base/Exception.h>
#include <Base/Reader.h>
#include <Base/Writer.h>

#include "PropertyToolList.h"

using namespace App;
using namespace Base;
using namespace std;


//**************************************************************************
// PropertyToolList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TYPESYSTEM_SOURCE(App::PropertyToolList, App::Property);

//**************************************************************************
// Construction/Destruction


PropertyToolList::PropertyToolList()
{

}

PropertyToolList::~PropertyToolList()
{
}

void PropertyToolList::setValue(const Tools& tools)
{
	aboutToSetValue();
	_ToolList = tools;
	hasSetValue();
}

//PyObject *PropertyToolList::getPyObject(void)
//{
//	std::map<QString, GrindingWheelData> toolList = _ToolList.getDataList();
//	PyObject* list = PyList_New(toolList.size());
//	for (int i = 0; i < toolList.size(); i++)
//        PyList_SetItem( list, i, 0);
//    return list;
//}

//void PropertyToolList::setPyObject(PyObject *value)
//{
    // check container of this property to notify about changes
    /*Part2DObject* part2d = dynamic_cast<Part2DObject*>(this->getContainer());

    if (PyList_Check(value)) {
        Py_ssize_t nSize = PyList_Size(value);
        std::vector<Geometry*> values;
        values.resize(nSize);

        for (Py_ssize_t i=0; i < nSize; ++i) {
            PyObject* item = PyList_GetItem(value, i);
            if (!PyObject_TypeCheck(item, &(GeometryPy::Type))) {
                std::string error = std::string("types in list must be 'Geometry', not ");
                error += item->ob_type->tp_name;
                throw Base::TypeError(error);
            }

            values[i] = static_cast<GeometryPy*>(item)->getGeometryPtr();
        }

        setValues(values);
        if (part2d)
            part2d->acceptGeometry();
    }
    else if (PyObject_TypeCheck(value, &(GeometryPy::Type))) {
        GeometryPy  *pcObject = static_cast<GeometryPy*>(value);
        setValue(pcObject->getGeometryPtr());
        if (part2d)
            part2d->acceptGeometry();
    }
    else {
        std::string error = std::string("type must be 'Geometry' or list of 'Geometry', not ");
        error += value->ob_type->tp_name;
        throw Base::TypeError(error);
    }*/
//}

void PropertyToolList::Save(Writer &writer) const
{
	std::map<QString, GrindingWheelData> toolList = _ToolList.getDataList();
	writer.Stream() << writer.ind() << "<ToolList count=\"" << toolList.size() << "\">" << endl;
    writer.incInd();

	for (std::map<QString, GrindingWheelData>::iterator it = toolList.begin();
		it != toolList.end(); it++)
	{
		it->second.Save(writer);
	}
    writer.decInd();
    writer.Stream() << writer.ind() << "</ToolList>" << endl ;
}

void PropertyToolList::Restore(Base::XMLReader &reader)
{
    // read my element
    reader.readElement("ToolList");
    // get the value of my attribute
    int count = reader.getAttributeAsInteger("count");

	Tools tools;
	std::map<QString, GrindingWheelData> values;
    //values.reserve(count);
    for (int i = 0; i < count; i++) {
		GrindingWheelData data;
		//reader.readElement("Tool");
        
		data.Restore(reader);
		QString name = data.getName();
		std::string ss = name.toStdString();
        values[name] = data;
    }

    reader.readEndElement("ToolList");

	tools.setDataList(values);
    // assignment
	setValue(tools);
}

App::Property *PropertyToolList::Copy(void) const
{
    PropertyToolList *p = new PropertyToolList();
    p->setValue(_ToolList);
    return p;
}

void PropertyToolList::Paste(const Property &from)
{
    const PropertyToolList& FromList = dynamic_cast<const PropertyToolList&>(from);
    setValue(FromList._ToolList);
}

unsigned int PropertyToolList::getMemSize(void) const
{
    int size = sizeof(PropertyToolList);
    //for (int i = 0; i < getSize(); i++)
    //    size += _ToolList[i]->getMemSize();
    return size;
}
