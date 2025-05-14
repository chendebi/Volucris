#ifndef __volucris_material_proxy_h__
#define __volucris_material_proxy_h__

#include <memory>
#include <string>
#include <vector>
#include "Engine/Renderer/OpenGL/ogl_program_object.h"
#include "Engine/Renderer/OpenGL/ogl_render_state.h"
#include "Engine/Renderer/OpenGL/ogl_uniform.h"

namespace volucris
{
	class Material;
	class MaterialResource;
	//struct UniformDescription;

	class MaterialResourceProxy
	{
	public:
		MaterialResourceProxy(MaterialResource* resource);

	private:
		friend class MaterialProxy;
		std::string m_vss;
		std::string m_fss;
		std::unique_ptr<OGLProgramObject> m_program;
		std::vector<std::shared_ptr<UniformDescription>> m_descriptions;
	};

	class MaterialProxy
	{
	public:
		MaterialProxy(Material* material);

		~MaterialProxy();

		void setResource(MaterialResourceProxy* resource);

		const OGLProgramState& getState() const { return m_state; }

	private:
		MaterialResourceProxy* m_resource;
		OGLProgramState m_state;
		std::vector<uint8> m_parameterData;
	};
}

#endif // !__volucris_material_proxy_h__
