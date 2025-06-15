#ifndef __volucris_imgui_renderer_h__
#define __volucris_imgui_renderer_h__

#include <Engine/Render/Renderer.h>
#include <glm/glm.hpp>

struct GLFWwindow;
class ImGuiContext;

namespace volucris
{
	class ImGuiRenderer : public Object
	{
	public:
		ImGuiRenderer(GLFWwindow* handle);

		~ImGuiRenderer() override;

		void render();

		void makeCurrent();

		glm::vec4 color;

	private:
		GLFWwindow* m_windowHandle;
		ImGuiContext* m_imguiContext;
	};
}


#endif // !
