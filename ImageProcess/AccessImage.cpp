#include "DiscSolve.h"
#include "IndiscSolve.h"
#include <numeric>
using namespace cv;

struct MyRECT {
	Point leftTop, rightBottom;

	int GetWidth() { return rightBottom.x - leftTop.x; }
	int GetHeight() { return rightBottom.y - leftTop.y; }
};

Mat RectIncise(const Mat& src, MyRECT rect);


template <typename Vec_Ty>
double Kurtosis(std::vector<Vec_Ty> data_vec)
{
	double kurtosis;
	int n = data_vec.size();
	double s2 = 0, u4 = 0;
	double mean = (double)std::accumulate(std::begin(data_vec), std::end(data_vec), 0) / n;
	std::for_each(std::begin(data_vec), std::end(data_vec), [&](const Vec_Ty x) {
		s2 += pow(x - mean, 2);
		u4 += pow(x - mean, 4);
		});
	s2 /= n - 1;
	u4 /= n;
	kurtosis = u4 / double(s2 * s2) - 3;
	return kurtosis;
}

Mat GetIncise(const Mat& img, const Mat& label)
{
	CV_Assert(img.type() == CV_8UC1 && label.type() == CV_8UC1);
	Mat incise(img.rows, img.cols, CV_8UC1);
	for (size_t i = 0; i < label.rows; i++)
		for (size_t j = 0; j < label.cols; j++)
			if (label.at<uchar>(i, j) == 128)
				incise.at<uchar>(i, j) = img.at<uchar>(i, j);
	return std::move(incise);
}

double AccessBladder(const Mat& img, const Mat& label)
{
	std::vector<int> grayLevel(256);
	for (size_t i = 0; i < label.rows; i++)
		for (size_t j = 0; j < label.cols; j++)
			if (label.at<uchar>(i, j) == 128)
				grayLevel[(int)img.at<uchar>(i, j)]++;

	return Kurtosis(grayLevel);
}
double AccessBladder(const Mat& img)
{
	std::vector<int> grayLevel(256);
	for (size_t i = 0; i < img.rows; i++)
		for (size_t j = 0; j < img.cols; j++)
			grayLevel[(int)img.at<uchar>(i, j)]++;

	return Kurtosis(grayLevel);
}

Mat HistogramOfIncise(const Mat& img, const Mat& label)
{
	std::vector<int> grayLevel(256);
	for (size_t i = 0; i < label.rows; i++)
		for (size_t j = 0; j < label.cols; j++)
			if (label.at<uchar>(i, j) == 128)
				grayLevel[(int)img.at<uchar>(i, j)]++;

	Mat histogram = Mat::zeros(std::max_element(grayLevel.begin(),grayLevel.end())[0] + 1, grayLevel.size(), CV_8UC3);
	
	int lastX = -1, lastY = -1;
	for (size_t i = 0; i < grayLevel.size(); i++)
	{
		line(histogram, Point(i, histogram.rows), Point(i, histogram.rows - 1 - grayLevel[i]), Scalar(0, 0, 255));
		if (grayLevel[i] > 0)
		{
			if (lastY > 0)
				line(histogram,
					Point(lastX, histogram.rows - 1 - lastY),
					Point(i, histogram.rows - 1 - grayLevel[i]),
					Scalar(255, 0, 0));
			lastX = i, lastY = grayLevel[i];
		}
	}

	return std::move(histogram);
}

MyRECT GetRect(const Mat& img, int enlarge = 0)
{
	Point leftTop = { img.cols,img.rows };
	Point rightBottom = { 0,0 };
	for (size_t i = 0; i < img.rows; i++) {
		for (size_t j = 0; j < img.cols; j++) {
			if (img.at<uchar>(i, j) != 0) {
				if (i < leftTop.y) leftTop.y = i;
				if (i > rightBottom.y) rightBottom.y = i;
				if (j < leftTop.x) leftTop.x = j;
				if (j > rightBottom.x) rightBottom.x = j;
			}
		}
	}
	leftTop.x -= enlarge;
	leftTop.y -= enlarge;
	rightBottom.x += enlarge;
	rightBottom.y += enlarge;
	return { leftTop,rightBottom };
}

void PutText(Mat& img, const Str& txt)
{
	int width = 40 * txt.size();

	Point root_points[1][4];
	root_points[0][0] = Point(10, 0);
	root_points[0][1] = Point(10 + width, 0);
	root_points[0][2] = Point(10 + width, 60);
	root_points[0][3] = Point(10, 60);

	const Point* ppt[1] = { root_points[0] };
	int npt[] = { 4 };
	fillPoly(img, ppt, npt, 1, Scalar(200, 200, 200), 8, 0);

	putText(img, txt, Point(10, 50), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar(0, 0, 0));
}

/***************BladderHistogram******************/
ADD_SUMMARY(AccessImage, "����ֱ��ͼ")
DISCSOLVE_CLASS_DEFINE(AccessImage);
DISCSOLVE_CREATE_FUNC(AccessImage)
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
		paraNameVec.push_back("���Ŀ¼");		paraDefaultVec.push_back(".\\Access");
	}
	return new DISCSOLVE_CLASSNAME(AccessImage)(paraNameVec, paraDefaultVec);
}
DISCSOLVE_SOLVE_FUNC(AccessImage)
{
	/*
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
	*/

	Str inImgDir	= Param<Str>(0);
	Str inLabelDir	= Param<Str>(1);
	Str outDir		= Param<Str>(2);

	StrVec img_files = GetFileList(inImgDir);
	StrVec label_files = GetFileList(inLabelDir);
	Create_Directory(outDir);
	//IndiscSolve* indiscSolve = IndiscSolve::Create("GrayLayer");

	ParallelWithCounter(0, img_files.size(), [&](int i)
		//for (size_t i = 0; i < img_files.size(); i++)
		{
			Mat img = imread(inImgDir + "\\" + img_files[i], IMREAD_GRAYSCALE);
			Mat label = imread(inLabelDir + "\\" + label_files[i], IMREAD_GRAYSCALE);

			//Mat out;

			////��ȡ�Զ����
			//Mat element = getStructuringElement(MORPH_RECT, Size(15, 15)); //��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�

			////�߼���̬ѧ����������������Ϳ����ˣ�����Ҫѡ�����ֲ��������޸ĵ����������Ϳ����ˡ�������ʾ������̬ѧ�ݶȴ���
			//morphologyEx(img, out, MORPH_BLACKHAT, element);

			//out = indiscSolve->ImageSolve(img);

			//float access_img = AccessBladder(RectIncise(img,GetRect(label)));
			Mat histogram = HistogramOfIncise(img, label);
			Mat incise = RectIncise(GetIncise(img, label), GetRect(label, 5));

			float access_img = AccessBladder(img, label);
			{
				char txt[10];
				sprintf_s(txt, "%.2f", access_img);
				PutText(img, txt);
			}

			Mat mask;
			cvtColor(histogram, mask, CV_RGB2GRAY);
			Mat out;
			cvtColor(img, out, CV_GRAY2RGB);
			Mat rgbIncise;
			cvtColor(incise, rgbIncise, CV_GRAY2RGB);


			histogram.copyTo(out(Rect(0, out.rows - histogram.rows - 1, histogram.cols, histogram.rows)), mask);
			rgbIncise.copyTo(out(Rect(out.cols - incise.cols - 1, out.rows - incise.rows - 1, incise.cols, incise.rows)),
				Mat(incise.rows, incise.cols, CV_8UC1, Scalar(255)));
			imwrite(outDir + "\\" + img_files[i], out);

			//imshow(img_files[i], img);
			//waitKey(0);
			//destroyAllWindows();
		}, name);
	//delete indiscSolve;
}
