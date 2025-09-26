#include <arcball/arcball.h>

#include <gtest/gtest.h>

TEST(TestArcBallControl, initialState)
{
    arcball::Control c;

    EXPECT_EQ((glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}), c.get_center());
    EXPECT_EQ(1.0f, c.get_radius());
    EXPECT_EQ(arcball::AxisSet::NoAxes, c.get_constraint());
    EXPECT_FALSE(c.get_shown());
}

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

TEST(TestArcBallControl, constraint)
{
    arcball::Control c;

    c.set_constraint(arcball::AxisSet::CameraAxes);

    EXPECT_EQ(arcball::AxisSet::CameraAxes, c.get_constraint());
}


TEST(TestArcBallControl, show)
{
    arcball::Control c;

    c.show(true);

    EXPECT_TRUE(c.get_shown());
}
