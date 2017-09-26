#include "CreateToolsDlg.h"
#include "CreatePathDlg.h"

CreateToolsDlg::CreateToolsDlg(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(onCancle()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onNewTool()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(onEditTool()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(onDelTool()));
}

CreateToolsDlg::~CreateToolsDlg()
{

}

void CreateToolsDlg::onOK()
{
	this->accept();
}

void CreateToolsDlg::onCancle()
{
	this->reject();
}

void CreateToolsDlg::onNewTool()
{
	GrindingWheelDlg* dlg = new GrindingWheelDlg(this);
 	this->getValueFromUI();
 	dlg->setNameList(m_nameList);
 	if (dlg->exec() == QDialog::Accepted)
 	{
 		GrindingWheelData grindingWheel = dlg->getData();
 		m_nameList.append(grindingWheel.getName());
 		m_data.addGrindingWheel(grindingWheel);
		ui.listWidget->addItem(grindingWheel.getName());
 	}
}

void CreateToolsDlg::onEditTool()
{
	GrindingWheelDlg* dlg = new GrindingWheelDlg(this);
	this->getValueFromUI();

	QListWidgetItem* item = ui.listWidget->selectedItems().at(0);
	if (item)
	{
		QString name = item->text();
		GrindingWheelData grindingWheel = m_data.findGrindingWheel(name);
		dlg->setData(grindingWheel);
		dlg->setNameEnable(false);
		if (dlg->exec() == QDialog::Accepted)
		{
			GrindingWheelData data = dlg->getData();
			m_data.rmvGrindingWheel(data.getName());
			m_data.addGrindingWheel(data);
		}
	}
	
}

void CreateToolsDlg::onDelTool()
{
	QListWidgetItem* item = ui.listWidget->selectedItems().at(0);
	if (item)
	{
		QString name = item->text();
		m_data.rmvGrindingWheel(name);

		int index = ui.listWidget->row(item);
		ui.listWidget->takeItem(index);
	}
}

void CreateToolsDlg::getValueFromUI()
{
	int rows = ui.listWidget->model()->rowCount();
	for (int i = 0; i < rows; i++)
	{
		QListWidgetItem* item = ui.listWidget->item(i);
		if (item)
		{
			m_nameList.append(item->text());
		}
	}
}

void CreateToolsDlg::setValueToUI()
{
	ui.listWidget->clear();
	std::map<QString, GrindingWheelData> tools = m_data.getDataList();
	for (std::map<QString, GrindingWheelData>::iterator it = tools.begin();
		it != tools.end(); ++it)
	{
		QString name = it->first;
		ui.listWidget->addItem(name);
	}
}

#include "GeneratedFiles/moc_CreateToolsDlg.cpp"