#ifndef __volucris_editor_application_h__
#define __volucris_editor_application_h__

#include <Engine/Application/Application.h>
#include <Engine/Game/World.h>

namespace volucris
{
	class ContentBrowserWidget;

	class EditorWorld : public World
	{
	public:
		EditorWorld();
	};

	class EditorApplication : public Application
	{
	public:
		EditorApplication();

		EditorWorld* getEditorWorld() { return m_world.get(); }

	private:
		std::unique_ptr<EditorWorld> m_world;
	};
}

#define GEditorWorld ((EditorApplication*)gApp)->getEditorWorld()

#endif // !__volucris_editor_application_h__
