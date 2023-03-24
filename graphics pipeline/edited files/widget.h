#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>

#include "group.h"
#include "camera.h"
#include "skybox.h"
#include "dimensional.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include <QFile>

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    int SelectObject(int x, int y, const QVector<WorldEngineObject *> &objs);

    void add_skybox(WorldEngineObject* s, const QString &texture);
    void add_scene_object(WorldEngineObject* d, const QString& diffuseMap_path, const QString& normalMap_path);

    void keyPressEvent(QKeyEvent *event);

protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void initShaders();

    void init_block(float width, float height, float depth);

private:

    QVector3D screenCoordsToWorldCoords(const QVector2D &mousePos);

    template<typename T>
    void clear_vector(QVector<T>& vec) {
        for (qsizetype i = 0; i < vec.size(); ++i)
            delete vec[i];
        vec.clear();
    }

private:

    QMatrix4x4 ProjectionMatrix;
    QMatrix4x4 ProjectionLightMatrix;

    QOpenGLShaderProgram SceneObjectShaderProgram;
    QOpenGLShaderProgram SkyBoxShaderProgram;
    QOpenGLShaderProgram SelectShaderProgram;

    QVector2D MousePosition;

    QVector<dimensional *> blocks;

    QVector<group *> groups;

    QVector<WorldEngineObject *> WorldObjects;

    camera* m_camera;

    QVector<skybox *> skyboxes;
};
#endif
