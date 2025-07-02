#ifndef __volucris_content_item_widget_h__
#define __volucris_content_item_widget_h__

#include <Engine/Application/Widget.h>
#include <Engine/Core/Size.h>
#include <Engine/Core/Point.h>
#include <glm/glm.hpp>

namespace volucris
{
	class RHITexture2D;

	class ContentItemWidget
	{
	public:
		ContentItemWidget();

		ContentItemWidget(RHITexture2D* texture, Point iconPos, Size iconSize);

		void setIcon(RHITexture2D* texture, Point iconPos, Size iconSize);

		void setScale(float scale);

		void build();

	private:
		RHITexture2D* m_texture;
		glm::vec2 m_minUV;
		glm::vec2 m_maxUV;
		glm::vec2 m_size;
		glm::vec2 m_iconSpace;
		glm::vec2 m_iconSize;
		float m_fontSize;
		glm::vec2 m_fontRect;
		glm::vec4 m_hoverColor;
		glm::vec4 m_selectedColor;
		bool m_selected;
	};
}

#endif // !__volucris_content_item_widget_h__
