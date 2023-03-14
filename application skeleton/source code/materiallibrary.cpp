#include "materiallibrary.h"

inline void material_library::addMaterial(material *material)
{
    if (!material)
        return;

    if (materials.contains(material))
        return;

    materials.append(material);
}

void material_library::loadMaterialsFromFile(const QString &path)
{
    QFile file(path);
    QTextStream inputStream(&file);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "\nmtl file is not found\n";
        throw;
    }

    QFileInfo fileInfo(path);

    for (int i = 0; i < materials.size(); ++i)
        delete materials[i];
    materials.clear();

    material *newMtl = 0;
    Object3dBuilder builder;

    while (!inputStream.atEnd()) {

        QString str = inputStream.readLine();

        QStringList list = str.split(" ");

        if (list[0] == "newmtl") {

            addMaterial(newMtl);

            builder.refresh();

            newMtl = builder.getMaterialFromFile(path, list);
        }

        else
            newMtl = builder.getMaterialFromFile(path, list);
    }

    addMaterial(newMtl);

    file.close();
}

material *material_library::getMaterial(qsizetype index)
{
    return materials[index];
}

material *material_library::getMaterial(const QString &mtlName)
{
    for (int i = 0; i < materials.size(); ++i)
        if (materials[i]->MtlName() == mtlName)
            return materials[i];

    qDebug() << "\nmaterial " << mtlName << " was not found\n";
    throw;
}

qsizetype material_library::getCountMaterials()
{
    return materials.size();
}
