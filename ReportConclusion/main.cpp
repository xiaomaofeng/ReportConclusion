#include"ReportConclusion.h"
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")		//����ʾ�ڴ�


int main()
{
	ReportConclusion Main;
	Main.Init();
	if (Main.bInitNormal == true)
	{
		Main.RunTask();
	}
}