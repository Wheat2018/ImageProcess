/*
	By.Wheat
	2019.06.09

	文件：Sobel.cpp
	简述：Sobel边缘检测算法。
*/
#include "IndiscSolve.h"
using namespace cv;

/***************Sobel******************/
ADD_SUMMARY(Sobel, "Sobel边缘检测算法")
INDISCSOLVE_CLASS_DEFINE(Sobel);
INDISCSOLVE_CREATE_FUNC(Sobel)
{
	/*
		@retval	IndiscSolve*: 返回新实例指针
		____实例构造函数ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: 处理算法附加参数名（对于每个参数都是必需的，该容器大小决定附加参数数量）
		____@param [const StrVec&] para_default_vec: 处理算法附加参数缺省值（对于每个参数都是非必需的）
	*/
	return new INDISCSOLVE_CLASSNAME(Sobel)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(Sobel)
{
	/*
		@param [const Mat&] src: 输入图像引用
		@retval	Mat: 返回处理结果
		@extra [StrVec] extraParaVec: 算法附加参数，使用Param<Ty>(int)模板获取。
	*/
	Mat dst(src.rows, src.cols, src.type());

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	//求x方向梯度
	Sobel(src, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	//求y方向梯度
	Sobel(src, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	//合并梯度
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);

	return std::move(dst);
}
