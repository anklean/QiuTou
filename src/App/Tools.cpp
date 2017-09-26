#include "Tools.h"


Tools::Tools()
{
	m_dataList[QLatin1String("1")] = GrindingWheelData();
}


Tools::~Tools()
{
}

bool Tools::IsExisted(QString name)
{
	bool ret = false;
	std::map<QString, GrindingWheelData>::iterator pos;
	pos = m_dataList.find(name);
	if (pos != m_dataList.end())
	{
		ret = true;
	}
	return ret;
}

GrindingWheelData Tools::findGrindingWheel(QString name)
{
	if (IsExisted(name))
	{
		return m_dataList[name];
	}
	return GrindingWheelData();
}

void Tools::addGrindingWheel(GrindingWheelData data)
{
	QString name = data.getName();	

	if (!name.isEmpty() || !name.isNull())
	{
		if (IsExisted(name))
		{
			return;
		}
		else
		{
			m_dataList[name] = data;
		}
	}
}

void Tools::rmvGrindingWheel(QString name)
{
	std::map<QString, GrindingWheelData>::iterator pos;
	pos = m_dataList.find(name);
	if (pos != m_dataList.end())
	{
		m_dataList.erase(pos);
	}
}
