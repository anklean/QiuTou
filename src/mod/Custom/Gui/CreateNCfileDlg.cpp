#include "CreateNCfileDlg.h"
#include <QFileDialog>
#include "QListWidget"

CreateNCfileDlg::CreateNCfileDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(onCancle()));
	connect(ui.btnOpenFileDialg, SIGNAL(clicked()), this, SLOT(onSetPath()));
}

CreateNCfileDlg::~CreateNCfileDlg()
{

}

void CreateNCfileDlg::onOK()
{
	getValueFromUI();
	this->accept();
}

void CreateNCfileDlg::onCancle()
{
	this->reject();
}

void CreateNCfileDlg::getValueFromUI()
{
	QString strMPos=ui.editMachinePos->text();
	QString strMRot= ui.editMachineRotate->text();

	double d[3];
	for (int i = 0; i < 3; i++)
	{
		d[i]=strMPos.section(QChar::Nbsp, i, i).toDouble();
	}
	Base::Vector3d vMPos(d[0], d[1], d[2]);

	for (int i = 0; i < 3; i++)
	{
		d[i] = strMRot.section(QChar::Nbsp, i, i).toDouble();
	}
	Base::Rotation vMRot;
	vMRot.setYawPitchRoll(d[0], d[1], d[2]);

	Base::Placement machineCS(vMPos, vMRot);
	


	QString strWPos = ui.editWorkPos->text();
	QString strWRot = ui.editWorkRotate->text();

	//double d[3];
	for (int i = 0; i < 3; i++)
	{
		d[i] = strWPos.section(QChar::Nbsp, i, i).toDouble();
	}
	Base::Vector3d vWPos(d[0], d[1], d[2]);

	for (int i = 0; i < 3; i++)
	{
		d[i] = strWRot.section(QChar::Nbsp, i, i).toDouble();
	}
	Base::Rotation vWRot;
	vWRot.setYawPitchRoll(d[0], d[1], d[2]);

	Base::Placement workCS(vWPos, vWRot);

	double safeHeight = ui.editSafeHeight->text().toDouble();
	m_data.setSafeHeight(safeHeight);

	double speed = ui.editSpeed->text().toDouble();
	m_data.setSpeed(speed);

	double deltax = ui.editDeltaX->text().toDouble();
	m_data.setDeltaX(deltax);

	QString sb =  ui.editBeforeGCode->toPlainText();
	m_data.setBeforeGCode(sb.split(QChar('\n')));

	QString sa = ui.editAfterGCode->toPlainText();
	m_data.setAfterGCode(sa.split(QChar('\n')));

	QString path = ui.outputFilePath->text();
	m_data.setSavePath(path);

	QStringList ssPaths;
	int ncount = ui.listPaths->count();
	for (int i = 0; i < ncount; i++)
	{
		QListWidgetItem* it = ui.listPaths->item(i);
		ssPaths.push_back(it->text());
	}
	m_data.setPathList(ssPaths);
}

void CreateNCfileDlg::setValueToUI()
{
	Base::Placement workcs = m_data.getWorkCS();

	double x = .0, y = .0, z = .0, a = .0, b = .0, c = .0;	
	workcs.getRotation().getYawPitchRoll(a, b, c);
	QString strWorkCsPos =QString::fromAscii("%1 %2 %3").arg(workcs.getPosition().x).arg( workcs.getPosition().y).arg( workcs.getPosition().z);	
	QString strWorkCsRot= QString::fromAscii("%1 %2 %3").arg(a).arg(b).arg(c);

	ui.editWorkPos->setText(strWorkCsPos);
	ui.editWorkRotate->setText(strWorkCsRot);

	Base::Placement machinecs = m_data.getMachineCS();
	x = .0; y = .0; z = .0; a = .0; b = .0; c = .0;
	machinecs.getRotation().getYawPitchRoll(a, b, c);
	QString strMachineCsPos = QString::fromAscii("%1 %2 %3").arg(machinecs.getPosition().x).arg(machinecs.getPosition().y).arg(machinecs.getPosition().z);
	QString strMachineCsRot = QString::fromAscii("%1 %2 %3").arg(a).arg(b).arg(c);

	ui.editMachinePos->setText(strMachineCsPos);
	ui.editMachineRotate->setText(strMachineCsRot);
	
	double safeHeight = m_data.getSafeHeight();	
	ui.editSafeHeight->setText(QString::fromAscii("%1").arg(safeHeight));

	double speed = m_data.getSpeed();
	ui.editSpeed->setText(QString::fromAscii("%1").arg(speed));


	double deltax = m_data.getDeltaX();
	ui.editDeltaX->setText(QString::fromAscii("%1").arg(deltax));
	
	QString sBG="";
	QStringList bg =m_data.getBeforeGCode();
	for (int i = 0; i < bg.size(); i++)
	{
		sBG += bg[i] + "\n";
	}
	ui.editBeforeGCode->setPlainText(sBG);

	QString sAG = "";
	QStringList ag = m_data.getAfterGCode();
	for (int i = 0; i < ag.size(); i++)
	{
		sAG += ag[i] + "\n";
	}
	ui.editAfterGCode->setPlainText(sAG);
	
	QString path = m_data.getSavePath();
	ui.outputFilePath->setText(path);

	QStringList pathlist = m_data.getPathList();	
	ui.listPaths->addItems(pathlist);
	
}

void CreateNCfileDlg::onSetPath()
{
	QString path = QFileDialog::getSaveFileName(this, tr("指定保存的路径"), tr("d://"), tr("*.anc"));

	ui.outputFilePath->setText(path);
}

#include "GeneratedFiles/moc_CreateNCfileDlg.cpp"