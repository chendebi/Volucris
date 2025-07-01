#include <Engine/Core/Delegate.h>
#include <Engine/Core/Object.h>

DECLARE_EVENT_MUTI_DELEGATE(NumChanged, void, int)

void numChangedHandler(int newValue)
{
	// Handle the number change
	printf("Number changed to: %d\n", newValue);
}

void numChangedHandler2(int newValue)
{
	printf("Number changed to: %d\n", newValue);
}

class TestObject : public volucris::Object
{
public:
	TestObject() : Object() {}

	void onNumChanged(int newValue)
	{
		printf("TestObject: Number changed to: %d\n", newValue);
	}
};

int main()
{
	NumChanged onNumChanged = NumChanged();
	onNumChanged.bind(numChangedHandler);

	auto obj = std::make_shared<TestObject>();
	onNumChanged.bindObject(obj.get(), &TestObject::onNumChanged);
	onNumChanged.invoke(42);
	//obj = nullptr;
	onNumChanged.unbind(obj.get());
	onNumChanged.invoke(41);
}