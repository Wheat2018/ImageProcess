/*
	By.Wheat
	2019.06.09

	�ļ���EqualizeHist.cpp
	������ֱ��ͼ�����㷨��
*/
#include "IndiscSolve.h"
using namespace cv;

/***************EqualizeHist******************/
ADD_SUMMARY(EqualizeHist, "ֱ��ͼ�����㷨")
INDISCSOLVE_CLASS_DEFINE(EqualizeHist);
INDISCSOLVE_CREATE_FUNC(EqualizeHist)
{
	/*
		@retval	IndiscSolve*: ������ʵ��ָ��
		____ʵ�����캯��ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: �����㷨���Ӳ�����������ÿ���������Ǳ���ģ���������С�������Ӳ���������
		____@param [const StrVec&] para_default_vec: �����㷨���Ӳ���ȱʡֵ������ÿ���������ǷǱ���ģ�
	*/
	return new INDISCSOLVE_CLASSNAME(EqualizeHist)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(EqualizeHist)
{
	/*
		@param [const Mat&] src: ����ͼ������
		@retval	Mat: ���ش�����
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
	*/
	CV_Assert(src.depth() == CV_8UC3 || src.depth() == CV_8UC1);
	//Mat dst(src.rows, src.cols, src.type());

	Mat dst, gray;

	//ת��Ϊ�Ҷ�ͼ
	if (src.type() == CV_8UC3) cvtColor(src, gray, COLOR_RGB2GRAY);
	else gray = src.clone();

	equalizeHist(gray, dst);

	return std::move(dst);
}
