/*
	By.Wheat
	2019.06.09

	文件：EqualizeHist.cpp
	简述：直方图均衡算法。
*/
#include "IndiscSolve.h"
using namespace cv;

/***************EqualizeHist******************/
ADD_SUMMARY(EqualizeHist, "直方图均衡算法")
INDISCSOLVE_CLASS_DEFINE(EqualizeHist);
INDISCSOLVE_CREATE_FUNC(EqualizeHist)
{
	/*
		@retval	IndiscSolve*: 返回新实例指针
		____实例构造函数ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: 处理算法附加参数名（对于每个参数都是必需的，该容器大小决定附加参数数量）
		____@param [const StrVec&] para_default_vec: 处理算法附加参数缺省值（对于每个参数都是非必需的）
	*/
	return new INDISCSOLVE_CLASSNAME(EqualizeHist)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(EqualizeHist)
{
	/*
		@param [const Mat&] src: 输入图像引用
		@retval	Mat: 返回处理结果
		@extra [StrVec] extraParaVec: 算法附加参数，使用Param<Ty>(int)模板获取。
	*/
	CV_Assert(src.depth() == CV_8UC3 || src.depth() == CV_8UC1);
	//Mat dst(src.rows, src.cols, src.type());

	Mat dst, gray;

	//转换为灰度图
	if (src.type() == CV_8UC3) cvtColor(src, gray, COLOR_RGB2GRAY);
	else gray = src.clone();

	equalizeHist(gray, dst);

	return std::move(dst);
}
