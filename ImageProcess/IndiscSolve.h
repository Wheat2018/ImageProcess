/*
	By.Wheat
	2019.06.07

	�ļ���IndiscSolve.h
	�������޲��컯�����㷨������
*/
#pragma once
#include "Solve.hpp"

class IndiscSolve: public Solve
{
public:
	virtual cv::Mat ImageSolve(const cv::Mat& src) = 0;

	static IndiscSolve* Create(const Str& solve_name);

	static bool IndiscFactoryRegist(const Str& name, IndiscSolve* (*create_fun)());	//��ע�ắ���̰߳�ȫ

protected:
	IndiscSolve(const Str& name, const StrVec para_name_vec, const StrVec para_default_vec) :
		Solve(name, para_name_vec, para_default_vec) 
	{ ; }

};

extern std::map<Str, IndiscSolve* (*)(), NameLess> *indiscFactory;


#define INDISCSOLVE_CLASSNAME(solve_name) Solve_##solve_name
#define INDISCSOLVE_CLASS_DEFINE(solve_name)													   							\
class Solve_##solve_name :public IndiscSolve													   							\
{																								   							\
private:																						   							\
	Solve_##solve_name(const StrVec paraName, const StrVec paraDefault) :	   												\
		IndiscSolve(name, paraName, paraDefault) { ; }											   							\
public:																							   							\
	constexpr static char name[] = #solve_name;													   							\
	virtual cv::Mat ImageSolve(const cv::Mat& src);														   					\
	static IndiscSolve* Create();																   							\
};																								   							\
bool regist##Solve_##solve_name = IndiscSolve::IndiscFactoryRegist(Solve_##solve_name::name, Solve_##solve_name::Create);

#define INDISCSOLVE_CREATE_FUNC(solve_name) inline IndiscSolve* Solve_##solve_name::Create()
#define INDISCSOLVE_SOLVE_FUNC(solve_name) inline cv::Mat Solve_##solve_name::ImageSolve(const cv::Mat& src)


/*=====================================================�����㷨����ʾ��===================================================*/

#define INDISCSOLVE_EXAMPLE
#ifndef INDISCSOLVE_EXAMPLE
#include "IndiscSolve.h"
using namespace cv;
/***************Solve_name******************/
ADD_SUMMARY(Solve_name, "����...")
INDISCSOLVE_CLASS_DEFINE(Solve_name);
INDISCSOLVE_CREATE_FUNC(Solve_name)
{
	/*
		@retval	IndiscSolve*: ������ʵ��ָ��
		____ʵ�����캯��ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: �����㷨���Ӳ�����������ÿ���������Ǳ���ģ���������С�������Ӳ���������
		____@param [const StrVec&] para_default_vec: �����㷨���Ӳ���ȱʡֵ������ÿ���������ǷǱ���ģ�
	*/
	return new INDISCSOLVE_CLASSNAME(Solve_name)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(Solve_name)
{
	/*
		@param [const Mat&] src: ����ͼ������
		@retval	Mat: ���ش�����
		@extra [StrVec] extraParaVec: �㷨���Ӳ�����ʹ��Param<Ty>(int)ģ���ȡ��
	*/
	Mat dst;
	return std::move(dst);
}
/******************************************/
#endif