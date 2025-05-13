#ifndef __volucris_material_proxy_h__
#define __volucris_material_proxy_h__

#include <memory>
#include <string>

namespace volucris
{
	class Material;
	class MaterialResource;

	class MaterialResourceProxy
	{
	public:
		MaterialResourceProxy(MaterialResource* resource);

	private:
		std::string m_vss;
		std::string m_fss;
	};

	class MaterialProxy
	{
	public:
		MaterialProxy(Material* material);

		~MaterialProxy();

		void setResource(MaterialResourceProxy* resource)
		{
			m_resource = resource;
		}

	private:
		MaterialResourceProxy* m_resource;
	};
}

#endif // !__volucris_material_proxy_h__
