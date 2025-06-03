#ifndef __volucris_material_loader_h__
#define __volucris_material_loader_h__

#include <Engine/Resource/material_resource.h>

namespace volucris 
{
	class MaterialLoader
	{
	public:
		MaterialLoader() = default;

		std::shared_ptr<MaterialResource> load(const std::string& vsf, const std::string& fsf);

		static bool reload(Material* material);
	};
}

#endif // !__volucris_material_loader_h__
