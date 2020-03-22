#include "SDL_Wrapper/SDL_Video.hpp"
#include "Debug.hpp"
#include <iostream>
using namespace std;

struct Texture::Texture_Data
{
	unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
	//static Renderer & activeRenderer;

	Uint32 format;
	int access;
	int width;
	int height;
};

struct Window::Window_Data
{
	unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
};

struct Renderer::Renderer_Data
{
	unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
};



Texture::Texture() : _data(nullptr) {}

Texture::Texture(Renderer & renderer, const int & width, const int & height)
{
	DEBUG_LOG("Creating Texture....");

	//create a SDL_Texture from SDL_Surface.
	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);

	//if SDL_Texture could not be created.
	if(texture == NULL){
		//log error and exit program.
		DEBUG_LOG("ERROR: SDL_Texture could not be created: " << SDL_GetError());
		exit(1);
	}

	_data = unique_ptr<Texture_Data>{ new Texture_Data{
										unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>{ move(texture), &SDL_DestroyTexture }
									  }
									};

	//query and cache SDL_Texture data.
	int queryFailed = SDL_QueryTexture(*this, &(_data->format), &(_data->access), &(_data->width), &(_data->height));

	//if query not successful.
	if(queryFailed){
		//log error and exit program.
		DEBUG_LOG("ERROR: Could not query texture data: " << SDL_GetError());
		exit(1);
	}
}

//Texture Desc: Creates an SDL_Texture and stores its data in cache.
Texture::Texture(Renderer & renderer, const char * file_name)
{
	DEBUG_LOG("Creating Texture....");

	//create a SDL_Surface from an image file.
	SDL_Surface * texture_surface = IMG_Load(file_name);

	//if SDL_Surface could not be created.
	if(texture_surface == NULL){
		//log error and exit program.
		DEBUG_LOG("ERROR: SDL_Surface could not be created: " << SDL_GetError());
		exit(1);
	}

	//create a SDL_Texture from SDL_Surface.
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, texture_surface);

	//if SDL_Texture could not be created.
	if(texture == NULL){
		//log error and exit program.
		DEBUG_LOG("ERROR: SDL_Texture could not be created: " << SDL_GetError());
		exit(1);
	}

	//free SDL_Surface.
	SDL_FreeSurface(texture_surface);

	_data = unique_ptr<Texture_Data>{ new Texture_Data{
										unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>{ move(texture), &SDL_DestroyTexture }
									  }
								   	};

	//query and cache SDL_Texture data.
	int queryFailed = SDL_QueryTexture(*this, &(_data->format), &(_data->access), &(_data->width), &(_data->height));

	//if query not successful.
	if(queryFailed){
		//log error and exit program.
		DEBUG_LOG("ERROR: Could not query texture data: " << SDL_GetError());
		exit(1);
	}
}

Texture::Texture(Texture && rh) : _data(move(rh._data)) {}

Texture & Texture::operator=(Texture && rh)
{
	_data = move(rh._data);

	return *this;
}

Texture::operator SDL_Texture*(void)
{
	return (_data->texture).get();
}

const Uint32 & Texture::GetFormat(void) const
{
	return _data->format;
}

const int & Texture::GetTextureAccess(void) const
{
	return _data->access;
}

const int & Texture::GetWidth(void) const
{
	return _data->width;
}

const int & Texture::GetHeight(void) const
{
	return _data->height;
}

/*
void Texture::SetDrawingTarget(Texture * texture)
{
	int switchTargetFailed;

	if(texture == NULL){
		switchTargetFailed = SDL_SetRenderTarget(*activeRendererPtr, NULL);
	} else {
		switchTargetFailed = SDL_SetRenderTarget(*activeRendererPtr, *texture);
	}

	if(switchTargetFailed){
		DEBUG_LOG("ERROR: Could not change the Render Target: " << SDL_GetError());
		exit(1);
	}

	return;
}
*/

/*
void Texture::Draw(const Rectangle * srcrect, const Rectangle * dstrect)
{

	const auto && drawFailed = SDL_RenderCopy(
					*activeRendererPtr,
					*this,
					srcrect,
					dstrect
					);

	if(drawFailed){
		//log error and exit program.
		DEBUG_LOG("ERROR: Could not draw the texture: " << SDL_GetError());
		exit(1);
	}

	return;
}
*/

/*
void Texture::InitializeTextureClass(Renderer * renderer)
{
	DEBUG_LOG("Initializing the Texture Class....");

	activeRendererPtr = renderer;

	return;
}
*/

Texture::~Texture()	{ DEBUG_LOG("Destroying Texture...."); }

/*
SDL_Texture * Texture::GetSDLTexture(void)
{
	return (_data->texture).get();
}
*/

/*
Window::Window()
	: _data(nullptr)	{}
*/

//Window Constructor: Creates a window for the user.
Window::Window(const char * title, int x, int y, int width, int height, Uint32 flags)
{
	DEBUG_LOG("Creating Window....");
	//if SDL_INIT_VIDEO has not been intialized.
	if(SDL_WasInit(SDL_INIT_VIDEO) == 0){
		//log error and exit program.
		DEBUG_LOG("ERROR: SDL_INIT_VIDEO was not initialized.");
		exit(1);
	}

	_data = unique_ptr<Window_Data>{ new Window_Data{
										unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>{
											SDL_CreateWindow(title, x, y, width, height, flags), &SDL_DestroyWindow
										}
									}};

	//if Window could not be created.
	if(*this == NULL){
		//log error and exit program.
		DEBUG_LOG("ERROR: Could not create window: " << SDL_GetError());
		exit(1);
	}
	DEBUG_LOG("Window Created.");
}

Window::operator SDL_Window*(void)
{
	return (_data->window).get();
}

//Window Destructor: Free resources by destroying SDL_Window*.
Window::~Window()	{}

/*
	RENDERER CLASS.
*/

/*
Renderer::Renderer()
	: _data(nullptr)	{}
*/

Renderer::Renderer(Window & window, int index, Uint32 flags)
{
	DEBUG_LOG("Creating Renderer....");
	//Initialize Renderer_Data _data.
	_data = unique_ptr<Renderer_Data>{new Renderer_Data{
										unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>{
											SDL_CreateRenderer(window, index, flags), &SDL_DestroyRenderer
										}
									}};

	//if an SDL_Renderer could not be created.
	if(*this == NULL){
		//log error and exit program.
		DEBUG_LOG("ERROR: Could not create renderer: " << SDL_GetError());
		exit(1);
	}

	//Prepare the Renderer for Drawing.
	SDL_SetRenderDrawColor(*this, 255, 255, 255, 255);
	SDL_RenderClear(*this);
	SDL_RenderPresent(*this);

	DEBUG_LOG("Renderer Created.");
}

Renderer::operator SDL_Renderer*(void)
{
	return (_data->renderer).get();
}

void Renderer::SetRenderTarget(Texture * texture)
{
	int switchTargetFailed;

	if(texture == NULL){
		switchTargetFailed = SDL_SetRenderTarget(*this, NULL);
	} else {
		switchTargetFailed = SDL_SetRenderTarget(*this, *texture);
	}

	if(switchTargetFailed){
		DEBUG_LOG("ERROR: Could not change the Render Target: " << SDL_GetError());
		exit(1);
	}

	return;
}

void Renderer::DrawTexture(Texture & texture, const Rectangle * srcrect, const Rectangle * dstrect)
{

	//Attempt to draw Texture.
	int drawFailed = SDL_RenderCopy(*this, texture, srcrect, dstrect);

	//if Texture could not be drawn.
	if(drawFailed){
		//log error and exit program.
		DEBUG_LOG("ERROR: Could not draw the texture: " << SDL_GetError());
		exit(1);
	}

	return;
}

void Renderer::Present(void)
{
	SDL_RenderPresent(*this);

	return;
}

void Renderer::Clear(void)
{
	//Attempt to clear the Renderer.
	int clearingFailed = SDL_RenderClear(*this);

	//If clearing the Renderer failed.
	if(clearingFailed){
		//log error and exit program.
		DEBUG_LOG("ERROR: Could not clear the renderer: " << SDL_GetError());
		exit(1);
	}

	return;
}

void Renderer::SetLogicalResolution(const int & width, const int & height)
{
	//Attempt to set the Logical Size.
	int setLogicalSizeFailed = SDL_RenderSetLogicalSize(*this, width, height);

	//If setting the Logical Size failed.
	if(setLogicalSizeFailed){
		//log error and exit program.
		DEBUG_LOG("ERROR: Could not set the logical size: " << SDL_GetError());
		exit(1);
	}

	return;
}

Renderer::~Renderer()	{}
