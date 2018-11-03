#include"ReportConclusion.h"



int main()
{
	ReportConclusion Main;
	Main.Init();
	if (Main.bInitNormal == true)
	{
		Main.RunTask();
	}
}