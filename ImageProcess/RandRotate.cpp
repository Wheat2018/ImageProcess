/*
	By.Wheat
	2019.06.08

	文件：RandRoate.cpp
	简述：膀胱及掩码图像随机（正态）旋转算法。
*/
#include "DiscSolve.h"
#include <random>

using namespace cv;

/***************Incise******************/
ADD_SUMMARY(RandRotate, "膀胱及掩码图像随机（正态）旋转算法")
DISCSOLVE_CLASS_DEFINE(RandRotate);
DISCSOLVE_CREATE_FUNC(RandRotate)
{
	/*
		@retval	DiscSolve*: 返回新实例指针
		____实例构造函数ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: 处理算法附加参数名（对于每个参数都是必需的，该容器大小决定附加参数数量）
		____@param [const StrVec&] para_default_vec: 处理算法附加参数缺省值（对于每个参数都是非必需的）
	*/

	StrVec paraNameVec;
	StrVec paraDefaultVec;
	{
		paraNameVec.push_back("输入原图目录");	paraDefaultVec.push_back(".\\Image");
		paraNameVec.push_back("输入掩码目录");	paraDefaultVec.push_back(".\\Label");
		paraNameVec.push_back("输出原图目录");	paraDefaultVec.push_back(Str(".\\") + name + "Image");
		paraNameVec.push_back("输出掩码目录");	paraDefaultVec.push_back(Str(".\\") + name + "Label");
	}
	{
		paraNameVec.push_back("旋转角度方差");			paraDefaultVec.push_back("10");
	}
	return new DISCSOLVE_CLASSNAME(RandRotate)(paraNameVec, paraDefaultVec);
}
DISCSOLVE_SOLVE_FUNC(RandRotate)
{
	/*
		@extra [StrVec] extraParaVec: 算法附加参数，使用Param<Ty>(int)模板获取。
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

	//default_random_engine generator;//如果用这个默认的引擎，每次生成的随机序列是相同的。
	//random_device rd;
	std::mt19937 gen(0);			//每次随机序列相同

	//normal(0,1)中0为均值，1为方差
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

			double angle = rotate_angles[i]; // 角度  

			int tempLength = sqrt(img.rows * img.rows + img.cols * img.cols) + 10;

			//旋转数组map
			// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
			// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]

			// 旋转中心
			CvPoint2D32f center = cvPoint2D32f(img.cols / 2, img.rows / 2);
			Mat map_matrix = getRotationMatrix2D(center, angle, 1.0);
			map_matrix.at<double>(0, 2) += (tempLength - img.cols) / 2;
			map_matrix.at<double>(1, 2) += (tempLength - img.rows) / 2;

			//对图像做仿射变换
			//CV_WARP_FILL_OUTLIERS - 填充所有输出图像的象素。
			//如果部分象素落在输入图像的边界外，那么它们的值设定为 fillval.
			//CV_WARP_INVERSE_MAP - 指定 map_matrix 是输出图像到输入图像的反变换，
			Mat img_rotate(tempLength, tempLength, CV_8UC3);
			warpAffine(img, img_rotate, map_matrix, img_rotate.size(), CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS);
			Mat label_rotate(tempLength, tempLength, CV_8UC3);
			warpAffine(label, label_rotate, map_matrix, label_rotate.size(), CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS);

			imwrite(outImgDir + "\\" + img_files[i], img_rotate);
			imwrite(outLabelDir + "\\" + img_files[i], label_rotate);

		},name);

}
