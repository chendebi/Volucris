#include <Engine/Core/GUID.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <Engine/Core/Logging.h>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <memory> // 添加智能指针支持
#include <boost/serialization/shared_ptr.hpp>
#include <random>
#include <boost/serialization/vector.hpp>
#include <Engine/Core/TextureDefines.h>
#include <Engine/Core/Assert.h>

VOLUCRIS_STATIC_LOG(Serialization, Trace)

using namespace volucris;


struct TestData
{
	std::vector<uint8> data;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& data;
	}
};

int main()
{
    std::random_device rd;

    // 2. 初始化随机数引擎（使用 Mersenne Twister 算法）
    std::mt19937_64 engine(rd()); // 使用 64 位版本以获得更好的随机性

    // 3. 创建均匀分布（范围：0 到 uint32_t 的最大值）
    std::uniform_int_distribution<uint32_t> dist(0, std::numeric_limits<uint32_t>::max());

    std::vector<int> data;
    // 生成 10 个随机 uint32_t 示例
    size_t count = 11;
    for (int i = 0; i < count; ++i) {
        //uint32_t random_value = dist(engine);
        data.push_back(i);
    }

    //Texture::TextureData t;
    //t.data.resize(data.size() * sizeof(uint32));
   // memcpy(t.data.data(), data.data(), t.data.size());

    {
        std::ofstream fout("D:\\Projects\\Volucris\\Binaries\\test1.data", std::ios::binary | std::ios::trunc);
        boost::archive::binary_oarchive oa(fout);
        oa << data;

        {
            auto array = boost::serialization::make_array<const int, size_t>(
                static_cast<const int*>(&data[0]),
                count
            );
            std::vector<uint8> data;
            data.resize(44);
            std::memcpy(data.data(), array.address(), 44);
            std::ofstream fout2("D:\\Projects\\Volucris\\Binaries\\test2.data", std::ios::binary | std::ios::trunc);
            fout2.write((const char*)data.data(), 44);
        }

        {
            std::vector<uint8> data;
            data.resize(44);

            std::ifstream fin("D:\\Projects\\Volucris\\Binaries\\test2.data", std::ios::binary);
            fin.read((char*)data.data(), 44);
        }
    }

    {
        std::ifstream fin("D:\\Projects\\Volucris\\Binaries\\test1.data", std::ios::binary);
        boost::archive::binary_iarchive ia(fin);
        std::vector<int> rt;
        ia >> rt;

        for (auto i = 0; i < data.size(); ++i)
        {
            v_check(rt[i] == data[i]);
        }
    }
}