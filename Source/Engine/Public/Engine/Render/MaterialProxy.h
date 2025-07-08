#ifndef __volucris_material_proxy_h__
#define __volucris_material_proxy_h__

#include <Engine/Core/Object.h>
#include <Engine/Render/MaterialParameterInfo.h>
#include <string>
#include <vector>

namespace volucris
{
	class RHICmdList;
	class RHIProgram;

	class MaterialProxy : public Object
	{
	public:
		MaterialProxy();

		void setSource(const std::string& vss, const std::string& fss);

		void setParameters(const std::vector<MaterialParameterInfo>& parameters);

	private:
		std::unique_ptr<RHIProgram> m_program;
	};
}

#endif // !__volucris_material_proxy_h__
