#ifndef __volucris_glm_help_h__
#define __volucris_glm_help_h__
#include <glm/glm.hpp>
#include <boost/serialization/split_free.hpp> // 非侵入式支持

// 声明非侵入式序列化
BOOST_SERIALIZATION_SPLIT_FREE(glm::vec3)
BOOST_SERIALIZATION_SPLIT_FREE(glm::vec4)

namespace boost::serialization {
    template <typename Archive>
    void save(Archive& ar, const glm::vec3& v, const unsigned int version) {
        ar << v.x << v.y << v.z; // 直接序列化三个分量
    }

    template <typename Archive>
    void load(Archive& ar, glm::vec3& v, const unsigned int version) {
        ar >> v.x >> v.y >> v.z; // 反序列化三个分量
    }

    template <typename Archive>
    void save(Archive& ar, const glm::vec4& v, const unsigned int version) {
        ar << v.x << v.y << v.z << v.w; // 直接序列化三个分量
    }

    template <typename Archive>
    void load(Archive& ar, glm::vec4& v, const unsigned int version) {
        ar >> v.x >> v.y >> v.z >> v.w; // 反序列化三个分量
    }
}

namespace volucris
{
	namespace GlmHelp
	{
	}
}

#endif // !__volucris_glm_help_h__
