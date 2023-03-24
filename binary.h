#ifndef BINARY_H
#define BINARY_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QRegularExpression>

#include "object3dbuilder.h"


#define NAME "mtlName"

#define D_COLOR "diffuseColor"
#define A_COLOR "ambienceColor"
#define S_COLOR "specularColor"

#define SHINNES "shinnes"

#define D_MAP "diffuseMap"
#define N_MAP "normalMap"

struct json_helper {

    static void marshalize(const material* m, const QString& dest_file) {

        auto json_data = parse_data(m);

        QJsonDocument doc(json_data);

        QFile file(dest_file);

        if (!file.open(QIODevice::WriteOnly))
            throw;

        file.write(doc.toJson());

        file.close();
    }

    static void marshalize(Object3dBuilder& texture, const QString& path) {
        marshalize(texture.getMaterial(), path);
    }

    static material* unmarshalize(const QString& source_file) {

        QFile file(source_file);

        if (!file.open(QIODevice::ReadOnly))
            return nullptr;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

        file.close();

        auto json_data = doc.object();

        Object3dBuilder texture;

        texture.set_name(json_data.value(NAME).toString());

        texture.add_diffuse_map(json_data.value(D_MAP).toString()).
                add_normal_map(json_data.value(N_MAP).toString()).
                add_shinnes(json_data.value(SHINNES).toDouble());

        texture.add_diffuse_color(build_vector(json_data.value(D_COLOR).toObject()));
        texture.add_ambience_color(build_vector(json_data.value(A_COLOR).toObject()));
        texture.add_specular_color(build_vector(json_data.value(S_COLOR).toObject()));

        return texture.getMaterial();
    }

  private:

    static QJsonObject parse_data(const material* m) {

        QJsonObject obj;

        obj.insert(NAME, m->MtlName());

        obj.insert(SHINNES, m->getShinnes());

        obj.insert(D_MAP, m->getDiffuseMapPath());
        obj.insert(N_MAP, m->getNormalMapPath());

        obj.insert(D_COLOR, decompose_vector(m->getDiffuseColor()));
        obj.insert(A_COLOR, decompose_vector(m->getAmbienceColor()));
        obj.insert(S_COLOR, decompose_vector(m->getSpecularColor()));

        return obj;
    }

    static QJsonObject decompose_vector(const QVector3D &color) {

        QJsonObject color_json;

        float r = color.x();
        float g = color.y();
        float b = color.z();

        color_json.insert("b", b);
        color_json.insert("g", g);
        color_json.insert("r", r);

        return color_json;
    }

    static QVector3D build_vector(QJsonObject json_color) {

        float r = json_color.value("r").toDouble();
        float g = json_color.value("g").toDouble();
        float b = json_color.value("b").toDouble();

        return QVector3D(r, g, b);
    }
};

#endif // BINARY_H
