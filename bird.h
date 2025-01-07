#pragma once

#include "nucleus.h"

namespace flappybird 
{
    typedef struct bird 
    {
        ScePspFVector3 pos;
        ScePspFVector3 vel;
        ScePspFVector3 acc;
        float mass;
	    std::unique_ptr<nucleus::texture_quad> bird_quad; 
    } bird;
}