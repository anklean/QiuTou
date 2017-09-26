
#include "PreCompiled.h"
#ifndef _PreComp_
# include <cmath>
# include <Bnd_Box.hxx>
# include <BRepBndLib.hxx>
# include <gp_Pln.hxx>
# include <gp_Trsf.hxx>
# include <BRep_Tool.hxx>
# include <BRepAdaptor_Surface.hxx>
# include <BRepCheck_Analyzer.hxx>
# include <BRepOffsetAPI_MakeOffset.hxx>
# include <BRepBuilderAPI_Copy.hxx>
# include <BRepBuilderAPI_MakeFace.hxx>
# include <BRepBuilderAPI_MakeWire.hxx>
# include <BRepBuilderAPI_Transform.hxx>
# include <BRepOffsetAPI_ThruSections.hxx>
# include <BRepPrimAPI_MakePrism.hxx>
# include <Geom_Plane.hxx>
# include <IntTools_FClass2d.hxx>
# include <Precision.hxx>
# include <ShapeAnalysis.hxx>
# include <ShapeAnalysis_Surface.hxx>
# include <ShapeFix_Shape.hxx>
# include <ShapeFix_Wire.hxx>
# include <TopoDS.hxx>
# include <TopoDS_Iterator.hxx>
# include <TopExp.hxx>
# include <TopExp_Explorer.hxx>
# include <TopTools_IndexedMapOfShape.hxx>
#endif


#include "NCOutPutter.h"
#include "PathObject.h"
#include "App\PropertyContainer.h"
#include "Base\Tools.h"
#include "PathBuilder.h"

using namespace Custom;

#define _UTF8(X) (const char*)(QString::fromLocal8Bit(X).toAscii())

PROPERTY_SOURCE(Custom::NCOutPutter, Part::Feature)

NCOutPutter::NCOutPutter()
{
	const char* strPara = _UTF8("参数");

	ADD_PROPERTY(PathList, (0,0));
	ADD_PROPERTY(BeforeGcode, (""));
	ADD_PROPERTY(AfterGcode, (""));
	ADD_PROPERTY(OutputFile, ("d:\\ccc.anc"));
	ADD_PROPERTY(Speed, (1000));

	ADD_PROPERTY_TYPE(SafeHeight, (15), strPara, App::Prop_None, _UTF8("安全高度"));

	std::vector<std::string> befores;
	befores.push_back("M06 T1; 设置使用刀具1");//
	befores.push_back("M03 S5000; 设置主轴顺时针旋转 转速5000");//
	befores.push_back("M08; 冷却打开 ");//
	befores.push_back("G90 G54 G64; 几何数据的基本设定:使用绝对坐标, 使用工件坐标系1, 使用连续路径运行");//
	befores.push_back("G00 Z150; Z轴运行至安全高度");//
	befores.push_back("G00 X - 7.2 Y - 7.2; XY运行到起始点");//
	//	G43 H1 Z150.
	BeforeGcode.setValues(befores);

	std::vector<std::string> afters;	
	afters.push_back("M05 M09; 主轴停止，冷却液关闭");//
	afters.push_back("M30; 程序结束");//
	AfterGcode.setValues(afters);

}

short NCOutPutter::mustExecute() const
{
	if (PathList.isTouched() ||
		SafeHeight.isTouched() ||
		Speed.isTouched())
		return 1;
	return 0;
}

void NCOutPutter::getAngle(Base::Vector3d nrm, double& a, double &b, double & c, Base::Placement place)
{
	Base::Matrix4D mtx=place.toMatrix();
	mtx.transpose();

	double dmtx[16];
	mtx.getMatrix(dmtx);
// 	Base::Vector3d x_axis(dmtx[0], dmtx[1], dmtx[2]);
// 	Base::Vector3d y_axis(dmtx[4], dmtx[5], dmtx[6]);
// 	Base::Vector3d z_axis(dmtx[8], dmtx[9], dmtx[10]);
	Base::Vector3d x_axis(1, 0, 0);
	Base::Vector3d y_axis(0, 1, 0);
	Base::Vector3d z_axis(0, 0, 1);
	//a
	//nrm在YZ平面上绕X轴旋转的角度
	//Base::Vector3d va = nrm.ProjToPlane(y_axis, x_axis);
	double da = nrm.GetAngle(y_axis);
	a = - Base::toDegrees<double>(da);
	
	//b
	//nrm 在XZ平面上绕Y轴旋转的角度
	//Base::Vector3d vb = nrm.ProjToPlane(x_axis, y_axis);
	double db = nrm.GetAngle(x_axis);
	b = -Base::toDegrees<double>(db);	
	
	//c
	//nrm在XY平面上绕Z轴旋转的角度
	//Base::Vector3d vc = nrm.ProjToPlane(x_axis, z_axis);
	double dc = nrm.GetAngle(y_axis);
	c = -Base::toDegrees<double>(dc);

	if (fabs(a) < Precision::Confusion()) a = 0;
	if (fabs(b) < Precision::Confusion()) b = 0;
	if (fabs(c) < Precision::Confusion()) c = 0;
}

//输出程序头
void NCOutPutter::printHeader(std::ofstream& ofs, int& nlines)
{
	ofs << "%" << std::endl;
	std::vector<std::string> befores = BeforeGcode.getValues();
	for (int i = 0; i < befores.size(); i++)
	{
		ofs << "N" << nlines++ << " ";
		ofs << befores[i] << std::endl;
	}
}
//输出程序尾
void NCOutPutter::printFooter(std::ofstream& ofs, int& nlines)
{
	std::vector<std::string> afters = AfterGcode.getValues();
	for (int i = 0; i < afters.size(); i++)
	{
		ofs << "N" << nlines++ << " ";
		ofs << afters[i] << std::endl;
	}
	ofs << "%" << std::endl;
}

//输出一段程序开始前的进刀
void NCOutPutter::printFeed(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement place)
{
	double h = SafeHeight.getValue();
	
	if (paths.size() > 0)
	{
		Custom::NCStepInfo step = paths[0];
	
		double da =.0, db =.0, dc = .0;
		getAngle(step.Normal, da, db, dc,place);
		ofs << "N" << nlines++ << " " << "G00 Z" << h/*<< "; Z轴运行至安全高度"*/ << std::endl;
		ofs << "N" << nlines++ << " " << "G01 G90 B" << db /*<< "; B旋转到相应角度"*/ << std::endl;
		ofs << "N" << nlines++ << " " << "G01 G90 A" << da /*<< "; A旋转到相应角度"*/<< std::endl;
		ofs << "N" << nlines++ << " " << "G01 G90 Y" << step.Point.y << " " << std::endl;
		ofs << "N" << nlines++ << " " << "G01 G90 Z" << step.Point.z << " " << std::endl;
			ofs << "N" << nlines++ << " " << "G01 G90 X" << step.Point.x << " F" << step.speed/*<< ";x, y" */<< std::endl;
	}
}
//输出一段程序结束后的退刀
void NCOutPutter::printTract(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement place)
{
	double h = SafeHeight.getValue();
	if (paths.size() > 0)
	{
		int last = paths.size() - 1;
		Custom::NCStepInfo step = paths[last];

		ofs << "N" << nlines++ << " " << "G00 Z" << h/*<< "; Z轴运行至安全高度" */<< std::endl;
		ofs << "N" << nlines++ << " " << "G00 X"<<0 <<" Y" << 0 /*<< "; XY到起始点" */<< 
			" A" << 0 << " B" <<0 <<std::endl;
	}
}
//输出一段程序
void NCOutPutter::printAPath(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement place)
{
	double tol = 0.0001;
	double local_speed = Speed.getValue();
	if (paths.size() > 0)
	{
		ofs << "\n\n\n;----Path Begin----------" << std::endl;
		printFeed(ofs, nlines, paths , place);

		Custom::NCStepInfo lastStep = paths[0];
		for (int i = 1; i < paths.size(); i++)
		{
			Custom::NCStepInfo step = paths[i];

			double da = .0, db = .0, dc = .0;
			getAngle(step.Normal, da, db, dc,place);
			
			double la = .0, lb = .0, lc = .0;
			getAngle(lastStep.Normal, la, lb, lc, place);
			
			ofs << "N" << nlines++ << " G01 G90 ";
			if (fabs(step.Point.x - lastStep.Point.x) > tol)
				ofs << " X" << step.Point.x << " ";
			if (fabs(step.Point.y - lastStep.Point.y) > tol)
				ofs << " Y" << step.Point.y << " ";
			if (fabs(step.Point.z - lastStep.Point.z) > tol)
				ofs << " Z" << step.Point.z << " ";
			//if (fabs(da - la) > tol)
				ofs << " A" << da  << " B" << db << " C" << dc;
			//if (fabs(step.speed - lastStep.speed) > tol)
				ofs << " F" << local_speed << " ";
			ofs << std::endl;

			lastStep = step;
		}

		printTract(ofs, nlines, paths, place);
		ofs << ";----Path End----------\n\n\n" << std::endl;
	}
	
}
// App::DocumentObjectExecReturn *NCOutPutter::execute(void)
// {	
// 	std::string strFileName = OutputFile.getStrValue();
// 	std::ofstream ofs(strFileName);
// 	ofs.setf(ios::fixed);//设置不用科学计数法
// 	ofs.setf(ios::showpoint);//设置要显示小数点
// 	ofs.precision(2);//设置默认精度2位
// 
// 	int nlines = 0;
// 	printHeader(ofs, nlines);
// 
// 	std::vector<App::DocumentObject*>listPathObject = PathList.getValues();
// 
// 	for (int i = 0; i < listPathObject.size(); i++)
// 	{
// 		App::DocumentObject * pObj = listPathObject[i];
// 		Custom::PathObject* pthObject = dynamic_cast<Custom::PathObject*>(pObj);
// 		if (pthObject)
// 		{
// 			Base::Placement place;
// 			Custom::PathObject::PathPointList pntlist = pthObject->getPointList();
// 			App::DocumentObject *pFace = pthObject->Face.getValue();
// 			Part::Feature* pFaceObject = dynamic_cast<Part::Feature*>(pFace);
// 			if (pFaceObject)
// 			{
// 				place = pFaceObject->Placement.getValue();
// 			}
// 
// 			printAPath(ofs, nlines, pntlist, place);
// 		}
// 	}
// 
// 	printFooter(ofs, nlines);
// 	
// 	return App::DocumentObject::StdReturn;
// }

App::DocumentObjectExecReturn *NCOutPutter::execute(void)
{
	std::string strFileName = OutputFile.getStrValue();
	std::ofstream ofs(strFileName);
	ofs.setf(ios::fixed);//设置不用科学计数法
	ofs.setf(ios::showpoint);//设置要显示小数点
	ofs.precision(2);//设置默认精度2位

	int nlines = 0;
	printHeader(ofs, nlines);


	std::vector<App::DocumentObject*>listPathObject = PathList.getValues();
	std::vector<std::string> subs = PathList.getSubValues();
	for (int i = 0; i < subs.size(); i++)
	{
		App::DocumentObject* pObj = listPathObject[i];
		std::string asub = subs[i];

		Part::Feature * pFaceFeature = dynamic_cast<Part::Feature *>(pObj);
		Part::TopoShape topoFaceFeature = pFaceFeature->Shape.getShape();
		
		std::vector<NCStepInfo> pntlist;
		BuildPath(topoFaceFeature, asub, pntlist);

		Base::Placement place;
		if (pFaceFeature)
		{
			place = pFaceFeature->Placement.getValue();
		}
		printAPath(ofs, nlines, pntlist, place);
		m_nclist.push_back(pntlist);
	}
	
	printFooter(ofs, nlines);

	Part::TopoShape S;
	this->Shape.setValue(S);

	return App::DocumentObject::StdReturn;
}
void NCOutPutter::onChanged(const App::Property* prop)
{
	if (prop == &PathList
		|| prop == &SafeHeight)
	{
			App::DocumentObjectExecReturn *r = this->recompute();
			delete r;	
	}

	Part::Feature::onChanged(prop);
}
