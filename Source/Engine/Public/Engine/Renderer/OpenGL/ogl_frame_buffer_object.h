#ifndef __volucris_ogl_framebuffer_object_h__
#define __volucris_ogl_framebuffer_object_h__

#include <unordered_map>
#include "Engine/Core/types_help.h"
#include "Engine/Core/shareable_object.h"

namespace volucris
{
	class Context;
	class Texture2DObject;
	class RenderBufferObject;
	class FrameBufferComponent;

	class FrameBufferObject : public ShareableObject
	{
	public:
		FrameBufferObject();

		~FrameBufferObject() override;

		bool create();

		void release();

		bool updateAttachments(Context* context);

		bool initialize(Context* context);

		bool isValid() const { return m_valid; }

		uint32 getID() const { return m_id; }

		void resize(int w, int h);

		void attachColor(int index, const std::shared_ptr<FrameBufferComponent>& texture)
		{
			m_colorAttachments.insert_or_assign(index, texture);
			dirty();
		}

		void attachDepth(const std::shared_ptr<FrameBufferComponent>& texture)
		{
			m_depthAttachment = texture;
			dirty();
		}

		std::shared_ptr<FrameBufferComponent> getColorAttachment(int index)
		{
			auto it = m_colorAttachments.find(index);
			if (it != m_colorAttachments.end())
			{
				return it->second;
			}
			return nullptr;
		}

		std::shared_ptr<FrameBufferComponent> getDepthAttachment() const
		{
			return m_depthAttachment;
		}

	protected:
		void dirty()
		{
			m_valid = false;
		}

	private:
		uint32 m_id;
		uint8 m_valid;
		std::unordered_map<int, std::shared_ptr<FrameBufferComponent>> m_colorAttachments;
		std::shared_ptr<FrameBufferComponent> m_depthAttachment;
	};
}

#endif // !__volucris_ogl_framebuffer_object_h__
