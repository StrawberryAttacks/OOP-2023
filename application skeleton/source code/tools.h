#ifndef TOOLS_H
#define TOOLS_H

#include <QVector3D>
#include <QDebug>
#include <QString>

namespace tools {

struct VertexData {

    VertexData() {}

    VertexData(QVector3D p, QVector2D t, QVector3D n) : position (p),
        texCoord(t), normal(n) {}

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;

    QVector3D tangent,
              bitangent;
};

}
#endif // TOOLS_H
