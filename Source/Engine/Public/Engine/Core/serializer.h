#ifndef __volucris_serializer_h__
#define __volucris_serializer_h__

#include <vector>
#include <string>
#include <Engine/Core/types_help.h>

namespace volucris
{
    struct Serializer
    {
    public:
        // 序列化到二进制
        template <typename T>
        void serialize(const T& value);

        // 反序列化
        template <typename T>
        bool deserialize(T& value);

        template <typename T>
        void serialize(const std::vector<T>& values)
        {
            uint32 size = values.size();
            serialize<uint32>(size);
            for (const auto& value : values)
            {
                serialize<T>(value);
            }
        }

        template <typename T>
        bool deserialize(std::vector<T>& outValues)
        {
            uint32 size;
            if (!deserialize<uint32>(size))
            {
                return false;
            }

            std::vector<T> values;
            values.reserve(size);
            for (auto i = 0; i < size; ++i)
            {
                values.emplace_back();
                if (!deserialize<T>(*(values.rbegin())))
                {
                    return false;
                }
            }

            outValues = std::move(values);
            return true;
        }

		template <typename T>
        Serializer& operator<<(const T& value)
        {
            serialize(value);
            return *this;
		}

        template <typename T>
        Serializer& operator>>(T& value)
        {
            if (!deserialize(value))
            {
                throw std::runtime_error("Failed to deserialize value");
            }
            return *this;
		}

        void setData(std::vector<uint8> data)
        {
            m_buffer = std::move(data);
        }

        const std::vector<uint8>& getData() const
        {
            return m_buffer;
		}

    private:
        // 基础类型序列化
        template <typename T>
        void serializePOD(const T& value);

        // 基础类型反序列化
        template <typename T>
        bool deserializePOD(T& value);

    private:
        std::vector<uint8> m_buffer;
        size_t m_pos = 0; // 反序列化时的读取位置
    };
    
    template <typename T>
    inline void Serializer::serializePOD(const T& value) {
        static_assert(std::is_pod<T>::value, "Type must be POD");
        const uint8* bytes = reinterpret_cast<const uint8*>(&value);
        m_buffer.insert(m_buffer.end(), bytes, bytes + sizeof(T));
    }

    // POD类型反序列化
    template <typename T>
    inline bool Serializer::deserializePOD(T& value) {
        static_assert(std::is_pod<T>::value, "Type must be POD");
        if (m_pos + sizeof(T) > m_buffer.size()) {
            return false;
        }
        std::memcpy(&value, &m_buffer[m_pos], sizeof(T));
        m_pos += sizeof(T);
        return true;
    }

#define DEFINE_POD_SERIALIZE(type) \
    template <> \
    inline void Serializer::serialize<type>(const type& value) \
    { \
        serializePOD<type>(value); \
    }\
\
    template <> \
    inline bool Serializer::deserialize<type>(type& value) \
    { \
        return deserializePOD<type>(value); \
    }

    DEFINE_POD_SERIALIZE(int)
    DEFINE_POD_SERIALIZE(float)
    DEFINE_POD_SERIALIZE(double)
    DEFINE_POD_SERIALIZE(char)
    DEFINE_POD_SERIALIZE(uint8)
    DEFINE_POD_SERIALIZE(uint32);


#undef DEFINE_POD_SERIALIZE(type)

    template <>
    inline void Serializer::serialize<std::string>(const std::string& value)
    {
        uint32 length = value.length();
        serializePOD<uint32>(length);
        m_buffer.insert(m_buffer.end(), value.begin(), value.end());
    }

    template <>
    inline bool Serializer::deserialize<std::string>(std::string& value)
    {
        uint32 length;
        if (!deserializePOD<uint32>(length) || m_pos + length > m_buffer.size())
        {
            return false;
        }
        if (length == 0)
        {
            return true;
        }
        std::string str(reinterpret_cast<char*>(&m_buffer[m_pos]), length);
        m_pos += length;
        value = std::move(str);
        return true;
    }

    template <typename T>
    class has_serialize_method {
        template <typename U>
        static auto test(int) -> decltype(std::declval<U>().serialize(std::declval<Serializer&>()), std::true_type());

        template <typename>
        static std::false_type test(...);

    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };

    template<typename T>
    inline void Serializer::serialize(const T& value)
    {
        if constexpr (has_serialize_method<T>::value) {
            // 如果类型有自己的serialize方法
            const_cast<T&>(value).serialize(*this);
        }
        else if constexpr (std::is_pod<T>::value) {
            // POD类型
            serializePOD(value);
        }
        else {
            static_assert(sizeof(T) == 0, "Type not supported for serialization");
        }
    }

    template<typename T>
    inline bool Serializer::deserialize(T& value)
    {
        if constexpr (has_serialize_method<T>::value) {
            // 如果类型有自己的反序列化构造
            value.deserialize(*this);
            return true;
        }
        else if constexpr (std::is_pod<T>::value) {
            // POD类型
            return deserializePOD<T>(value);
        }
        return false;
    }
}

#endif // !__volucris_serializer_h__
