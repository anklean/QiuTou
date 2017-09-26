#ifndef CREATENCFILEDLG_H
#define CREATENCFILEDLG_H

#include <QDialog>
#include "GeneratedFiles/ui_CreateNCfileDlg.h"
#include "../App/CreateNCData.h"

class CreateNCfileDlg : public QDialog
{
	Q_OBJECT

public:
	CreateNCfileDlg(QWidget *parent = 0);
	~CreateNCfileDlg();

	CreateNCData getData() const { return m_data; }
	void setData(CreateNCData val) { m_data = val; setValueToUI(); }

private slots:
void onOK();
void onCancle();
void onSetPath();
private:
	Ui::CreateNCfileDlg ui;
	CreateNCData m_data;

	void getValueFromUI();
	void setValueToUI();

};

#endif // CREATENCFILEDLG_H
