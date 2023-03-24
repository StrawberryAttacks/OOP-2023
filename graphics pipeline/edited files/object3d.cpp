#include "object3d.h"

object3D::object3D() :
    IndexBuffer(QOpenGLBuffer::IndexBuffer),  diffuseMap(0), normalMap(0), m_scale(1.0)
{
}

object3D::object3D(const QVector<VertexData> &data, const QVector<GLuint> &indexes, const QString &texture_path)
    : object3D()
{
    Init(data, indexes, texture_path);
}

object3D::object3D(const QVector<VertexData> &data, const QVector<GLuint> &indexes, material *material)
    : object3D()
{
    Init(data, indexes, material);
}

object3D::~object3D()
{
    if (VertexBuffer.isCreated())
        VertexBuffer.destroy();

    if (IndexBuffer.isCreated())
        IndexBuffer.destroy();

    if (diffuseMap != 0)
        if (diffuseMap->isCreated())
            delete diffuseMap;

    if (normalMap != 0)
        if (normalMap->isCreated())
            delete normalMap;
}

void object3D::Init(const QVector<VertexData> &data, const QVector<GLuint> &indexes, const QString &texture_path)
{
    if (diffuseMap != 0)
        if (diffuseMap->isCreated()) {
            delete diffuseMap;
            diffuseMap = 0;
        }

    AllocateVertexBuffer(data);
    AllocateIndexBuffer(indexes);

    diffuseMap = new QOpenGLTexture(QImage(texture_path).mirrored());
    set_map_filters(diffuseMap);
}

void object3D::Init(const QVector<VertexData> &data, const QVector<GLuint> &indexes, material *material)
{
    if (diffuseMap != 0)
        if (diffuseMap->isCreated()) {
            delete diffuseMap;
            diffuseMap = 0;
        }


    AllocateVertexBuffer(data);
    AllocateIndexBuffer(indexes);


    m_material = material;


    if (material->isDiffuseMapSet()) {

        diffuseMap = new QOpenGLTexture(material->getDiffuseMap().mirrored());
        set_map_filters(diffuseMap);
    }

    if (material->isNormalMapSet()) {

        normalMap = new QOpenGLTexture(material->getNormalMap().mirrored());
        set_map_filters(normalMap);
    }
}

const QVector3D &object3D::GetLocation() const
{
    return m_translate;
}

void object3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    if (!VertexBuffer.isCreated() || !IndexBuffer.isCreated())
        return;

    QMatrix4x4 ModelMatrix;
    ModelMatrix.setToIdentity();

    //порядок следующий преобразований важен!! Реальные преобразования матрицы (внутри программы) делаются снизу вверх.
    ModelMatrix.translate(m_translate);
    ModelMatrix.rotate(m_rotate);
    ModelMatrix.scale(m_scale);
    ModelMatrix = m_globalTransforms * ModelMatrix;


    if (usingTextures) {

        if (m_material->isDiffuseMapSet()) {

            diffuseMap->bind(0);
            program->setUniformValue("u_diffuseMap", 0);
        }

        if (m_material->isNormalMapSet()) {

            normalMap->bind(1); //номер слота, на который биндим
            program->setUniformValue("u_normalMap", 1);
        }
    }

    program->setUniformValue("u_modelMatrix", ModelMatrix);

    program->setUniformValue("u_materialPropery.diffuseColor", m_material->getDiffuseColor());
    program->setUniformValue("u_materialPropery.ambienceColor", m_material->getAmbienceColor());
    program->setUniformValue("u_materialPropery.specularColor", m_material->getSpecularColor());
    program->setUniformValue("u_materialPropery.shinnes", m_material->getShinnes());

    program->setUniformValue("u_isUsingDiffuseMap", m_material->isDiffuseMapSet());
    program->setUniformValue("u_isUsingNormalMap", m_material->isNormalMapSet());

    VertexBuffer.bind();
    IndexBuffer.bind();


    int VertLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(VertLoc);
    program->setAttributeBuffer(VertLoc, GL_FLOAT, 0, 3, sizeof(VertexData));

    int texLoc = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texLoc);
    program->setAttributeBuffer(texLoc, GL_FLOAT, sizeof(QVector3D), 2, sizeof(VertexData));


    int offset = sizeof(QVector3D) + sizeof(QVector2D);

    int normalLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLoc);
    program->setAttributeBuffer(normalLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int tangentLoc = program->attributeLocation("a_tangent");
    program->enableAttributeArray(tangentLoc);
    program->setAttributeBuffer(tangentLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int BiTangentLoc = program->attributeLocation("a_bitangent");
    program->enableAttributeArray(BiTangentLoc);
    program->setAttributeBuffer(BiTangentLoc, GL_FLOAT, offset, 3, sizeof(VertexData));


    functions->glDrawElements(GL_TRIANGLES, IndexBuffer.size(), GL_UNSIGNED_INT, 0);

    VertexBuffer.release();
    IndexBuffer.release();

    if (usingTextures) {

        if (m_material->isDiffuseMapSet())
            diffuseMap->release();

        if (m_material->isNormalMapSet())
            normalMap->release();
    }
}

void object3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;
}

void object3D::translate(const QVector3D &t)
{
    m_translate += t;
}

void object3D::scale(float s)
{
    m_scale *= s;
}

void object3D::SetGlobalTransform(const QMatrix4x4 &q)
{
    m_globalTransforms = q;
}

void object3D::AllocateVertexBuffer(const QVector<VertexData> &vertexes)
{
    if (VertexBuffer.isCreated())
        VertexBuffer.destroy();

    VertexBuffer.create();
    VertexBuffer.bind();
    VertexBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    VertexBuffer.release();
}

void object3D::AllocateIndexBuffer(const QVector<GLuint> &indexes)
{
    if (IndexBuffer.isCreated())
        IndexBuffer.destroy();

    IndexBuffer.create();
    IndexBuffer.bind();
    IndexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    IndexBuffer.release();
}

void object3D::set_map_filters(QOpenGLTexture *map)
{
    map->setMinificationFilter(QOpenGLTexture::Nearest);

    map->setMinificationFilter(QOpenGLTexture::Linear);

    map->setWrapMode(QOpenGLTexture::Repeat);
}
