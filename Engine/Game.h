#ifndef PURE2D_ENGINE_GAME_H
#define PURE2D_ENGINE_GAME_H

#include <Pure2D/Define.h>
#include <Pure2D/Window/Window.h>
#include <Pure2D/Graphics/Renderer.h>

namespace pure
{
	struct WindowEvent;

	struct PURE2D_API Game
	{
		Game();
		virtual ~Game();

		void run();
	protected:
		virtual void load() {}
		virtual void update(float dt) {}
		virtual void draw() {}
		virtual void windowEvent(WindowEvent& event) {}

		Window m_window;
		Renderer m_renderer;
	};
}

#endif 
