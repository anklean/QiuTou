#include "PreCompiled.h"
#include "BallCutterDialog.h"
#include "../App/BallCutter.h"
#include "App/Application.h"
#include "App/Document.h"
#include "App/DocumentObject.h"
#include "Gui/BitmapFactory.h"

using namespace CustomGui;

BallCutterDialog::BallCutterDialog(App::DocumentObject* pObj,QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);
	m_pObject = pObj;
}

BallCutterDialog::~BallCutterDialog()
{

}

void BallCutterDialog::accept()
{
	QDialog::accepted();
}

void CustomGui::BallCutterDialog::setData(App::DocumentObject* pObj)
{

	if (pObj && pObj->getTypeId() == Custom::BallCutter::getClassTypeId())
	{
		m_pObject = pObj;
		Custom::BallCutter* pBallCutter = dynamic_cast<Custom::BallCutter*>(pObj);
		if (pBallCutter)
		{
			ui.editA->setValue(pBallCutter->A.getValue()); //球头直径
			ui.editB->setValue(pBallCutter->B.getValue()); //螺旋角
			ui.editC->setValue(pBallCutter->C.getValue()); //砂轮直径C
			ui.editD->setValue(pBallCutter->D.getValue()); //弧开始角
			ui.editE->setValue(pBallCutter->E.getValue()); //弧结束角
			ui.editF->setValue(pBallCutter->F.getValue()); //安全高度
			ui.editG->setValue(pBallCutter->G.getValue()); //下切深度
			ui.editH->setValue(pBallCutter->H.getValue()); //前角
			ui.editI->setValue(pBallCutter->I.getValue()); //毛坯头部余量
			ui.editJ->setValue(pBallCutter->J.getValue()); //组数
			ui.editK->setValue(pBallCutter->K.getValue()); //每组刃数
			ui.editL->setValue(pBallCutter->L.getValue()); //总刃数

			 ui.editCylinderDiameter->setValue(pBallCutter->CylinderDiameter.getValue());
			 ui.editCylinderLength->setValue(pBallCutter->CylinderLength.getValue());
			 ui.editCylinderChamferA->setValue(pBallCutter->CylinderChamferA.getValue());
			 ui.editCylinderChamferB->setValue(pBallCutter->CylinderChamferB.getValue());
			ui.editGrindingWheelThick->setValue(pBallCutter->GrindingWheelThick.getValue());
			 ui.editGrindingWheelAngle->setValue(pBallCutter->GrindingWheelAngle.getValue());
			 ui.checkSpine->setChecked(pBallCutter->CheckSpine.getValue());
		}		
	}
}

void CustomGui::BallCutterDialog::apply()
{
	double dA = ui.editA->value(); //球头直径
	double dB = ui.editB->value(); //螺旋角
	double dC = ui.editC->value(); //砂轮直径C
	double dD = ui.editD->value(); //弧开始角
	double dE = ui.editE->value(); //弧结束角
	double dF = ui.editF->value(); //安全高度
	double dG = ui.editG->value(); //下切深度
	double dH = ui.editH->value(); //前角
	double dI = ui.editI->value(); //毛坯头部余量
	double dJ = ui.editJ->value(); //组数
	double dK = ui.editK->value(); //每组刃数
	double dL = ui.editL->value(); //总刃数

	double cD=ui.editCylinderDiameter->value();
	double cL= ui.editCylinderLength->value();
	double cA = ui.editCylinderChamferA->value();
	double cB = ui.editCylinderChamferB->value();
	double gT = ui.editGrindingWheelThick->value();
	double gA = ui.editGrindingWheelAngle->value();



	App::Document* pActiveDoc = App::GetApplication().getActiveDocument();

	if (m_pObject == 0)
	{
		m_pObject = pActiveDoc->addObject("Custom::BallCutter", "BallCutter");
	}

	if (m_pObject && m_pObject->getTypeId() == Custom::BallCutter::getClassTypeId())
	{
		Custom::BallCutter* pBallCutter = dynamic_cast<Custom::BallCutter*>(m_pObject);
		pBallCutter->setStatus(App::ObjectStatus::Restore, true);
		pBallCutter->A.setValue(dA); //球头直径
		pBallCutter->B.setValue(dB); //螺旋角
		pBallCutter->C.setValue(dC); //砂轮直径C
		pBallCutter->D.setValue(dD); //弧开始角
		pBallCutter->E.setValue(dE); //弧结束角
		pBallCutter->F.setValue(dF); //安全高度
		pBallCutter->G.setValue(dG); //下切深度
		pBallCutter->H.setValue(dH); //前角
		pBallCutter->I.setValue(dI); //毛坯头部余量
		pBallCutter->J.setValue(dJ); //组数
		pBallCutter->K.setValue(dK); //每组刃数
		pBallCutter->L.setValue(dL); //总刃数

		pBallCutter->CylinderDiameter.setValue(cD);
		pBallCutter->CylinderLength.setValue(cL);
		pBallCutter->CylinderChamferA.setValue(cA);
		pBallCutter->CylinderChamferB.setValue(cB);
		pBallCutter->GrindingWheelThick.setValue(gT);
		pBallCutter->GrindingWheelAngle.setValue(gA);

		pBallCutter->CheckSpine.setValue(ui.checkSpine->isChecked());
		
		pBallCutter->setStatus(App::ObjectStatus::Restore, false);
		pBallCutter->recompute();
	}

}
void BallCutterDialog::reject()
{
	QDialog::rejected();
}



// ---------------------------------------

TaskBallCutter::TaskBallCutter(App::DocumentObject* pObj)
{
	widget = new BallCutterDialog(pObj);
	taskbox = new Gui::TaskView::TaskBox(
		Gui::BitmapFactory().pixmap("BallCutter"),
		widget->windowTitle(), true, 0);
	taskbox->groupLayout()->addWidget(widget);
	Content.push_back(taskbox);
}

TaskBallCutter::~TaskBallCutter()
{
	// automatically deleted in the sub-class
}

bool TaskBallCutter::accept()
{
	widget->accept();
	return (widget->result() == QDialog::Accepted);
}

void TaskBallCutter::clicked(int id)
{
	if (id == QDialogButtonBox::Apply) {
		widget->apply();
	}
}

#include "GeneratedFiles/moc_BallCutterDialog.cpp"

