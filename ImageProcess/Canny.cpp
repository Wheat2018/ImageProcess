/*
	By.Wheat
	2019.06.09

	文件：Canny.cpp
	简述：Canny边缘检测算法。
*/
#include "IndiscSolve.h"
using namespace cv;

/***************Canny******************/
ADD_SUMMARY(Canny, "Canny边缘检测算法")
INDISCSOLVE_CLASS_DEFINE(Canny);
INDISCSOLVE_CREATE_FUNC(Canny)
{
	/*
		@retval	IndiscSolve*: 返回新实例指针
		____实例构造函数ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: 处理算法附加参数名（对于每个参数都是必需的，该容器大小决定附加参数数量）
		____@param [const StrVec&] para_default_vec: 处理算法附加参数缺省值（对于每个参数都是非必需的）
	*/
	StrVec paraNameVec;
	StrVec paraDefaultVec;
	{
		paraNameVec.push_back("降噪内核大小");							paraDefaultVec.push_back("(3,3)");
		paraNameVec.push_back("Canny滞后过程阈值1");					paraDefaultVec.push_back("3");
		paraNameVec.push_back("Canny滞后过程阈值2");					paraDefaultVec.push_back("9");
	}

	return new INDISCSOLVE_CLASSNAME(Canny)(paraNameVec, paraDefaultVec);
}
INDISCSOLVE_SOLVE_FUNC(Canny)
{
	/*
		@param [const Mat&] src: 输入图像引用
		@retval	Mat: 返回处理结果
		@extra [StrVec] extraParaVec: 算法附加参数，使用Param<Ty>(int)模板获取。
	*/
	Size ksize = Size(Param<IntVec>(0)[0], Param<IntVec>(0)[1]);
	int threshold1 = Param<int>(1);
	int threshold2 = Param<int>(2);

	CV_Assert(src.depth() == CV_8UC3 || src.depth() == CV_8UC1);
	//Mat dst(src.rows, src.cols, src.type());

	Mat edge, gray;

	//转换为灰度图
	if (src.type() == CV_8UC3) cvtColor(src, gray, COLOR_RGB2GRAY);
	else gray = src.clone();

	//先使用3*3内核来降噪
	blur(gray, edge, ksize);

	//运行canny算子
	Canny(edge, edge, threshold1, threshold2, 3);

	return std::move(edge);
}
