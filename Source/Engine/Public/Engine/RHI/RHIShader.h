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
		RHIShader(ShaderType shaderType);

		~RHIShader() override;

		uint32 getId();

		bool init(const std::string& source);

		ShaderType getShaderType() const { return m_shaderType; }

	private:
		ShaderType m_shaderType;
		bool m_valid;
		uint32 m_id;
	};
}

#endif // !__volucris_rhi_shader_h__
