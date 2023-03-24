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

#include <QMap>

#include <QFile>

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:

    void initializeGL(); //вызывается один раз, при создании приложения
    void resizeGL(int w, int h); //вызывается при масштабировании окна
    void paintGL(); //можно вызвать принудительно методом update()

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void initShaders();

    void initFloor(float width, float height, float depth, const QImage &diffuseMap, const QImage &normalMap);

    int SelectObject(int x, int y, QVector<WorldEngineObject *> &objs);

private:

    QVector3D screenCoordsToWorldCoords(const QVector2D &mousePos);

    void pullup(int code, const QString& message);

private:

    QMatrix4x4 ProjectionMatrix;
    QMatrix4x4 ProjectionLightMatrix;

    QOpenGLShaderProgram SceneObjectShaderProgram;
    QOpenGLShaderProgram SkyBoxShaderProgram;
    QOpenGLShaderProgram SelectShaderProgram;

    QVector2D MousePosition;

    QVector<dimensional *> characters;
    QVector<dimensional *> floor;

    QVector<WorldEngineObject *> WorldObjects;
    QVector<WorldEngineObject *> selectObjects;

    QVector<group *> groups;

    camera* m_camera;

    skybox* m_skybox;

    QMap<qsizetype, qsizetype> character_floor;

    qsizetype square;

};
#endif // WIDGET_H
