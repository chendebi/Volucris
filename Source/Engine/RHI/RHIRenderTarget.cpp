#include <RHI/RHIRenderTarget.h>
#include <glad/glad.h>
#include <RHI/RHIState.h>
#include <RHI/RHICommandList.h>
#include <Core/Volucris.h>
#include "RHIOpenGL.h"

namespace volucris
{
	static GLenum getGLRenderTarget(RHIRenderTarget::Usage usage)
	{
		GLenum target = GL_NONE;
		switch (usage)
		{
		case volucris::RHIRenderTarget::ReadOnly:
			target = GL_READ_FRAMEBUFFER;
			break;
		case volucris::RHIRenderTarget::WriteOnly:
			target = GL_DRAW_FRAMEBUFFER;
			break;
		case volucris::RHIRenderTarget::ReadWrite:
			target = GL_FRAMEBUFFER;
			break;
		default:
			break;
		}
		return usage;
	}

	RHIRenderTarget::RHIRenderTarget(const Size& size, Usage usage)
		: RHIResource()
		, m_size(size)
		, m_usage(usage)
		, m_colorAttachments()
		, m_depthAttachment()
		, m_valid(false)
		, m_id(0)
	{

	}

	RHIRenderTarget::~RHIRenderTarget()
	{
		if (m_id > 0)
		{
			glDeleteBuffers(1, &m_id);
		}
	}

	void RHIRenderTarget::setUsage(RHICommandList* cmdList, Usage usage)
	{
		m_usage = usage;
	}

	void RHIRenderTarget::attachColor(const std::shared_ptr<RHITexture>& texture, int32 index)
	{
		auto target = getGLRenderTarget(m_usage);
		glFramebufferTexture(target, GL_COLOR_ATTACHMENT0 + index, texture->getId(), 0);
		m_colorAttachments[index] = texture;
		GL_CHECK()
	}

	void RHIRenderTarget::attachDepth(const std::shared_ptr<RHITexture2D>& texture)
	{
		auto target = getGLRenderTarget(m_usage);
		glFramebufferTexture(target, GL_DEPTH_ATTACHMENT, texture->getId(), 0);
		m_depthAttachment = texture;
		GL_CHECK()
	}

	uint32 RHIRenderTarget::getId()
	{
		if (m_id == 0)
		{
			glGenFramebuffers(1, &m_id);
		}
		return m_id;
	}

	bool RHIRenderTarget::update()
	{
		auto target = getGLRenderTarget(m_usage);
		v_check(target != GL_NONE)
		if (glCheckFramebufferStatus(target) != GL_FRAMEBUFFER_COMPLETE) 
		{
			V_LOG_WARN(Engine, "render target not complete");
			return false;
		}
		m_valid = true;
		return true;
	}

}