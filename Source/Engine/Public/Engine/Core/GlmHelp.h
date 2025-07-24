#ifndef __volucris_glm_help_h__
#define __volucris_glm_help_h__

#include <glm/glm.hpp>
#include <boost/serialization/split_free.hpp> // 非侵入式支持
#include <boost/serialization/array.hpp>

// 声明非侵入式序列化
BOOST_SERIALIZATION_SPLIT_FREE(glm::vec3)
BOOST_SERIALIZATION_SPLIT_FREE(glm::vec4)

namespace boost::serialization {
    template <typename Archive>
    void save(Archive& ar, const glm::vec3& v, const unsigned int version) {
        float* data = glm::value_ptr(v);
        ar& boost::serialization::make_array<float>(data, 3);
    }

    template <typename Archive>
    void load(Archive& ar, glm::vec3& v, const unsigned int version) {
        float* data = glm::value_ptr(v);
        ar& boost::serialization::make_array<float>(data, 3);
    }

    template <typename Archive>
    void save(Archive& ar, const glm::vec4& v, const unsigned int version) {
        float* data = glm::value_ptr(v);
        ar& boost::serialization::make_array<float>(data, 4);
    }

    template <typename Archive>
    void load(Archive& ar, glm::vec4& v, const unsigned int version) {
        float* data = glm::value_ptr(v);
        ar & boost::serialization::make_array<float>(data, 4);
    }

    template <typename Archive>
    void save(Archive& ar, const glm::mat4& mat, const unsigned int version) {
        float* data = glm::value_ptr(mat);
        ar & boost::serialization::make_array<float>(data, 16);
    }

    template <typename Archive>
    void load(Archive& ar, const glm::mat4& mat, const unsigned int version) {
        float* data = glm::value_ptr(matrix);
        ar& make_array<float>(data, 16); // 反序列化整个矩阵数据块
    }
}

namespace volucris
{
	namespace GlmHelp
	{
	}
}

#endif // !__volucris_glm_help_h__
