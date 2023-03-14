#ifndef DIMENSIONAL_H
#define DIMENSIONAL_H

#include "materiallibrary.h"
#include "object3d.h"

#include <QVector>

#include <QFile>
#include <QFileInfo>

class dimensional : public WorldEngineObject
{
public:

    dimensional(const QString& FilePath = ":/aloe_vera_plant/aloevera.obj");

    virtual ~dimensional() {

        for(auto object: object_pieces)
            delete object;

    }

    const QString &GetObject3DFilePath() const;

    void loadObjectFromFile(const QString &path);
    void addObject(object3D *object );

    object3D *getObject(quint32 index);

    void calculateTBN(QVector<VertexData> &data); //для карты нормалей

    const QVector3D& GetLocation() const;


    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);

    void rotate(const QQuaternion &r);

    void translate(const QVector3D &t);

    void scale(float s);

    void SetGlobalTransform(const QMatrix4x4 &q);

protected:

    void parse_file_data(QStringList &file_text_splitted_by_space, const QString &filepath,
                         QVector<VertexData>& vertexes, QVector<GLuint>& indexes);

private:

    QVector<object3D *> object_pieces;

    material_library materialLibrary;

    QString ObjFilePath;
};
#endif // DIMENSIONAL_H
