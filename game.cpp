#include "game.h"

namespace flappybird
{
    // game data
    bool running = true; // used in app loop
    static unsigned int __attribute__((aligned(16))) gu_list[GU_LIST_SIZE]; // used to send commands to the Gu
    std::unique_ptr<camera2D> game_camera; // have camera looking at the center of the screen
	SceCtrlData ctrlData;
    u64 lastTime; // for delta time
    texture_manager game_textures = texture_manager();

    // bird data
	ScePspFVector3 bird_pos = {PSP_SCR_WIDTH / 2, PSP_SCR_HEIGHT / 2, 0.0f};
    ScePspFVector3 bird_vel;
    ScePspFVector3 bird_acc;
    float bird_mass;
	std::unique_ptr<texture_quad> bird_quad; 

    void initGame(void)
    {
        setupCallbacks();
	    initGraphics(gu_list);
	    initMatrices();
	    setRenderMode(render_mode::NUCLEUS_TEXTURE2D, gu_list);	
	    sceRtcGetCurrentTick(&lastTime);
        game_camera = std::make_unique<camera2D>(0.0f, 0.0f);
        game_textures.addTexture("bird.png");
        bird_quad = std::make_unique<texture_quad>(
            game_textures.textures.at("bird.png").getPixelWidth(),
            game_textures.textures.at("bird.png").getPixelHeight(),
            &bird_pos,
            0xFFFFFFFF);
    }

    void readController(void)
    {
        sceCtrlReadBufferPositive(&ctrlData, 1);

		if (ctrlData.Buttons & PSP_CTRL_UP) 
		{
			game_camera->updateCameraTarget(game_camera->getCameraPosition().x, game_camera->getCameraPosition().y - CAMERA_CLAMPING);
		}
		if (ctrlData.Buttons & PSP_CTRL_DOWN) 
		{
			game_camera->updateCameraTarget(game_camera->getCameraPosition().x, game_camera->getCameraPosition().y + CAMERA_CLAMPING);
		}
		if (ctrlData.Buttons & PSP_CTRL_LEFT) 
		{
			game_camera->updateCameraTarget(game_camera->getCameraPosition().x - CAMERA_CLAMPING, game_camera->getCameraPosition().y);
		}
		if (ctrlData.Buttons & PSP_CTRL_RIGHT) 
		{
			game_camera->updateCameraTarget(game_camera->getCameraPosition().x + CAMERA_CLAMPING, game_camera->getCameraPosition().y);
		}
    }

    void loop(void)
    {
        char buff[256];
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

		    readController();
		    updateGame(dt);
		    renderGame();
		
		    endFrame();
	    }
    }

    void updateBird(float dt)
    {

    }

    void updatePipes(float dt)
    {

    }

    void updateGame(float dt)
    {
        updateBird(dt);
        updatePipes(dt);
        game_camera->smoothCameraUpdate(dt);
		game_camera->setCamera();
    }

    void renderBird(void)
    {
        // bind texture
        game_textures.textures.at("bird.png").bindTexture();
        bird_quad->render();
    }

    void renderPipes(void)
    {

    }

    void renderGame(void)
    {
        renderBird();
        renderPipes();
    }

    void cleanupGame(void)
    {
        nucleus::termGraphics();
	    sceKernelExitGame();
    }
}