#ifndef SKYBOX_H
#define SKYBOX_H

#include "worldenginebase.h"
#include "object3d.h"


class skybox : public WorldEngineObject
{
public:

    skybox() = default;

    skybox(float width, const QImage &texture);

    virtual ~skybox();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);

    void rotate(const QQuaternion &r);

    void translate(const QVector3D &t);

    void scale(float s);

    void SetGlobalTransform(const QMatrix4x4 &q);

protected:

    QVector<VertexData> init_vertexes(float w_by2);
    QVector<GLuint> init_indexes();

private:

    object3D *cube;
};

class SkyBoxFactory : public GraphicsPipeline {
public:
    WorldEngineObject* pullout() override {
        return new skybox();
    }
};

#endif // SKYBOX_H
