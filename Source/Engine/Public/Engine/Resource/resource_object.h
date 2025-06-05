#ifndef __volucris_resource_object_h__
#define __volucris_resource_object_h__

#include <vector>
#include <string>
#include "Engine/Resource/resource_path.h"
#include <Engine/Resource/asset_path.h>
#include <Engine/Core/serializer.h>
#include <Engine/Core/types_help.h>
#include "Engine/Resource/meta_data.h"

namespace volucris
{
	class ResourceObject
	{
	public:
		ResourceObject(AssetType type);

		virtual ~ResourceObject() = default;

		void setAsset(const Asset& asset);

		const Asset& getAsset() const { return m_asset; }

		void dirty() { markDirty(true); }

		void markDirty(bool dirty) { m_dirty = dirty; if (dirty) { onResourceDirty(); } }

		bool isDirty() const { return m_dirty; }

		virtual bool serialize(Serializer& serializer) const = 0;
		
		virtual void deserialize(Serializer& serializer) = 0;

	protected:
		virtual void onResourceDirty() {}

	private:
		uint8 m_dirty;
		Asset m_asset;
	};
}

#endif // !__volucris_resource_object_h__
