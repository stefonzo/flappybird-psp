#include "game.h"

namespace flappybird
{
    // game data
    texture_manager game_textures = texture_manager();
	ScePspFVector3 bird_pos = {PSP_SCR_WIDTH / 2, PSP_SCR_HEIGHT / 2, 0.0f};
	std::unique_ptr<texture_quad> bird_quad; 

    void initGame(void)
    {
        game_textures.addTexture("bird.png");
        bird_quad = std::make_unique<texture_quad>(
            game_textures.textures.at("bird.png").getPixelWidth(),
            game_textures.textures.at("bird.png").getPixelHeight(),
            &bird_pos,
            0xFFFFFFFF);
    }

    void renderBird(void)
    {
        // bind texture
        game_textures.textures.at("bird.png").bindTexture();
        bird_quad->render();
    }
}