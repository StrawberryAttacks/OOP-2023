#ifndef OBJECT3DBUILDER_H
#define OBJECT3DBUILDER_H

#include "graphics_builder.h"
#include "tools.h"

#include <QOpenGLBuffer>
#include <QFileInfo>

using tools::VertexData;

class Object3dBuilder : public GraphicsBuilder
{
public:

    Object3dBuilder();

    Object3dBuilder& build_vertex_buffer(float width, float height, float depth);
    Object3dBuilder& build_index_buffer();

    Object3dBuilder& add_diffuse_map(const QImage &texture);
    Object3dBuilder& add_normal_map(const QImage &texture);

    Object3dBuilder& add_shinnes(float shinnes);

    Object3dBuilder& add_diffuse_color(const QVector3D& color);
    Object3dBuilder& add_specular_color(const QVector3D& color);
    Object3dBuilder& add_ambience_color(const QVector3D& color);

    QVector<VertexData> &getVertexBuff();
    QVector<GLuint> &getIndexBuff();

    void refresh();

    virtual material *getMaterialFromFile(const QString& path, const QStringList &file_text_splitted_by_space) override;

    virtual material* getMaterial() override;

    virtual ~Object3dBuilder() = default;

protected:

    template<typename ...Args>
    void bind_material(Args&& ...args);

    void parse_file_data(const QStringList &list, QFileInfo& fileInfo);

private:

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;

    material *mtl;
};


#endif // OBJECT3DBUILDER_H
