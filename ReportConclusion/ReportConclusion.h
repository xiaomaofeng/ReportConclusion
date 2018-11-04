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
	string SetOutputFilePath();
	string GetImageSource(vector<vector<string>>ReportData);
	string GetExecInformation(string exeInfo);
	string GetSuffix(string Name);
public:
	vector<string>filePath;
	vector<string>OutputFilePath;
	vector<vector<string>>concluCSV;//be used to save information of output
	vector<string>exeInfo;//the informatin of the exec_6.4.1
	vector<string>exeInfoLastVersion;//the information of the exec_6.4.0
	string ImageSource;
	string outputPath;
	bool bInitNormal;

};