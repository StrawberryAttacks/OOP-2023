#include "widget.h"

#include <QtMath>
#include <QOpenGLFunctions>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_camera = new camera();
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));

    square = 3;
}

Widget::~Widget()
{
    delete m_camera;
    delete m_skybox;

    for (qsizetype i = 0; i < characters.size(); ++i)
        delete characters[i];

    characters.clear();

    for (qsizetype i = 0; i < groups.size(); ++i)
        delete groups[i];

    groups.clear();
}

void Widget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); //не рисовать задние грани

    initShaders();

    QImage FloorTexture(":/pantone-very-peri-2022.jpg"),
            FloorNormalMap(":/ice1_n.jpg"),

            PlTexture(":/cl_ppl.jpg");

    groups.append(new group);

    float x = -0.5f, y = -3.5f, z = -8.0;

    for (short X = 0; X < square * square; ++X) {

        if (X > 0 && !(X % square)) {
            z += 5.5f;
            x = -0.5;
        }

        if (X == 0 || X == 1 || X == square * square - 1 || X == square * square - 2)

            initFloor(3.0f, 3.0f, 3.0f, PlTexture, QImage(":/ice1_n.jpg"));
        else
            initFloor(3.0f, 3.0f, 3.0f, FloorTexture, FloorNormalMap);

        floor.last()->translate(QVector3D(x, y, z));

        groups.first()->addObject(floor.last());

        x += 4.5f;

        selectObjects.append(floor.last());

    }

    WorldObjects.append(groups.first());

    QVector3D floor_pos(-square - 3.0f, -3.5f, -square * 4.0f);
    groups.first()->translate(QVector3D(floor_pos));


    groups.append(new group);

    characters.append(new dimensional());

    characters.last()->loadObjectFromFile(characters.last()->GetObject3DFilePath());


    QVector3D pos = floor.last()->GetLocation() + floor_pos;
    characters.last()->translate(QVector3D(pos.x(), -6.0f, pos.z()));


    groups.last()->addObject(characters.last());


    WorldObjects.append(groups.last());

    characters.last()->scale(15.0f);


    character_floor.insert(0, square * square - 1);


    m_skybox = new skybox(100.0f, QImage(":/skybox0.png"));
}

void Widget::resizeGL(int w, int h)
{
    float aspect = w / (float)h;

    ProjectionMatrix.setToIdentity();
    ProjectionMatrix.perspective(45, aspect, /*0.1f*/ 0.01f, /*10.0f*/ 1000.0f); //3 параметр: передняя плоскость отсечения,
                                                                                //4 - задняя
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SkyBoxShaderProgram.bind();
    SkyBoxShaderProgram.setUniformValue("u_projectionMatrix", ProjectionMatrix);

    m_camera->draw(&SkyBoxShaderProgram);

    m_skybox->draw(&SkyBoxShaderProgram, context()->functions());

    SkyBoxShaderProgram.release();

    SceneObjectShaderProgram.bind();

    SceneObjectShaderProgram.setUniformValue("u_projectionMatrix", ProjectionMatrix);
    SceneObjectShaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    SceneObjectShaderProgram.setUniformValue("u_lightPower", 1.0f);

    m_camera->draw(&SceneObjectShaderProgram);

    for (qsizetype i = 0; i < WorldObjects.size(); ++i)
           WorldObjects[i]->draw(&SceneObjectShaderProgram, context()->functions());

    SceneObjectShaderProgram.release();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        MousePosition = QVector2D(event->position()); //координаты указателя относительно левого верхнего угла данного окна

    else if (event->button() == Qt::RightButton) {

        int index = SelectObject(event->position().x(), event->position().y(), selectObjects);

        if (!index)
            return;

        --index;

        qDebug() << index;
    }

    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{

    QVector2D diff = QVector2D(event->position()) - MousePosition;

    MousePosition = QVector2D(event->position()); //localPos()

    float angX = diff.y() / 2.0f,
          angY = diff.x() / 2.0f;

    m_camera->RotateX(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angX));
    m_camera->RotateY(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, angY));

    update();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.25f));

    else if (event->angleDelta().y() < 0)
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));

    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {

    case Qt::Key_Left:
        m_camera->translate(QVector3D(0.5f, 0.0f, 0.0f));
        break;

    case Qt::Key_Right:
        m_camera->translate(QVector3D(-0.5f, 0.0f, 0.0f));
        break;

    case Qt::Key_Up:
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.5f));
        break;

    case Qt::Key_Down:
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.5f));
    }

    update();
}

void Widget::initShaders()
{
   if (!SceneObjectShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vscene_objects.vsh"))
       pullup(-230, "vertex shader for scene objects cannot be loaded");

   if (!SceneObjectShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fscene_objects.fsh"))
       pullup(-229, "fragment shader for scene objects cannot be loaded");

   if (!SceneObjectShaderProgram.link())
       pullup(-231, "shaders for scene objects cannot be linked");



   if (!SkyBoxShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vskybox.vsh"))
       pullup(-232, "vertex shader for skybox cannot be loaded");

   if (!SkyBoxShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/fskybox.fsh"))
       pullup(-233, "fragment shader for skybox cannot be loaded");

   if (!SkyBoxShaderProgram.link())
       pullup(-234, "shaders for skybox cannot be linked");



   if (!SelectShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vselect.vsh"))
       pullup(-235, "vertex shader for select-objects cannot be loaded");

   if (!SelectShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/fselect.fsh"))
       pullup(-236, "fragment shader for select-objects cannot be loaded");

   if (!SelectShaderProgram.link())
       pullup(-237, "shaders for select-objects cannot be linked");
}

void Widget::initFloor(float width, float height, float depth, const QImage &diffuseMap, const QImage &normalMap)
{
    Object3dBuilder texture;

    texture.build_vertex_buffer(width, height, depth).
            build_index_buffer().
            add_diffuse_map(diffuseMap).
            add_normal_map(normalMap).
            add_shinnes(96.0f).
            add_diffuse_color(QVector3D(1.0f, 1.0f, 1.0f)).
            add_ambience_color(QVector3D(1.0f, 1.0f, 1.0f)).
            add_specular_color(QVector3D(1.0f, 1.0f, 1.0f));

    dimensional *newObj = new dimensional;

    newObj->calculateTBN(texture.getVertexBuff());

    newObj->addObject(new object3D(texture.getVertexBuff(), texture.getIndexBuff(), texture.getMaterial()));
    floor.append(newObj);
}

int Widget::SelectObject(int x, int y, QVector<WorldEngineObject *> &objs)
{

    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST); //для корректной работы оси z: дальние объекты не должны перекрывать ближние

    SelectShaderProgram.bind();
    SelectShaderProgram.setUniformValue("u_projectionMatrix", ProjectionMatrix);;
    m_camera->draw(&SelectShaderProgram);

    for (qsizetype i = 0; i < objs.size(); ++i) {

        SelectShaderProgram.setUniformValue("u_code", float(i + 1)); //i + 1 чтоб не совпадал с цветом фона (чёрный)
        objs[i]->draw(&SelectShaderProgram, context()->functions(), false);
    }

    SelectShaderProgram.release();

    GLint viewport[4]; //x, y, w, h
    glGetIntegerv(GL_VIEWPORT, viewport);

    unsigned char res[4]; //4 компоненты RGBA, каждый по байту, [0-255], поэтому такой тип данных

    //1, 1 - ширина и высота пикселя который нужно считать под указателем мыши
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

    glDisable(GL_DEPTH_TEST);

    return res[0]; //красная компонента
}

void Widget::pullup(int code, const QString &message)
{
   qDebug() << message;

   exit(code);
}

