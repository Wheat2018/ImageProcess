/*
	By.Wheat
	2019.06.09

	�ļ���Laplacian.cpp
	������Laplacian��Ե����㷨��
*/
#include "IndiscSolve.h"
using namespace cv;

/***************Laplacian******************/
ADD_SUMMARY(LaplacianSobel, "Laplacian��Ե����㷨")
INDISCSOLVE_CLASS_DEFINE(Laplacian);
INDISCSOLVE_CREATE_FUNC(Laplacian)
{
	/*
		@retval	IndiscSolve*: ������ʵ��ָ��
		____ʵ�����캯��ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: �����㷨���Ӳ�����������ÿ���������Ǳ���ģ���������С�������Ӳ���������
		____@param [const StrVec&] para_default_vec: �����㷨���Ӳ���ȱʡֵ������ÿ���������ǷǱ���ģ�
	*/
	return new INDISCSOLVE_CLASSNAME(Laplacian)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(Laplacian)
{
	/*
		@param [const Mat&] src: ����ͼ������
		@retval	Mat: ���ش�����
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
	*/
	CV_Assert(src.depth() == CV_8UC3 || src.depth() == CV_8UC1);
	//Mat dst(src.rows, src.cols, src.type());

	Mat img = src.clone();
	Mat gray, dst, abs_dst;
	//��˹�˲���������
	GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);
	//ת��Ϊ�Ҷ�ͼ
	if (img.type() == CV_8UC3) cvtColor(img, gray, COLOR_RGB2GRAY);
	else gray = img;
	//ʹ��Laplace����
	//������������Ŀ��ͼ����ȣ����ĸ��������˲����׾��ߴ磻������������������ӣ���������������ʾ�������Ŀ��ͼ
	Laplacian(gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	//�������ֵ���������תΪ8λ
	convertScaleAbs(dst, abs_dst);

	return std::move(abs_dst);
}
