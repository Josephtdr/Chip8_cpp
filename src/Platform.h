#ifndef H_PLATFORM
#define H_PLATFORM

#include<SDL2/SDL.h>

class Platform
{
private:
    SDL_Window* window{};
	SDL_Renderer* renderer{};
	SDL_Texture* texture{};

public:
	Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
	~Platform();
	
	void Update(void const* buffer, int pitch);
	
	bool ProcessInput(uint8_t* keys);
};




#endif