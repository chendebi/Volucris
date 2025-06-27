#ifndef __volucris_imgui_renderer_h__
#define __volucris_imgui_renderer_h__

#include <Engine/Core/Object.h>
#include <glm/glm.hpp>
#include <Engine/RHI/RHIState.h>

struct GLFWwindow;
class ImGuiContext;

namespace volucris
{
	class Window;
	class RHICommandList;

	class ImGuiRenderer
	{
	public:
		ImGuiRenderer(Window* window);

		~ImGuiRenderer();

		void render();

		void makeCurrent();

	private:
		ImGuiContext* m_imguiContext;
		std::unique_ptr<RHICommandList> m_cmdList;
		RHIClearState m_clear;
	};
}


#endif // !
