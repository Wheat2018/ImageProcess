/*
	By.Wheat
	2019.06.09

	�ļ���Canny.cpp
	������Canny��Ե����㷨��
*/
#include "IndiscSolve.h"
using namespace cv;

/***************Canny******************/
ADD_SUMMARY(Canny, "Canny��Ե����㷨")
INDISCSOLVE_CLASS_DEFINE(Canny);
INDISCSOLVE_CREATE_FUNC(Canny)
{
	/*
		@retval	IndiscSolve*: ������ʵ��ָ��
		____ʵ�����캯��ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: �����㷨���Ӳ�����������ÿ���������Ǳ���ģ���������С�������Ӳ���������
		____@param [const StrVec&] para_default_vec: �����㷨���Ӳ���ȱʡֵ������ÿ���������ǷǱ���ģ�
	*/
	StrVec paraNameVec;
	StrVec paraDefaultVec;
	{
		paraNameVec.push_back("�����ں˴�С");							paraDefaultVec.push_back("(3,3)");
		paraNameVec.push_back("Canny�ͺ������ֵ1");					paraDefaultVec.push_back("3");
		paraNameVec.push_back("Canny�ͺ������ֵ2");					paraDefaultVec.push_back("9");
	}

	return new INDISCSOLVE_CLASSNAME(Canny)(paraNameVec, paraDefaultVec);
}
INDISCSOLVE_SOLVE_FUNC(Canny)
{
	/*
		@param [const Mat&] src: ����ͼ������
		@retval	Mat: ���ش�����
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
	*/
	Size ksize = Size(Param<IntVec>(0)[0], Param<IntVec>(0)[1]);
	int threshold1 = Param<int>(1);
	int threshold2 = Param<int>(2);

	CV_Assert(src.depth() == CV_8UC3 || src.depth() == CV_8UC1);
	//Mat dst(src.rows, src.cols, src.type());

	Mat edge, gray;

	//ת��Ϊ�Ҷ�ͼ
	if (src.type() == CV_8UC3) cvtColor(src, gray, COLOR_RGB2GRAY);
	else gray = src.clone();

	//��ʹ��3*3�ں�������
	blur(gray, edge, ksize);

	//����canny����
	Canny(edge, edge, threshold1, threshold2, 3);

	return std::move(edge);
}
