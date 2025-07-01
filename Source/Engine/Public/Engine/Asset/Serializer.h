#ifndef __volucris_serializer_h__
#define __volucris_serializer_h__

#include <Engine/Core/TypesHelp.h>
#include <vector>

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

        void serialize(const uint8* memory, uint64 size)
        {
            m_buffer.reserve(sizeof(uint64) + size);
            serialize(size);
            m_buffer.insert(m_buffer.end(), memory, memory + size);
        }

        bool deserialize(std::vector<uint8>& memory, uint64& size)
        {
            if (deserialize(size) && m_pos + size <= m_buffer.size())
            {
                memory.resize(size);
                memcpy(memory.data(), &m_buffer[m_pos], size);
                m_pos += size;
                return true;
            }
            return false;
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
}

#endif // !__volucris_serializer_h__
