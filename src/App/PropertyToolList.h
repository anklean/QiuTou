#ifndef APP_PropertyToolList_H
#define APP_PropertyToolList_H

// Std. configurations


#include <vector>
#include <string>
#include <App/Property.h>
#include "Tools.h"


namespace Base {
class Writer;
}

namespace App
{

class AppExport PropertyToolList: public App::Property
{
    TYPESYSTEM_HEADER();

public:
    /**
     * A constructor.
     * A more elaborate description of the constructor.
     */
    PropertyToolList();

    /**
     * A destructor.
     * A more elaborate description of the destructor.
     */
    virtual ~PropertyToolList();

    /** Sets the property
     */
    void setValue(const Tools& tools);
    //void setValues(const std::vector<Geometry*>&);

    const Tools &getValue(void) const {
        return _ToolList;
    }

    //virtual PyObject *getPyObject(void);
    //virtual void setPyObject(PyObject *);

    virtual void Save(Base::Writer &writer) const;
    virtual void Restore(Base::XMLReader &reader);

    virtual App::Property *Copy(void) const;
    virtual void Paste(const App::Property &from);

    virtual unsigned int getMemSize(void) const;

private:
    Tools _ToolList;
};

} // namespace App


#endif // APP_PropertyToolList_H
