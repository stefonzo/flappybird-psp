#pragma once
#include "nucleus.h"

namespace flappybird {
    class pipe
        {   // I'm really lazy so I'm rendering a rectangle over another one to create the illusion of a rectangle with a border
            public:
                pipe(float width, float height, float padding, ScePspFVector3 *pos);
                void setPosition(ScePspFVector3 *pos);
                ScePspFVector3 getPosition(void);
                void renderPipe(void);
                float getWidth(void) {return m_width;}
                float getHeight(void) {return m_height;}
            private:
                float m_padding, m_width, m_height;
                std::unique_ptr<nucleus::primitive::rectangle> boundary;
                std::unique_ptr<nucleus::primitive::rectangle> inside;
        };
}