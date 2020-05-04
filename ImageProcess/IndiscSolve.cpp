/*
	By.Wheat
	2019.06.07

	文件：IndiscSolve.cpp
	简述：无差异化处理算法工厂。
*/
#include "IndiscSolve.h"
#include <mutex>
std::map<Str, IndiscSolve* (*)(), NameLess> *indiscFactory;

bool IndiscSolve::IndiscFactoryRegist(const Str& name, IndiscSolve* (*create_fun)())
{
	static std::mutex mt_indiscFactory;
	if (indiscFactory == nullptr)
	{//双重锁定提高效率
		mt_indiscFactory.lock();
		if (indiscFactory == nullptr) indiscFactory = new std::map<Str, IndiscSolve* (*)(), NameLess>();
		mt_indiscFactory.unlock();
	}

	mt_indiscFactory.lock();
	(*indiscFactory)[name] = create_fun;
	mt_indiscFactory.unlock();
	return true;
}

IndiscSolve* IndiscSolve::Create(const Str& solve_name)
{
	auto iter = indiscFactory->find(solve_name);
	if (iter != indiscFactory->end())
		return iter->second();
	return nullptr;
}