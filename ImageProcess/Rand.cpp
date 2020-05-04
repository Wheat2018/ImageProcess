/*
	By.Wheat
	2019.06.07

	文件：Rand.cpp
	简述：膀胱及掩码图像随机分集、分折算法。
*/
#include "DiscSolve.h"
using namespace cv;

int DepartSet(const Str & img_dir,				//图像目录
	const Str & label_dir,						//掩码目录
	std::vector<Str> & img_files,				//图像目录下所有文件名
	std::vector<Str> & label_files,				//掩码目录下所有文件名
	std::vector<int> & all,						//抽取序号
	const Str & set_dir,						//输出图像折目录		例：折1、折2、折3...
	const Str & set_name,						//输出图像集名			例：训练集、交叉验证集、测试集
	float rate,									//抽取比例
	volatile long& count						//计数器
);

/***************Rand******************/
ADD_SUMMARY(Rand, "膀胱及掩码图像随机分集、分折算法")
DISCSOLVE_CLASS_DEFINE(Rand);
DISCSOLVE_CREATE_FUNC(Rand)
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
		paraNameVec.push_back("输出目录");		paraDefaultVec.push_back(".\\RandSet");
		paraNameVec.push_back("交叉验证集");	paraDefaultVec.push_back("Cross");
		paraNameVec.push_back("测试集");		paraDefaultVec.push_back("Test");
		paraNameVec.push_back("训练集");		paraDefaultVec.push_back("Train");
	}
	{
		paraNameVec.push_back("折数");			paraDefaultVec.push_back("10");
	}
	return new DISCSOLVE_CLASSNAME(Rand)(paraNameVec, paraDefaultVec);
}
DISCSOLVE_SOLVE_FUNC(Rand)
{
	/*
		@extra [StrVec] extraParaVec: 算法附加参数，使用Param<Ty>(int)模板获取。
	*/

	Str imgDir	 = Param<Str>(0);
	Str labelDir = Param<Str>(1);
	Str saveDir	 = Param<Str>(2);
	Str cross	 = Param<Str>(3);
	Str test	 = Param<Str>(4);
	Str train	 = Param<Str>(5);
	int times	 = Param<int>(6);


	if (IF_ERROR(Create_Directory(saveDir))) PAUSE_EXIT;

	std::vector<Str> img_files = GetFileList(imgDir);
	std::vector<Str> label_files = GetFileList(labelDir);

	std::vector<std::vector<int>> alls(times);
	for (int n = 0; n < times; n++)
	{
		COUT << '\r' << "随机 " << n + 1 << "/" << times << "                 ";
		alls[n] = std::vector<int>(img_files.size());
		std::vector<int> tmp(img_files.size());
		for (int i = alls[n].size() - 1; i >= 0; i--) tmp[i] = i;
		for (int i = alls[n].size() - 1; i >= 0; i--)
		{
			int index = rand() % tmp.size();
			alls[n][i] = tmp[index];
			tmp.erase(tmp.begin() + index);
		}
	}
	COUT << "已完成" << ENDL;


	ParallelWithCounter(0, times, [&](volatile long& count, int idx)
		{	//处理每折
			Str set_dir = saveDir + "\\" + (char)(idx + '0');
			if (IF_ERROR(Create_Directory(set_dir))) PAUSE_EXIT;

			//交叉验证集0.2
			DepartSet(imgDir, labelDir, img_files, label_files, alls[idx], set_dir, cross, 0.2, count);
			//测试集0.1
			DepartSet(imgDir, labelDir, img_files, label_files, alls[idx], set_dir, test, 0.1, count);
			//训练集0.7
			DepartSet(imgDir, labelDir, img_files, label_files, alls[idx], set_dir, train, 1, count);
		}, name, times * img_files.size());
}
/*************************************/

void CopySplitPic(const Str& in_img,			//输入图像文件
	const Str& in_label,						//输入掩码文件
	const Str& out_img,							//输出图像文件
	const Str& out_tumour,						//输出肿瘤掩码文件
	const Str& out_bladder						//输出膀胱掩码文件
)
{
	CopyFile(in_img.c_str(),
		out_img.c_str(), false);

	Mat label = imread(in_label, IMREAD_GRAYSCALE);
	Mat tumour = Mat::zeros(label.rows, label.cols, CV_8UC1);
	Mat bladder = Mat::zeros(label.rows, label.cols, CV_8UC1);

	for (int i = 0; i < label.rows; i++) {
		for (int j = 0; j < label.cols; j++) {
			if (label.at<uchar>(i, j) == 255)
				tumour.at<uchar>(i, j) = 255;
			else if (label.at<uchar>(i, j) == 128)
				bladder.at<uchar>(i, j) = 255;
		}
	}

	imwrite(out_tumour, tumour);
	imwrite(out_bladder, bladder);

}

int DepartSet(const Str& img_dir,				//图像目录
	const Str& label_dir,						//掩码目录
	std::vector<Str>& img_files,				//图像目录下所有文件名
	std::vector<Str>& label_files,				//掩码目录下所有文件名
	std::vector<int>& all,						//抽取序号
	const Str& set_dir,							//输出图像折目录		例：折1、折2、折3...
	const Str& set_name,						//输出图像集名			例：训练集、交叉验证集、测试集
	float rate,									//抽取比例
	volatile long& count						//计数器
)
{
	if (IF_ERROR(Create_Directory(set_dir + "\\" + set_name)))							PAUSE_EXIT;
	if (IF_ERROR(Create_Directory(set_dir + "\\" + set_name + "\\" + "Image")))			PAUSE_EXIT;
	if (IF_ERROR(Create_Directory(set_dir + "\\" + set_name + "\\" + "Tumour")))		PAUSE_EXIT;
	if (IF_ERROR(Create_Directory(set_dir + "\\" + set_name + "\\" + "Bladder")))		PAUSE_EXIT;
	for (int k = img_files.size() * rate; k > 0; k--) {
		if (all.empty()) break;
		int img_idx = all[0];

		CopySplitPic(
			img_dir + "\\" + img_files[img_idx],
			label_dir + "\\" + label_files[img_idx],
			set_dir + "\\" + set_name + "\\" + "Image" + "\\" + img_files[img_idx],
			set_dir + "\\" + set_name + "\\" + "Tumour" + "\\" + label_files[img_idx],
			set_dir + "\\" + set_name + "\\" + "Bladder" + "\\" + label_files[img_idx]
		);
		all.erase(all.begin());
		IncrementCounter(count);
	}
	TRUE_EXIT;
}
