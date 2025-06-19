#ifndef __volucris_bind_fbo_command_h__
#define __volucris_bind_fbo_command_h__

#include <Engine/Render/Command/RenderCommand.h>

namespace volucris
{
	class BindFboCommand : public RenderCommand
	{
	public:
		BindFboCommand();

		BindFboCommand(int width, int height);

		~BindFboCommand();

		void setSize(int width, int height);

		void execute() override;

	private:
		struct Impl;
		Impl* m_impl;
	};
}

#endif // !__volucris_bind_fbo_command_h__
