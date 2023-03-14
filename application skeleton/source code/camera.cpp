#include "camera.h"

camera::camera() : m_scale(1.0)
{
    m_globalTransforms.setToIdentity();
}


void camera::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    Q_UNUSED(usingTextures);

    if (functions)
        return;

    program->setUniformValue("u_viewMatrix", m_ViewMatrix);
}

void camera::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;

    updateViewMatrix();
}

void camera::translate(const QVector3D &t)
{
    m_translate += t;

    updateViewMatrix();
}

void camera::scale(float s)
{
    m_scale *= s;

    updateViewMatrix();
}

void camera::SetGlobalTransform(const QMatrix4x4 &q)
{
    m_globalTransforms = q;

    updateViewMatrix();
}

void camera::RotateX(const QQuaternion &r)
{
    m_rotateX = r * m_rotateX;

    m_rotate = m_rotateX * m_rotateY;

    updateViewMatrix();
}

void camera::RotateY(const QQuaternion &r)
{
    m_rotateY = r * m_rotateY;
    m_rotate = m_rotateX * m_rotateY;

    updateViewMatrix();
}

const QMatrix4x4 &camera::getViewMatrix() const
{
    return m_ViewMatrix;
}

void camera::updateViewMatrix()
{
    m_ViewMatrix.setToIdentity();
    m_ViewMatrix.translate(m_translate);
    m_ViewMatrix.rotate(m_rotate);
    m_ViewMatrix.scale(m_scale);
    m_ViewMatrix = m_ViewMatrix * m_globalTransforms.inverted();
}
