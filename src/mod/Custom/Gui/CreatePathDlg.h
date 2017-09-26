#ifndef CREATEPATHDLG_H
#define CREATEPATHDLG_H

#include "PreCompiled.h"
#include <QDialog>
#include <QStringList>
#include "GeneratedFiles/ui_CreatePathDlg.h"
#include "../App/CreatePathData.h"

class CreatePathDlg : public QDialog
{
	Q_OBJECT

public:
	CreatePathDlg(QWidget *parent = 0);
	~CreatePathDlg();

	CreatePathData getData() const { return m_data; }
	void setData(CreatePathData val) { m_data = val; setValueToUI(); }

private Q_SLOTS:
	void onOK();
	void onCancle();

private:
	Ui::CreatePathDlg ui;

	void getValueFromUI();
	void setValueToUI();

	CreatePathData m_data;
};

#endif // CREATEPATHDLG_H
