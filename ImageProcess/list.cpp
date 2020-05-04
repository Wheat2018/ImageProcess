/*
	By.Wheat
	2019.06.07

	文件：list.cpp
	简述：列出所有算法。
*/
#include "DiscSolve.h"
#include "IndiscSolve.h"
using namespace cv;

/***************list******************/
ADD_SUMMARY(list, "列出所有算法")
DISCSOLVE_CLASS_DEFINE(list);
DISCSOLVE_CREATE_FUNC(list)
{
	/*
		@retval	DiscSolve*: 返回新实例指针
		____实例构造函数ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: 处理算法附加参数名（对于每个参数都是必需的，该容器大小决定附加参数数量）
		____@param [const StrVec&] para_default_vec: 处理算法附加参数缺省值（对于每个参数都是非必需的）
	*/
	return new DISCSOLVE_CLASSNAME(list)(StrVec(), StrVec());
}
DISCSOLVE_SOLVE_FUNC(list)
{
	/*
		@extra [StrVec] extraParaVec: 算法附加参数，使用Param<Ty>(int)模板获取。
	*/
	COUT << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>命令行参数格式<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << ENDL;
	COUT << ENDL;
	COUT << "    >>ImageProcess.exe 【参数1】 【参数2】 ..." << ENDL;
	COUT << ENDL;
	COUT << "   ---------------------------DiscSolve类算法-------------------------   " << ENDL;
	COUT << ENDL;
	COUT << "   【参数1】   [处理算法名称]          如Rand、Incise等" << ENDL;
	COUT << "   【参数2】   [结束后暂停]            0(不暂停)或1(暂停),默认为1" << ENDL;
	COUT << "   【参数..】  [算法附加参数若干...]   算法参数中介绍" << ENDL;
	COUT << ENDL;
	COUT << "   --------------------------IndiscSolve类算法------------------------   " << ENDL;
	COUT << ENDL;
	COUT << "   【参数1】   [处理算法名称]          如HorizonMirror等" << ENDL;
	COUT << "   【参数2】   [结束后暂停]            0(不暂停)或1(暂停),默认为1" << ENDL;
	COUT << "   【参数3】   [输入目录数]            整数，影响输入目录参数个数" << ENDL;
	COUT << "   【参数4】   [输出目录数]            整数，影响输出目录参数个数" << ENDL;
	COUT << "   【参数..】  [输入目录]              个数与参数3匹配" << ENDL;
	COUT << "   【参数..】  [输出目录]              个数与参数4匹配" << ENDL;
	COUT << "   【参数..】  [算法附加参数若干...]   算法参数中介绍" << ENDL;
	COUT << ENDL;
	COUT << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>算法参数<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << ENDL;
	COUT << ENDL;
	COUT << "   ---------------------------DiscSolve类算法-------------------------   " << ENDL;
	for (auto iter = discFactory->begin(); iter != discFactory->end(); iter++)
	{
		COUT << "   算法名称：" << iter->first << ENDL;
		COUT << "   算法简介：" << Solve::Summary(iter->first) << ENDL;
		COUT << "   算法参数：" << ENDL;
		auto tmp = DiscSolve::Create(iter->first);
		tmp->Report("             ");
		COUT << ENDL;
		delete tmp;
	}
	COUT << "   --------------------------IndiscSolve类算法------------------------   " << ENDL;
	for (auto iter = indiscFactory->begin(); iter != indiscFactory->end(); iter++)
	{
		COUT << "   算法名称：" << iter->first << ENDL;
		COUT << "   算法简介：" << Solve::Summary(iter->first) << ENDL;
		COUT << "   算法参数：" << ENDL;
		auto tmp = IndiscSolve::Create(iter->first);
		tmp->Report("             ");
		COUT << ENDL;
		delete tmp;
	}
	COUT << "   -------------------------------------------------------------------   " << ENDL;
}
/******************************************/
