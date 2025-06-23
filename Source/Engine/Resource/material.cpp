#include "Resource/material.h"
#include "Resource/material_resource.h"
#include "Core/volucris.h"
#include "Renderer/material_proxy.h"
#include <Application/application.h>
#include "Renderer/renderer.h"
#include "Core/assert.h"
#include <Resource/resource_registry.h>
#include <Resource/material_parameter.h>
#include <Resource/texture2d.h>

namespace volucris
{
	Material::Material()
		: ResourceObject(Asset::MATERIAL)
		, m_resource(nullptr)
		, m_parameters()
		, m_proxy()
	{
		
	}

	Material::Material(const std::shared_ptr<MaterialResource>& resource)
		: Material()
	{
		setMaterialResource(resource);
	}

	Material::Material(const std::shared_ptr<Material>& parent)
		: Material()
	{
		m_parent = parent;
		setMaterialResource(m_parent->getResource());
	}

	Material::~Material()
	{
#if WITH_EDITOR
		if (m_resource)
		{
			m_resource->Rebuild.removeAll(this);
		}
#endif
	}

	std::shared_ptr<MaterialProxy> Material::getRenderProxy()
	{
		auto proxy = m_proxy.lock();
		if (!proxy)
		{
			proxy = std::make_shared<MaterialProxy>();
			std::vector<std::shared_ptr<Texture2DProxy>> textures;
			for (const auto& parameter : m_textureParameters)
			{
				if (auto texture2d = parameter->getTexture())
				{
					textures.push_back(texture2d->getProxy());
				}
				else
				{
					textures.push_back(nullptr);
				}
			}
			proxy->setResource(m_resource->getRenderProxy());
			proxy->updateParameterRenderData(m_bufferData, std::move(textures));
			m_proxy = proxy;

			V_LOG_DEBUG(Engine, "create material: {}", getAsset().getAssetPath())
		}
		return proxy;
	}

	void Material::update()
	{
		if (isDirty())
		{
			m_resource->update();
			if (auto proxy = m_proxy.lock())
			{
				auto resource = m_resource->getRenderProxy();
				std::vector<std::shared_ptr<Texture2DProxy>> textures;
				for (const auto& parameter : m_textureParameters)
				{
					if (auto texture2d = parameter->getTexture())
					{
						textures.push_back(texture2d->getProxy());
					}
					else
					{
						textures.push_back(nullptr);
					}
				}
				
				gApp->getRenderer()->pushCommand([proxy, resource, bufferData = m_bufferData, textures]() {
					proxy->setResource(resource);
					proxy->updateParameterRenderData(bufferData, std::move(textures));
					});
			}
			markDirty(false);
		}
		else
		{
			bool update = false;
			for (const auto& paramter : m_parameters)
			{
				if (paramter->isDirty())
				{
					updateParameterData(m_bufferData.data(), paramter->getDescription().offset, paramter->getDescription().size);
					update = true;
				}
			}
			if (!update)
			{
				return;
			}
			if (auto proxy = m_proxy.lock())
			{
				std::vector<std::shared_ptr<Texture2DProxy>> textures;
				for (const auto& parameter : m_textureParameters)
				{
					if (auto texture2d = parameter->getTexture())
					{
						textures.push_back(texture2d->getProxy());
					}
					else
					{
						textures.push_back(nullptr);
					}
				}

				gApp->getRenderer()->pushCommand([proxy, bufferData = m_bufferData, textures]() {
					proxy->updateParameterRenderData(bufferData, std::move(textures));
					});
			}
		}
	}

	MaterialParameter* Material::getParameterByName(const std::string& name)
	{
		for (auto& parameter : m_parameters)
		{
			if (parameter->getDescription().name == name)
			{
				return parameter.get();
			}
		}

		for (const auto& parameter : m_textureParameters)
		{
			if (parameter->getDescription().name == name)
			{
				return parameter.get();
			}
		}
		return nullptr;
	}

	bool Material::serialize(Serializer& serializer) const
	{
		if (m_parent)
		{
			serializer << 1 << m_parent->getAsset().getAssetPath();
		}
		else
		{
			serializer << 2;
			m_resource->serialize(serializer);
		}
		
		serializer.serialize(m_bufferData.data(), m_bufferData.size());

		uint32 textureParameterSize = m_textureParameters.size();
		serializer.serialize(textureParameterSize);
		for (const auto& texture : m_textureParameters)
		{
			serializer.serialize(texture->getTexture()->getAsset().assetPath);
		}

		return true;
	}

	void Material::deserialize(Serializer& serializer)
	{
		int32 type = 0;
		serializer >> type;
		if (type == 1)
		{
			std::string parentPath;
			serializer >> parentPath;
			if (auto parent = ResourceRegistry::Instance().loadResource<Material>(parentPath))
			{
				m_parent = parent;
				setMaterialResource(m_parent->getResource());
			}
			else
			{
				V_LOG_ERROR(Engine, "Failed to load material parent: {}", parentPath);
			}
		}
		else if (type == 2)
		{
			auto resource = std::make_shared<MaterialResource>();
			resource->deserialize(serializer);
			setMaterialResource(resource);
		}

		std::vector<uint8> bufferData;
		size_t size = 0;
		serializer.deserialize(bufferData, size);

		v_check(size == m_resource->getBufferSize());

		m_bufferData = std::move(bufferData);

		uint32 textureParameterSize = 0;
		serializer.deserialize(textureParameterSize);
		v_check(textureParameterSize == m_textureParameters.size())
		for (auto idx = 0; idx < textureParameterSize; ++idx)
		{
			TSoftObjectPtr<Texture2D> texture;
			serializer.deserialize(texture);
			m_textureParameters[idx]->setTexture(texture.tryLoad());
		}
	}

	void Material::setMaterialResource(const std::shared_ptr<MaterialResource>& resource)
	{
		m_resource = resource;
		onSourceRebuild(resource.get());
#if WITH_EDITOR
		resource->Rebuild.addObject(this, &Material::onSourceRebuild);
#endif
	}

	void Material::onSourceRebuild(MaterialResource* resource)
	{
		v_check(resource == m_resource.get());
		m_parameters.clear();
		const auto& descriptions = resource->getParameterDescriptions();
		m_parameters.reserve(descriptions.size());
		m_bufferData.resize(resource->getBufferSize());
		int texSlot = 0;
		for (const auto& description : descriptions)
		{
			switch (description.type)
			{
			case MaterialParameterType::FLOAT:
				m_parameters.push_back(std::make_shared<MaterialParameterFloat>(description));
				break;
			case MaterialParameterType::VEC2:
				break;
			case MaterialParameterType::VEC3:
				m_parameters.push_back(std::make_shared<MaterialParameterVec3>(description));
				break;
			case MaterialParameterType::VEC4:
				break;
			case MaterialParameterType::TEXTURE2D:
				updateParameterData(&texSlot, description.offset, description.size);
				m_textureParameters.push_back(std::make_shared<MaterialParameterTexture2D>(description));
				break;
			default:
				break;
			}
		}
		dirty();
	}

	void Material::updateParameterData(void* data, size_t offset, size_t size)
	{
		memcpy(m_bufferData.data() + offset, data, size);
	}
}
