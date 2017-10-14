#include "GrindingWheelData.h"
#include "Base\Tools.h"


GrindingWheelData::GrindingWheelData()
{
	 name=QLatin1String("1"); //�������ƻ��߱��

	//��������==ɰ��
	 diameter = 0.8;//���ߵ�ֱ��
	 width=0.3;//���ߵĿ��
	 angle=90;//���ߵĽǶ�

	direction=Base::Vector3d(0,-1,0);//���ߵķ���
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