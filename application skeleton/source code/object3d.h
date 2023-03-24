#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "worldenginebase.h"
#include "object3dbuilder.h"

class object3D : public WorldEngineObject
{
public:

    object3D();

    ~object3D();

    object3D(const QVector<VertexData> &data,
                   const QVector<GLuint> &indexes, const QImage &texture);

    object3D(const QVector<VertexData> &data, const QVector<GLuint> &indexes, material *material);

    void Init(const QVector<VertexData> &data,
              const QVector<GLuint> &indexes, const QImage &texture);


    void Init(const QVector<VertexData> &data,
              const QVector<GLuint> &indexes, material *material);

    const QVector3D &GetLocation() const;

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);

    void rotate(const QQuaternion &r);

    void translate(const QVector3D &t);

    void scale(float s);

    void SetGlobalTransform(const QMatrix4x4 &q);

protected:

    void AllocateVertexBuffer(const QVector<VertexData> &vertexes);

    void AllocateIndexBuffer(const QVector<GLuint> &indexes);

    void set_map_filters(QOpenGLTexture* map);

private:

    QOpenGLBuffer VertexBuffer;
    QOpenGLBuffer IndexBuffer; //чтоб было понятно, как пройтись треугольниками по объявленному выше массиву с данными


    QOpenGLTexture *diffuseMap;
    QOpenGLTexture *normalMap;

    material *m_material;

    //составляющие модельно-видовой матрицы:
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransforms;
};


class SceneObjectsFactory : public GraphicsPipeline {
public:
    WorldEngineObject* pullout() override {
        return new object3D();
    }
};

#endif // OBJECT3D_H
