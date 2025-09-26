#include <arcball/arcball.h>

#include <cassert>
#include <cmath>

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

glm::vec4 mouse_on_sphere(const glm::vec4 &mouse, const glm::vec4 &center, const double radius)
{
    glm::vec4 ball_mouse;
    double mag;
    ball_mouse.x = (mouse.x - center.x) / radius;
    ball_mouse.y = (mouse.y - center.y) / radius;
    mag = ball_mouse.x * ball_mouse.x + ball_mouse.y * ball_mouse.y;
    if (mag > 1.0)
    {
        double scale = 1.0 / sqrt(mag);
        ball_mouse.x *= scale;
        ball_mouse.y *= scale;
        ball_mouse.z = 0.0;
    }
    else
    {
        ball_mouse.z = sqrt(1 - mag);
    }
    ball_mouse.w = 0.0;
    return ball_mouse;
}

/* Return difference of v1 minus v2 */
glm::vec4 sub3(const glm::vec4 &v1, const glm::vec4 &v2)
{
    glm::vec4 v{};
    v.x = v1.x - v2.x;
    v.y = v1.y - v2.y;
    v.z = v1.z - v2.z;
    return v;
}

/* Return version of v scaled by s */
static glm::vec4 scale3(const glm::vec4 &v, float s)
{
    glm::vec4 u{};
    u.x = s*v.x; u.y = s*v.y; u.z = s*v.z; u.w = v.w;
    return u;
}

/* Return dot product of v1 and v2 */
static float dot3(const glm::vec4 &v1, const glm::vec4 &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/* Return norm of v, defined as sum of squares of components */
static float norm3(const glm::vec4 &v)
{
    const glm::vec3 v3{v.x, v.y, v.z};
    return glm::dot(v3, v3);
}

/* Return negative of v */
static glm::vec4 negate3(const glm::vec4 &v)
{
    return glm::vec4{-v.x, -v.y, -v.z, 0.0f};
}

/* Return unit magnitude vector in direction of v */
static glm::vec4 unit3( const glm::vec4& v )
{
    glm::vec4 u{};
    if (const float len = std::sqrt(norm3(v)); len != 0.0f)
    {
        u.x = v.x / len;
        u.y = v.y / len;
        u.z = v.z / len;
    }
    return (u);
}

/* Force sphere point to be perpendicular to axis. */
static glm::vec4 constrain_to_axis(const glm::vec4 &loose, const glm::vec4 &axis)
{
    glm::vec4 onPlane = sub3(loose, scale3(axis, dot3(axis, loose)));
    float norm = norm3(onPlane);
    if (norm > 0.0)
    {
        if (onPlane.z < 0.0)
            onPlane = negate3(onPlane);
        return (scale3(onPlane, 1 / sqrt(norm)));
    } /* else drop through */

    if (axis.z == 1.0f)
    {
        onPlane = glm::vec4{1.0, 0.0, 0.0, 0.0};
    }
    else
    {
        onPlane = unit3(glm::vec4{(-axis.y), (axis.x), 0.0, 0.0});
    }
    return onPlane;
}

/* Construct a unit quaternion from two points on unit sphere */
static glm::quat from_ball_points( const glm::vec4& from, const glm::vec4& to )
{
    glm::quat qu;
    qu.x = from.y*to.z - from.z*to.y;
    qu.y = from.z*to.x - from.x*to.z;
    qu.z = from.x*to.y - from.y*to.x;
    qu.w = from.x*to.x + from.y*to.y + from.z*to.z;
    return qu;
}

/* Return quaternion product qL * qR.  Note: order is important!
 * To combine rotations, use the product Mul(qSecond, qFirst),
 * which gives the effect of rotating by qFirst then qSecond. */
static glm::quat mul(const glm::quat &qL, const glm::quat &qR)
{
    glm::quat qq;
    qq.w = qL.w*qR.w - qL.x*qR.x - qL.y*qR.y - qL.z*qR.z;
    qq.x = qL.w*qR.x + qL.x*qR.w + qL.y*qR.z - qL.z*qR.y;
    qq.y = qL.w*qR.y + qL.y*qR.w + qL.z*qR.x - qL.x*qR.z;
    qq.z = qL.w*qR.z + qL.z*qR.w + qL.x*qR.y - qL.y*qR.x;
    return qq;
}

/* Find the index of nearest arc of axis set. */
static int nearest_constraint_axis( const glm::vec4& loose, const glm::vec4& axes, int nAxes )
{
    float max = -1.0f;
    int nearest = 0;
    for (int i = 0; i < nAxes; i++)
    {
        const glm::vec4 onPlane = constrain_to_axis(loose, (&axes)[i]);
        const float dot = dot3(onPlane, loose);
        if (dot > max)
        {
            max = dot;
            nearest = i;
        }
    }
    return nearest;
}

/* Convert a unit quaternion to two points on unit sphere */
static void to_ball_Points(const glm::quat &q, glm::vec4 &arcFrom, glm::vec4 &arcTo)
{
    double s = sqrt(q.x * q.x + q.y * q.y);
    if (s == 0.0)
    {
        arcFrom = glm::vec4{0.0, 1.0, 0.0, 0.0};
    }
    else
    {
        arcFrom = glm::vec4{(-q.y / s), (q.x / s), 0.0, 0.0};
    }
    arcTo.x = q.w * arcFrom.x - q.z * arcFrom.y;
    arcTo.y = q.w * arcFrom.y + q.z * arcFrom.x;
    arcTo.z = q.x * arcFrom.y - q.y * arcFrom.x;
    if (q.w < 0.0)
    {
        arcFrom = glm::vec4{(-arcFrom.x), (-arcFrom.y), 0.0, 0.0};
    }
}

constexpr int X = 0;
constexpr int Y = 1;
constexpr int Z = 2;
constexpr int W = 3;

/* Construct rotation matrix from (possibly non-unit) quaternion.
 * Assumes matrix is used to multiply column vector on the left:
 * vnew = mat vold.  Works correctly for right-handed coordinate system
 * and right-handed rotations. */
static glm::mat4x4 to_matrix(const glm::quat &q)
{
    double Nq = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
    double s = (Nq > 0.0) ? (2.0 / Nq) : 0.0;
    double xs = q.x*s;
    double ys = q.y*s;
    double zs = q.z*s;
    double wx = q.w*xs;
    double wy = q.w*ys;
    double wz = q.w*zs;
    double xx = q.x*xs;
    double xy = q.x*ys;
    double xz = q.x*zs;
    double yy = q.y*ys;
    double yz = q.y*zs;
    double zz = q.z*zs;
    glm::mat4x4 out;
    out[X][X] = 1.0f - (yy + zz);
    out[Y][X] = xy + wz;
    out[Z][X] = xz - wy;
    out[X][Y] = xy - wz;
    out[Y][Y] = 1.0f - (xx + zz);
    out[Z][Y] = yz + wx;
    out[X][Z] = xz + wy;
    out[Y][Z] = yz - wx;
    out[Z][Z] = 1.0f - (xx + yy);
    out[X][W] = 0.0f;
    out[Y][W] = 0.0f;
    out[Z][W] = 0.0f;
    out[W][X] = 0.0f;
    out[W][Y] = 0.0f;
    out[W][Z] = 0.0f;
    out[W][W] = 1.0f;
    return out;
}

void Control::update()
{
    int i;
    int set_size = m_constraint_set_sizes[m_axis_set];
    const glm::vec4 *set = reinterpret_cast<const glm::vec4 *>(m_constraint_sets[m_axis_set]);
    m_v_from = mouse_on_sphere(m_v_down, m_center, m_radius);
    m_v_to = mouse_on_sphere(m_v_now, m_center, m_radius);
    if (m_dragging)
    {
        if (m_axis_set != NoAxes)
        {
            m_v_from = constrain_to_axis(m_v_from, set[m_axis_index]);
            m_v_to = constrain_to_axis(m_v_to, set[m_axis_index]);
        }
        m_q_drag = from_ball_points(m_v_from, m_v_to);
        m_q_now = mul(m_q_drag, m_q_down);
    }
    else
    {
        if (m_axis_set != NoAxes)
        {
            m_axis_index = nearest_constraint_axis(m_v_to, *set, set_size);
        }
    }
    to_ball_Points(m_q_down, m_vr_from, m_vr_to);
    m_now = to_matrix(glm::conjugate(m_q_now)); /* Gives transpose for GL. */
}

} // namespace arcball
