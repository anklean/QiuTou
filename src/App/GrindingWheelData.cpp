#include "GrindingWheelData.h"
#include "Base\Tools.h"


GrindingWheelData::GrindingWheelData()
{
	 name=QLatin1String("1"); //刀具名称或者编号

	//刀具类型==砂轮
	 diameter = 0.8;//刀具的直径
	 width=0.3;//刀具的宽度
	 angle=90;//刀具的角度

	direction=Base::Vector3d(0,-1,0);//刀具的方向
}


GrindingWheelData::~GrindingWheelData()
{
}

void GrindingWheelData::Save(Base::Writer &writer) const
{
	writer.Stream()
		<< writer.ind()
		<< "<GrindingWheel "
		<< "name=\"" << Base::Tools::toStdString(name) <<
		"\" diameter=\"" << diameter <<
		"\" width=\"" << width <<
		"\" angle=\"" << angle <<
		"\"/>" << std::endl;
}
void GrindingWheelData::Restore(Base::XMLReader &reader)
{
	double X, Y, Z;
	// read my Element
	reader.readElement("GrindingWheel");
	// get the value of my Attribute
	name = QString::fromUtf8(reader.getAttribute("name"));
	diameter = reader.getAttributeAsFloat("diameter");
	width = reader.getAttributeAsFloat("width");
	angle = reader.getAttributeAsFloat("angle");
}