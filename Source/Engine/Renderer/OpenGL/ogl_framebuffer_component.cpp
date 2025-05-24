#include <Engine/Renderer/OpenGL/ogl_framebuffer_component.h>

namespace volucris
{
	FrameBufferComponent::FrameBufferComponent(ComponentType componentType)
		: ShareableObject()
		, m_id(0)
		, m_valid(0)
		, m_format(GL_NONE)
		, m_componentType(componentType)
	{
	}

	void FrameBufferComponent::setFormat(GLint format)
	{
		m_format = format;
		dirty();
	}
}