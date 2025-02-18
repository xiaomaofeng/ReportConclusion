//-----RapidJson-------
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "istreamwrapper.h"
using namespace rapidjson;
//------logger--------
#include<logger.h>
using namespace LOGGER;
//---------other------
#include<iostream>
#include<time.h>
#include<windows.h>
#include"ReportConclusion.h"
#include<fstream>
#include<sstream>

int LightLog;					//Lightweight log




CLogger logger(LogLevel_Info, CLogger::GetAppPathA().append("log\\"));


void ReportConclusion::GetReportCSVs(const string srcPath, vector<string>& srcCSVs, string exeInfo)
{
	/*TCHAR szSearchPath[_MAX_PATH] = { 0 };
	wsprintf(szSearchPath, "%s\\*.*", srcPath.c_str());*/
	WIN32_FIND_DATA findData;
	string tempPath = srcPath + "\\*.*";
	size_t pos = 0;
	vector<string>tempString;
	string tempStr;
	//HANDLE curFile = FindFirstFile(szSearchPath.c_str(), &findData);
	HANDLE curFile = FindFirstFile(tempPath.c_str(), &findData);
	//Suffix = GetSuffix(findData.cFileName);
	if (curFile == INVALID_HANDLE_VALUE)
	{
		logger.TraceError("INVALID_HANDLE_VALUE");
		return;
	}
	else
	{
		do
		{
			bool bDirectory = ((findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
			if (bDirectory)
			{
				if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
					continue;
				tempPath = srcPath + "\\" + findData.cFileName;
				GetReportCSVs(tempPath, srcCSVs, exeInfo);
			}
			else
			{
				if (GetSuffix(findData.cFileName) == ".csv")
				{
					string tempFilePath = srcPath + "\\" + findData.cFileName;


					int iret = tempFilePath.find(exeInfo, pos);
					if (iret == string::npos)
					{
						continue;
					}
					else
					{
						bool bifSameFlag = false;
						bool firstInput = srcCSVs.size() == 0;
						if (firstInput)
						{
							srcCSVs.push_back(tempFilePath);
							continue;
						}
						for (int k = 0; k < srcCSVs.size(); k++)
						{
							size_t tempPos;
							tempStr = srcCSVs[k];
							tempPos = tempStr.find_last_of('\\') + 1;
							string tempFileName = tempStr.substr(tempPos);
							tempString.push_back(tempFileName);

						}
						if (!firstInput)
						{
							for (int j = 0; j < srcCSVs.size(); j++)
							{
								if (findData.cFileName == tempString[j])
								{
									bifSameFlag = true;
									break;
								}
	
							}
							if (!bifSameFlag)
							{
								srcCSVs.push_back(tempFilePath);
							}
							else
							{
								continue;
							}
						}						
					}


				}
			}

		} while (FindNextFile(curFile, &findData));
	}



}


string ReportConclusion::GetSuffix(string Name)
{
	size_t Pos = Name.rfind(".", Name.size());
	if (Pos == string::npos)
	{
		string SuffixStr;
		return SuffixStr;
	}
	else
	{
		string SuffixStr(Name.begin() + Pos, Name.end());
		return SuffixStr;
	}
}

string GetCurrentTimeString()
{
	time_t timer;
	struct tm pTm;
	timer = time(NULL);
	localtime_s(&pTm, &timer);
	char szFileName[256] = { 0 };
	sprintf_s(szFileName, "%d%02d%02d%02d%02d%02d", pTm.tm_year + 1900, pTm.tm_mon + 1, pTm.tm_mday, pTm.tm_hour, pTm.tm_min, pTm.tm_sec);
	string strTime = szFileName;
	return strTime;
}

string ReportConclusion::GetExecInformation(string exeInfo)
{
	string ExecInformation;
	size_t pos = exeInfo.find_last_of('_');
	ExecInformation = exeInfo.substr(0, pos);
	return ExecInformation;
}


string ReportConclusion::GetImageSource(vector<vector<string>>ReportData)
{
	string imageSourceString;
	if (ReportData.size() < 15)
	{
		logger.TraceInfo("Can not get image source.");
		return 0;
	}
	else
	{
		imageSourceString=ReportData[15][1];
		size_t temppos = imageSourceString.find_last_of('\\');
		string tempImageSource;
		tempImageSource = imageSourceString.substr(0, temppos);
		size_t pos = tempImageSource.find_last_of('\\');
		ImageSource = tempImageSource.substr(pos+1,string::npos );
		return ImageSource;


	}

	
	return ImageSource;
}


string ReportConclusion::SetOutputFilePath()
{
	if (OutputFilePath.size() == 0)
	{
		logger.TraceInfo("The output file path is the default path(the input file path)");
		outputPath = filePath[0];
		return outputPath;
	}
	else
	{
		for (size_t i=0 ; i<OutputFilePath.size();i++)
		{
			outputPath = OutputFilePath[i];
			return outputPath;
		}
	}
}


void ReportConclusion::OutputConclusion(vector<string>&srcCSVs, string ExecInformation)
{
	
	ofstream conclusionCSV;
	string strTime = GetCurrentTimeString();
	string ReportConclusionName = outputPath + ".\\ReportConclusion" + ExecInformation + "_" + strTime + ".csv";
	conclusionCSV.open(ReportConclusionName, ios::app);
	vector<string>title;
	title.push_back("Sample");
	title.push_back("Image source");
	title.push_back("Total Image Count:");
	title.push_back("Images with Barcodes Recognized:");
	title.push_back("Total Barcode Count:");
	title.push_back("Time Cost of All Images(ms):");
	title.push_back("Recognition Rate:");
	title.push_back("Avg Time of Total Scan Per Image (ms):");
	title.push_back("Avg Time of Total Scan Per Barcode (ms):");
	title.push_back("Time Cost of Images with Barcodes Recognized(ms):");
	title.push_back("Avg Time of Successful Scan Per Image (ms):");
	title.push_back("Resume times:");
	concluCSV.push_back(title);

	for (unsigned int i = 0; i < srcCSVs.size(); i++)
	{
		//ifstream tempFile(srcCSVs[i], ios::in);
		ifstream tempFile(srcCSVs[i], ios::in|ios::binary);
		if (tempFile.fail())
		{
			logger.TraceError("error opening file");
			return;
		}
		vector<string>vcrStrTemp;
		vector<vector<string>>ReportData;//save the information in stringstream
		
		string lineStr;
		while (getline(tempFile, lineStr))
		{
			vector<string>dataArray;//save a picture information in stringsteam
			stringstream DataStringStream(lineStr);
			string strTemp;
			while (getline(DataStringStream, strTemp, ','))
			{
				size_t CarriageReturnPos;
				CarriageReturnPos = strTemp.rfind('\r', string::npos);
				if (CarriageReturnPos)
				{
					strTemp = strTemp.substr(0, strTemp.rfind('\r', string::npos));
				}
				dataArray.push_back(strTemp);
			}
			ReportData.push_back(dataArray);
		}
		tempFile.close();


		GetImageSource(ReportData);
		
		if (i < exeInfo.size())
		{
			vcrStrTemp.push_back("6.4.1");
			vcrStrTemp.push_back(ImageSource);
		}
		else
		{
			vcrStrTemp.push_back("6.4.0");
			vcrStrTemp.push_back(ImageSource);
		}
		if (ReportData[0][0] == "NO.")
		{
			int istartIndex = ReportData.size() - 13;
			if (istartIndex <= 0)
			{
				logger.TraceError("ifstream  filed");
				continue;
			}
			else
			{		
			vcrStrTemp.push_back(ReportData[istartIndex][1]);
			vcrStrTemp.push_back(ReportData[istartIndex + 1][1]);
			vcrStrTemp.push_back(ReportData[istartIndex + 2][1]);
			vcrStrTemp.push_back(ReportData[istartIndex + 3][1]);
			vcrStrTemp.push_back(ReportData[istartIndex + 4][1]);
			vcrStrTemp.push_back(ReportData[istartIndex + 5][1]);
			vcrStrTemp.push_back(ReportData[istartIndex + 6][1]);
			vcrStrTemp.push_back(ReportData[istartIndex + 8][1]);
			vcrStrTemp.push_back(ReportData[istartIndex + 9][1]);
			vcrStrTemp.push_back(ReportData[istartIndex + 11][1]);
			concluCSV.push_back(vcrStrTemp);
			}
		}
		else
		{
			vcrStrTemp.push_back(ReportData[0][1]);
			vcrStrTemp.push_back(ReportData[1][1]);
			vcrStrTemp.push_back(ReportData[2][1]);
			vcrStrTemp.push_back(ReportData[3][1]);
			vcrStrTemp.push_back(ReportData[4][1]);
			vcrStrTemp.push_back(ReportData[5][1]);
			vcrStrTemp.push_back(ReportData[6][1]);
			vcrStrTemp.push_back(ReportData[8][1]);
			vcrStrTemp.push_back(ReportData[9][1]);
			vcrStrTemp.push_back(ReportData[11][1]);

			concluCSV.push_back(vcrStrTemp);
		}

		
			
		
	}

	for (unsigned int j = 0; j < concluCSV.size(); j++)
	{
		for (unsigned int m = 0; m < concluCSV[j].size(); m++)
		{
			if (m != concluCSV[j].size() - 1)
			{
				conclusionCSV << concluCSV[j][m] << ",";
			}
			else
			{
				conclusionCSV << concluCSV[j][m] << endl;
			}
		}
	}
	conclusionCSV.close();
}





void ReportConclusion::RunTask()
{
	vector<string>srcCSVs;
	SetOutputFilePath();
	for (auto srcPath : filePath)
	{
		for (unsigned int i = 0; i < exeInfo.size(); i++)
		{
			GetReportCSVs(srcPath, srcCSVs, exeInfo[i]);
		}

		for (unsigned int i = 0; i < exeInfoLastVersion.size(); i++)
		{
			GetReportCSVs(srcPath, srcCSVs, exeInfoLastVersion[i]);
		}

		if (srcCSVs.size() == 0)
		{
			logger.TraceError("NO files were found!");
			return ;
		}

		if (exeInfo.size() != 0)
		{
			string ExecInformation = GetExecInformation(exeInfo[0]);
			OutputConclusion(srcCSVs, ExecInformation);
		}
		else
		{
			string ExecInformation = GetExecInformation(exeInfoLastVersion[0]);
			OutputConclusion(srcCSVs, ExecInformation);

		}
		
	}
}
void ReportConclusion::Init()
{
	ifstream fJsonFile(CLogger::GetAppPathA() + "init.json");
	logger.TraceInfo("	Open init.json.	");

	IStreamWrapper iStreamWrap(fJsonFile);
	Document Document;
	Document.ParseStream(iStreamWrap);

	//Init information
//-------------------------------------------------------------------------------------
	if (Document.HasMember("LightLog"))
	{
		LightLog = atoi(Document["LightLog"].GetString());
		logger.TraceInfo("	Import Param [LightLog]=%d.	", LightLog);
	}
	else
	{
		logger.TraceWarning("	init.json have not Param[LightLog].	");
		bInitNormal = FALSE;
		return;
	}
	//--------------------------------------------------------------------------------------
	if (Document.HasMember("exeInfo"))
	{
		if (Document["exeInfo"].IsArray())
		{
			for (size_t i = 0; i < Document["exeInfo"].Size(); ++i)
			{
				exeInfo.push_back(Document["exeInfo"][i].GetString());
				logger.TraceInfo("  Import Param [exeInfo]=%s.    ", exeInfo[i].c_str());
			}
		}
		else
		{
			logger.TraceError("   init.json have not Param[exeInfo].    ");
			bInitNormal = FALSE;
			return;
		}
	}
	//--------------------------------------------------------------------------------------
	if (Document.HasMember("exeInfoLastVersion"))
	{
		if (Document["exeInfoLastVersion"].IsArray())
		{
			for (size_t i = 0; i < Document["exeInfoLastVersion"].Size(); ++i)
			{
				exeInfoLastVersion.push_back(Document["exeInfoLastVersion"][i].GetString());
				logger.TraceInfo("  Import Param [exeInfoLastVersion]=%s.    ", exeInfoLastVersion[i].c_str());
			}
		}
		else
		{
			logger.TraceError("   init.json have not Param[exeInfo_6_4].    ");
			bInitNormal = FALSE;
			return;
		}
	}
	//-------------------------------------------------------------------------------------
	if (Document.HasMember("filePath"))
	{
		if (Document["filePath"].IsArray())
		{
			for (size_t i = 0; i < Document["filePath"].Size(); ++i)
			{
				filePath.push_back(Document["filePath"][i].GetString());
				logger.TraceInfo("	Import Param [filePath]=%s.	", filePath[i].c_str());
			}
		}
		else
		{
			logger.TraceError("	init.json have not Param[filePath].	");
			bInitNormal = FALSE;
			return;
		}
	}
	//-------------------------------------------------------------------------------------
	if (Document.HasMember("OutputFilePath"))
	{
		if (Document["OutputFilePath"].IsArray())
		{
			for (size_t i = 0; i < Document["OutputFilePath"].Size(); ++i)
			{
				OutputFilePath.push_back(Document["OutputFilePath"][i].GetString());
				logger.TraceInfo("	Import Param [filePath]=%s.	", OutputFilePath[i].c_str());
			}
		}
		else
		{
			logger.TraceError("	init.json have not Param[OutputFilePath].	");
			return;
		}
	}
	fJsonFile.close();
}

ReportConclusion::ReportConclusion()
{
	bInitNormal = TRUE;
}
ReportConclusion::~ReportConclusion()
{

}
