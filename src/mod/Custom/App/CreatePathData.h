#pragma once
#include "PreCompiled.h"
#include <QString>
#include "QStringList"

class CustomExport CreatePathData
{
public:
	CreatePathData();
	virtual ~CreatePathData();

	QString getToolName() const { return toolName; }
	void setToolName(QString val) { toolName = val; }

	QString getBeforeG() const { return beforeG; }
	void setBeforeG(QString val) { beforeG = val; }

	QString getAfterG() const { return afterG; }
	void setAfterG(QString val) { afterG = val; }

	int getOpType() const { return opType; }
	void setOpType(int val) { opType = val; }

	QStringList getFaces() const { return Faces; }
	void setFaces(QStringList& val) { Faces = val; }
	
	QStringList getCurves() const { return Curves; }
	void setCurves(QStringList& val) { Curves = val; }

	double getTolerance() const { return Tolerance; }
	void setTolerance(double val) { Tolerance = val; }

private:
	QString toolName;
	QString beforeG;
	QString afterG;
	QStringList Faces;
	QStringList Curves;
	int opType;
	double Tolerance;

};

