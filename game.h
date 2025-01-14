#pragma once

#include "callbacks.h"
#include "nucleus.h"
#include "bird.h"
#include "pipe.h"

#define GRAVITY_ACC 100.0f
#define N_PIPES 16
#define RNG_SEED 2

using namespace nucleus;

namespace flappybird 
{
    enum class game_state
    {
        paused, unpaused
    };
    
    // game functions
    void initGame(void);
    void initPipes(void);
    void readController(void);
    void updateButtonState(void);
    bool isButtonPressed(unsigned int button); // move to nucleus
    bool detectCollision(bird *b, pipe *p);
    bool checkBoundaries(void);
    void loop(void);
    void updateBird(float dt);
    void updatePipes(float dt);
    void updateCamera(float dt);
    void updateGame(float dt);
    void renderBird(void); 
    void renderPipes(void);
    void renderGame(void);
    void cleanupGame(void);
}