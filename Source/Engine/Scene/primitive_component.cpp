#include "Scene/primitive_component.h"
#include <Core/assert.h>

namespace volucris
{
	PrimitiveComponent::PrimitiveComponent()
		: SceneComponent()
	{
	}

	void PrimitiveComponent::setVertices(float* data, size_t size)
	{
		auto capacity = size / sizeof(glm::vec3);
		checkf(capacity * sizeof(glm::vec3) == size, Engine, "vertex data invalid");
	}
}
