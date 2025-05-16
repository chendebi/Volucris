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
		MaterialResource();

		~MaterialResource();

		MaterialResource(const std::string& vss, const std::string& fss);

		void setSource(const std::string& vss, const std::string& fss);

		void setParameters(const std::vector<MaterialParameterDesc>& parameters)
		{
			m_descriptions = parameters;
		}

		MaterialResourceProxy* getProxy() const { return m_proxy; }

		MaterialResourceProxy* attachProxy();

		void deattachProxy();

		std::string getVertexShaderSource() const { return m_vss; }
		std::string getFragmentShaderSource() const { return m_fss; }

		const std::vector<MaterialParameterDesc>& getParameterDescriptions() const { return m_descriptions; }

	protected:
		void releaseRenderProxy() override;

	private:
		friend class Material;
		std::string m_vss;
		std::string m_fss;
		std::vector<MaterialParameterDesc> m_descriptions;
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

		void updateParametersToRenderer();

		std::shared_ptr<MaterialResource> getResource() const { return m_resource; }

		std::vector<uint8> getParameterData() const { return  m_parameterData; }

		MaterialParameter* getParameterByName(const std::string& name);

		MaterialParameter* getParameterByType(MaterialParameterDesc::Type type);

	protected:
		void releaseRenderProxy() override;

	private:
		std::shared_ptr<MaterialResource> m_resource;
		std::vector<std::unique_ptr<MaterialParameter>> m_parameters;
		std::vector<uint8> m_parameterData;
		MaterialProxy* m_proxy;
	};
}

#endif // !__volucris_material_h__
