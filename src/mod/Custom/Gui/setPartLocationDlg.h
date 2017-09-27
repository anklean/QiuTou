#ifndef SetPartLocationDlg_H
#define SetPartLocationDlg_H

#include <QDialog>
#include "GeneratedFiles/ui_SetPartLocationDlg.h"
#include "Base/Vector3D.h"

class SetPartLocationDlg : public QDialog
{
	Q_OBJECT

public:
	SetPartLocationDlg(QWidget *parent = 0);
	~SetPartLocationDlg();

	Base::Vector3d getLocation();

private slots:
void onOK();
void onCancle();
private:
	Ui::SetPartLocationDlg ui;
};

#endif // SetPartLocationDlg_H
