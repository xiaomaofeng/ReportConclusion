#include"ReportConclusion.h"
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")		//Do not display windows console;


int main()
{
	ReportConclusion Main;
	Main.Init();
	if (Main.bInitNormal == true)
	{
		Main.RunTask();
	}
}