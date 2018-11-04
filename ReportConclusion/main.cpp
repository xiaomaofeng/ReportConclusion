#include"ReportConclusion.h"
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")		//²»ÏÔÊ¾ºÚ´°


int main()
{
	ReportConclusion Main;
	Main.Init();
	if (Main.bInitNormal == true)
	{
		Main.RunTask();
	}
}