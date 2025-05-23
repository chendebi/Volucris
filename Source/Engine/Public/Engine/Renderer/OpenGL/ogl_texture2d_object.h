#ifndef __volucris_ogl_texture2d_object_h__
#define __volucris_ogl_texture2d_object_h__

#include <vector>
#include <glad/glad.h>
#include <Engine/Core/size.h>
#include <Engine/Core/types_help.h>

namespace volucris
{
	class Context;

	class Texture2DObject
	{
	public:
		Texture2DObject();

		~Texture2DObject();

		void setSize(int width, int height);

		void setFormat(GLenum format)
		{
			m_format = format;
		}

		void setFormat(GLint format)
		{
			m_format = format;
		}

		void setType(GLenum type)
		{
			m_type = type;
		}

		bool create();

		void release();

		bool initialize(Context* content);

		bool isValid() const { return m_id > 0; }

		uint32 getID() const { return m_id; }

	private:
		uint32 m_id;
		Size m_size;
		GLint m_format;
		GLenum m_type;
		GLint m_filter;
		std::vector<uint8> m_data;
	};
}

#endif // !__volucris_ogl_texture2d_object_h__
