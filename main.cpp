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
PSP_MODULE_INFO("Squares", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int main() 
{
    bool running = true; // used in app loop
    static unsigned int __attribute__((aligned(16))) gu_list[GU_LIST_SIZE]; // used to send commands to the Gu

	nucleus::setupCallbacks();
	nucleus::initGraphics(gu_list);
	nucleus::initMatrices();

	nucleus::camera2D camera = nucleus::camera2D(0.0f, 0.0f); // have camera looking at the center of the screen
	SceCtrlData ctrlData;

	nucleus::setRenderMode(nucleus::render_mode::NUCLEUS_TEXTURE2D, gu_list);	

	u64 lastTime;
	sceRtcGetCurrentTick(&lastTime);

	flappybird::initGame();


	while (running)
	{
		nucleus::startFrame(gu_list);
		float dt = nucleus::calculateDeltaTime(lastTime);

		sceGuDisable(GU_DEPTH_TEST);
	
		// blending
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		sceGuEnable(GU_BLEND);

		// clear background to blue
		sceGuClearColor(0xFFFE0000);
		sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT | GU_STENCIL_BUFFER_BIT);

		nucleus::readController(ctrlData, &camera);

		// update and set camera
		camera.smoothCameraUpdate(dt);
		camera.setCamera();

		flappybird::renderBird();
		
		nucleus::endFrame();
	}
	nucleus::termGraphics();
	sceKernelExitGame();
	return 0;
}