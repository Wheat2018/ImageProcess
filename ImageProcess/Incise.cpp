/*
	By.Wheat
	2019.06.07

	�ļ���Incise.cpp
	���������׼�����ͼ��ֲ��и��㷨��
*/
#include "DiscSolve.h"
using namespace cv;

struct MyRECT {
	Point leftTop, rightBottom;

	int GetWidth() { return rightBottom.x - leftTop.x; }
	int GetHeight() { return rightBottom.y - leftTop.y; }
};

Mat RectIncise(const Mat& src, MyRECT rect)
{
	CV_Assert(src.depth() == CV_8UC3 || src.depth() == CV_8UC1);
	Mat result = Mat::zeros(rect.GetHeight(), rect.GetWidth(), src.type());
	int top = max(rect.leftTop.y, 0);
	int bottom = min(rect.rightBottom.y, src.rows);
	int left = max(rect.leftTop.x, 0);
	int right = min(rect.rightBottom.x, src.cols);
	switch (src.depth())
	{
	case CV_8UC3:
		for (int i = top; i < bottom; i++)
			for (int j = left; j < right; j++)
				result.at<Vec3b>(i - rect.leftTop.y, j - rect.leftTop.x) = src.at<Vec3b>(i, j);
		break;
	case CV_8UC1:
		for (int i = top; i < bottom; i++)
			for (int j = left; j < right; j++)
				result.at<uchar>(i - rect.leftTop.y, j - rect.leftTop.x) = src.at<uchar>(i, j);
		break;
	default:
		break;
	}
	return std::move(result);
}

/***************Incise******************/
ADD_SUMMARY(Incise, "���׼�����ͼ��ֲ��и��㷨")
DISCSOLVE_CLASS_DEFINE(Incise);
DISCSOLVE_CREATE_FUNC(Incise)
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
		paraNameVec.push_back("�����α߽�");								paraDefaultVec.push_back("1");
		paraNameVec.push_back("�߽�����ֵ");								paraDefaultVec.push_back("(5, 5)");
		paraNameVec.push_back("����Ӧ�߽�0/�̶��߽�1/���߽�2");			paraDefaultVec.push_back("2");
		paraNameVec.push_back("�̶��߽糤��(�̶��߽�ʱ��Ч)");				paraDefaultVec.push_back("(10, 20)");
	}
	return new DISCSOLVE_CLASSNAME(Incise)(paraNameVec, paraDefaultVec);
}
DISCSOLVE_SOLVE_FUNC(Incise)
{
	/*
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
	*/

	Str inImgDir	= Param<Str>(0);
	Str inLabelDir	= Param<Str>(1);
	Str outImgDir	= Param<Str>(2);
	Str outLabelDir = Param<Str>(3);
	bool square		= Param<bool>(4);
	IntVec enlarge	= Param<IntVec>(5);
	int mode		= Param<int>(6);
	IntVec bound	= Param<IntVec>(7);

	StrVec img_files = GetFileList(inImgDir);
	StrVec label_files = GetFileList(inLabelDir);

	if (IF_ERROR(Create_Directory(outImgDir)))	PAUSE_EXIT;
	if (IF_ERROR(Create_Directory(outLabelDir)))	PAUSE_EXIT;

	std::vector<MyRECT> rects(img_files.size());

	ParallelWithCounter(0, img_files.size(), [&](int i)
		{
			Mat label = imread(inLabelDir + "\\" + label_files[i], IMREAD_GRAYSCALE);

			Point leftTop = { label.cols,label.rows };
			Point rightBottom = { 0,0 };
			for (size_t i = 0; i < label.rows; i++) {
				for (size_t j = 0; j < label.cols; j++) {
					if (label.at<uchar>(i, j) != 0) {
						if (i < leftTop.y) leftTop.y = i;
						if (i > rightBottom.y) rightBottom.y = i;
						if (j < leftTop.x) leftTop.x = j;
						if (j > rightBottom.x) rightBottom.x = j;
					}
				}
			}
			if (leftTop.x < rightBottom.x && leftTop.y < rightBottom.y) {
				//bound
				int rangeX, rangeY;
				switch (mode)
				{
				default:
				case 2://���߽�
				case 0://����Ӧ�߽�
					rangeX = rightBottom.x - leftTop.x;
					rangeY = rightBottom.y - leftTop.y;
					if (square) rangeX = rangeY = max(rangeX, rangeY);
					break;
				case 1://�̶��߽�
					rangeX = bound[0], rangeY = bound[1];
					break;
				}
				rangeX += 2 * enlarge[0];
				rangeY += 2 * enlarge[1];

				//apply
				leftTop.x -= (rangeX - (rightBottom.x - leftTop.x)) / 2;
				rightBottom.x = leftTop.x + rangeX;
				leftTop.y -= (rangeY - (rightBottom.y - leftTop.y)) / 2;
				rightBottom.y = leftTop.y + rangeY;

				if (mode != 1)
				{
					//shrink
					int shrinkX = 0, shrinkY = 0;
					shrinkX = max(shrinkX, -leftTop.x);
					shrinkX = max(shrinkX, rightBottom.x - (label.cols - 1));
					shrinkY = max(shrinkY, -leftTop.y);
					shrinkY = max(shrinkY, rightBottom.y - (label.rows - 1));
					if (square) shrinkX = shrinkY = max(shrinkX, shrinkY);

					//apply
					leftTop.x += shrinkX;
					leftTop.y += shrinkY;
					rightBottom.x -= shrinkX;
					rightBottom.y -= shrinkY;
				}
			}
			rects[i] = { leftTop,rightBottom };
		},"�������߽�");

	int largestRangeX = 0;
	int largestRangeY = 0;

	for (size_t i = 0; i < (int)img_files.size(); ++i) {
		if (rects[i].GetWidth() > largestRangeX) largestRangeX = rects[i].GetWidth();
		if (rects[i].GetHeight() > largestRangeY) largestRangeY = rects[i].GetHeight();
	}

	ParallelWithCounter(0, img_files.size(), [&](int i)
		{
			Mat img = imread(inImgDir + "\\" + img_files[i], IMREAD_GRAYSCALE);
			Mat label = imread(inLabelDir + "\\" + label_files[i], IMREAD_GRAYSCALE);

			Point& leftTop = rects[i].leftTop;
			Point& rightBottom = rects[i].rightBottom;
			if (leftTop.x < rightBottom.x && leftTop.y < rightBottom.y) {
				//squre
				if (mode == 2)
				{
					leftTop.x -= (largestRangeX - (rightBottom.x - leftTop.x)) / 2;
					rightBottom.x = leftTop.x + largestRangeX;
					leftTop.y -= (largestRangeY - (rightBottom.y - leftTop.y)) / 2;
					rightBottom.y = leftTop.y + largestRangeY;
				}
				//Mat incise(img, Rect(leftTop.x, leftTop.y, rightBottom.x - leftTop.x, rightBottom.y - leftTop.y));
				//Mat inciseLabel(label, Rect(leftTop.x, leftTop.y, rightBottom.x - leftTop.x, rightBottom.y - leftTop.y));
				Mat incise = RectIncise(img,rects[i]);
				Mat inciseLabel = RectIncise(label, rects[i]);
				imwrite(outImgDir + "\\" + img_files[i], incise);
				imwrite(outLabelDir + "\\" + label_files[i], inciseLabel);
			}
			else {
				COUT << "\"" << img_files[i] << "\"-\"" << label_files[i] << "\"" << "����߽�!" << ENDL;
			}
		}, "��ȡ���򲢱���");
}
