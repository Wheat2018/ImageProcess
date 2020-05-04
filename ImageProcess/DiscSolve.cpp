/*
	By.Wheat
	2019.06.07

	文件：DiscSolve.cpp
	简述：差异化处理算法工厂。
*/
#include "DiscSolve.h"
#include <mutex>
std::map<Str, DiscSolve* (*)(), NameLess> *discFactory;

bool DiscSolve::DiscFactoryRegist(const Str& name, DiscSolve* (*create_fun)())
{
	static std::mutex mt_discFactory;
	if (discFactory == nullptr)
	{//双重锁定提高效率
		mt_discFactory.lock();
		if (discFactory == nullptr) discFactory = new std::map<Str, DiscSolve* (*)(), NameLess>();
		mt_discFactory.unlock();
	}

	mt_discFactory.lock();
	(*discFactory)[name] = create_fun;
	mt_discFactory.unlock();
	return true;
}

DiscSolve* DiscSolve::Create(const Str& solve_name) 
{
	auto iter = discFactory->find(solve_name);
	if (iter != discFactory->end())
		return iter->second();
	return nullptr;
}
