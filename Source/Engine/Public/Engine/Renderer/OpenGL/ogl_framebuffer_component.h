#ifndef __ogl_framebuffer_component_h__
#define __ogl_framebuffer_component_h__

#include <Engine/Core/types_help.h>
#include <Engine/Core/size.h>
#include <Engine/Core/shareable_object.h>
#include <glad/glad.h>

namespace volucris
{
	class Context;

	class FrameBufferComponent : public ShareableObject
	{
	public:
		enum ComponentType
		{
			NONE,
			TEXTURE,
			RENDER_BUFFER
		};

	public:
		FrameBufferComponent(ComponentType componentType);

		void setFormat(GLint format);

		void setSize(int width, int height)
		{
			m_size = { width, height };
			dirty();
		}

		virtual bool create() = 0;

		virtual bool initialize(Context* context) = 0;

		uint32 getID() const { return m_id; }

		bool isValid() const { return m_valid; }

		ComponentType getComponentType() const { return m_componentType; }

	protected:
		void dirty()
		{
			m_valid = false;
		}

	protected:
		uint32 m_id;
		uint8 m_valid;
		GLint m_format;
		Size m_size;
		ComponentType m_componentType;
	};
}

#endif // !__ogl_framebuffer_component_h__
