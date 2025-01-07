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
    bird player;
    pipe *pipes; // these pipes are rendered to the screen

    void initGame(void)
    {
        setupCallbacks();
	    initGraphics(gu_list);
	    initMatrices();
        initPipes();
	    setRenderMode(render_mode::NUCLEUS_TEXTURE2D, gu_list);	
        sceCtrlSetSamplingCycle(0);  // Set to fastest sampling rate
        sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);  // Use digital mode
	    sceRtcGetCurrentTick(&lastTime);

        player.pos = {(PSP_SCR_WIDTH / 2.0f), (PSP_SCR_HEIGHT / 2.0f), 0.0f};
        player.mass = 1.5;
        player.vel = {35.0f, 0.0f, 0.0f};
        player.acc = {0.0f, (GRAVITY_ACC/player.mass), 0.0f};

        is_paused = game_state::paused;
        game_camera = std::make_unique<camera2D>(0.0f, 0.0f);
        game_textures.addTexture("bird.png");
        player.bird_quad = std::make_unique<texture_quad>(game_textures.textures.at("bird.png").getPixelWidth(), game_textures.textures.at("bird.png").getPixelHeight(), &player.pos, 0xFFFFFFFF);
    }

    void initPipes(void)
    {
        pipes = (pipe*)memalign(16, sizeof(pipe) * N_PIPES);
        ScePspFVector3 pipe_pos = {PSP_SCR_WIDTH / 2.0f, PSP_SCR_HEIGHT + 1, 0.0f};
        for (unsigned int i = 0; i < N_PIPES / 2; i++) // bottom pipes
        {
            pipes[i] = pipe(30, 70, 4, &pipe_pos);
            pipe_pos.x += 120;
        }
        pipe_pos = {PSP_SCR_WIDTH / 2.0f, 70.0f, 0.0f};
        for (unsigned int i = N_PIPES / 2; i < N_PIPES; i++) // top pipes
        {
            pipes[i] = pipe(30, 71, 4, &pipe_pos);
            pipe_pos.x += 120;
        }
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

        // prevent building up unnecessary velocity when game is paused...
        if (is_paused != game_state::paused) {
            if (isButtonPressed(PSP_CTRL_CROSS))
            {
                player.vel.y -= 45.0f;
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
		    sceGuClearColor(0xFFEE9999);
		    sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT | GU_STENCIL_BUFFER_BIT);

		    readController();
		    updateGame(dt);
            renderGame();
		
		    endFrame();
	    }
    }

    void updateBird(float dt)
    {   
        player.pos.x = player.pos.x + (player.vel.x * dt);
        player.pos.y = player.pos.y + (player.vel.y * dt);
        player.bird_quad->changePosition(&player.pos);
        player.vel.x += player.acc.x * dt;
        player.vel.y += player.acc.y * dt;
    }

    void updatePipes(float dt)
    {

    }

    void updateCamera(float dt)
    {
        game_camera->updateCameraTarget(player.pos.x - (PSP_SCR_WIDTH / 2.0f), 0.0f); // I have no clue why this works...
        game_camera->smoothCameraUpdate(dt);
		game_camera->setCamera();
    }

    void updateGame(float dt)
    {
        if (is_paused != game_state::paused)
        {
            updateBird(dt);
            updatePipes(dt);
            updateCamera(dt);
        }
    }

    void renderBird(void)
    {
        sceGuEnable(GU_TEXTURE_2D);
        game_textures.textures.at("bird.png").bindTexture();
        player.bird_quad->render();
    }

    void renderPipes(void)
    {
        sceGuDisable(GU_TEXTURE_2D);
        for (unsigned int i = 0; i < N_PIPES; i++)
        {
            pipes[i].renderPipe();
        }
    }

    void renderGame(void)
    {
        renderPipes();
        renderBird();
    }

    void cleanupGame(void)
    {
        free(pipes);
        nucleus::termGraphics();
	    sceKernelExitGame();
    }
}