#include <arcball/arcball.h>

#include <gtest/gtest.h>

TEST(TestArcBallControl, centerPoint)
{
    arcball::Control c;

    c.set_center(glm::vec4{2.0f, 3.0f, 4.0f, 1.0f});

    glm::vec4 center{c.get_center()};
    EXPECT_EQ((glm::vec4{2.0f, 3.0f, 4.0f, 1.0f}), center);
}
