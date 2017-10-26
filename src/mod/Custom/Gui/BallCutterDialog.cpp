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
			ui.editA->setValue(pBallCutter->A.getValue()); //��ͷֱ��
			ui.editB->setValue(pBallCutter->B.getValue()); //������
			ui.editC->setValue(pBallCutter->C.getValue()); //ɰ��ֱ��C
			ui.editD->setValue(pBallCutter->D.getValue()); //����ʼ��
			ui.editE->setValue(pBallCutter->E.getValue()); //��������
			ui.editF->setValue(pBallCutter->F.getValue()); //��ȫ�߶�
			ui.editG->setValue(pBallCutter->G.getValue()); //�������
			ui.editH->setValue(pBallCutter->H.getValue()); //ǰ��
			ui.editI->setValue(pBallCutter->I.getValue()); //ë��ͷ������
			ui.editJ->setValue(pBallCutter->J.getValue()); //����
			ui.editK->setValue(pBallCutter->K.getValue()); //ÿ������
			ui.editL->setValue(pBallCutter->L.getValue()); //������

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
	double dA = ui.editA->value(); //��ͷֱ��
	double dB = ui.editB->value(); //������
	double dC = ui.editC->value(); //ɰ��ֱ��C
	double dD = ui.editD->value(); //����ʼ��
	double dE = ui.editE->value(); //��������
	double dF = ui.editF->value(); //��ȫ�߶�
	double dG = ui.editG->value(); //�������
	double dH = ui.editH->value(); //ǰ��
	double dI = ui.editI->value(); //ë��ͷ������
	double dJ = ui.editJ->value(); //����
	double dK = ui.editK->value(); //ÿ������
	double dL = ui.editL->value(); //������

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
		pBallCutter->A.setValue(dA); //��ͷֱ��
		pBallCutter->B.setValue(dB); //������
		pBallCutter->C.setValue(dC); //ɰ��ֱ��C
		pBallCutter->D.setValue(dD); //����ʼ��
		pBallCutter->E.setValue(dE); //��������
		pBallCutter->F.setValue(dF); //��ȫ�߶�
		pBallCutter->G.setValue(dG); //�������
		pBallCutter->H.setValue(dH); //ǰ��
		pBallCutter->I.setValue(dI); //ë��ͷ������
		pBallCutter->J.setValue(dJ); //����
		pBallCutter->K.setValue(dK); //ÿ������
		pBallCutter->L.setValue(dL); //������

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

