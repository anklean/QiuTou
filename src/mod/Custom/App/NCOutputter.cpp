
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
#include "gp_Quaternion.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "Geom_OffsetCurve.hxx"
#include "BRepAdaptor_Curve.hxx"

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
	ADD_PROPERTY(DeltaX, (18.0));
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

	nrm.x -= -16;

	Base::Vector3d base = place.getPosition();
	//Base::Vector3d base(0, 0, 0);
	Base::Vector3d va = nrm.ProjToPlane(base, z_axis);
	double dx = va.GetAngle(x_axis);
	double dy = va.GetAngle(y_axis);
	double dz = va.GetAngle(z_axis);
	double a = Base::toDegrees<double>(dx);
	a = -a;
	
	return a;
}


double NCOutPutter::getA(Base::Vector3d nrm, Base::Placement place, double B)
{
	Base::Vector3d x_axis(1, 0, 0);
	Base::Vector3d y_axis(0, 1, 0);
	Base::Vector3d z_axis(0, 0, 1);

	Base::Vector3d base(0, 0, 0);// = place.getPosition();

	double db = Base::toRadians<double>(B);

	Base::Matrix4D mtx = place.toMatrix();
	mtx.transpose();
	mtx.rotX(db);

	double dmtx[16] = { 0 };
	mtx.getMatrix(dmtx);

	Base::Vector3d newx(dmtx[0], dmtx[1], dmtx[2]);
	Base::Vector3d newy(dmtx[4], dmtx[5], dmtx[6]);
	Base::Vector3d newz(dmtx[8], dmtx[9], dmtx[10]);

	Base::Vector3d newnrm= mtx*nrm;
	
	Base::Vector3d va = newnrm.ProjToPlane(base, x_axis);
	double dx = va.GetAngle(x_axis);
	double dy = va.GetAngle(y_axis);
	double dz = va.GetAngle(z_axis);
	double ra = Base::toDegrees<double>(dx);
	double rb = Base::toDegrees<double>(dy);
	double rc = Base::toDegrees<double>(dz);
	// 	if (va.x > 0)
	// 		a = 90 - a;

	//适应a的旋转方向，a=-a;
	//rb *= -1;

	return rb;
}


double NCOutPutter::getA(Base::Vector3d pnt1, Base::Vector3d pnt2, Base::Placement place, double B)
{
	Base::Vector3d x_axis(1, 0, 0);
	Base::Vector3d y_axis(0, 1, 0);
	Base::Vector3d z_axis(0, 0, 1);

	Base::Vector3d base = place.getPosition();
	// 
	double db = Base::toRadians<double>(B);

	Base::Matrix4D mtx = place.toMatrix();
	mtx.transpose();
	//	mtx.rotZ(db);

	double dmtx[16] = { 0 };
	mtx.getMatrix(dmtx);

	Base::Vector3d newx(dmtx[0], dmtx[1], dmtx[2]);
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
	Base::Vector3d tempNormal = nrm;
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

	Base::Vector3d origin(0, 0, 0);
	Base::Vector3d zu_axis(0, 0, 1);
	Base::Vector3d vb = base.ProjToPlane(origin, x_axis); //z_axis
	Base::Vector3d vn = tempNormal.ProjToPlane(origin, x_axis); //z_axis
	Base::Vector3d vv = vn-vb;
	double db = vv.GetAngle(zu_axis);//z_axis
	double b = Base::toDegrees<double>(db);

	//
	// 通过getAngle求得的角度范围在0~180 
	// 所以需要根据vv 与y_axis的方向来判断是在0~180范围内，还是在180~360范围内
	// 若vv 与y_axis 反向，则说明在180~360
	// 判断两向量是否反向，对两向量做点乘,若结果大于0,就是同向,等于0垂直,小于0反向
	double ddd = nrm.Normalize()*y_axis.Normalize();
	if (ddd < 0)
	{
		b = 360 - b;
	}

	return b;
}

//
// 调整刀具点的位置
// 计算时使用的是砂轮的边缘，仿真时使用的砂轮的中心作为位置点，因此需要矫正
//
Base::Vector3d NCOutPutter::adjustPointByTools(Base::Vector3d& point, Base::Placement& place, double B)
{
	double tool_radius = getToolDiameter() / 2.0;
	// 
	// 		double b = B - 150;	
	// 		if (b < 0) b = 360 - b;
	// 		double db = 0.0;
	// 		if (b<180)
	// 			db = Base::toRadians<double>(b-45);
	// 		else
	// 			db = Base::toRadians<double>( 360-b);
	double db = Base::toRadians<double>(B);

	//Base::Vector3d tpnt = point;

	Base::Vector3d base = place.getPosition();
	Base::Vector3d bbase = base;
	Base::Matrix4D mtx = place.toMatrix();
	mtx.transpose();
	//mtx.rotX(db);

	double dmtx[16] = { 0 };
	mtx.getMatrix(dmtx);

	Base::Vector3d newx(dmtx[0], dmtx[1], dmtx[2]);
	Base::Vector3d newy(dmtx[4], dmtx[5], dmtx[6]);
	Base::Vector3d newz(dmtx[8], dmtx[9], dmtx[10]);

	Base::Matrix4D mmtx(m_curPtCenter, newx, db);
	Base::Vector3d tpnt = /*mmtx**/point;

	Base::Vector3d x_axis(1, 0, 0);
	Base::Vector3d y_axis(0, 1, 0);
	Base::Vector3d z_axis(0, 0, 1);
	// 
	Base::Vector3d origin(0, 0, 0);
	//Base::Vector3d vb = m_curPtCenter.ProjToPlane(tpnt, x_axis); //z_axis
	//Base::Vector3d vn = tpnt.ProjToPlane(origin, x_axis); //z_axis
	Base::Vector3d vv = m_curPtCenter - tpnt;
	vv = vv.Normalize();
	Base::Vector3d r = tpnt + vv * tool_radius;

	// 	Base::Matrix4D mmtx(m_curPtCenter, newx, db);
	// 	Base::Vector3d r = mmtx*tt;

	// 		Base::Vector3d vv = base -tt;
	// 			vv = vv.Normalize();
	// 			Base::Vector3d r = tt + vv * tool_radius;

	// 		Base::Vector3d bbase = base.ProjToPlane(tpnt, newx);
	// 		Base::Vector3d nrm_vec = bbase- tpnt  ;
	// 		nrm_vec = nrm_vec.Normalize();
	// 		Base::Vector3d tt = point + nrm_vec * tool_radius;


	return r;

	// 	Base::Vector3d tempNormal = point;
	// 	Base::Vector3d base = place.getPosition();
	// 	Base::Matrix4D mtx = place.toMatrix();
	// 	mtx.transpose();
	// 
	// 	double dmtx[16] = { 0 };
	// 	mtx.getMatrix(dmtx);
	// 
	// 	Base::Vector3d x_axis(dmtx[0], dmtx[1], dmtx[2]);
	// 	Base::Vector3d y_axis(dmtx[4], dmtx[5], dmtx[6]);
	// 	Base::Vector3d z_axis(dmtx[8], dmtx[9], dmtx[10]);
	// 	// 	Base::Vector3d x_axis(1, 0, 0);
	// 	// 	Base::Vector3d y_axis(0,1, 0);
	// 	//	Base::Vector3d z_axis(0, 0, 1);
	// 
	// 	Base::Vector3d origin(0, 0, 0);
	// 	Base::Vector3d zu_axis(0, 0, 1);
	// 	Base::Vector3d vb = base.ProjToPlane(origin, x_axis); //z_axis
	// 	Base::Vector3d vn = tempNormal.ProjToPlane(origin, x_axis); //z_axis
	// 	Base::Vector3d vv = vb - vn;
	// 
	// 
	// 	Base::Vector3d tt = point + z_axis * tool_radius;
	// 
	// 	return point;
	// 	return  tt;
}



Base::Vector3d NCOutPutter::adjustPointByTools(Custom::PathObject::PathPointList& pathPointList, int i, Base::Placement& place, double B)
{
	if (pathPointList.size() < 3)
		return Base::Vector3d();

	double dDeltax=DeltaX.getValue();
	double tool_radius = getToolDiameter() / 2.0;
	double db = Base::toRadians<double>(B - dDeltax);

	Base::Vector3d dir1, dir2;
	Base::Vector3d this_point;
	if (i == 0)
	{
		this_point = pathPointList[0].Point;
		Base::Vector3d point1 = pathPointList[1].Point;
		Base::Vector3d point2 = pathPointList[2].Point;

		dir1 = this_point - point1;
		dir2 = point1 - point2;
	}
	else if (i == pathPointList.size()-1)
	{
		this_point = pathPointList[i].Point;
		Base::Vector3d point1 = pathPointList[i - 2].Point;
		Base::Vector3d point2 = pathPointList[i - 1].Point;

		dir1 = point1 - point2;
		dir2 = point2 - this_point;
	}
	else
	{
		Base::Vector3d point1 = pathPointList[i - 1].Point;
		this_point = pathPointList[i].Point;
		Base::Vector3d point2 = pathPointList[i + 1].Point;

		dir1 = point1 - this_point;
		dir2 = this_point - point2;
	}

	gp_Dir dd1(dir1.x, dir1.y, dir1.z);
	gp_Dir dd2(dir2.x, dir2.y, dir2.z);
	gp_Dir nrmdir = dd1.Crossed(dd2);
	gp_Dir vdir = nrmdir.Crossed(dd2);
	Base::Vector3d vvdir(vdir.X(), vdir.Y(), vdir.Z());
	Base::Vector3d tt = this_point + vvdir * tool_radius;

	Base::Vector3d base = place.getPosition();
	Base::Matrix4D mtx = place.toMatrix();
	mtx.transpose();

	double dmtx[16] = { 0 };
	mtx.getMatrix(dmtx);

	Base::Vector3d newx(dmtx[0], dmtx[1], dmtx[2]);
	Base::Vector3d newy(dmtx[4], dmtx[5], dmtx[6]);
	Base::Vector3d newz(dmtx[8], dmtx[9], dmtx[10]);

	Base::Matrix4D mmtx(base, newx, db);
	Base::Vector3d r = mmtx *tt;

	return r;
}

double NCOutPutter::getToolDiameter()
{
	double tool_Diameter = 0;
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
	Base::Matrix4D mtx = place.toMatrix();
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
	a = -(180 - Base::toDegrees<double>(da));

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
	double tool_radius = getToolDiameter() / 2.0;

	if (paths.size() > 0)
	{
		Custom::NCStepInfo step = paths[0];
		Custom::NCStepInfo step1 = paths[1];

		double db = getB(step.Point, place);
// 		double rb = Base::toRadians<double>(db);
// 
// 		Base::Matrix4D mtx;// = place.toMatrix();
// 		mtx.transpose();
// 		mtx.rotX(rb);
// 		Base::Vector3d this_point = mtx*step.Point;
// 		Base::Vector3d next_point = mtx*step1.Point;
// 		double da = getA(this_point - next_point, place);

		Base::Vector3d r = adjustPointByTools(paths , 0, place, db);
		double da = getA(r, place);

		ofs << "N" << nlines++ << " " << "G01  A" << da /*<< "; B旋转到相应角度"*/ << std::endl;
		ofs << "N" << nlines++ << " " << "G01  B" << db /*<< "; A旋转到相应角度"*/ << std::endl;
		ofs << "N" << nlines++ << " " << "G01 Z" << h + tool_radius/*<< "; Z轴运行至安全高度"*/ << std::endl;
		//ofs << "N" << nlines++ << " " << "G01 X" << -h  << std::endl;
		ofs << "N" << nlines++ << " " << "G01  Y" << r.y << " " << std::endl;
		ofs << "N" << nlines++ << " " << "G01  X" << r.x << " F" << step.speed/*<< ";x, y" */ << std::endl;
		ofs << "N" << nlines++ << " " << "G01 G90 Z" << r.z << " " << std::endl;
	}
}
//输出一段程序结束后的退刀
void NCOutPutter::printTract(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement place)
{
	double h = SafeHeight.getValue();
	double tool_radius = getToolDiameter() / 2.0;
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
		ofs << "N" << nlines++ << " " << "G01 Z" << h +tool_radius << std::endl;
		//ofs << "N" << nlines++ << " " << "G01 X" << -h  << std::endl;
		//ofs << "N" << nlines++ << " " << "G01 X0 Y0 Z0  " << std::endl;
		//ofs << "N" << nlines++ << " " << "G01 A" << 0 << " B" <<0 <<std::endl;
	}
}
//输出一段程序
void NCOutPutter::printAPath(std::ofstream& ofs, int& nlines, Custom::PathObject::PathPointList& paths, Base::Placement place)
{
	double tol = 0.0001;
	double local_speed = Speed.getValue();
	double tool_radius = getToolDiameter() / 2.0;
	if (paths.size() > 0)
	{
		//ofs << "\n\n\n;----Path Begin----------" << std::endl;
		ofs << "\n\n\n" << std::endl;
		printFeed(ofs, nlines, paths, place);

		Custom::NCStepInfo lastStep = paths[0];
		double db = getB(lastStep.Point, place);
		for (int i = 1; i < paths.size(); i++)
		{
			Custom::NCStepInfo step = paths[i];		

			double dbt = getB(step.Point, place);
			double la = 0;// getA(step.Point, place, db);

// 			double rb = Base::toRadians<double>(db);
// 	
// 			Base::Matrix4D mtx;// = place.toMatrix();
// 			mtx.transpose();
// 			mtx.rotX(rb);
// 			Base::Vector3d this_point = mtx*step.Point;
// 			Base::Vector3d last_point = mtx*lastStep.Point;
			//double da = getA( this_point, place);

			Base::Vector3d r = adjustPointByTools(paths , i, place, db);
			double da = getA(r, place);

			ofs << "N" << nlines++ << " G01 G90 ";
			if (fabs(step.Point.x - lastStep.Point.x) > tol)
				ofs << " X" << r.x << " ";
			if (fabs(step.Point.y - lastStep.Point.y) > tol)
				ofs << " Y" << r.y << " ";
			if (fabs(step.Point.z - lastStep.Point.z) > tol)
				ofs << " Z" << r.z << " ";
			if (fabs(da - la) > tol)
				ofs << " A" << da /* << " B" << db << " C" << dc*/;
			if (fabs(da - la) > tol)
				ofs << " B" << dbt /* << " B" << db << " C" << dc*/;
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

	double tool_radius = getToolDiameter() / 2.0;
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
			TopoDS_Compound spineComp = pBallCutter->getSpineEdge();

			Base::Placement place = pBallCutter->Placement.getValue();
			m_curPtCenter = pBallCutter->getCenter();

			double q[4];
			place.getRotation().getValue(q[0], q[1], q[2], q[3]);
			Base::Vector3d pp = place.getPosition();

// 			gp_Trsf TT;
// 			TT.SetTransformation(gp_Quaternion(q[0], q[1], q[2], q[3]), gp_Vec(pp.x, pp.y, pp.z));
// 			BRepBuilderAPI_Transform theTrsfT(TT);
// 			theTrsfT.Perform(spineComp, Standard_True);

			TopExp_Explorer Ex;
			for (Ex.Init(spineComp/*theTrsfT.Shape()*/, TopAbs_EDGE); Ex.More(); Ex.Next()) {
				TopoDS_Edge aEdge = TopoDS::Edge(Ex.Current());

				std::vector<NCStepInfo> pntlist;
				BuildPath(aEdge, pntlist, pBallCutter);
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
