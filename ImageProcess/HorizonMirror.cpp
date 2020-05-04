/*
	By.Wheat
	2019.06.08

	文件：HorizonMirror.cpp
	简述：水平镜像算法。
*/
#include "IndiscSolve.h"
using namespace cv;

/***************HorizonMirror******************/
ADD_SUMMARY(HorizonMirror,"水平镜像算法")
INDISCSOLVE_CLASS_DEFINE(HorizonMirror);
INDISCSOLVE_CREATE_FUNC(HorizonMirror)
{
	/*
		@retval	IndiscSolve*: 返回新实例指针
		____实例构造函数ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: 处理算法附加参数名（对于每个参数都是必需的，该容器大小决定附加参数数量）
		____@param [const StrVec&] para_default_vec: 处理算法附加参数缺省值（对于每个参数都是非必需的）
	*/
	return new INDISCSOLVE_CLASSNAME(HorizonMirror)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(HorizonMirror)
{
	/*
		@param [const Mat&] src: 输入图像引用
		@retval	Mat: 返回处理结果
		@extra [StrVec] extraParaVec: 算法附加参数，使用Param<Ty>(int)模板获取。
	*/
	CV_Assert(src.depth() == CV_8UC3 || src.depth() == CV_8UC1);
	Mat dst(src.rows, src.cols, src.type());

	int rows = src.rows;
	int cols = src.cols;

	switch (src.channels())
	{
	case 1:
		const uchar * origal;
		uchar* p;
		for (int i = 0; i < rows; i++) {
			origal = src.ptr<uchar>(i);
			p = dst.ptr<uchar>(i);
			for (int j = 0; j < cols; j++) {
				p[j] = origal[cols - 1 - j];
			}
		}
		break;
	case 3:
		const Vec3b * origal3;
		Vec3b* p3;
		for (int i = 0; i < rows; i++) {
			origal3 = src.ptr<Vec3b>(i);
			p3 = dst.ptr<Vec3b>(i);
			for (int j = 0; j < cols; j++) {
				p3[j] = origal3[cols - 1 - j];
			}
		}
		break;
	default:
		break;
	}
	return std::move(dst);
}
