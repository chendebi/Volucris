#ifndef __volucris_material_proxy_h__
#define __volucris_material_proxy_h__

#include <memory>
#include <string>
#include <vector>
#include "Engine/Renderer/OpenGL/ogl_program_object.h"
#include "Engine/Renderer/OpenGL/ogl_render_state.h"
#include "Engine/Renderer/OpenGL/ogl_uniform.h"
#include <Engine/Core/material_global.h>

namespace volucris
{
	class Material;
	class MaterialResource;
	class Texture2DProxy;

	struct MaterialRenderData
	{
		std::string vss;
		std::string fss;
		std::vector<MaterialParameterDescription> descriptions;
	};

	class MaterialResourceProxy
	{
	public:
		MaterialResourceProxy();

		void update(const MaterialRenderData& data);

		OGLProgramObject* getProgramObject() const { return m_program.get(); }

		std::vector<MaterialParameterDescription> getDescriptions() const { return m_descriptions; }

		std::vector<std::unique_ptr<UniformValue>> createUniformValues() const;

		bool ready();

		Uniform* getCameraInfoBlockUniform() const { return m_cameraInfoUniformBlock.get(); }
		Uniform* getPrimitiveInfoBlockUniform() const { return m_primitiveInfoUniformBlock.get(); }
		Uniform* getDirectionLightBlockUniform() const { return m_directLightUniformBlock.get(); }

	private:
		uint8 m_dirty;
		std::string m_vss;
		std::string m_fss;
		std::unique_ptr<OGLProgramObject> m_program;
		std::vector<MaterialParameterDescription> m_descriptions;
		std::vector<std::shared_ptr<Uniform>> m_uniforms;
		std::unique_ptr<Uniform> m_cameraInfoUniformBlock;
		std::unique_ptr<Uniform> m_primitiveInfoUniformBlock;
		std::unique_ptr<Uniform> m_directLightUniformBlock;
	};

	class MaterialProxy
	{
	public:
		MaterialProxy();

		~MaterialProxy();

		void setResource(const std::shared_ptr<MaterialResourceProxy>& resource) 
		{ 
			m_resource = resource;
			markResourceDirty();
		}

		void updateParameterRenderData(std::vector<uint8> buffer, std::vector<std::shared_ptr<Texture2DProxy>> textures);

		const MaterialState& getState() const { return m_state; }

		const std::shared_ptr<MaterialResourceProxy>& getResource() const { return m_resource; }

		void update();

		void markResourceDirty() { m_initialized = false; }

		void dirty() { m_dirty = true; }

	private:
		uint8 m_initialized;
		uint8 m_dirty;
		std::shared_ptr<MaterialResourceProxy> m_resource;
		std::vector<std::shared_ptr<Texture2DProxy>> m_textures;
		std::vector<uint8> m_bufferData;
		std::vector<std::unique_ptr<UniformValue>> m_uniformValues;
		MaterialState m_state;
	};
}

#endif // !__volucris_material_proxy_h__
