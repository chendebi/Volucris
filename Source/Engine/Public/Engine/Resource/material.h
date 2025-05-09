#ifndef __volucris_material_h__
#define __volucris_material_h__

#include <string>
#include "Engine/Resource/resource_object.h"

namespace volucris
{
	class MaterialProxy;

	class MaterialParameter
	{

	};

	class Material : public ResourceObject
	{
	public:
		Material();

		Material(const std::string& vss, const std::string& fss);

		void setSource(const std::string& vss, const std::string& fss);

		MaterialProxy* getProxy() const { return m_proxy; }

	private:
		std::string m_vss;
		std::string m_fss;
		MaterialProxy* m_proxy;
	};
}

#endif // !__volucris_material_h__
