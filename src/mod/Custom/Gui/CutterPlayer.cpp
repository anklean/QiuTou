#include "CutterPlayer.h"
#include "ViewProviderCustomPath.h"
#include "Inventor/nodes/SoTransform.h"

CutterPlayer::CutterPlayer()
{
	cutterTimer = 0;
}


CutterPlayer::CutterPlayer(CustomGui::ViewProviderCustomPath *path)
{
	cutterTimer = 0;
	m_pViewProvider = path;
}

CutterPlayer::~CutterPlayer()
{
}

void CutterPlayer::Start()
{	
	//创建定时器
	if (cutterTimer == 0)
		cutterTimer = new QTimer(this);
	if (cutterTimer != 0)
	{
		//将定时器超时信号与槽(功能函数)联系起来
		connect(cutterTimer, SIGNAL(timeout()), this, SLOT(onPlay()));
		//开始运行定时器，定时时间间隔为1000ms
		cutterTimer->start(500);	
	}
}

void CutterPlayer::Stop()
{
	//停止运行定时器
	if (cutterTimer)
	{
		if (cutterTimer->isActive())
			cutterTimer->stop();
	}
}

void CutterPlayer::onPlay()
{
	int n = m_pViewProvider->cutterOnPointIndex;
	int nCount = m_pViewProvider->pathPointList.size();
	m_pViewProvider->cutterOnPointIndex += 2;

	if (n < 0 || n >= nCount)
		m_pViewProvider->cutterOnPointIndex = 1;

	if (n < nCount-5)
	{
		Custom::NCStepInfo si = m_pViewProvider->pathPointList[n];

		Base::Vector3d ppnt = si.Point;
		Base::Rotation rot(Base::Vector3d(0, 1, 0), si.Normal);
		m_pViewProvider->cutterTransform->translation.setValue(ppnt.x,ppnt.y,ppnt.z);
		double q0, q1, q2, q3;
		rot.getValue(q0, q1, q2, q3);
		m_pViewProvider->cutterTransform->rotation.setValue(q0, q1, q2, q3);
	}
	
}

#include "moc_CutterPlayer.cpp"