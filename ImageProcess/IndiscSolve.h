/*
	By.Wheat
	2019.06.07

	文件：IndiscSolve.h
	简述：无差异化处理算法工厂。
*/
#pragma once
#include "Solve.hpp"

class IndiscSolve: public Solve
{
public:
	virtual cv::Mat ImageSolve(const cv::Mat& src) = 0;

	static IndiscSolve* Create(const Str& solve_name);

	static bool IndiscFactoryRegist(const Str& name, IndiscSolve* (*create_fun)());	//该注册函数线程安全

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


/*=====================================================处理算法定义示例===================================================*/

#define INDISCSOLVE_EXAMPLE
#ifndef INDISCSOLVE_EXAMPLE
#include "IndiscSolve.h"
using namespace cv;
/***************Solve_name******************/
ADD_SUMMARY(Solve_name, "描述...")
INDISCSOLVE_CLASS_DEFINE(Solve_name);
INDISCSOLVE_CREATE_FUNC(Solve_name)
{
	/*
		@retval	IndiscSolve*: 返回新实例指针
		____实例构造函数ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: 处理算法附加参数名（对于每个参数都是必需的，该容器大小决定附加参数数量）
		____@param [const StrVec&] para_default_vec: 处理算法附加参数缺省值（对于每个参数都是非必需的）
	*/
	return new INDISCSOLVE_CLASSNAME(Solve_name)(StrVec(), StrVec());
}
INDISCSOLVE_SOLVE_FUNC(Solve_name)
{
	/*
		@param [const Mat&] src: 输入图像引用
		@retval	Mat: 返回处理结果
		@extra [StrVec] extraParaVec: 算法附加参数，使用Param<Ty>(int)模板获取。
	*/
	Mat dst;
	return std::move(dst);
}
/******************************************/
#endif