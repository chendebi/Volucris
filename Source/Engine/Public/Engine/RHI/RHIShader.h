#ifndef __volucris_rhi_shader_h__
#define __volucris_rhi_shader_h__

#include <Engine/RHI/RHIResource.h>
#include <string>

namespace volucris
{
	class RHIShader : public RHIResource
	{
	public:
		enum ShaderType
		{
			VertexShader,
			FragmentShader
		};

	public:
		RHIShader(ShaderType shaderType, const std::string& source);

		bool init(RHICommandList* command) override;

		ShaderType getShaderType() const { return m_shaderType; }

	protected:
		uint32 create(RHIState* state);

		void destroy(RHIState* state);

	private:
		ShaderType m_shaderType;
		std::string m_source;
	};
}

#endif // !__volucris_rhi_shader_h__
