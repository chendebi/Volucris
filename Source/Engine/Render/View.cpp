#include <Render/View.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIBuffer.h>
#include <RHI/RHIRenderTarget.h>
#include <Core/Volucris.h>
#include <Render/Renderer.h>
#include <RHI/RHIPixelBuffer.h>
#include <RHI/RHIVertexBuffer.h>
#include <RHI/RHIVertexArray.h>
#include <RHI/RHIProgram.h>
#include <RHI/RHIShader.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

constexpr int FrameCount = 2;

namespace volucris
{

	static std::shared_ptr<RHIVertexArray> vao = nullptr;
	static std::shared_ptr<RHIElementBuffer> ebo = nullptr;
	static std::shared_ptr<RHIProgram> shader = nullptr;

	static glm::vec3 vertices[] = {
		{-0.5f, -0.5f, 1.0f},
		{ 0.5f, -0.5f, 1.0f},
		{ 0.5f,  0.5f, 1.0f},
		{-0.5f,  0.5f, 1.0f},
	};

	static uint32 indices[] = {
		0, 1, 2, 0, 2, 3
	};

	static char* vss = R"(
		#version 330 core
		layout(location=0) in vec3 v_pos;
		void main() {
		gl_Position=vec4(v_pos, 1.0);}
	)";

	static char* fss = R"(
		#version 330 core
		layout (location=0) out vec4 color;
		void main() {  color = vec4(1.0, 0.0, 0.0, 1.0); }
	)";

	View::View()
		: m_targets()
		, m_targetReaders()
		, m_targetData()
		, m_current(0)
		, m_scene(nullptr)
	{
	}

	View::View(const std::shared_ptr<Scene>& scene)
		: View()
	{
		m_scene = scene;
	}


	View::~View()
	{
		for (auto& target : m_targets)
		{
			RHICmdList->unsetRenderTarget(target.get());
		}
		ebo = nullptr;
		vao = nullptr;
	}

	void View::resize(int width, int height)
	{
		if (!vao)
		{
			auto vbo = std::make_shared<RHIVertexBuffer>(RHICmdList);
			vbo->createGpuResource();
			RHICmdList->setBuffer(vbo.get());
			vbo->init((uint8*)vertices, sizeof(vertices));

			RHIVertexBuffer::Description desc;
			desc.location = 0;
			desc.normalized = false;
			desc.offset = 0;
			desc.size = 3;
			desc.type = RHIVertexBuffer::Float;
			desc.stride = 3 * sizeof(float);
			vbo->setDescriptions({ desc });

			vao = std::make_shared<RHIVertexArray>(RHICmdList);
			vao->init(vbo);

			ebo = std::make_shared<RHIElementBuffer>(RHICmdList);
			ebo->createGpuResource();
			v_check(RHICmdList->setBuffer(ebo.get()));
			ebo->init((uint8*)indices, (uint32)sizeof(indices), RHIElementBuffer::UInt);

			auto vs = std::make_shared<RHIShader>(RHIShader::VertexShader);
			vs->init(vss);

			auto fs = std::make_shared<RHIShader>(RHIShader::FragmentShader);
			fs->init(fss);

			shader = std::make_shared<RHIProgram>();
			shader->init({ vs, fs });
		}

		if (width <= 8 || height <= 8)
		{
			width = height = 8;
		}

		V_LOG_INFO(Engine, "resize view to {}x{}", width, height);

		for (auto& target : m_targets)
		{
			RHICmdList->unsetRenderTarget(target.get());
		}
		
		m_targets.clear();
		m_targetReaders.clear();

		RHITextureDesc desc;
		desc.size = { width, height };
		desc.pixelFormat = Texture::EPixelFormat::R8G8B8;

		auto size = 3 * width * height;
		m_targetData.format = getSourceFormat(desc.pixelFormat);
		m_targetData.size = { width, height };
		m_targetData.data.resize(size);

		for (int i = 0; i < FrameCount; ++i)
		{
			// 初始化贴图
			auto texture = std::make_shared<RHITexture2D>(desc);
			texture->setContext(RHICmdList);
			texture->createGpuResource();
			RHICmdList->setTexture2D(texture.get());
			texture->init();

			// 
			auto target = std::make_unique<RHIRenderTarget>(Size(width, height));
			target->setContext(RHICmdList);
			target->createGpuResource();
			RHICmdList->setRenderTarget(target.get());
			target->attachColor(texture, 0);
			v_check(target->update())
			m_targets.emplace_back(std::move(target));

			auto reader = std::make_unique<RHIReadPixelBuffer>(RHIBuffer::StreamRead);
			reader->setContext(RHICmdList);
			reader->createGpuResource();
			v_check(RHICmdList->setBuffer(reader.get()))
			reader->init(nullptr, (uint32)size);
			m_targetReaders.emplace_back(std::move(reader));
		}

		m_current = 0;
	}

	void View::render(RHICommandList* cmdList)
	{
		RENDER_SCOPE(View);
		Rect vp;
		vp.setPoint({ 0,0 });
		vp.setSize(m_targets[m_current]->getSize());
		cmdList->setRenderTarget(m_targets[m_current].get(), vp);
		RHIClearState state;
		state.color = { 0.0, 0.8, 1.0, 1.0 };
		cmdList->clear(state);

		RHICmdList->drawPrimitive(shader.get(), vao.get(), ebo.get());

		swapViewData(cmdList);
	}

	void View::swapViewData(RHICommandList* cmdList)
	{
		Rect rect = { 0,0,0,0 };
		rect.setSize(m_targets[m_current]->getSize());

		int next = (m_current + 1) % FrameCount;
		m_targetReaders[next]->startRead(rect, m_targets[m_current].get(), 0);

		m_targetReaders[m_current]->readColorTo(m_targetData.data);
		m_current = next;
	}
}