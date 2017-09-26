#pragma once
#include "QtCore\QObject"
#include "QtCore\QTimer"
#include "ViewProviderCustomPath.h"

class CutterPlayer :	public QObject
{
	Q_OBJECT
public:
	CutterPlayer();
	CutterPlayer(CustomGui::ViewProviderCustomPath *path);
	~CutterPlayer();

	void Start();
	void Stop();

public	Q_SLOTS:
	void onPlay();

protected:
	QTimer *cutterTimer;
	CustomGui::ViewProviderCustomPath *m_pViewProvider;
};

