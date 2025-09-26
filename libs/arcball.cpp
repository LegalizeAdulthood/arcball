#include <arcball/arcball.h>

#include <cassert>

namespace arcball
{

static const glm::mat4x4 s_identity{glm::identity<glm::mat4x4>()};
static const glm::vec4 s_other_axis{-0.48f, 0.80f, 0.36f, 1.00f};

Control::Control()
{
    m_constraint_sets[AxisSet::CameraAxes] = &s_identity[0].x;
    m_constraint_set_sizes[AxisSet::CameraAxes] = 3;
    m_constraint_sets[AxisSet::BodyAxes] = &m_down[0].x;
    m_constraint_set_sizes[AxisSet::BodyAxes] = 3;
    m_constraint_sets[AxisSet::OtherAxes] = &s_other_axis.x;
    m_constraint_set_sizes[AxisSet::OtherAxes] = 1;
}

void Control::mouse_move(float x, float y)
{
    assert(x >= -1.0f && x <= 1.0f);
    assert(y >= -1.0f && y <= 1.0f);
    m_v_now.x = x;
    m_v_now.y = y;
}

} // namespace arcball
