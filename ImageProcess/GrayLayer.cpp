/*
	By.Wheat
	2019.06.09

	�ļ���GrayLayer.cpp
	�������Ҷȼ��ֲ��㷨��
*/
#include "IndiscSolve.h"
using namespace cv;

/***************GrayLayer******************/
ADD_SUMMARY(GrayLayer, "�Ҷȼ��ֲ��㷨")
INDISCSOLVE_CLASS_DEFINE(GrayLayer);
INDISCSOLVE_CREATE_FUNC(GrayLayer)
{
	/*
		@retval	IndiscSolve*: ������ʵ��ָ��
		____ʵ�����캯��ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: �����㷨���Ӳ�����������ÿ���������Ǳ���ģ���������С�������Ӳ���������
		____@param [const StrVec&] para_default_vec: �����㷨���Ӳ���ȱʡֵ������ÿ���������ǷǱ���ģ�
	*/
	return new INDISCSOLVE_CLASSNAME(GrayLayer)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(GrayLayer)
{
	/*
		@param [const Mat&] src: ����ͼ������
		@retval	Mat: ���ش�����
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
	*/
	CV_Assert(src.depth() == CV_8UC3 || src.depth() == CV_8UC1);
	//Mat dst(src.rows, src.cols, src.type());

	Mat gray;

	//ת��Ϊ�Ҷ�ͼ
	if (src.type() == CV_8UC3) cvtColor(src, gray, COLOR_RGB2GRAY);
	else gray = src.clone();

	for (size_t i = 0; i < gray.rows; i++)
	{
		for (size_t j = 0; j < gray.cols; j++)
		{
			if (gray.at<uchar>(i, j) < (uchar)(256 * 0.25) && gray.at<uchar>(i, j) >= (uchar)(256 * 0.1))
			{
				gray.at<uchar>(i, j) = 256 * 0.25;
			}
		}
	}

	return std::move(gray);
}
