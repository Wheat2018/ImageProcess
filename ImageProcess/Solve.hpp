#pragma once
#include <ParameterManager.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
//#include <map>

struct NameLess
{
	bool operator()(const Str& left, const Str& right)const { return _stricmp(left.c_str(), right.c_str()) < 0; }
};

class Solve
{
public:

	//�����㷨���踽�Ӳ���ǩ�����㷨�๹��ʱȷ����
	const Str name;
	const int paraNum;
	const StrVec paraNameVec;
	const StrVec paraDefaultVec;

	//�����㷨���踽�Ӳ������轫�ó�Աͨ��ParameterManager::PushBatch�󶨵�ParameterManager�С�
	StrVec extraParaVec;

	void Report(const Str& retract = "");

	static bool AddSummary(const Str& name, const Str& sum);			//�ú����̰߳�ȫ


	static Str Summary(const Str& name);


protected:
	Solve(const Str& name, const StrVec para_name_vec, const StrVec para_default_vec) :
		name(name), paraNum(para_name_vec.size()), paraNameVec(para_name_vec), paraDefaultVec(para_default_vec), extraParaVec(paraNum) 
	{ ; }

	template <typename Ty>
	Ty Param(size_t idx)
	{
		Ty result;
		if(idx < paraNum) ParameterManager::Convert(extraParaVec[idx], result);
		else COUT << "Solve::Param Warning: ��������!" << ENDL;
		return std::move(result);
	}
};

extern std::map<Str, Str, NameLess>* summary;

#define ADD_SUMMARY(solve_name, sum) \
bool summary##Solve_##solve_name = Solve::AddSummary(#solve_name, sum);
