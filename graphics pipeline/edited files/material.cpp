#include "material.h"

material::material() : isUsingDiffuseMap(false), isUsingNormalMap (false)
{
}

material::material(const QString &mtlName) : mtlName(mtlName), isUsingDiffuseMap(false), isUsingNormalMap (false)
{
}

void material::set_name(const QString &name)
{
    mtlName = name;
}

const QString &material::MtlName() const
{
    return mtlName;
}

void material::setDiffuseColor(const QVector3D &diffuseColor)
{
    this->diffuseColor = diffuseColor;

    isUsingDiffuseMap = true;
}

const QVector3D &material::getDiffuseColor() const
{
    return diffuseColor;
}

void material::setAmbienceColor(const QVector3D &ambienceColor)
{
    this->ambienceColor = ambienceColor;
}

const QVector3D &material::getAmbienceColor() const
{
    return ambienceColor;
}

void material::setSpecularColor(const QVector3D &specularColor)
{
    this->specularColor = specularColor;
}

const QVector3D &material::getSpecularColor() const
{
    return specularColor;
}

void material::setShinnes(qreal shinnes)
{
    this->shinnes = shinnes;
}

const float &material::getShinnes() const
{
    return shinnes;
}

void material::setDiffuseMap(const QString &path)
{
    diffuseMap = QImage(path);
    DiffuseMapPath = path;

    isUsingDiffuseMap = true;
}

const QImage &material::getDiffuseMap() const
{
    return diffuseMap;
}

const QString &material::getDiffuseMapPath() const
{
    return DiffuseMapPath;
}

bool material::isDiffuseMapSet() const
{
    return isUsingDiffuseMap;
}

void material::setNormalMap(const QString &path)
{
    normalMap = QImage(path);
    NormalMapPath = path;

    isUsingNormalMap = true;
}

const QImage &material::getNormalMap() const
{
    return normalMap;
}

const QString &material::getNormalMapPath() const
{
    return NormalMapPath;
}

bool material::isNormalMapSet() const
{
    return isUsingNormalMap;
}
