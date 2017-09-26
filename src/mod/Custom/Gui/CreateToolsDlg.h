#ifndef CREATTOOLSDLG_H
#define CREATTOOLSDLG_H

#include "PreCompiled.h"
#include <QDialog>
#include <QStringList>
#include "GeneratedFiles/ui_CreateToolsDlg.h"
#include "App/Tools.h"
#include "GrindingWheelDlg.h"

class CreateToolsDlg : public QDialog
{
	Q_OBJECT

public:
	CreateToolsDlg(QWidget *parent = 0);
	~CreateToolsDlg();

	QStringList getNameList() const { return m_nameList; }
	void setNameList(QStringList val) { m_nameList = val; }

	Tools getData() const { return m_data; }
	void setData(Tools val) { m_data = val; setValueToUI(); }

private Q_SLOTS:
	void onOK();
	void onCancle();
	void onNewTool();
	void onEditTool();
	void onDelTool();
private:
	Ui::CreateToolsDlg ui;
	QStringList m_nameList;

	void getValueFromUI();
	void setValueToUI();

	Tools m_data;
};

#endif // CREATTOOLSDLG_H
