#ifndef __volucris_add_resource_dialog_h__
#define __volucris_add_resource_dialog_h__

#include <vector>
#include <Engine/Application/widget.h>

namespace volucris
{
	class PathTreeWidget;

	struct MatreialInfo
	{
		std::vector<char> vsPath;
		std::vector<char> fsPath;

		MatreialInfo()
			: vsPath()
			, fsPath()
		{
			vsPath.resize(128);
			fsPath.resize(128);
		}
	};

	class AddResourceDialog : public Widget
	{
	public:
		AddResourceDialog(PathTreeWidget* pathWidget);

		void build() override;

		void setOpened()
		{
			m_opened = true;
			m_showHint = false;
		}

	protected:
		bool addResource();
		bool addMaterialResource();

	protected:
		void buildMaterialInput();

	private:
		bool m_opened;
		bool m_showHint;
		int m_currentIdx;
		std::vector<char> m_buffer;
		MatreialInfo m_materialInfo;
		PathTreeWidget* m_pathWidget;
	};
}

#endif // !__volucris_add_resource_widget_h__
