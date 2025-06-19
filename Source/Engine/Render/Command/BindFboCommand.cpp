#include "Render/Command/BindFboCommand.h"
#include <glad/glad.h>
#include <Core/Size.h>

namespace volucris
{
	struct BindFboCommand::Impl
	{
		Size size;
	};

	BindFboCommand::BindFboCommand()
		: RenderCommand()
		, m_impl(new Impl)
	{

	}

	BindFboCommand::BindFboCommand(int width, int height)
		: BindFboCommand()
	{
	}

	BindFboCommand::~BindFboCommand()
	{
		delete m_impl;
	}

	void BindFboCommand::setSize(int width, int height)
	{
		Size size = { width, height };
		if (m_impl->size != size)
		{
			m_impl->size = size;
		}
	}

	void BindFboCommand::execute()
	{
	}
}
