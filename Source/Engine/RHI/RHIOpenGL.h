#ifndef __volucris_rhi_opengl_h__
#define __volucris_rhi_opengl_h__

#include <glad/glad.h>
#include <Core/Assert.h>
#include <Core/Volucris.h>
#include <Core/TextureDefines.h>
#include <RHI/RHIBuffer.h>
#include <RHI/RHIVertexBuffer.h>

inline std::string getGLErrorDesc(GLenum code)
{
	if (code == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM";
	}
	else if (code == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION";
	}
	else if (code == GL_INVALID_INDEX)
	{
		return "GL_INVALID_INDEX";
	}
	else if (code == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE";
	}
	return fmt::format("Unknown error: {}", code);
}

#define GL_CHECK() \
	{\
		auto err = glGetError(); \
		while (err != GL_NO_ERROR) \
		{ \
			v_checkf(false, Engine, "gl error at {} : {}, {}", __FILE__, __LINE__, getGLErrorDesc(err)); \
			err = glGetError(); \
		} \
	}

#define GL_CALL(exp) \
	exp; \
	GL_CHECK()

namespace volucris
{
	static GLenum getGLFormat(Texture::EPixelFormat format)
	{
		switch (format)
		{
		case volucris::Texture::EPixelFormat::R8G8B8:
			return GL_RGB8;
		case volucris::Texture::EPixelFormat::R8G8B8A8:
			return GL_RGBA8;
		default:
			break;
		}
		v_check(false)
		return GL_NONE;
	}

	static GLenum getGLTarget(RHIBuffer::Type type)
	{
		switch (type)
		{
		case volucris::RHIBuffer::VertexBuffer:
			return GL_ARRAY_BUFFER;
		case volucris::RHIBuffer::ElementBuffer:
			return GL_ELEMENT_ARRAY_BUFFER;
		case volucris::RHIBuffer::PixelPackBuffer:
			return GL_PIXEL_PACK_BUFFER;
		case volucris::RHIBuffer::PixelUnpackBuffer:
			return GL_PIXEL_UNPACK_BUFFER;
		default:
			break;
		}
		return GL_NONE;
	}

	static GLenum getGLElementDrawMode(RHIElementBuffer::DrawMode mode)
	{
		switch (mode)
		{
		case volucris::RHIElementBuffer::Points:
			return GL_POINTS;
		case volucris::RHIElementBuffer::LineStrip:
			return GL_LINE_STRIP;
		case volucris::RHIElementBuffer::LineLoop:
			return GL_LINE_LOOP;
		case volucris::RHIElementBuffer::Lines:
			return GL_LINES;
		case volucris::RHIElementBuffer::LineStripAdjacency:
			return GL_LINE_STRIP_ADJACENCY;
		case volucris::RHIElementBuffer::LinesAdjacency:
			return GL_LINES_ADJACENCY;
		case volucris::RHIElementBuffer::TraingleStrip:
			return GL_TRIANGLE_STRIP;
		case volucris::RHIElementBuffer::TraingleFan:
			return GL_TRIANGLE_FAN;
		case volucris::RHIElementBuffer::Traingles:
			return GL_TRIANGLES;
		default:
			break;
		}
		return GL_NONE;
	}

	static GLenum getGLElementType(RHIElementBuffer::DataType type)
	{
		switch (type)
		{
		case volucris::RHIElementBuffer::UByte:
			return GL_UNSIGNED_BYTE;
		case volucris::RHIElementBuffer::UShort:
			return GL_UNSIGNED_SHORT;
		case volucris::RHIElementBuffer::UInt:
			return GL_UNSIGNED_INT;
		default:
			break;
		}
		return GL_NONE;
	}
}

#endif // !__volucris_rhi_opengl_h__
