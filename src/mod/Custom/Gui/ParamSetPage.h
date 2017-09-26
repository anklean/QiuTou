#ifndef PARAMSETPAGE_H
#define PARAMSETPAGE_H

#include <QDialog>
#include "GeneratedFiles/ui_ParamSetPage.h"

class ParamSetPage : public QDialog
{
	Q_OBJECT

public:
	ParamSetPage(QWidget *parent = 0);
	~ParamSetPage();

private slots:
void onOK();
void onCancle();
private:
	Ui::ParamSetPage ui;
};

#endif // PARAMSETPAGE_H
