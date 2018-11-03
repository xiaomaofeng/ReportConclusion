#pragma once

#include<string>
#include<vector>


using namespace std;

class ReportConclusion
{
public:
	ReportConclusion();
	~ReportConclusion();
	
	void Init();
	void RunTask();
	void GetReportCSVs(const string srcPath, vector<string>& srcCSVs, string exeInfo);
	void OutputConclusion(vector<string>&srcCSVs, string ExecInformation);
	void GetImageSource();
	string GetExecInformation(string exeInfo);
	string GetSuffix(string Name);
public:
	vector<string>filePath;
	vector<string>exeInfo;//执行的exe的信息
	bool bInitNormal;

	//static string Suffix;
};