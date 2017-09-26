#ifndef GRINDINGWHEELDLG_H
#define GRINDINGWHEELDLG_H

#include "PreCompiled.h"
#include <QDialog>
#include <QStringList>
#include "GeneratedFiles/ui_GrindingWheelDlg.h"
#include "App/GrindingWheelData.h"

class GrindingWheelDlg : public QDialog
{
	Q_OBJECT

public:
	GrindingWheelDlg(QWidget *parent = 0);
	~GrindingWheelDlg();

	GrindingWheelData getData() const { return m_data; }
	void setData(GrindingWheelData val) { m_data = val; setValueToUI(); }
	
	QStringList getNameList() const { return nameList; }
	void setNameList(QStringList val) { nameList = val; }

	void setNameEnable(bool bEnable);

private Q_SLOTS:
	void onOK();
	void onCancle();
private:
	Ui::GrindingWheelDlg ui;	
	GrindingWheelData m_data;
	QStringList nameList;

	void getValueFromUI();
	void setValueToUI();
};

#endif // GRINDINGWHEELDLG_H
