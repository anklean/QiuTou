#pragma once
#include "PreCompiled.h"
#include "Base\Vector3D.h"
#include <QString>
#include "Base\Placement.h"
#include <vector>
#include "QStringList"

class CustomExport CreateNCData
{
public:
	CreateNCData();
	virtual ~CreateNCData();

	Base::Placement getMachineCS() const { return machineCS; }
	void setMachineCS(Base::Placement val) { machineCS = val; }

	Base::Placement getWorkCS() const { return workCS; }
	void setWorkCS(Base::Placement val) { workCS = val; }

	QStringList getPathList() const { return PathList; }
	void setPathList(QStringList& val) { PathList = val; }

	bool IsToStartPtAgain() const { return toStartPtAgain; }
	void setToStartPtAgain(bool val) { toStartPtAgain = val; }

	double getSafeHeight() const { return safeHeight; }
	void setSafeHeight(double val) { safeHeight = val; }

	double getSpeed() const { return speed; }
	void setSpeed(double val) { speed = val; }

	QStringList getBeforeGCode() const { return beforeGCode; }
	void setBeforeGCode(QStringList& val) { beforeGCode = val; }

	QStringList getAfterGCode() const { return afterGCode; }
	void setAfterGCode(QStringList& val) { afterGCode = val; }

	QString getSavePath() const { return savePath; }
	void setSavePath(QString val) { savePath = val; }

	double getDeltaX() const { return deltaX; }
	void setDeltaX(double val) { deltaX = val; }
private:
	Base::Placement machineCS;//机床坐标系
	Base::Placement workCS;//工件坐标系
	QStringList PathList; 
	bool toStartPtAgain;
	double safeHeight;
	double speed;
	double deltaX;
	QStringList beforeGCode;
	QStringList afterGCode;
	QString savePath;
};

