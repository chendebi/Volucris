#ifndef __volucris_material_resource_h__
#define __volucris_material_resource_h__

#include <Engine/Resource/resource_object.h>
#include "Engine/Resource/material_parameter.h"
#include <Engine/Core/event.h>
#include <Engine/Renderer/material_inner_data.h>

namespace volucris
{
#if WITH_EDITOR
	class MaterialResource;
	DECLARE_EVENT(OnMaterialResouceRebuild, MaterialResource*)
#endif

	class MaterialResourceProxy;

	class MaterialResource
	{
	public:
#if WITH_EDITOR
		OnMaterialResouceRebuild Rebuild;
#endif

	public:
		MaterialResource();

		~MaterialResource();

		MaterialResource(const std::string& vss, const std::string& fss);

		void setSource(const std::string& vss, const std::string& fss);

		void setParameterDescriptions(const std::vector<MaterialParameterDescription>& descriptions);

		void setEngineInnerParameters(MaterialInnerParameters parameters) { m_innerParameters = parameters; }

		void dirty();

		void update();

		std::shared_ptr<MaterialResourceProxy> getRenderProxy();

		std::string getVertexShaderSource() const { return m_vss; }

		std::string getFragmentShaderSource() const { return m_fss; }

		MaterialInnerParameters getEngineInnerParameters() const { return m_innerParameters; }

		const std::vector<MaterialParameterDescription>& getParameterDescriptions() const { return m_descriptions; }

		bool serialize(Serializer& serializer) const;

		void deserialize(Serializer& serializer);

	private:
		uint8 m_dirty;
		std::string m_vss;
		std::string m_fss;
		MaterialInnerParameters m_innerParameters;
		std::vector<MaterialParameterDescription> m_descriptions;
		std::weak_ptr<MaterialResourceProxy> m_proxy;
	};

}

#endif // !__volucris_material_resource_h__
