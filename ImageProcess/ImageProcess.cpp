#include "DiscSolve.h"
#include "IndiscSolve.h"
using namespace cv;

//#pragma comment(lib,"opencv_world341d.lib")
#pragma comment(lib,"opencv_world341.lib")


int main(int argc, void* argv[])
{

	Str solveName;
	bool pause;
	DiscSolve* discSolve = nullptr;
	IndiscSolve* indiscSolve = nullptr;

	//�ǲ��컯�㷨���
	int inNum;
	int outNum;
	StrVec inDirs;
	StrVec outDirs;

	ParameterManager manager(argc, argv);
	{
		//����
		{
			manager.Push(PARAMETER_ELEMENT_DEFAULT);
			manager.Push("�����㷨", "list", solveName);
			manager.Push("��������ͣ", "0", pause);
		}

		//���
		{
			//���µ�ǰ���в���
			if (IF_ERROR(manager.Catch())) PAUSE_EXIT;

			discSolve = DiscSolve::Create(solveName);
			indiscSolve = IndiscSolve::Create(solveName);
			
			if (discSolve)
			{
				COUT << "DiscSolve" << ENDL;
				//���컯�㷨���
				manager.PushBatch(discSolve->paraNameVec, discSolve->paraDefaultVec, discSolve->extraParaVec);
			}
			else if (indiscSolve)
			{
				COUT << "IndiscSolve" << ENDL;
				//�ǲ��컯�㷨����
				{
					manager.Push("����Ŀ¼��", "2", inNum);
					manager.Push("���Ŀ¼��", "2", outNum);
				}

				//�ǲ��컯�㷨���
				{
					if (IF_ERROR(manager.Catch())) PAUSE_EXIT;

					inDirs = StrVec(inNum);
					outDirs = StrVec(outNum);

					StrVec inDirDefault;
					StrVec outDirDefault;
					{
						inDirDefault.push_back(".\\Image");
						inDirDefault.push_back(".\\Label");
						outDirDefault.push_back(".\\" + indiscSolve->name + "Image");
						outDirDefault.push_back(".\\" + indiscSolve->name + "Label");
					}

					manager.PushBatch("����Ŀ¼", inDirDefault, inDirs);
					manager.PushBatch("���Ŀ¼", outDirDefault, outDirs);

					manager.PushBatch(indiscSolve->paraNameVec, indiscSolve->paraDefaultVec, indiscSolve->extraParaVec);
				}
			}
			else
			{
				COUT << "�Ҳ����㷨����" << solveName << ENDL;
				PAUSE_EXIT;
			}

		}
		if (IF_ERROR(manager.Run())) PAUSE_EXIT;  //���¼�����в���

		manager.Print();
	}

	if (discSolve)
	{
		discSolve->ImageSolve();
	}
	else if (indiscSolve)
	{
		int SolveFunc(const Str & in_dir, const Str & out_dir, IndiscSolve * solve);

		if (inNum != outNum)
		{
			COUT << "����Ŀ¼�����Ŀ¼��ƥ��!" << ENDL;
			PAUSE_EXIT;
		}
		for (size_t i = 0; i < inNum; i++)
		{
			COUT << ">>����Ŀ¼" << i + 1 << "/" << inNum << " :" << inDirs[i] << ENDL;
			if (IF_ERROR(SolveFunc(inDirs[i], outDirs[i], indiscSolve)))
				PAUSE_EXIT;
		}

	}

	if (discSolve) delete discSolve;
	if (indiscSolve) delete indiscSolve;

	if (pause) system("pause");
}

int SolveFunc(const Str& in_dir, const Str& out_dir, IndiscSolve* indiscSolve)
{
	if (IF_ERROR(Create_Directory(out_dir))) PAUSE_EXIT;

	StrVec in_files = GetFileList(in_dir);

	ParallelWithCounter(0, in_files.size(), [&](int i)
		{
			Mat in = imread(in_dir + "\\" + in_files[i], IMREAD_GRAYSCALE);
			Mat out = indiscSolve->ImageSolve(in);
			imwrite(out_dir + "\\" + in_files[i], out);

		}, indiscSolve->name);

	TRUE_EXIT;
}
