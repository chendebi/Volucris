#ifndef __volucris_render_command_h__
#define __volucris_render_command_h__

#include <Core/Object.h>
#include <Core/TypesHelp.h>
#include <glm/glm.hpp>

namespace volucris
{
	class RenderCommand : public Object
	{
	public:
		RenderCommand() : Object() {}

		virtual ~RenderCommand() = default;

		virtual void execute() = 0;
	};

	class ClearCommand : public RenderCommand
	{
	public:
		enum Buffer
		{
			ColorBuffer = 0x01,
			DepthBuffer = 0x02,
			ColorAndDepth = ColorBuffer | DepthBuffer,
			All = ColorBuffer | DepthBuffer,
		};

	public:
		ClearCommand(float r, float g, float b, float a = 1.0, int32 buffers = Buffer::ColorAndDepth);

		~ClearCommand();

		void execute() override;

	private:
		struct Impl;
		Impl* m_impl;
	};
}

#endif // !__volucris_render_command_h__
