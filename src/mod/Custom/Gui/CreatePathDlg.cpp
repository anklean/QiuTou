#include "CreatePathDlg.h"
#include "App/Document.h"
#include "App/Application.h"
#include "QMessageBox"

CreatePathDlg::CreatePathDlg(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);

	App::Document* pDoc = App::GetApplication().getActiveDocument();

	if (pDoc)
	{
		QStringList toolNames;
		Tools tools = pDoc->ToolList.getValue();
		std::map<QString, GrindingWheelData> toolList = tools.getDataList();
		for (std::map<QString, GrindingWheelData>::iterator it = toolList.begin();
			it != toolList.end(); ++it)
		{
			toolNames.append(it->first);
		}

		ui.cmbToolNumber->addItems(toolNames);
	}

	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(onCancle()));
}

CreatePathDlg::~CreatePathDlg()
{

}

void CreatePathDlg::onOK()
{
	getValueFromUI();
	this->accept();
}

void CreatePathDlg::onCancle()
{
	this->reject();
}

void CreatePathDlg::getValueFromUI()
{
	QString toolName = ui.cmbToolNumber->currentText();
	if (toolName.isEmpty())
	{
		QMessageBox::warning(this, tr("Error"), tr("刀具不能为空"));
	}

	App::Document* pDoc = App::GetApplication().getActiveDocument();

	Tools ctools = pDoc->ToolList.getValue();
	if (ctools.IsExisted(toolName) == false)
	{
		QMessageBox::warning(this, tr("Error"), QString::fromUtf8("没有找到编号为%1的刀具").arg(toolName));
	}

	m_data.setToolName(toolName);

	QString beforeG = ui.editBeforeCode->toPlainText();
	m_data.setBeforeG(beforeG);

	QString afterG = ui.editAfterCode->toPlainText();
	m_data.setAfterG(afterG);

	QStringList ssFaces;
	int ncount = ui.listFaces->count();
	for (int i = 0; i < ncount; i++)
	{
		QListWidgetItem* it = ui.listFaces->item(i);
		ssFaces.push_back(it->text());
	}
	m_data.setFaces(ssFaces);

	QStringList ssCurves;
	 ncount = ui.listCurves->count();
	for (int i = 0; i < ncount; i++)
	{
		QListWidgetItem* it = ui.listCurves->item(i);
		ssCurves.push_back(it->text());
	}
	m_data.setCurves(ssCurves);

	QString utol = ui.editUTolerance->text();
	m_data.setTolerance(utol.toDouble());

	//QString vtol = ui.editUTolerance->text();
	//m_data.setVTolerance(vtol.toDouble());
	
	int iOpType = ui.cmbOpType->currentIndex();
	m_data.setOpType(iOpType);

// 	QString edgeDirection = ui.cmbEdgeType->currentText();
// 	m_data.setEdgeDirection(edgeDirection);

}

void CreatePathDlg::setValueToUI()
{
	QString toolName = m_data.getToolName();
	int index = ui.cmbToolNumber->findText(toolName);
	ui.cmbToolNumber->setCurrentIndex(index);
	
	QString beforeG = m_data.getBeforeG();
	ui.editBeforeCode->setPlainText(beforeG);

	QString afterG = m_data.getAfterG();
	ui.editAfterCode->setPlainText(afterG);

	QStringList facelist = m_data.getFaces();
	ui.listFaces->addItems(facelist);

	QStringList curvelist = m_data.getCurves();
	ui.listCurves->addItems(curvelist);
// 	QString edgeDirection = m_data.getEdgeDirection();
// 	int j = ui.cmbEdgeType->findText(edgeDirection);
// 	ui.cmbEdgeType->setCurrentIndex(j);

	double utol = m_data.getTolerance();
	ui.editUTolerance->setValue(utol);
	
	//double vtol = m_data.getVTolerance();
	//ui.editVTolerance->setValue(vtol);

}

#include "GeneratedFiles/moc_CreatePathDlg.cpp"