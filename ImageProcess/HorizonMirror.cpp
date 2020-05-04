/*
	By.Wheat
	2019.06.08

	�ļ���HorizonMirror.cpp
	������ˮƽ�����㷨��
*/
#include "IndiscSolve.h"
using namespace cv;

/***************HorizonMirror******************/
ADD_SUMMARY(HorizonMirror,"ˮƽ�����㷨")
INDISCSOLVE_CLASS_DEFINE(HorizonMirror);
INDISCSOLVE_CREATE_FUNC(HorizonMirror)
{
	/*
		@retval	IndiscSolve*: ������ʵ��ָ��
		____ʵ�����캯��ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: �����㷨���Ӳ�����������ÿ���������Ǳ���ģ���������С�������Ӳ���������
		____@param [const StrVec&] para_default_vec: �����㷨���Ӳ���ȱʡֵ������ÿ���������ǷǱ���ģ�
	*/
	return new INDISCSOLVE_CLASSNAME(HorizonMirror)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(HorizonMirror)
{
	/*
		@param [const Mat&] src: ����ͼ������
		@retval	Mat: ���ش�����
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
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
