#include "widget.h"

#include <QtMath>
#include <QOpenGLFunctions>

#include "binary.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_camera = new camera();
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
}

Widget::~Widget()
{
    delete m_camera;

    clear_vector(skyboxes);
    clear_vector(groups);
}

void Widget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST | GL_CULL_FACE);

    initShaders();

    groups.append(new group);

    auto square = 3;
    float x = -0.5f, y = -3.5f, z = -8.0;

    for (short X = 0; X < square * square; ++X) {
        if (X > 0 && !(X % square)) {
            z += 5.5f;
            x = -0.5;
        }
        if (X == 0 || X == 1 || X == square * square - 1 || X == square * square - 2)
            init_block(3.0f, 3.0f, 3.0f);
        else
            init_block(3.0f, 3.0f, 3.0f);

        blocks.last()->translate(QVector3D(x, y, z));
        groups.first()->addObject(blocks.last());

        x += 4.5f;
    }

    WorldObjects.append(groups.first());

    QVector3D floor_pos(-square - 3.0f, -3.5f, -square * 4.0f);
    groups.first()->translate(QVector3D(floor_pos));
}

void Widget::resizeGL(int w, int h)
{
    float aspect = w / (float)h;

    ProjectionMatrix.setToIdentity();
    ProjectionMatrix.perspective(45, aspect, /*0.1f*/ 0.01f, /*10.0f*/ 1000.0f); //3 параметр: передняя плоскость отсечения,                                                                             //4 - задняя
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SkyBoxShaderProgram.bind();
    SkyBoxShaderProgram.setUniformValue("u_projectionMatrix", ProjectionMatrix);

    m_camera->draw(&SkyBoxShaderProgram);

    for (qsizetype i = 0; i < skyboxes.size(); ++i)
        skyboxes[i]->draw(&SkyBoxShaderProgram, context()->functions());

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

        int index = SelectObject(event->position().x(), event->position().y(), groups.first()->get_objects());

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

    case Qt::Key_A:
        m_camera->translate(QVector3D(0.5f, 0.0f, 0.0f));
        break;

    case Qt::Key_D:
        m_camera->translate(QVector3D(-0.5f, 0.0f, 0.0f));
        break;

    case Qt::Key_W:
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.5f));
        break;

    case Qt::Key_S:
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.5f));
    }

    update();
}

void Widget::initShaders()
{
   if (!SceneObjectShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vscene_objects.vsh"))
       exit(-230);

   if (!SceneObjectShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fscene_objects.fsh"))
       exit(-229);

   if (!SceneObjectShaderProgram.link())
       exit(-231);



   if (!SkyBoxShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vskybox.vsh"))
       exit(-232);

   if (!SkyBoxShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/fskybox.fsh"))
       exit(-233);

   if (!SkyBoxShaderProgram.link())
       exit(-234);



   if (!SelectShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vselect.vsh"))
       exit(-235);

   if (!SelectShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/fselect.fsh"))
       exit(-236);

   if (!SelectShaderProgram.link())
       exit(-237);
}

void Widget::init_block(float width, float height, float depth)
{
    Object3dBuilder texture;
    texture.build_vertex_buffer(width, height, depth).
            build_index_buffer();

    dimensional *newObj = new dimensional;
    newObj->calculateTBN(texture.getVertexBuff());

    auto m = json_helper::unmarshalize("material_data.json");

    newObj->addObject(new object3D(texture.getVertexBuff(), texture.getIndexBuff(), m));

    blocks.append(newObj);
}

int Widget::SelectObject(int x, int y, const QVector<WorldEngineObject *> &objs)
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

void Widget::add_skybox(WorldEngineObject* s, const QString &texture)
{
    static float pos = skyboxes.size() * 25.0f; // начать или с 0, или с другой позиции,
                                            //если были добавлены через skyboxes.append

    skyboxes.append(dynamic_cast<skybox*>(s));

    if (skyboxes.size() & 1)
        skyboxes.last()->init(25.0f, texture);
    else
        skyboxes.last()->init(25.0f, ":/skybox5.png");

    skyboxes.last()->translate(QVector3D(pos, 0.0f, 0.0f));
    pos += 25.0f;

    update();
}

void Widget::add_scene_object(WorldEngineObject* d, const QString& diffuseMap_path, const QString& normalMap_path)
{
    Object3dBuilder texture;

    texture.build_vertex_buffer(3.0f, 3.0f, 3.0f).
            build_index_buffer().
            add_diffuse_map(diffuseMap_path).
            add_normal_map(normalMap_path).
            add_shinnes(96.0f).
            add_diffuse_color(QVector3D(1.0f, 1.0f, 1.0f)).
            add_ambience_color(QVector3D(1.0f, 1.0f, 1.0f)).
            add_specular_color(QVector3D(1.0f, 1.0f, 1.0f));

    dimensional *newObj = new dimensional;

    newObj->calculateTBN(texture.getVertexBuff());

    newObj->addObject(dynamic_cast<object3D*>(d));
    newObj->last()->Init(texture.getVertexBuff(), texture.getIndexBuff(), texture.getMaterial());

    blocks.append(newObj);

    static float pos = 2.5;

    blocks.last()->translate(QVector3D(-pos - 5.0f, -3.5f, 4.0f));

    pos += 5.0f;

    groups.first()->addObject(blocks.last());

    update();
}
