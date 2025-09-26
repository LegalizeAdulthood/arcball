#pragma once

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace arcball
{

enum AxisSet
{
    NoAxes,
    CameraAxes,
    BodyAxes,
    OtherAxes,
    NSets
};

class Control
{
public:
    Control();
    Control(const Control &rhs) = default;
    Control(Control &&rhs) = delete;
    Control &operator=(const Control &rhs) = default;
    Control &operator=(Control &&rhs) = delete;
    ~Control() = default;

    glm::vec4 get_center() const
    {
        return m_center;
    }
    void set_center( const glm::vec4& value )
    {
        m_center = value;
    }

    void set_radius(double value)
    {
        m_radius = value;
    }
    double get_radius() const
    {
        return m_radius;
    }

private:
    glm::vec4 m_center{0.0f, 0.0f, 0.0f, 1.0f};
    double m_radius{1.0f};
    glm::quat m_q_now{0.0f, 0.0f, 0.0f, 1.0f};
    glm::quat m_q_down{0.0f, 0.0f, 0.0f, 1.0f};
    glm::quat m_q_drag{0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 m_v_now{0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 m_v_down{0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 m_v_from{0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 m_v_to{0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 m_vr_from{0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 m_vr_to{0.0f, 0.0f, 0.0f, 1.0f};
    glm::mat4x4 m_now{glm::identity<glm::mat4x4>()};
    glm::mat4x4 m_down{glm::identity<glm::mat4x4>()};
    bool m_show_result{};
    bool m_dragging{};
    const float *m_constraint_sets[AxisSet::NSets]{};
    int m_constraint_set_sizes[AxisSet::NSets]{};
    AxisSet m_axis_set{AxisSet::NoAxes};
    int m_axis_index{};
};

} // namespace arcball
