#ifndef MATERIALLIBRARY_H
#define MATERIALLIBRARY_H

#include <QVector>

#include "object3dbuilder.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include <QDebug>

class material_library
{
public:

    void addMaterial(material* material);
    void loadMaterialsFromFile(const QString &path);

    material* getMaterial(qsizetype index);
    material* getMaterial(const QString &mtlName);

    qsizetype getCountMaterials();

private:

    QVector<material *> materials;
};

#endif // MATERIALLIBRARY_H
