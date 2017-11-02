
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
#include "App\Application.h"
#include <vector>
#include "App\Tools.h"
#include "App\Document.h"
#include "BallCutter.h"

using namespace Custom;

#define _UTF8(X) (const char*)(QString::fromLocal8Bit(X).toAscii())

PROPERTY_SOURCE(Custom::NCOutPutter, Part::Feature)

NCOutPutter::NCOutPutter()
{
	const char* strPara = _UTF8("参数");

	ADD_PROPERTY(PathList, (0));
	ADD_PROPERTY(BeforeGcode, (""));
	ADD_PROPERTY(AfterGcode, (""));
	ADD_PROPERTY(OutputFile, ("d:\\ccc.anc"));
	ADD_PROPERTY(Speed, (1000.0));
	ADD_PROPERTY(ToolNum, ("1"));

	ADD_PROPERTY_TYPE(SafeHeight, (15), strPara, App::Prop_None, _UTF8("安全高度"));

	std::vector<std::string> befores;
//	befores.push_back("M06 T" + ToolNum .getStrValue()+ "; 设置使用刀具1");//
	befores.push_back("M03 S5000");//; 设置主轴顺时针旋转 转速5000
	befores.push_back("M08 ");//; 冷却打开
	befores.push_back("G90 G54");// G64; 几何数据的基本设定:使用绝对坐标, 使用工件坐标系1, 使用连续路径运行
	//befores.push_back("G00 Z150; Z轴运行至安全高度");//
	//befores.push_back("G00 X - 7.2 Y - 7.2; XY运行到起始点");//
	//	G43 H1 Z150.
	BeforeGcode.setValues(befores);

	std::vector<std::string> afters;	
	afters.push_back("M05");//; 主轴停止
	afters.push_back("M09");//; 冷却液关闭
	afters.push_back("M30");//; 程序结束
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

double NCOutPutter::getA(Base::Vector3d nrm, Base::Placement place)
{
	Base::Vector3d x_axis(1, 0, 0);
	Base::Vector3d y_axis(0, 1, 0);
	Base::Vector3d z_axis(0, 0, 1);

	//Base::Vector3d base = place.getPosition();
	Base::Vector3d base(0, 0, 0);
	Base::Vector3d va = nrm.ProjToPlane(base, z_axis);
	double da = va.GetAngle(x_axis);
	double a = Base::toDegrees<double>(da);
// 	if (va.x > 0)
// 		a = 90 - a;

	return a;
}


double NCOutPutter::getA(Base::Vector3d nrm, Base::Placement place ,double B)
{
	Base::Vector3d x_axis(1, 0, 0);
	Base::Vector3d y_axis(0, 1, 0);
	Base::Vector3d z_axis(0, 0, 1);
	
	Base::Vector3d base(0, 0, 0);//= place.getPosition();

	double db = Base::toRadians<double>(B);

	Base::Matrix4D mtx = place.toMatrix();
	mtx.transpose();
	mtx.rotZ(db);

	double dmtx[16] = { 0 };
	mtx.getMatrix(dmtx);

	Base::Vector3d newx(dmtx[0],dmtx[1],dmtx[2]);
	Base::Vector3d newy(dmtx[4], dmtx[5], dmtx[6]);
	Base::Vector3d newz(dmtx[8], dmtx[9], dmtx[10]);

	Base::Vector3d va = nrm.ProjToPlane(base, newy);
	double da = va.GetAngle(newz);
	double a = Base::toDegrees<double>(da);
	// 	if (va.x > 0)
	// 		a = 90 - a;
	
	//适应a的旋转方向，a=-a;
	a *= -1;

	return a;
}


double NCOutPutter::getA(Base::Vector3d pnt1 ,Base::Vector3d pnt2, Base::Placement place, double B)
{
	Base::Vector3d x_axis(1, 0, 0);
	Base::Vector3d y_axis(0, 1, 0);
	Base::Vector3d z_axis(0, 0, 1);

	Base::Vector3d base= place.getPosition();
	// 
	double db = Base::toRadians<double>(B);
	
	Base::Matrix4D mtx = place.toMatrix();
	mtx.transpose();
	mtx.rotZ(db);

	double dmtx[16] = { 0 };
	mtx.getMatrix(dmtx);

	Base::Vector3d newx(dmtx[0],dmtx[1],dmtx[2]);
	Base::Vector3d newy(dmtx[4], dmtx[5], dmtx[6]);
	Base::Vector3d newz(dmtx[8], dmtx[9], dmtx[10]);

	Base::Vector3d p1 = pnt1.ProjToPlane(base, newy);
	Base::Vector3d p2 = pnt2.ProjToPlane(base, newy);
	Base::Vector3d va = p1 - p2;

//	Base::Vector3d va = vv.ProjToPlane(base, newy);
	double da = va.GetAngle(newz);
	double a = Base::toDegrees<double>(da);
	// 	if (va.x > 0)
	// 		a = 90 - a;

	return a;
}

double NCOutPutter::getB(Base::Vector3d nrm, Base::Placement place)
{
	Base::Vector3d base = place.getPosition();
	Base::Matrix4D mtx = place.toMatrix();
	mtx.transpose();

	double dmtx[16] = { 0 };
	mtx.getMatrix(dmtx);

	Base::Vector3d x_axis(dmtx[0], dmtx[1], dmtx[2]);
	Base::Vector3d y_axis(dmtx[4], dmtx[5], dmtx[6]);
	Base::Vector3d z_axis(dmtx[8], dmtx[9], dmtx[10]);
	// 	Base::Vector3d x_axis(1, 0, 0);
	// 	Base::Vector3d y_axis(0,1, 0);
	//	Base::Vector3d z_axis(0, 0, 1);

	Base::Vector3d vb = nrm.ProjToPlane(base, z_axis); //z_axis

	Base::Vector3d vv = vb - base;
	double db = vv.GetAngle(y_axis);//y_axis
	double b = Base::toDegrees<double>(db);

	//
	// 通过getAngle求得的角度范围在0~180 
	// 所以需要根据vv 与y_axis的方向来判断是在0~180范围内，还是在180~360范围内
	// 若vv 与y_axis 反向，则说明在180~360
	// 判断两向量是否反向，对两向量做点乘,若结果大于0,就是同向,等于0垂直,小于0反向
	double ddd = vv.Normalize()*x_axis.Normalize();
	if (ddd < 0)
	{
		b = 360 - b;
	}

	if (fabs(b) < Precision::Confusion()) b = 0;

	return b;
}

//
// 调整刀具点的位置
// 计算时使用的是砂轮的边缘，仿真时使用的砂轮的中心作为位置点，因此需要矫正
//
Base::Vector3d NCOutPutter::adjustPointByTools(Base::Vector3d& point, Base::Placement& place , double B)
{
	double tool_radius = getToolDiameter() / 2.0;
	double db = Base::toRadians<double>(B-90);

	Base::Vector3d tpnt = point;

 	Base::Vector3d base = place.getPosition();
 	Base::Matrix4D mtx = place.toMatrix();
 	mtx.transpose();

	double dmtx[16] = { 0 };
	mtx.getMatrix(dmtx);

	Base::Vector3d newx(dmtx[0], dmtx[1], dmtx[2]);
	Base::Vector3d newy(dmtx[4], dmtx[5], dmtx[6]);
	Base::Vector3d newz(dmtx[8], dmtx[9], dmtx[10]);
	
	Base::Vector3d bbase = base.ProjToPlane(tpnt, newz);
	Base::Vector3d nrm_vec = tpnt -bbase  ;
	nrm_vec = nrm_vec.Normalize();
	Base::Vector3d tt = tpnt + nrm_vec * tool_radius;
	
	Base::Matrix4D mmtx(bbase,newz,db);
	Base::Vector3d r = mmtx*tpnt;

// 	Base::Vector3d rclBase; Base::Vector3d rclDir; double fAngle; double fTranslation;
// 	mtx.toAxisAngle(rclBase, rclDir, fAngle, fTranslation) ;
// 
// 	Base::Matrix4D mmtx(base, rclDir, fAngle);
// 	mmtx.rotZ(db);
// 	mmtx.toAxisAngle(rclBase, rclDir, fAngle, fTranslation);
// 	Base::Vector3d r = mmtx*tt;

	return  r;
}

double NCOutPutter::getToolDiameter()
{
	double tool_Diameter= 0;
	App::Document* pDoc = App::GetApplication().getActiveDocument();
	if (pDoc)
	{
		Tools tools = pDoc->ToolList.getValue();
		std::map<QString, GrindingWheelData> maps = tools.getDataList();

		std::string strToolNum = ToolNum.getStrValue();
		std::map<QString, GrindingWheelData>::iterator it = maps.find(Base::Tools::fromStdString(strToolNum));
		if (it != maps.end())
		{
			tool_Diameter = it->second.getDiameter();
		}
	}
	return tool_Diameter;
}

void NCOutPutter::getAngle(Base::Vector3d nrm, double& a, double &b, double & c, Base::Placement place)
{
	Base::Matrix4D mtx=place.toMatrix();
	mtx.transpose();

	Base::Vector3d base = place.getPosition();

	double dmtx[16];
	mtx.getMatrix(dmtx);
// 	Base::Vector3d x_axis(dmtx[0], dmtx[1], dmtx[2]);
// 	Base::Vector3d y_axis(dmtx[4], dmtx[5], dmtx[6]);
// 	Base::Vector3d z_axis(dmtx[8], dmtx[9], dmtx[10]);
	Base::Vector3d x_axis(1, 0, 0);
	Base::Vector3d y_axis(0, 1, 0);
	Base::Vector3d z_axis(0, 0, 1);
	//a
	//nrm在XY平面上与X轴的夹角
	Base::Vector3d va = nrm.ProjToPlane(base, z_axis);
	double da = va.GetAngle(x_axis);
	a = - (180-Base::toDegrees<double>(da));
	
	//b
	//nrm 在YZ平面上与Y轴的夹角
	Base::Vector3d vb = nrm.ProjToPlane(base, x_axis);
	double db = vb.GetAngle(z_axis);
	b = -Base::toDegrees<double>(db);	
	
	//c
	//nrm在XY平面上绕Z轴旋转的角度
	Base::Vector3d vc = nrm.ProjToPlane(base, y_axis);
	double dc = vc.GetAngle(x_axis);
	c = -Base::toDegrees<double>(dc);

	if (fabs(a) < Precision::Confusion()) a = 0;
	if (fabs(b) < Precision::Confusion()) b = 0;
	if (fabs(c) < Precision::Confusion()) c = 0;
}

//输出程序头
void NCOutPutter::printHeader(std::ofstream& ofs, int& nlines)
{
	//ofs << "%" << std::endl;
	std::vector<std::string> befores = BeforeGcode.getValues();
	for (int i = 0; i < befores.size(); i++)
	{
		std::string s = befores[i];
		if (s.size() > 0 && s != "")
		{
			ofs << "N" << nlines++ << " ";
			ofs << befores[i] << std::endl;
		}
	}
}
//输出程序尾
void NCOutPutter::printFooter(std::ofstream& ofs, int& nlines)
{
	std::vector<std::string> afters = AfterGcode.getValues();
	for (int i = 0; i < afters.size(); i++)
	{
		std::string s = afters[i];
		if (s.size() > 0 && s != "")
		{
			ofs << "N" << nlines++ << " ";
			ofs << s << std::endl;
		}
	}
//	ofs << "%" << std::endl;
}

//输出一段程序开始前的进刀
void NCOutPutter::printFeed(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement place)
{
	double h = SafeHeight.getValue();
	
	if (paths.size() > 0)
	{
		Custom::NCStepInfo step = paths[0];
		Custom::NCStepInfo step1 = paths[1];
	
		double db = getB(step.Point, place);
		double da = getA(step.Point - step1.Point, place, db);
		
		Base::Vector3d r = adjustPointByTools(step.Point , place,db);

		ofs << "N" << nlines++ << " " << "G00 X" << h << " Z" << h/*<< "; Z轴运行至安全高度"*/ << std::endl;
		ofs << "N" << nlines++ << " " << "G01 G90 A" << db /*<< "; B旋转到相应角度"*/ << std::endl;
		ofs << "N" << nlines++ << " " << "G01 G90 B" << da /*<< "; A旋转到相应角度"*/<< std::endl;
		ofs << "N" << nlines++ << " " << "G01 G90 Y" << r.y << " " << std::endl;
		ofs << "N" << nlines++ << " " << "G01 G90 Z" << r.z << " " << std::endl;
		ofs << "N" << nlines++ << " " << "G01 G90 X" << r.x << " F" << step.speed/*<< ";x, y" */ << std::endl;
	}
}
//输出一段程序结束后的退刀
void NCOutPutter::printTract(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement place)
{
	double h = SafeHeight.getValue();
	//if (paths.size() > 0)
	{
// 		int last = paths.size() - 1;
// 
// 		Custom::NCStepInfo step1 = paths[last-1];
// 		Custom::NCStepInfo step = paths[last];
// 
// 		Base::Vector3d dir = step.Point - step1.Point;		
// 		dir = dir.Normalize();
// 		double tool_radius = getToolDiameter() ;
// 		Base::Vector3d ppnt = step.Point + dir*tool_radius;
// 
// 		double db = getB(step.Point, place);
// 		double da = getA(step1.Point - step.Point, place, db);
// 
// 		Base::Vector3d r = adjustPointByTools(ppnt, place,db);
// 
// 		ofs << "N" << nlines++ << " G01 G90 ";		
// 		ofs << " X" << r.x << " ";
// 		ofs << " Y" << r.y << " ";
// 		ofs << " Z" << r.z << " ";
// 		ofs << std::endl;
		//修改退刀的轨迹是 先走一段X，然后Z向上提，
		ofs << "N" << nlines++ << " " << "G00 X" << h << std::endl;
		ofs << "N" << nlines++ << " " << "G00 Z" << h << std::endl;
		//ofs << "N" << nlines++ << " " << "G00 X0 Y0 Z0  " << std::endl;
		//ofs << "N" << nlines++ << " " << "G00 A" << 0 << " B" <<0 <<std::endl;
	}
}
//输出一段程序
void NCOutPutter::printAPath(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement place)
{
	double tol = 0.0001;
	double local_speed = Speed.getValue();
	if (paths.size() > 0)
	{
		//ofs << "\n\n\n;----Path Begin----------" << std::endl;
		ofs << "\n\n\n" << std::endl;
		printFeed(ofs, nlines, paths , place);
		
		Custom::NCStepInfo lastStep = paths[0];
		double db = getB(lastStep.Point, place);
		for (int i = 1; i < paths.size(); i++)
		{
			Custom::NCStepInfo step = paths[i];
			
			double da = getA(lastStep.Point -step.Point, place, db);
			double la = getA(lastStep.Point, place);
			
			Base::Vector3d r = adjustPointByTools(step.Point, place,db);		

			ofs << "N" << nlines++ << " G01 G90 ";
			if (fabs(step.Point.x - lastStep.Point.x) > tol)
				ofs << " X" << r.x << " ";
			if (fabs(step.Point.y - lastStep.Point.y) > tol)
				ofs << " Y" << r.y << " ";
			if (fabs(step.Point.z - lastStep.Point.z) > tol)
				ofs << " Z" << r.z << " ";
			//if (fabs(da - la) > tol)
				ofs << " B" << da /* << " B" << db << " C" << dc*/;
			//if (fabs(step.speed - lastStep.speed) > tol)
				ofs << " F" << local_speed << " ";
			ofs << std::endl;

			lastStep = step;
		}

		printTract(ofs, nlines, paths, place);
		//ofs << ";----Path End----------\n\n\n" << std::endl;
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


	std::vector<App::DocumentObject*>   listPathObject = PathList.getValues();

	std::vector<TopoDS_Edge> pthEdgeList;
	for (int i = 0; i < listPathObject.size(); i++)
	{
		App::DocumentObject* pObj = listPathObject[i];
		Custom::BallCutter * pBallCutter = dynamic_cast<Custom::BallCutter *>(pObj);
		if (pBallCutter)
		{
			TopoDS_Edge aEdge = pBallCutter->getSpineEdge();
			

			std::vector<NCStepInfo> pntlist;
			BuildPath(aEdge, pntlist, pBallCutter);
			Base::Placement place;
			if (pBallCutter)
			{
				place = pBallCutter->Placement.getValue();
			}
			printAPath(ofs, nlines, pntlist, place);
			m_nclist.push_back(pntlist);

			int nCount = pBallCutter->L.getValue();
			for (int i = 1; i <= nCount; i++)
			{
				gp_Trsf T;
				T.SetRotation(gp_Ax1(gp_Pnt(0., 0., 0.), gp_Vec(0., 0., -1.)), i * 2 * M_PI / nCount);
				BRepBuilderAPI_Transform theTrsf(T);
				theTrsf.Perform(aEdge, Standard_True);

				TopoDS_Edge edge = TopoDS::Edge(theTrsf.Shape());
				
				std::vector<NCStepInfo> pntlist;
				BuildPath(edge, pntlist, pBallCutter);
				Base::Placement place;
				if (pBallCutter)
				{
					place = pBallCutter->Placement.getValue();
				}
				printAPath(ofs, nlines, pntlist, place);
				m_nclist.push_back(pntlist);
			}
		}
	}

#if 0
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
#endif
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
