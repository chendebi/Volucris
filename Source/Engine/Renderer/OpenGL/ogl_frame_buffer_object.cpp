#include "Renderer/OpenGL/ogl_frame_buffer_object.h"
#include "Renderer/context.h"
#include <glad/glad.h>
#include <Renderer/OpenGL/ogl_texture2d_object.h>
#include <Core/volucris.h>
#include <Renderer/OpenGL/ogl_check.h>

namespace volucris
{
	FrameBufferObject::FrameBufferObject()
		: ShareableObject()
		, m_id(0)
		, m_valid(false)
		, m_colorAttachments()
		, m_depthAttachment(nullptr)
	{

	}

	FrameBufferObject::~FrameBufferObject()
	{
		release();
	}

	bool FrameBufferObject::create()
	{
		if (m_id == 0)
		{
			glGenFramebuffers(1, &m_id);
		}
		return m_id > 0;
	}

	void FrameBufferObject::release()
	{
		if (m_id > 0)
		{
			glDeleteFramebuffers(1, &m_id);
			m_id = 0;
		}
	}

	bool FrameBufferObject::updateAttachments(Context* context)
	{
		if (!m_depthAttachment->isValid() && (!m_depthAttachment->create() || !m_depthAttachment->initialize(context)))
		{
			V_LOG_WARN(Engine, "depeth texture initialize failed");
			return false;
		}
		GL_CHECK();
		for (const auto& [idx, attachment] : m_colorAttachments)
		{
			if (!attachment->isValid() && (!attachment->create() || !attachment->initialize(context)))
			{
				V_LOG_WARN(Engine, "color attachment [{}] initialize failed", idx);
				continue;
			}
		}
		GL_CHECK();
		return true;
	}

	bool FrameBufferObject::initialize(Context* context)
	{

		if (m_id == 0)
		{
			return false;
		}

		// 更新依赖的对象
		{
			bool status = updateAttachments(context);;

			if (!status)
			{
				return false;
			}

			if (m_valid)
			{
				return true;
			}
		}

		context->bindFrameBuffer(this);

		if (m_depthAttachment)
		{
			switch (m_depthAttachment->getComponentType())
			{
			case FrameBufferComponent::TEXTURE:
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthAttachment->getID(), 0);
				break;
			case FrameBufferComponent::RENDER_BUFFER:
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthAttachment->getID());
				break;
			default:
				V_LOG_WARN(Engine, "initialize framebuffer object failed. unsupported attachment type");
				return false;
			}
		}

		for (const auto& [idx, attachment] : m_colorAttachments)
		{
			switch (attachment->getComponentType())
			{
			case FrameBufferComponent::TEXTURE:
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, attachment->getID(), 0);
				break;
			case FrameBufferComponent::RENDER_BUFFER:
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_RENDERBUFFER, attachment->getID());
				break;
			default:
				V_LOG_WARN(Engine, "initialize framebuffer object failed. unsupported attachment type");
				return false;
			}
		}

		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			V_LOG_WARN(Engine, "initialize framebuffer object failed. status: {}", (int)status);
			return false;
		}
		GL_CHECK();
		m_valid = true;
		return true;
	}

	void FrameBufferObject::resize(int w, int h)
	{
		if (m_depthAttachment)
		{
			m_depthAttachment->setSize(w, h);
		}

		for (const auto& [idx, attachment] : m_colorAttachments)
		{
			attachment->setSize(w, h);
		}
	}
}
