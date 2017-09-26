#pragma once
#include "PreCompiled.h"
#include <QString>
#include "Base\Writer.h"
#include "Base\Reader.h"
#include "Base\Vector3D.h"

class AppExport GrindingWheelData
{
public:
	GrindingWheelData();
	virtual ~GrindingWheelData();

	QString getName() const { return name; }
	void setName(QString val) { name = val; }

	double getDiameter() const { return diameter; }
	void setDiameter(double val) { diameter = val; }

	double getWidth() const { return width; }
	void setWidth(double val) { width = val; }

	double getAngle() const { return angle; }
	void setAngle(double val) { angle = val; }

	void Save(Base::Writer &writer) const;
    void Restore(Base::XMLReader &reader);

	Base::Vector3d getDirection() const { return direction; }
	void setDirection(Base::Vector3d val) { direction = val; }
private:
	QString name; //�������ƻ��߱��

	//��������==ɰ��
	double diameter;//���ߵ�ֱ��
	double width;//���ߵĿ��
	double angle;//���ߵĽǶ�

	Base::Vector3d direction;//���ߵķ���
};

