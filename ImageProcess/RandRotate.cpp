/*
	By.Wheat
	2019.06.08

	�ļ���RandRoate.cpp
	���������׼�����ͼ���������̬����ת�㷨��
*/
#include "DiscSolve.h"
#include <random>

using namespace cv;

/***************Incise******************/
ADD_SUMMARY(RandRotate, "���׼�����ͼ���������̬����ת�㷨")
DISCSOLVE_CLASS_DEFINE(RandRotate);
DISCSOLVE_CREATE_FUNC(RandRotate)
{
	/*
		@retval	DiscSolve*: ������ʵ��ָ��
		____ʵ�����캯��ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: �����㷨���Ӳ�����������ÿ���������Ǳ���ģ���������С�������Ӳ���������
		____@param [const StrVec&] para_default_vec: �����㷨���Ӳ���ȱʡֵ������ÿ���������ǷǱ���ģ�
	*/

	StrVec paraNameVec;
	StrVec paraDefaultVec;
	{
		paraNameVec.push_back("����ԭͼĿ¼");	paraDefaultVec.push_back(".\\Image");
		paraNameVec.push_back("��������Ŀ¼");	paraDefaultVec.push_back(".\\Label");
		paraNameVec.push_back("���ԭͼĿ¼");	paraDefaultVec.push_back(Str(".\\") + name + "Image");
		paraNameVec.push_back("�������Ŀ¼");	paraDefaultVec.push_back(Str(".\\") + name + "Label");
	}
	{
		paraNameVec.push_back("��ת�Ƕȷ���");			paraDefaultVec.push_back("10");
	}
	return new DISCSOLVE_CLASSNAME(RandRotate)(paraNameVec, paraDefaultVec);
}
DISCSOLVE_SOLVE_FUNC(RandRotate)
{
	/*
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
	*/

	Str inImgDir	= Param<Str>(0);
	Str inLabelDir	= Param<Str>(1);
	Str outImgDir	= Param<Str>(2);
	Str outLabelDir = Param<Str>(3);
	float variance	= Param<float>(4);

	StrVec img_files = GetFileList(inImgDir);
	StrVec label_files = GetFileList(inLabelDir);

	if (IF_ERROR(Create_Directory(outImgDir)))	PAUSE_EXIT;
	if (IF_ERROR(Create_Directory(outLabelDir)))	PAUSE_EXIT;

	std::vector<double> rotate_angles(img_files.size());

	//default_random_engine generator;//��������Ĭ�ϵ����棬ÿ�����ɵ������������ͬ�ġ�
	//random_device rd;
	std::mt19937 gen(0);			//ÿ�����������ͬ

	//normal(0,1)��0Ϊ��ֵ��1Ϊ����
	std::normal_distribution<double> normal(0, variance);


	for (int n = 0; n < img_files.size(); ++n) {
		rotate_angles[n] = normal(gen);
		while (rotate_angles[n] == 0 || abs(rotate_angles[n]) > 45) {
			rotate_angles[n] = normal(gen);
		}
	}


	ParallelWithCounter(0, img_files.size(), [&](int i)
		{
			Mat img = imread(inImgDir + "\\" + img_files[i], IMREAD_GRAYSCALE);
			Mat label = imread(inLabelDir + "\\" + label_files[i], IMREAD_GRAYSCALE);

			double angle = rotate_angles[i]; // �Ƕ�  

			int tempLength = sqrt(img.rows * img.rows + img.cols * img.cols) + 10;

			//��ת����map
			// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
			// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]

			// ��ת����
			CvPoint2D32f center = cvPoint2D32f(img.cols / 2, img.rows / 2);
			Mat map_matrix = getRotationMatrix2D(center, angle, 1.0);
			map_matrix.at<double>(0, 2) += (tempLength - img.cols) / 2;
			map_matrix.at<double>(1, 2) += (tempLength - img.rows) / 2;

			//��ͼ��������任
			//CV_WARP_FILL_OUTLIERS - ����������ͼ������ء�
			//�������������������ͼ��ı߽��⣬��ô���ǵ�ֵ�趨Ϊ fillval.
			//CV_WARP_INVERSE_MAP - ָ�� map_matrix �����ͼ������ͼ��ķ��任��
			Mat img_rotate(tempLength, tempLength, CV_8UC3);
			warpAffine(img, img_rotate, map_matrix, img_rotate.size(), CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS);
			Mat label_rotate(tempLength, tempLength, CV_8UC3);
			warpAffine(label, label_rotate, map_matrix, label_rotate.size(), CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS);

			imwrite(outImgDir + "\\" + img_files[i], img_rotate);
			imwrite(outLabelDir + "\\" + img_files[i], label_rotate);

		},name);

}
