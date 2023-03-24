#ifndef MATERIAL_H
#define MATERIAL_H

#include <QString>
#include <QVector3D>
#include <QImage>

/*
  Ka - амбиент
  Ks - спекьюлар
  Kd - диффьюз составляющие материала

  Ns - шины (вместе с Ks используется, отвечает за блик  : чем больше это число, тем меньше диаметр бликов на материале)
  map_Kd - текстура для Kd
  map_Ks, map_Ka
*/

class material
{
public:

    material();

    material(const QString &mtlName);

    ~material() = default;

    void set_name(const QString& name);
    const QString &MtlName() const;


    void setDiffuseColor(const QVector3D &diffuseColor);
    const QVector3D &getDiffuseColor() const;


    void setAmbienceColor(const QVector3D &ambienceColor);
    const QVector3D &AmbienceColor() const;

    void setSpecularColor(const QVector3D &specularColor);
    const QVector3D &SpecularColor() const;

    void setShinnes(qreal shinnes);
    const float &getShinnes() const;

    void setDiffuseMap(const QString &path);
    void setDiffuseMap(const QImage &image);
    const QImage &getDiffuseMap() const;
    bool isDiffuseMapSet() const;

    void setNormalMap(const QString &path);
    void setNormalMap(const QImage &image);
    const QImage &getNormalMap() const;
    bool isNormalMapSet() const;

private:

    QString mtlName;

    QVector3D diffuseColor;
    QVector3D ambienceColor;
    QVector3D specularColor;

    float shinnes;

    QImage diffuseMap;
    QImage normalMap;

    bool isUsingDiffuseMap;
    bool isUsingNormalMap;
};

#endif // MATERIAL_H
