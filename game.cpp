#include "game.h"

namespace flappybird
{
    // game data
    bool running = true; // used in app loop
    game_state is_paused;
    static unsigned int __attribute__((aligned(16))) gu_list[GU_LIST_SIZE]; // used to send commands to the Gu
    std::unique_ptr<camera2D> game_camera; // have camera looking at the center of the screen
	SceCtrlData current_state, previous_state;
    u64 lastTime; // for delta time
    texture_manager game_textures = texture_manager();

    // bird data
	ScePspFVector3 bird_pos;
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
        sceCtrlSetSamplingCycle(0);  // Set to fastest sampling rate
        sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);  // Use digital mode
	    sceRtcGetCurrentTick(&lastTime);

        bird_mass = 1.5f;
        bird_pos = {PSP_SCR_WIDTH / 2, PSP_SCR_HEIGHT / 2, 0.0f};
        bird_vel = {35.0f, 0.0f, 0.0f};
        bird_acc = {0.0f, (GRAVITY_ACC/bird_mass), 0.0f};

        is_paused = game_state::paused;
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
        updateButtonState();

        if (isButtonPressed(PSP_CTRL_START))
        {
            if (is_paused == game_state::paused) {
                is_paused = game_state::unpaused;
            } else if (is_paused == game_state::unpaused) {
                is_paused = game_state::paused;
            }
        }

		if (current_state.Buttons & PSP_CTRL_UP) 
		{
			game_camera->updateCameraTarget(game_camera->getCameraPosition().x, game_camera->getCameraPosition().y - CAMERA_CLAMPING);
		}
		if (current_state.Buttons & PSP_CTRL_DOWN) 
		{
			game_camera->updateCameraTarget(game_camera->getCameraPosition().x, game_camera->getCameraPosition().y + CAMERA_CLAMPING);
		}
		if (current_state.Buttons & PSP_CTRL_LEFT) 
		{
			game_camera->updateCameraTarget(game_camera->getCameraPosition().x - CAMERA_CLAMPING, game_camera->getCameraPosition().y);
		}
		if (current_state.Buttons & PSP_CTRL_RIGHT) 
		{
			game_camera->updateCameraTarget(game_camera->getCameraPosition().x + CAMERA_CLAMPING, game_camera->getCameraPosition().y);
		}

        // prevent building up unnecessary velocity when game is paused...
        if (is_paused != game_state::paused) {
            if (isButtonPressed(PSP_CTRL_CROSS))
            {
                bird_vel.y -= 45.0f;
            }
        }
    }

    void updateButtonState(void)
    {
        previous_state = current_state;
        sceCtrlReadBufferPositive(&current_state, 1);
    }

    bool isButtonPressed(unsigned int button)
    {
        return (current_state.Buttons & button) && !(previous_state.Buttons & button);
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
        bird_pos.x = bird_pos.x + (bird_vel.x * dt);
        bird_pos.y = bird_pos.y + ((bird_vel.y * dt) + (0.5f * bird_acc.y * pow(dt, 2.0f)));
        bird_quad->changePosition(&bird_pos);
        bird_vel.x += bird_acc.x * dt;
        bird_vel.y += bird_acc.y * dt;
    }

    void updatePipes(float dt)
    {

    }

    void updateGame(float dt)
    {
        if (is_paused != game_state::paused) 
        {
            updateBird(dt);
            updatePipes(dt);
        }
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