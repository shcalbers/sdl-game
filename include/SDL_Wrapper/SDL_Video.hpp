#ifndef SDL_VIDEO_HEADER
#define SDL_VIDEO_HEADER

#include "SDL.h"
#include "SDL_image.h"
#include <memory>
using namespace std;

using Rectangle = SDL_Rect;
class Texture;
class Window;
class Renderer;

class Texture
{

friend class Renderer;

public:

	//Texture(Renderer & renderer, const char * file_name);
	Texture();
	Texture(Renderer & renderer, const int & width, const int & height);
	Texture(Renderer & renderer, const char * file_name);

	Texture(const Texture &) = delete;
	Texture & operator=(const Texture &) = delete;

	Texture(Texture &&);
	Texture & operator=(Texture &&);

	const Uint32 	& 	GetFormat(void) 		const;
	const int 	&	GetTextureAccess(void)		const;
	const int 	&	GetWidth(void) 			const;
	const int 	&	GetHeight(void) 		const;

	//void SetDrawingTarget(Texture * texture);
	//void Draw(const Rectangle * srcrect, const Rectangle * dstrect);

	~Texture();

private:

	struct Texture_Data;
	unique_ptr<Texture_Data> _data;

	//inline static Renderer * activeRendererPtr = nullptr;

	/*inline*/ // static void InitializeTextureClass(Renderer * renderer);
	//{ activeRenderer = renderer; }

	operator SDL_Texture*(void);
};

class Window
{

friend class Renderer;

public:

	Window(const char * title, int x, int y, int width, int height, Uint32 flags);

	Window(const Window &) = delete;
	Window & operator=(const Window &) = delete;

	~Window();

private:

	struct Window_Data;
	unique_ptr<Window_Data> _data;

	/*
	inline static Renderer * activeRenderer = nullptr
	static void InitializeWindowClass(Renderer & renderer);
	*/

	operator SDL_Window*(void);

};

class Renderer
{

friend class Texture;

public:

	//Create Renderer from Window.
	Renderer(Window & window, int index, Uint32 flags);

	Renderer(const Renderer &) = delete;
	Renderer & operator=(const Renderer &) = delete;

	void SetRenderTarget(Texture * texture);

	void DrawTexture(Texture & texture, const Rectangle * srcrect, const Rectangle * dstrect);
	void Present(void);
	void Clear(void);
	void SetLogicalResolution(const int & width, const int & height);

	~Renderer();

private:

	struct Renderer_Data;
	unique_ptr<Renderer_Data> _data;

	operator SDL_Renderer*(void);
};

#endif
