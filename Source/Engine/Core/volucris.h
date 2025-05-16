#ifndef __volucris_volucris_h__
#define __volucris_volucris_h__

#include <string_view>
#include "Core/logging.h"
#include "assert.h"

VOLUCRIS_DEFINE_LOG(Engine)

namespace volucris
{
	constexpr std::string_view MODEL_MATRIX_UNIFORM_NAME = "v_modelMat";
	constexpr std::string_view CAMERA_INFO_BLOCK_NAME = "v_cameraInfo";
}

#endif // !__volucris_volucris_h__
