#include <iostream>
#include "MainController.h"

int main()
{
	MainController mctl;
	if (!mctl.Initialize())
		return 0;
	mctl.MainLoop();
    return 0;
}

