#include "dimensional.h"

#include <QDir>

void dimensional::loadObjectFromFile(const QString &path)
{
    QFile objFile(path);

    if (!objFile.open(QIODevice::ReadOnly)) {
        qDebug() << "\nfile cannot be open\n";
        throw;
    }

    QTextStream input(&objFile);

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;

    object3D *object = 0;

    QString MaterialName;

    while (!input.atEnd()) {

        QString str = input.readLine();

        QStringList list = str.split(" ");

        if (list[0] == "usemtl") {

            if (object)
                object->Init(vertexes, indexes, materialLibrary.getMaterial(MaterialName));

            MaterialName = list[1];

            addObject(object);
            object = new object3D;

            vertexes.clear();
            indexes.clear();
        }

        else
            parse_file_data(list, path, vertexes, indexes);
    }

    objFile.close();

    if (object)
        object->Init(vertexes, indexes, materialLibrary.getMaterial(MaterialName));

    addObject(object);
}

void dimensional::addObject(object3D *object)
{
    if (!object)
        return;

    if (object_pieces.contains(object))
        return;

    object_pieces.append(object);
}

object3D *dimensional::getObject(quint32 index)
{
    if (index < object_pieces.size())
        return object_pieces[index];
    else
        return nullptr;
}

void dimensional::calculateTBN(QVector<VertexData> &data)
{
    for (qsizetype i = 0; i < data.size(); i += 3) {

        QVector3D &v1 = data[i].position;
        QVector3D &v2 = data[i+1].position;
        QVector3D &v3 = data[i+2].position;

        QVector2D &uv1 = data[i].texCoord;
        QVector2D &uv2 = data[i+1].texCoord;
        QVector2D &uv3 = data[i+2].texCoord;


        QVector3D deltaPos1 = v2 - v1,
                  deltaPos2 = v3 - v1;

        QVector2D deltaUV1 = uv2 - uv1,
                  deltaUV2 = uv3 - uv1;

        float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
        QVector3D tangent = (deltaPos1 * deltaUV2.y() - deltaPos2 * deltaUV1.y()) * r;
        QVector3D bitangent = (deltaPos2 * deltaUV1.x() - deltaPos1 * deltaUV2.x()) * r;

        data[i].tangent = data[i + 1].tangent = data[i + 2].tangent = std::move(tangent);
        data[i].bitangent = data[i + 1].bitangent = data[i + 2].bitangent = std::move(bitangent);

    }
}

const QVector3D &dimensional::GetLocation() const
{
    return object_pieces.last()->GetLocation();
}

void dimensional::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{    
    for (qsizetype i = 0; i < object_pieces.size(); ++i)
        object_pieces[i]->draw(program, functions, usingTextures);

}

void dimensional::rotate(const QQuaternion &r)
{
    for (qsizetype i = 0; i < object_pieces.size(); ++i)
        object_pieces[i]->rotate(r);
}

void dimensional::translate(const QVector3D &t)
{
    for (qsizetype i = 0; i < object_pieces.size(); ++i)
        object_pieces[i]->translate(std::move(t));
}

void dimensional::scale(float s)
{
    for (qsizetype i = 0; i < object_pieces.size(); ++i)
        object_pieces[i]->scale(s);
}

void dimensional::SetGlobalTransform(const QMatrix4x4 &q)
{
    for (qsizetype i = 0; i < object_pieces.size(); ++i)
        object_pieces[i]->SetGlobalTransform(q);
}

object3D *dimensional::first()
{
    return object_pieces.first();
}

object3D *dimensional::last()
{
    return object_pieces.last();
}

void dimensional::parse_file_data(QStringList &file_text_splitted_by_space, const QString &filepath, QVector<VertexData>& vertexes, QVector<GLuint>& indexes)
{
    QStringList& list = file_text_splitted_by_space;

    static QVector<QVector3D> coords;
    static QVector<QVector3D> normals;
    static QVector<QVector2D> texCoords;


    if (list[0] == "mtllib") {
        QFileInfo info(filepath);
        materialLibrary.loadMaterialsFromFile(QString("%1/%2").arg(info.absolutePath()).arg(list[1]));
    }

    else if (list[0] == "v")
        coords.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));

    else if (list[0] == "vt")
        texCoords.append(QVector2D(list[1].toFloat(), list[2].toFloat()));

    else if (list[0] == "vn")
        normals.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));

    else if (list[0] == "f")

        for (int i = 1; i <= 3; ++i) {

            QStringList vert = list[i].split("/"); //индексы в obj файле начинаются с 1
            vertexes.append(VertexData(coords[abs(vert[0].toLong()) - 1], texCoords[abs(vert[1].toLong()) - 1], normals[abs(vert[2].toLong()) - 1]));
            indexes.append(indexes.size());
        }
}
