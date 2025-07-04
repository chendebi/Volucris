#include <Engine/Core/GUID.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <Engine/Core/Logging.h>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <memory> // 添加智能指针支持
#include <boost/serialization/shared_ptr.hpp>

VOLUCRIS_STATIC_LOG(Serialization, Trace)

using namespace volucris;

class Furniture
{
public:
	Furniture() = default;

	Furniture(const std::string& name) : m_name(name) {}

	virtual ~Furniture() = default;

	void setName(const std::string& name)
	{
		m_name = name;
	}

	std::string getName() const
	{
		return m_name;
	}

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& m_name;  // Serialize the device name
	}

private:
	std::string m_name;
};

class Electronics : public Furniture
{
public:
	Electronics() = default;

	Electronics(const std::string& brand) : Furniture(), m_brand(brand) {}

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<Furniture>(*this);
		ar& m_brand;  // Serialize the device name
	}

	void setBrand(const std::string& brand)
	{
		m_brand = brand;
	}

	std::string getBrand() const
	{
		return m_brand;
	}

private:
	std::string m_brand;;
};

class Wooden : public Furniture
{
public:
	enum WoodType
	{
		Unknown,
		Oak,
		Pine,
		Birch,
		Cherry
	};

	Wooden() : Furniture(), m_type(Unknown) {}

	Wooden(WoodType type) : Furniture(), m_type(type) {}

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<Furniture>(*this);
		ar& m_type;  // Serialize the device name
	}

private:
	WoodType m_type;
};

class Phone : public Electronics
{
public:
	Phone() : Electronics(), m_device("Unknown") {}

	Phone(const std::string& brand, const std::string& device) : Electronics(brand), m_device(device) {}

	// Serialization function
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<Electronics>(*this);
		ar& m_device;  // Serialize the device name
	}

	void setDevice(const std::string& device)
	{
		m_device = device;
	}

	std::string getDevice() const
	{
		return m_device;
	}

private:
	std::string m_device;
};

BOOST_CLASS_EXPORT(Furniture);
BOOST_CLASS_EXPORT(Electronics);
BOOST_CLASS_EXPORT(Wooden);
BOOST_CLASS_EXPORT(Phone);

int main()
{
	std::ofstream ofs("s.dat", std::ios::trunc);
	boost::archive::binary_oarchive oa(ofs);

	std::shared_ptr<Furniture> pPhone = std::make_shared<Phone>("Apple", "iPhone 14");
	pPhone->setName("My iPhone");

	oa << pPhone;  // Serialize the phone object
	ofs.close();

	std::ifstream ifs("s.dat");
	boost::archive::binary_iarchive ia(ifs);
	std::shared_ptr<Furniture> furniture = nullptr;
	ia >> furniture;  // Deserialize into a Furniture pointer
	ifs.close();

	if(auto phonePtr = dynamic_cast<Phone*>(furniture.get()))
	{
		V_LOG_INFO(Serialization, "Deserialized Phone: Name = {}, Brand = {}, Device = {}",
			phonePtr->getName(), phonePtr->getBrand(), phonePtr->getDevice());
	}
	else
	{
		V_LOG_ERROR(Serialization, "Deserialization failed or object is not a Phone.");
	}
}