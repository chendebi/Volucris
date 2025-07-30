#ifndef __volucris_asset_command_h__
#define __volucris_asset_command_h__

namespace volucris
{
	class ItemContext;
	class AssetContext;
	class FolderContext;

	class MenuContextCommands
	{
	public:
		MenuContextCommands();

		MenuContextCommands(ItemContext* context);

		virtual ~MenuContextCommands() = default;

		virtual void execute() = 0;

	protected:
		ItemContext* m_context;
	};

	class RenameCommand : public MenuContextCommands
	{
	public:
		RenameCommand(ItemContext* context);

		void execute() override;
	};

	class DeleteFolderCommand : public MenuContextCommands
	{
	public:
		DeleteFolderCommand(FolderContext* context);

		void execute() override;
	};

	class DeleteAssetCommand : public MenuContextCommands
	{
	public:
		DeleteAssetCommand(AssetContext* context);

		void execute() override;
	};
}

#endif // !__volucris_asset_command_h__
