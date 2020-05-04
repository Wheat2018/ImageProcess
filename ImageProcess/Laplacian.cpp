/*
	By.Wheat
	2019.06.09

	文件：Laplacian.cpp
	简述：Laplacian边缘检测算法。
*/
#include "IndiscSolve.h"
using namespace cv;

/***************Laplacian******************/
ADD_SUMMARY(LaplacianSobel, "Laplacian边缘检测算法")
INDISCSOLVE_CLASS_DEFINE(Laplacian);
INDISCSOLVE_CREATE_FUNC(Laplacian)
{
	/*
		@retval	IndiscSolve*: 返回新实例指针
		____实例构造函数ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: 处理算法附加参数名（对于每个参数都是必需的，该容器大小决定附加参数数量）
		____@param [const StrVec&] para_default_vec: 处理算法附加参数缺省值（对于每个参数都是非必需的）
	*/
	return new INDISCSOLVE_CLASSNAME(Laplacian)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(Laplacian)
{
	/*
		@param [const Mat&] src: 输入图像引用
		@retval	Mat: 返回处理结果
		@extra [StrVec] extraParaVec: 算法附加参数，使用Param<Ty>(int)模板获取。
	*/
	CV_Assert(src.depth() == CV_8UC3 || src.depth() == CV_8UC1);
	//Mat dst(src.rows, src.cols, src.type());

	Mat img = src.clone();
	Mat gray, dst, abs_dst;
	//高斯滤波消除噪声
	GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);
	//转换为灰度图
	if (img.type() == CV_8UC3) cvtColor(img, gray, COLOR_RGB2GRAY);
	else gray = img;
	//使用Laplace函数
	//第三个参数：目标图像深度；第四个参数：滤波器孔径尺寸；第五个参数：比例因子；第六个参数：表示结果存入目标图
	Laplacian(gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	//计算绝对值，并将结果转为8位
	convertScaleAbs(dst, abs_dst);

	return std::move(abs_dst);
}
