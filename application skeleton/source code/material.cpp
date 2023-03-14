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
    this->diffuseColor = std::move(diffuseColor);

    isUsingDiffuseMap = true;
}

const QVector3D &material::getDiffuseColor() const
{
    return diffuseColor;
}

void material::setAmbienceColor(const QVector3D &ambienceColor)
{
    this->ambienceColor = std::move(ambienceColor);
}

const QVector3D &material::AmbienceColor() const
{
    return ambienceColor;
}

void material::setSpecularColor(const QVector3D &specularColor)
{
    this->specularColor = std::move(specularColor);
}

const QVector3D &material::SpecularColor() const
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
}

void material::setDiffuseMap(const QImage &image)
{
    diffuseMap = image;
}

const QImage &material::getDiffuseMap() const
{
    return diffuseMap;
}

bool material::isDiffuseMapSet() const
{
    return isUsingDiffuseMap;
}

void material::setNormalMap(const QString &path)
{
    normalMap = QImage(path);

    isUsingNormalMap = true;
}

void material::setNormalMap(const QImage &image)
{
    normalMap = image;

    isUsingNormalMap = true;
}

const QImage &material::getNormalMap() const
{
    return normalMap;
}

bool material::isNormalMapSet() const
{
    return isUsingNormalMap;
}
