#include "GrindingWheelDlg.h"
#include <QMessageBox>
#include "Base/Vector3D.h"
#include "QString"
#include "QChar"

GrindingWheelDlg::GrindingWheelDlg(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(onCancle()));
}

GrindingWheelDlg::~GrindingWheelDlg()
{

}

void GrindingWheelDlg::onOK()
{
	QString name = ui.lineEdit->text();
	if (name.isEmpty() || name.isNull())
	{
		QMessageBox::information(this, tr("warning"), tr("name is need!"));
	}
	else if (nameList.contains(name))
	{
		QMessageBox::information(this, tr("warning"), tr("name has used!"));
	}
	else
	{
		this->getValueFromUI();
		this->accept();
	}
}

void GrindingWheelDlg::onCancle()
{
	this->reject();
}

void GrindingWheelDlg::getValueFromUI()
{
	QString name = ui.lineEdit->text();
	m_data.setName(name);

	double diameter = ui.doubleSpinBox->value();
	m_data.setDiameter(diameter);

	double width = ui.doubleSpinBox_2->value();
	m_data.setWidth(width);

	double angle = ui.doubleSpinBox_3->value();
	m_data.setAngle(angle);

	QString ss = ui.editToolDirection->text();
	Base::Vector3d v;
	int cc = ' ';
	QChar c(cc);
	v.x = ss.section(c, 0, 0).toDouble();
	v.y = ss.section(c, 1, 1).toDouble();
	v.z = ss.section(c, 2, 2).toDouble();
}

void GrindingWheelDlg::setValueToUI()
{
	QString name = m_data.getName();
	ui.lineEdit->setText(name);

	double diameter = m_data.getDiameter();
	ui.doubleSpinBox->setValue(diameter);
	
	double width = m_data.getWidth();
	ui.doubleSpinBox_2->setValue(width);

	double angle = m_data.getAngle();
	ui.doubleSpinBox_3->setValue(angle);

	Base::Vector3d dir = m_data.getDirection();
	QString s = QString::fromAscii("%1 %2 %3").arg(dir.x).arg(dir.y).arg(dir.z);
	ui.editToolDirection->setText(s);
}

void GrindingWheelDlg::setNameEnable(bool bEnable)
{
	ui.lineEdit->setEnabled(bEnable);
}


#include "GeneratedFiles/moc_GrindingWheelDlg.cpp"