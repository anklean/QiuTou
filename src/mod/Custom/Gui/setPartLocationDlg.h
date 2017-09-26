#ifndef SetPartLocationDlg_H
#define SetPartLocationDlg_H

#include <QDialog>
#include "GeneratedFiles/ui_SetPartLocationDlg.h"

class SetPartLocationDlg : public QDialog
{
	Q_OBJECT

public:
	SetPartLocationDlg(QWidget *parent = 0);
	~SetPartLocationDlg();

private slots:
void onOK();
void onCancle();
private:
	Ui::SetPartLocationDlg ui;
};

#endif // SetPartLocationDlg_H
