#ifndef SKYBOX_H
#define SKYBOX_H

#include "worldenginebase.h"
#include "object3d.h"


class skybox : public WorldEngineObject
{
public:

    skybox();

    skybox(float width, const QString &texture_path);

    void init(float width, const QString &texture_path);

    virtual ~skybox();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);

    void rotate(const QQuaternion &r);

    void translate(const QVector3D &t);

    void scale(float s);

    void SetGlobalTransform(const QMatrix4x4 &q);

    const QVector3D &getLocation() const;

protected:

    QVector<VertexData> init_vertexes(float w_by2);
    QVector<GLuint> init_indexes();

private:

    object3D *cube;
};

#endif // SKYBOX_H
