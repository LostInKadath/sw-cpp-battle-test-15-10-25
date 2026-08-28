#include "../Framework/TestFramework.hpp"

#include <IO/System/TypeRegistry.hpp>

using namespace sw::tests;

namespace
{
    struct Position { int x{ 0 }, y{ 0 }; };

    struct IShape
    {
        virtual ~IShape() = default;
        virtual int area() const = 0;
    };

    struct Circle : public IShape
    {
        int radius;
        explicit Circle(int r) : radius(r) {}
        int area() const override { return radius * radius; }
    };
}

SW_TEST(TypeRegistry, GetReturnsNullWhenNotRegistered)
{
    sw::TypeRegistry registry;
    SW_CHECK(registry.get<Position>() == nullptr);
}

SW_TEST(TypeRegistry, EmplaceAndGetByConcreteType)
{
    sw::TypeRegistry registry;
    registry.emplace<Position>(3, 4);

    auto position = registry.get<Position>();
    SW_CHECK(position != nullptr);
    SW_CHECK(position->x == 3 && position->y == 4);
}

SW_TEST(TypeRegistry, EmplaceWithInterfaceIsRetrievableByBothTypes)
{
    sw::TypeRegistry registry;
    registry.emplace<Circle, IShape>(5);

    SW_CHECK(registry.get<Circle>() != nullptr);

    auto shape = registry.get<IShape>();
    SW_CHECK(shape != nullptr);
    SW_CHECK(shape->area() == 25);
}

SW_TEST(TypeRegistry, EmplaceOverwritesExistingInstance)
{
    sw::TypeRegistry registry;
    registry.emplace<Position>(1, 1);
    registry.emplace<Position>(2, 2);

    auto position = registry.get<Position>();
    SW_CHECK(position->x == 2 && position->y == 2);
}

SW_TEST(TypeRegistry, RemoveErasesBothConcreteAndInterfaceEntries)
{
    sw::TypeRegistry registry;
    registry.emplace<Circle, IShape>(5);

    registry.remove<Circle, IShape>();

    SW_CHECK(registry.get<Circle>() == nullptr);
    SW_CHECK(registry.get<IShape>() == nullptr);
}
