#ifndef __volucris_imgui_renderer_h__
#define __volucris_imgui_renderer_h__

#include <Engine/Core/Object.h>
#include <glm/glm.hpp>

struct GLFWwindow;
class ImGuiContext;

namespace volucris
{
	class ImGuiRenderer
	{
	public:
		ImGuiRenderer(GLFWwindow* handle);

		~ImGuiRenderer();

		void render();

		void makeCurrent();

		glm::vec4 color;

	private:
		GLFWwindow* m_windowHandle;
		ImGuiContext* m_imguiContext;
	};
}


#endif // !
