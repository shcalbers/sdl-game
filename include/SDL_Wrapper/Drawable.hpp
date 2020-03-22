#ifndef DRAWABLE_HEADER
#define DRAWABLE_HEADER

#include "SDL_Wrapper/SDL_Wrapper.hpp"

class Drawable
{

public:

        Drawable() {};

        virtual void Draw() {};

        ~Drawable() {};
};

#endif
