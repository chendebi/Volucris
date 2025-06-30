#include <RHI/RHIRenderTarget.h>
#include <glad/glad.h>
#include <RHI/RHIState.h>
#include <RHI/RHICommandList.h>
#include <Core/Volucris.h>
#include "RHIOpenGL.h"

namespace volucris
{
	RHIRenderTarget::RHIRenderTarget(const Size& size)
		: RHIResource()
		, m_size(size)
		, m_colorAttachments()
		, m_depthAttachment()
	{

	}

	void RHIRenderTarget::attachColor(RHITextureDesc desc, int32 index)
	{
		desc.size = m_size;
		m_colorAttachments[index] = RHICreateTexture(desc);
	}


	bool RHIRenderTarget::init(RHICommandList* command)
	{
		command->bindResource(this);
		for (const auto& [idx, attachment] : m_colorAttachments)
		{
			if (!attachment->init(command))
			{
				return false;
			}

			if (attachment->isA<RHITexture>())
			{
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, attachment->getId(), 0);
			}
			else
			{
				// 支持RBO
				v_check(false);
				return false;
			}
		}

		if (m_depthAttachment)
		{
			if (!m_depthAttachment->init(command))
			{
				return false;
			}

			if (m_depthAttachment->isA<RHITexture>())
			{
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthAttachment->getId(), 1);
			}
			else
			{
				// 支持RBO
				v_check(false);
				return false;
			}
		}

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			V_LOG_WARN(Engine, "frame buffer init failed.{}", status)
		}

		GL_CHECK()
		return true;
	}

	uint32 RHIRenderTarget::create(RHIState* state)
	{
		uint32 id;
		glGenFramebuffers(1, &id);

		return id;
	}

	void RHIRenderTarget::bind(RHIState* state)
	{
		if (state->renderTarget == this)
		{
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, getId());
		state->renderTarget = this;
	}

	void RHIRenderTarget::destroy(RHIState* state)
	{
		auto command = state->commandList;
		for (auto & [idx, attachment] : m_colorAttachments)
		{
			if (attachment)
			{
				command->deleteResource(attachment.get());
			}
		}

		if (m_depthAttachment)
		{
			command->deleteResource(m_depthAttachment.get());
		}

		if (state->renderTarget == this)
		{
			state->renderTarget = nullptr;
		}

		uint32 id = getId();
		glDeleteFramebuffers(1, &id);
	}
}