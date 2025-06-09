#ifndef __volucris_ogl_texture2d_object_h__
#define __volucris_ogl_texture2d_object_h__

#include <vector>
#include <glad/glad.h>
#include <Engine/Core/size.h>
#include <Engine/Core/types_help.h>
#include <Engine/Renderer/OpenGL/ogl_framebuffer_component.h>

namespace volucris
{
	class Context;

	class Texture2DObject : public FrameBufferComponent
	{
	public:
		Texture2DObject();

		~Texture2DObject();

		void setType(GLenum type)
		{
			m_type = type;
			dirty();
		}

		bool create() override;

		void release();

		void setDataFormat(GLint format)
		{
			m_dataFormat = format;
		}

		void setData(std::vector<uint8> data);

		bool initialize(Context* content) override;

	private:
		GLenum m_type;
		GLint m_filter;
		GLint m_dataFormat;
		std::vector<uint8> m_data;
	};
}

#endif // !__volucris_ogl_texture2d_object_h__
