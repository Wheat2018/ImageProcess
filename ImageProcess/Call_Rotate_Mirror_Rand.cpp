/*
	By.Wheat
	2019.06.07

	�ļ���Call_Rotate_Mirror_Rand.h
	���������׼�����ͼ����ת����+������۵��á�
*/
#include "DiscSolve.h"
using namespace cv;
/***************RandRotateMirror******************/
ADD_SUMMARY(Call_Rotate_Mirror_Rand, "���׼�����ͼ����ת����+������۵���")
DISCSOLVE_CLASS_DEFINE(Call_Rotate_Mirror_Rand);
DISCSOLVE_CREATE_FUNC(Call_Rotate_Mirror_Rand)
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
		paraNameVec.push_back("���Ŀ¼");		paraDefaultVec.push_back(".\\RandRotMirSet");
		paraNameVec.push_back("������֤��");	paraDefaultVec.push_back("Cross");
		paraNameVec.push_back("���Լ�");		paraDefaultVec.push_back("Test");
		paraNameVec.push_back("ѵ����");		paraDefaultVec.push_back("Train");
	}
	{
		paraNameVec.push_back("��ת�Ƕȷ���");			paraDefaultVec.push_back("10");
		paraNameVec.push_back("����");			paraDefaultVec.push_back("10");
	}
	return new DISCSOLVE_CLASSNAME(Call_Rotate_Mirror_Rand)(paraNameVec, paraDefaultVec);
}
DISCSOLVE_SOLVE_FUNC(Call_Rotate_Mirror_Rand)
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
	Str variance = Param<Str>(6);
	Str times	 = Param<Str>(7);

#define TEMP_IMG_DIR ".\\_____Call_Rotate_Mirror_Rand_TmpImg"
#define TEMP_LABEL_DIR ".\\_____Call_Rotate_Mirror_Rand_TmpLabel"
	int main(int argc, void* argv[]);


	//RandRotate
	{
		void* argv[] = 
		{
			(void*)nullptr,
			(void*)"RandRotate" ,
			(void*)"0",
			(void*)imgDir.c_str(),
			(void*)labelDir.c_str(),
			(void*)TEMP_IMG_DIR,
			(void*)TEMP_LABEL_DIR,
			(void*)variance.c_str()
		};
		main(sizeof(argv) / sizeof(void*), argv);
	}

	//HorizonMirror
	{
		void* argv[] =
		{
			(void*)nullptr,
			(void*)"HorizonMirror" ,
			(void*)"0",
			(void*)"2",
			(void*)"2",
			(void*)TEMP_IMG_DIR,
			(void*)TEMP_LABEL_DIR,
			(void*)TEMP_IMG_DIR,
			(void*)TEMP_LABEL_DIR
		};
		main(sizeof(argv) / sizeof(void*), argv);
	}

	//Rand
	{
		void* argv[] =
		{
			(void*)nullptr,
			(void*)"Rand" ,
			(void*)"0",
			(void*)TEMP_IMG_DIR,
			(void*)TEMP_LABEL_DIR,
			(void*)(void*)saveDir.c_str(),
			(void*)(void*)cross.c_str(),
			(void*)(void*)test.c_str(),
			(void*)(void*)train.c_str(),
			(void*)(void*)times.c_str()
		};
		main(sizeof(argv) / sizeof(void*), argv);
	}

	Remove_Directory(TEMP_IMG_DIR);
	Remove_Directory(TEMP_LABEL_DIR);
}
