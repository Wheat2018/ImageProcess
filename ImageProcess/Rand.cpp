/*
	By.Wheat
	2019.06.07

	�ļ���Rand.cpp
	���������׼�����ͼ������ּ��������㷨��
*/
#include "DiscSolve.h"
using namespace cv;

int DepartSet(const Str & img_dir,				//ͼ��Ŀ¼
	const Str & label_dir,						//����Ŀ¼
	std::vector<Str> & img_files,				//ͼ��Ŀ¼�������ļ���
	std::vector<Str> & label_files,				//����Ŀ¼�������ļ���
	std::vector<int> & all,						//��ȡ���
	const Str & set_dir,						//���ͼ����Ŀ¼		������1����2����3...
	const Str & set_name,						//���ͼ����			����ѵ������������֤�������Լ�
	float rate,									//��ȡ����
	volatile long& count						//������
);

/***************Rand******************/
ADD_SUMMARY(Rand, "���׼�����ͼ������ּ��������㷨")
DISCSOLVE_CLASS_DEFINE(Rand);
DISCSOLVE_CREATE_FUNC(Rand)
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
		paraNameVec.push_back("���Ŀ¼");		paraDefaultVec.push_back(".\\RandSet");
		paraNameVec.push_back("������֤��");	paraDefaultVec.push_back("Cross");
		paraNameVec.push_back("���Լ�");		paraDefaultVec.push_back("Test");
		paraNameVec.push_back("ѵ����");		paraDefaultVec.push_back("Train");
	}
	{
		paraNameVec.push_back("����");			paraDefaultVec.push_back("10");
	}
	return new DISCSOLVE_CLASSNAME(Rand)(paraNameVec, paraDefaultVec);
}
DISCSOLVE_SOLVE_FUNC(Rand)
{
	/*
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
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
		COUT << '\r' << "��� " << n + 1 << "/" << times << "                 ";
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
	COUT << "�����" << ENDL;


	ParallelWithCounter(0, times, [&](volatile long& count, int idx)
		{	//����ÿ��
			Str set_dir = saveDir + "\\" + (char)(idx + '0');
			if (IF_ERROR(Create_Directory(set_dir))) PAUSE_EXIT;

			//������֤��0.2
			DepartSet(imgDir, labelDir, img_files, label_files, alls[idx], set_dir, cross, 0.2, count);
			//���Լ�0.1
			DepartSet(imgDir, labelDir, img_files, label_files, alls[idx], set_dir, test, 0.1, count);
			//ѵ����0.7
			DepartSet(imgDir, labelDir, img_files, label_files, alls[idx], set_dir, train, 1, count);
		}, name, times * img_files.size());
}
/*************************************/

void CopySplitPic(const Str& in_img,			//����ͼ���ļ�
	const Str& in_label,						//���������ļ�
	const Str& out_img,							//���ͼ���ļ�
	const Str& out_tumour,						//������������ļ�
	const Str& out_bladder						//������������ļ�
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

int DepartSet(const Str& img_dir,				//ͼ��Ŀ¼
	const Str& label_dir,						//����Ŀ¼
	std::vector<Str>& img_files,				//ͼ��Ŀ¼�������ļ���
	std::vector<Str>& label_files,				//����Ŀ¼�������ļ���
	std::vector<int>& all,						//��ȡ���
	const Str& set_dir,							//���ͼ����Ŀ¼		������1����2����3...
	const Str& set_name,						//���ͼ����			����ѵ������������֤�������Լ�
	float rate,									//��ȡ����
	volatile long& count						//������
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
