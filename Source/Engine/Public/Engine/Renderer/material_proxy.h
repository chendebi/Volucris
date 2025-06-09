#ifndef __volucris_material_proxy_h__
#define __volucris_material_proxy_h__

#include <memory>
#include <string>
#include <vector>
#include "Engine/Renderer/OpenGL/ogl_program_object.h"
#include "Engine/Renderer/OpenGL/ogl_render_state.h"
#include "Engine/Renderer/OpenGL/ogl_uniform.h"
#include <Engine/Renderer/material_inner_data.h>

namespace volucris
{
	class Material;
	class MaterialResource;
	class Texture2DProxy;

	struct MaterialRenderData
	{
		std::string vss;
		std::string fss;
		std::vector<std::string> parameterNames;
		MaterialUniformBlocks engineParameters;
	};

	class MaterialResourceProxy
	{
	public:
		MaterialResourceProxy();

		void update(const MaterialRenderData& data);

		OGLProgramObject* getProgramObject() const { return m_program.get(); }

	private:
		std::string m_vss;
		std::string m_fss;
		std::unique_ptr<OGLProgramObject> m_program;
		std::vector<std::string> m_parameterNames;
	};

	struct MaterialParameterRenderData
	{
		std::vector<std::shared_ptr<UniformValue>> values;
		std::vector<std::shared_ptr<Texture2DProxy>> textures;
	};

	class MaterialProxy
	{
	public:
		MaterialProxy();

		~MaterialProxy();

		void updateParameterRenderData(MaterialParameterRenderData renderData);

		const OGLProgramState& getState() const { return m_state; }

		const std::shared_ptr<MaterialResourceProxy>& getResource() const { return m_resource; }

	private:
		std::shared_ptr<MaterialResourceProxy> m_resource;
		std::vector<std::shared_ptr<Texture2DProxy>> m_textures;
		MaterialParameterRenderData m_renderData;
		OGLProgramState m_state;
	};
}

#endif // !__volucris_material_proxy_h__
