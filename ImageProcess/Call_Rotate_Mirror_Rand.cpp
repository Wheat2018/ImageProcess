/*
	By.Wheat
	2019.06.07

	文件：Call_Rotate_Mirror_Rand.h
	简述：膀胱及掩码图像旋转景象+随机分折调用。
*/
#include "DiscSolve.h"
using namespace cv;
/***************RandRotateMirror******************/
ADD_SUMMARY(Call_Rotate_Mirror_Rand, "膀胱及掩码图像旋转镜像+随机分折调用")
DISCSOLVE_CLASS_DEFINE(Call_Rotate_Mirror_Rand);
DISCSOLVE_CREATE_FUNC(Call_Rotate_Mirror_Rand)
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
		paraNameVec.push_back("输出目录");		paraDefaultVec.push_back(".\\RandRotMirSet");
		paraNameVec.push_back("交叉验证集");	paraDefaultVec.push_back("Cross");
		paraNameVec.push_back("测试集");		paraDefaultVec.push_back("Test");
		paraNameVec.push_back("训练集");		paraDefaultVec.push_back("Train");
	}
	{
		paraNameVec.push_back("旋转角度方差");			paraDefaultVec.push_back("10");
		paraNameVec.push_back("折数");			paraDefaultVec.push_back("10");
	}
	return new DISCSOLVE_CLASSNAME(Call_Rotate_Mirror_Rand)(paraNameVec, paraDefaultVec);
}
DISCSOLVE_SOLVE_FUNC(Call_Rotate_Mirror_Rand)
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
