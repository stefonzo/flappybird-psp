#pragma once

#include "callbacks.h"
#include "nucleus.h"

#define GRAVITY_ACC 100.0f

using namespace nucleus;

namespace flappybird 
{
    enum class game_state
    {
        paused, unpaused
    };
    
    class pipe
    {   // I'm really lazy so I'm rendering a rectangle over anothr one to create the illusion of a rectangle with a border
        primitive::rectangle boundary;
        primitive::rectangle inside;
    };
    // game functions
    void initGame(void);
    void readController(void);
    void updateButtonState(void);
    bool isButtonPressed(unsigned int button);
    void loop(void);
    void updateBird(float dt);
    void updatePipes(float dt);
    void updateGame(float dt);
    void renderBird(void); 
    void renderPipes(void);
    void renderGame(void);
    void cleanupGame(void);
}