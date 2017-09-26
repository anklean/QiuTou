#ifndef PLAYPATHDLG_H
#define PLAYPATHDLG_H

#include <QDialog>
#include "GeneratedFiles/ui_PlayPathDlg.h"
#include "ViewProviderCustomPath.h"

class PlayPathDlg : public QDialog
{
	Q_OBJECT

public:
	PlayPathDlg(QWidget *parent = 0);
	~PlayPathDlg();

	void setCurrentViewProvider(CustomGui::ViewProviderCustomPath* vp);
private slots:
void onPlay();
void onSpeedUp();
void onPause();
void onStop();
private:
	Ui::PlayPathDlg ui;
	CustomGui::ViewProviderCustomPath* pViewProvider;
};

#endif // PLAYPATHDLG_H
