#include "skybox.h"

#include <QImage>

skybox::skybox(float width, const QImage &texture)
{
    QVector<VertexData> vertexes = init_vertexes(width / 2.0f);
    QVector<GLuint> indexes = init_indexes();

    Object3dBuilder background;

    material* m = background.add_diffuse_map(texture).
                             add_shinnes(96.0f).
                             add_diffuse_color(QVector3D(1.0, 1.0, 1.0)).
                             add_specular_color(QVector3D(1.0, 1.0, 1.0)).
                             add_ambience_color(QVector3D(1.0, 1.0, 1.0)).
                             getMaterial();

    cube = new object3D(vertexes, indexes, m);
}


skybox::~skybox()
{
    delete cube;
}

void skybox::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    cube->draw(program, functions, usingTextures);
}

void skybox::rotate(const QQuaternion &r)
{
    //Q_UNUSED можно использовать только если класс унаследован от Q_OBJECT
    (void)r;
}

void skybox::translate(const QVector3D &t)
{
    (void)t;
}

void skybox::scale(float s)
{
    (void)s;
}

void skybox::SetGlobalTransform(const QMatrix4x4 &q)
{
    (void)q;
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
