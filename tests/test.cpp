#include <arcball/arcball.h>

#include <gtest/gtest.h>

TEST(TestArcBallControl, centerPoint)
{
    arcball::Control c;

    c.set_center(glm::vec4{2.0f, 3.0f, 4.0f, 1.0f});

    glm::vec4 center{c.get_center()};
    EXPECT_EQ((glm::vec4{2.0f, 3.0f, 4.0f, 1.0f}), center);
}

TEST(TestArcBallControl, radius)
{
    arcball::Control c;

    c.set_radius(5.0);

    double radius{c.get_radius()};
    EXPECT_EQ(5.0, radius);
}
