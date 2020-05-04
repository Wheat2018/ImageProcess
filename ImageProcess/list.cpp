/*
	By.Wheat
	2019.06.07

	�ļ���list.cpp
	�������г������㷨��
*/
#include "DiscSolve.h"
#include "IndiscSolve.h"
using namespace cv;

/***************list******************/
ADD_SUMMARY(list, "�г������㷨")
DISCSOLVE_CLASS_DEFINE(list);
DISCSOLVE_CREATE_FUNC(list)
{
	/*
		@retval	DiscSolve*: ������ʵ��ָ��
		____ʵ�����캯��ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: �����㷨���Ӳ�����������ÿ���������Ǳ���ģ���������С�������Ӳ���������
		____@param [const StrVec&] para_default_vec: �����㷨���Ӳ���ȱʡֵ������ÿ���������ǷǱ���ģ�
	*/
	return new DISCSOLVE_CLASSNAME(list)(StrVec(), StrVec());
}
DISCSOLVE_SOLVE_FUNC(list)
{
	/*
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
	*/
	COUT << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>�����в�����ʽ<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << ENDL;
	COUT << ENDL;
	COUT << "    >>ImageProcess.exe ������1�� ������2�� ..." << ENDL;
	COUT << ENDL;
	COUT << "   ---------------------------DiscSolve���㷨-------------------------   " << ENDL;
	COUT << ENDL;
	COUT << "   ������1��   [�����㷨����]          ��Rand��Incise��" << ENDL;
	COUT << "   ������2��   [��������ͣ]            0(����ͣ)��1(��ͣ),Ĭ��Ϊ1" << ENDL;
	COUT << "   ������..��  [�㷨���Ӳ�������...]   �㷨�����н���" << ENDL;
	COUT << ENDL;
	COUT << "   --------------------------IndiscSolve���㷨------------------------   " << ENDL;
	COUT << ENDL;
	COUT << "   ������1��   [�����㷨����]          ��HorizonMirror��" << ENDL;
	COUT << "   ������2��   [��������ͣ]            0(����ͣ)��1(��ͣ),Ĭ��Ϊ1" << ENDL;
	COUT << "   ������3��   [����Ŀ¼��]            ������Ӱ������Ŀ¼��������" << ENDL;
	COUT << "   ������4��   [���Ŀ¼��]            ������Ӱ�����Ŀ¼��������" << ENDL;
	COUT << "   ������..��  [����Ŀ¼]              ���������3ƥ��" << ENDL;
	COUT << "   ������..��  [���Ŀ¼]              ���������4ƥ��" << ENDL;
	COUT << "   ������..��  [�㷨���Ӳ�������...]   �㷨�����н���" << ENDL;
	COUT << ENDL;
	COUT << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>�㷨����<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << ENDL;
	COUT << ENDL;
	COUT << "   ---------------------------DiscSolve���㷨-------------------------   " << ENDL;
	for (auto iter = discFactory->begin(); iter != discFactory->end(); iter++)
	{
		COUT << "   �㷨���ƣ�" << iter->first << ENDL;
		COUT << "   �㷨��飺" << Solve::Summary(iter->first) << ENDL;
		COUT << "   �㷨������" << ENDL;
		auto tmp = DiscSolve::Create(iter->first);
		tmp->Report("             ");
		COUT << ENDL;
		delete tmp;
	}
	COUT << "   --------------------------IndiscSolve���㷨------------------------   " << ENDL;
	for (auto iter = indiscFactory->begin(); iter != indiscFactory->end(); iter++)
	{
		COUT << "   �㷨���ƣ�" << iter->first << ENDL;
		COUT << "   �㷨��飺" << Solve::Summary(iter->first) << ENDL;
		COUT << "   �㷨������" << ENDL;
		auto tmp = IndiscSolve::Create(iter->first);
		tmp->Report("             ");
		COUT << ENDL;
		delete tmp;
	}
	COUT << "   -------------------------------------------------------------------   " << ENDL;
}
/******************************************/
