#include "skybox.h"

#include <QImage>

skybox::skybox() : cube(0)
{
}

skybox::skybox(float width, const QString &texture_path) : cube(0)
{
    init(width, texture_path);
}

void skybox::init(float width, const QString &texture_path)
{
    QVector<VertexData> vertexes = init_vertexes(width / 2.0f);
    QVector<GLuint> indexes = init_indexes();

    Object3dBuilder background;

    material* m = background.add_diffuse_map(texture_path).
                             add_shinnes(96.0f).
                             add_diffuse_color(QVector3D(1.0, 1.0, 1.0)).
                             add_specular_color(QVector3D(1.0, 1.0, 1.0)).
                             add_ambience_color(QVector3D(1.0, 1.0, 1.0)).
                             getMaterial();

    if (cube != 0)
        delete cube;

    cube = new object3D(vertexes, indexes, m);
}


skybox::~skybox()
{
    if (cube != 0)
        delete cube;
}

void skybox::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    cube->draw(program, functions, usingTextures);
}

void skybox::rotate(const QQuaternion &r)
{
    //Q_UNUSED можно использовать только если класс унаследован от Q_OBJECT
    //(void)r;
    cube->rotate(r);
}

void skybox::translate(const QVector3D &t)
{
    cube->translate(t);
}

void skybox::scale(float s)
{
    cube->scale(s);
}

void skybox::SetGlobalTransform(const QMatrix4x4 &q)
{
    cube->SetGlobalTransform(q);
}

const QVector3D &skybox::getLocation() const
{
    return cube->GetLocation();
}

QVector<VertexData> skybox::init_vertexes(float w_by2)
{
    //Здесь натягивается текстура на фон
    QVector<VertexData> vertexes;

    //позади нас
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, w_by2), QVector2D(1.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, w_by2), QVector2D(1.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, w_by2), QVector2D(3.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, w_by2), QVector2D(3.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));

    //справа от нас
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, w_by2), QVector2D(3.0f/4.0f, 2.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, w_by2), QVector2D(3.0f/4.0f, 1.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, -w_by2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, -w_by2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));

    //над нами
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, w_by2), QVector2D(2.0f/4.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, -w_by2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, w_by2), QVector2D(1.0f/4.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, -w_by2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, -1.0, 0.0)));

    //перед нами
    vertexes.append(VertexData(QVector3D(w_by2, w_by2, -w_by2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, -w_by2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, -w_by2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, -w_by2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));

    //слева от нас
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, w_by2), QVector2D(0.0, 2.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, w_by2, -w_by2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, w_by2), QVector2D(0.0f, 1.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, -w_by2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));

    //под нами
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, w_by2), QVector2D(1.0f/4.0f, 0.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -w_by2, -w_by2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, w_by2), QVector2D(2.0f/4.0f, 0.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -w_by2, -w_by2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 1.0, 0.0)));

    return vertexes;
}

QVector<GLuint> skybox::init_indexes()
{
    QVector<GLuint> indexes;

    for (short i = 0; i < 24; i += 4) {

        indexes.append(i + 0);
        indexes.append(i + 2);
        indexes.append(i + 1);

        indexes.append(i + 2);
        indexes.append(i + 3);
        indexes.append(i + 1);
    }

    return indexes;
}
