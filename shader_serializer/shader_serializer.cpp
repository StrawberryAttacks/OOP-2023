#include "shader_serializer.h"

shader_wrapper::shader_wrapper(const QString& FRAGMENTshader_absolutePath) : shader_path(FRAGMENTshader_absolutePath) {

    QFile shader(shader_path);

    shader.open(QIODevice::ReadOnly);

    shader_data = shader.readAll();

    shader.close();
}

void shader_wrapper::marshalize(const QString& output_file) {

    QRegularExpression find_color("gl_FragColor.+;");
    QRegularExpressionMatch match = find_color.match(shader_data);

    QString frag_color_info = match.captured(0);

    QFile file(output_file);

    file.open(QIODevice::WriteOnly);
    file.write(frag_color_info.toUtf8());

    file.close();

}

void shader_wrapper::unmarshalize(const QString& source_file) {

    QString shader_data_copy = shader_data;

    QFile file(source_file);

    if (!file.open(QIODevice::ReadOnly))
        throw;

    QString custom_input = file.readAll();

    file.close();


    QRegularExpression find_color("=\\w*.+;");
    QRegularExpressionMatch match = find_color.match(custom_input);

    QString frag_color_info = match.captured(0);

    auto decomposed = shader_data.split(QRegularExpression("gl_FragColor.+;"));

    shader_data = decomposed[0] + "gl_FragColor " + frag_color_info + decomposed[1];


    QFile shader(shader_path);
    shader.open(QIODevice::WriteOnly);
    shader.write(shader_data.toUtf8());
    shader.close();

    shader_data = shader_data_copy;
}
