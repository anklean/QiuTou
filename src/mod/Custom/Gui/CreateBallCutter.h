#ifndef CreateBallCutter_H
#define CreateBallCutter_H

#include <QDialog>
#include "GeneratedFiles/ui_CreateBallCutter.h"

class CreateBallCutter : public QWidget
{
	Q_OBJECT

public:
	CreateBallCutter(QWidget *parent = 0);
	~CreateBallCutter();

private slots:
void onOK();
void onCancle();
private:
	Ui::CreateBallCutter ui;
};

#endif // CreateBallCutter_H
