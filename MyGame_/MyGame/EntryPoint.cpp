/*
	WinMain() is the entry point of a Windows program
	(instead of the familiar main() in a console program)
*/

// Includes
//=========

#include "cMyGame.h"

// Entry Point
//============

int WINAPI WinMain( HINSTANCE i_thisInstanceOfTheApplication, HINSTANCE, char* i_commandLineArguments, int i_initialWindowDisplayState )
{
	//Allocate a windows and read and write to it
	//Considering write out this output log
	{
		AllocConsole();
		FILE* fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
	}
	return eae6320::Application::Run<eae6320::cExampleGame>( i_thisInstanceOfTheApplication, i_commandLineArguments, i_initialWindowDisplayState );
}
