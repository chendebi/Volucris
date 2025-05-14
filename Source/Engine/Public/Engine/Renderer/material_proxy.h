#ifndef __volucris_material_proxy_h__
#define __volucris_material_proxy_h__

#include <memory>
#include <string>
#include "Engine/Renderer/OpenGL/ogl_program_object.h"
#include "Engine/Renderer/OpenGL/ogl_render_state.h"

namespace volucris
{
	class Material;
	class MaterialResource;

	class MaterialResourceProxy
	{
	public:
		MaterialResourceProxy(MaterialResource* resource);

	private:
		friend class MaterialProxy;
		std::string m_vss;
		std::string m_fss;
		std::unique_ptr<OGLProgramObject> m_program;
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
	};
}

#endif // !__volucris_material_proxy_h__
