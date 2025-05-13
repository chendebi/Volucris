#ifndef __volucris_material_h__
#define __volucris_material_h__

#include <string>
#include <memory>
#include <vector>
#include <map>
#include "material_parameter.h"
#include "Engine/Resource/render_resource.h"

namespace volucris
{
	class MaterialProxy;
	class MaterialResourceProxy;

	class MaterialResource : public RenderResource
	{
	public:
		using MaterialParameterMap = std::unordered_map<MaterialParameter::Type, std::vector<std::string>>;

		MaterialResource();

		MaterialResource(const std::string& vss, const std::string& fss);

		void setSource(const std::string& vss, const std::string& fss);

		void setParameters(const MaterialParameterMap& parameters)
		{
			m_parameters = parameters;
		}

		MaterialResourceProxy* getProxy() const { return m_proxy; }

		MaterialResourceProxy* attachProxy();

		void deattachProxy();

		std::string getVertexShaderSource() const { return m_vss; }
		std::string getFragmentShaderSource() const { return m_fss; }

	protected:
		void releaseRenderProxy() override;

	private:
		friend class Material;
		std::string m_vss;
		std::string m_fss;
		MaterialParameterMap m_parameters;
		MaterialResourceProxy* m_proxy;
	};

	class Material : public RenderResource
	{
	public:
		Material(const std::shared_ptr<MaterialResource>& resource);

		~Material();

		MaterialProxy* getProxy() const { return m_proxy; }

		MaterialProxy* attachProxy();

		void deattachProxy();

		std::shared_ptr<MaterialResource> getResource() const { return m_resource; }

	protected:
		void releaseRenderProxy() override;

	private:
		std::shared_ptr<MaterialResource> m_resource;
		std::unordered_map<std::string, std::unique_ptr<MaterialParameter>> m_parameters;
		MaterialProxy* m_proxy;
	};
}

#endif // !__volucris_material_h__
