#ifndef CreateBallCutter_H
#define CreateBallCutter_H

#include <QDialog>
#include "GeneratedFiles/ui_BallCutterDialog.h"
#include "Gui/Control.h"
#include "Gui/TaskView/TaskDialog.h"
#include "Gui/TaskView/TaskView.h"


namespace CustomGui{

	class CustomGuiExport BallCutterDialog : public QDialog
	{
		Q_OBJECT

	public:
		BallCutterDialog(App::DocumentObject* pObj ,QWidget *parent = 0);
		~BallCutterDialog();

	public:
		void setData(App::DocumentObject* pObj);
		App::DocumentObject* getData(){return m_pObject ; }
	public Q_SLOTS:
		void accept();
		void apply();
		void reject();
	private:
		Ui::BallCutterDialog ui;
		App::DocumentObject* m_pObject;
	};


	class TaskBallCutter : public Gui::TaskView::TaskDialog
	{
		Q_OBJECT

	public:
		TaskBallCutter(App::DocumentObject* pobj = 0);
		~TaskBallCutter();

	public:
		bool accept();
		void clicked(int id);

		virtual QDialogButtonBox::StandardButtons getStandardButtons() const
		{
			return QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel;
		}

	private:
		BallCutterDialog* widget;
		Gui::TaskView::TaskBox* taskbox;
	};

}
#endif // CreateBallCutter_H
