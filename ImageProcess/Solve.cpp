#include "Solve.hpp"
#include <mutex>

std::map<Str, Str, NameLess>* summary;

void Solve::Report(const Str& retract)
{
	ParameterManager manager(0, nullptr);
	manager.PushBatch(paraNameVec, paraDefaultVec, extraParaVec);
	manager.Report(retract);
}

bool Solve::AddSummary(const Str& name, const Str& sum)
{
	static std::mutex mt_summary;
	if (summary == nullptr)
	{//双重锁定提高效率
		mt_summary.lock();
		if (summary == nullptr) summary = new std::map<Str, Str, NameLess>();
		mt_summary.unlock();
	}

	mt_summary.lock();
	(*summary)[name] = sum;
	mt_summary.unlock();
	return true;
}

Str Solve::Summary(const Str& name)
{
	auto iter = summary->find(name);
	if (iter != summary->end())
		return iter->second;
	return Str();
}

void SingleThreadPutMsg(StrVec vec, const Str& str)
{
	static std::mutex mt_SingleThreadPutMsg;
	mt_SingleThreadPutMsg.lock();
	vec.push_back(str);
	mt_SingleThreadPutMsg.unlock();
}
