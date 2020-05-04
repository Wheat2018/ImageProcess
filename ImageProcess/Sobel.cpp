/*
	By.Wheat
	2019.06.09

	�ļ���Sobel.cpp
	������Sobel��Ե����㷨��
*/
#include "IndiscSolve.h"
using namespace cv;

/***************Sobel******************/
ADD_SUMMARY(Sobel, "Sobel��Ե����㷨")
INDISCSOLVE_CLASS_DEFINE(Sobel);
INDISCSOLVE_CREATE_FUNC(Sobel)
{
	/*
		@retval	IndiscSolve*: ������ʵ��ָ��
		____ʵ�����캯��ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: �����㷨���Ӳ�����������ÿ���������Ǳ���ģ���������С�������Ӳ���������
		____@param [const StrVec&] para_default_vec: �����㷨���Ӳ���ȱʡֵ������ÿ���������ǷǱ���ģ�
	*/
	return new INDISCSOLVE_CLASSNAME(Sobel)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(Sobel)
{
	/*
		@param [const Mat&] src: ����ͼ������
		@retval	Mat: ���ش�����
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
	*/
	Mat dst(src.rows, src.cols, src.type());

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	//��x�����ݶ�
	Sobel(src, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	//��y�����ݶ�
	Sobel(src, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	//�ϲ��ݶ�
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);

	return std::move(dst);
}
