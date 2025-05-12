#ifndef __volucris_material_proxy_h__
#define __volucris_material_proxy_h__

#include "proxy_object.h"
#include <memory>

namespace volucris
{
	class Material;
	class MaterialResourceProxy : public std::enable_shared_from_this<MaterialResourceProxy>, public ProxyObject
	{
	public:
		MaterialResourceProxy(MaterialResource* resource);

		std::shared_ptr<MaterialResourceProxy> getShared()
		{
			return shared_from_this();
		}

	private:
		std::string m_vss;
		std::string m_fss;
	};

	class MaterialProxy : public ProxyObject
	{
	public:
		MaterialProxy(Material* material);

	private:
		std::shared_ptr<MaterialResourceProxy> m_resource;
	};
}

#endif // !__volucris_material_proxy_h__
