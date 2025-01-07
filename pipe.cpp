#include "pipe.h"

namespace flappybird {
    pipe::pipe(float width, float height, float padding, ScePspFVector3 *pos)
    {
        float padding_offset = padding/2;
        boundary = std::make_unique<nucleus::primitive::rectangle>(width, height, 0xFF000000, pos);
        ScePspFVector3 inside_pos = {pos->x + (padding - padding_offset), pos->y - (padding - padding_offset), pos->z};
        inside = std::make_unique<nucleus::primitive::rectangle>(width - padding, height - padding, 0xFF00FF00, &inside_pos); 
    }

    void pipe::setPosition(ScePspFVector3 *pos)
    {
        
    }

    ScePspFVector3 pipe::getPosition(void)
    {
        return boundary->getPosition();
    }

    void pipe::renderPipe(void)
    {
        boundary->render();
        inside->render();
    }
}