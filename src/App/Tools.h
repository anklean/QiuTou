#pragma once
#include <map>
#include "PreCompiled.h"
#include "GrindingWheelData.h"

class AppExport Tools
{
public:
	Tools();
	virtual ~Tools();

	std::map<QString, GrindingWheelData> getDataList() const { return m_dataList; }
	void setDataList(std::map<QString, GrindingWheelData> val) { m_dataList = val; }

	void addGrindingWheel(GrindingWheelData data);
	void rmvGrindingWheel(QString name);

	GrindingWheelData findGrindingWheel(QString name);
	bool IsExisted(QString name);

private:
	std::map<QString, GrindingWheelData> m_dataList;
	
};

