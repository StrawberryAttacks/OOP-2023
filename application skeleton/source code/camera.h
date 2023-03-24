#ifndef CAMERA_H
#define CAMERA_H

#include "worldenginebase.h"

#include <QQuaternion>
#include <QVector3D>

#include <QMatrix4x4>

#include <QOpenGLShaderProgram>

class camera : public WorldEngineObject
{
public:

    camera();

    virtual ~camera() = default;

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions = 0, bool usingTextures = true);

    void rotate(const QQuaternion &r);

    void translate(const QVector3D &t);

    void scale(float s);

    void SetGlobalTransform(const QMatrix4x4 &q);

    void RotateX(const QQuaternion &r);
    void RotateY(const QQuaternion &r);

    const QMatrix4x4 &getViewMatrix() const;

protected:

    void updateViewMatrix();

private:

    QQuaternion m_rotate;
    QQuaternion m_rotateX;
    QQuaternion m_rotateY;

    QVector3D m_translate;

    float m_scale;

    QMatrix4x4 m_globalTransforms;
    QMatrix4x4 m_ViewMatrix;
};


class ViewCameraFactory : public GraphicsPipeline {
public:
    WorldEngineObject* pullout() override {
        return new camera();
    }
};


#endif // CAMERA_H
