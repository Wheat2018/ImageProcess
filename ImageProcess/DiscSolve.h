/*
	By.Wheat
	2019.06.07

	文件：DiscSolve.h
	简述：差异化处理算法工厂。
*/
#pragma once
#include "Solve.hpp"

class DiscSolve : public Solve
{
public:
	virtual void ImageSolve() = 0;

	static DiscSolve* Create(const Str& solve_name);

	static bool DiscFactoryRegist(const Str& name, DiscSolve* (*create_fun)());	//该注册函数线程安全

protected:
	DiscSolve(const Str& name, const StrVec para_name_vec, const StrVec para_default_vec) :
		Solve(name, para_name_vec, para_default_vec) 
	{ ; }

};

extern std::map<Str, DiscSolve* (*)(), NameLess> *discFactory;


#define DISCSOLVE_CLASSNAME(solve_name) Solve_##solve_name
#define DISCSOLVE_CLASS_DEFINE(solve_name)													   							\
class Solve_##solve_name :public DiscSolve													   							\
{																								   						\
private:																						   						\
	Solve_##solve_name(const StrVec paraName, const StrVec paraDefault) :	   											\
		DiscSolve(name, paraName, paraDefault) { ; }											   						\
public:																							   						\
	constexpr static char name[] = #solve_name;													   						\
	virtual void ImageSolve();														   									\
	static DiscSolve* Create();																   							\
};																								   						\
bool regist##Solve_##solve_name = DiscSolve::DiscFactoryRegist(Solve_##solve_name::name, Solve_##solve_name::Create);

#define DISCSOLVE_CREATE_FUNC(solve_name) inline DiscSolve* Solve_##solve_name::Create()
#define DISCSOLVE_SOLVE_FUNC(solve_name) inline void Solve_##solve_name::ImageSolve()



/*=====================================================处理算法定义示例===================================================*/

#define DISCSOLVE_EXAMPLE
#ifndef DISCSOLVE_EXAMPLE
#include "DiscSolve.h"
using namespace cv;
/***************Solve_name******************/
ADD_SUMMARY(Solve_name, "描述...")
DISCSOLVE_CLASS_DEFINE(Solve_name);
DISCSOLVE_CREATE_FUNC(Solve_name)
{
	/*
		@retval	DiscSolve*: 返回新实例指针
		____实例构造函数ClassName(para_name_vec, para_default_vec)
		____@param [const StrVec&] para_name_vec: 处理算法附加参数名（对于每个参数都是必需的，该容器大小决定附加参数数量）
		____@param [const StrVec&] para_default_vec: 处理算法附加参数缺省值（对于每个参数都是非必需的）
	*/
	return new DISCSOLVE_CLASSNAME(HorizonMirror)(StrVec(), StrVec());
}
DISCSOLVE_SOLVE_FUNC(Solve_name)
{
	/*
		@extra [StrVec] extraParaVec: 算法附加参数，使用Param<Ty>(int)模板获取。
	*/
}
/******************************************/
#endif
