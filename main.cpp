#include "nucleus.h"
#include "game.h"
#include "callbacks.h"

#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspkernel.h>
#include <pspdebug.h>

#define printf pspDebugScreenPrintf

// PSP Module Info (necessary to create EBOOT.PBP)
PSP_MODULE_INFO("Flappybird PSP", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int main() 
{
	flappybird::initGame();
	flappybird::loop();
	flappybird::cleanupGame();
	return 0;
}