#ifndef __volucris_add_resource_dialog_h__
#define __volucris_add_resource_dialog_h__

#include <vector>
#include <Engine/Application/widget.h>

namespace volucris
{
	class PathTreeWidget;

	class AddResourceDialog : public Widget
	{
	public:
		AddResourceDialog(PathTreeWidget* pathWidget);

		void build() override;

		void setOpened()
		{
			m_opened = true;
			m_showHint = false;
			m_filePath0[0] = '\0';
			m_filePath1[0] = '\0';
		}

	protected:
		bool addResource();
		bool addMaterialResource();
		bool addMeshResource();

	protected:
		void buildMaterialInput();
		void buildMeshInput();
		void buildPathSelector(const char* filter, const char* title, std::vector<char>& path);

	private:
		bool m_opened;
		bool m_showHint;
		int m_currentIdx;
		std::vector<char> m_buffer;
		std::vector<char> m_filePath0;
		std::vector<char> m_filePath1;
		PathTreeWidget* m_pathWidget;
	};
}

#endif // !__volucris_add_resource_widget_h__
