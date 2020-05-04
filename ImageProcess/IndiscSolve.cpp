/*
	By.Wheat
	2019.06.07

	�ļ���IndiscSolve.cpp
	�������޲��컯�����㷨������
*/
#include "IndiscSolve.h"
#include <mutex>
std::map<Str, IndiscSolve* (*)(), NameLess> *indiscFactory;

bool IndiscSolve::IndiscFactoryRegist(const Str& name, IndiscSolve* (*create_fun)())
{
	static std::mutex mt_indiscFactory;
	if (indiscFactory == nullptr)
	{//˫���������Ч��
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