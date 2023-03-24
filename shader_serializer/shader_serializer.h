#ifndef SHADER_SERIALIZER_H
#define SHADER_SERIALIZER_H

#include <QString>
#include <QRegularExpression>
#include <QFile>

#include "shader_serializer_global.h"

class SHADER_SERIALIZER_EXPORT shader_wrapper
{
public:
    shader_wrapper(const QString& FRAGMENTshader_absolutePath);

    void marshalize(const QString& output_file);
    void unmarshalize(const QString& source_file);

private:
      QString shader_path;
      QString shader_data;
};

#endif // SHADER_SERIALIZER_H
