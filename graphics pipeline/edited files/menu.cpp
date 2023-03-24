#include "menu.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    RegisterFactories();
}

Menu::~Menu()
{
    delete ui;
}

void Menu::ChangeBackgroundColor(int r, int g, int b)
{
    setAutoFillBackground(true);

    QPalette pal;
    pal.setColor(QPalette::Window, QColor(r, g, b));
    setPalette(pal);
}

void Menu::RegisterFactories()
{
    factory.add<skybox>(SceneObjects::Skybox);

    factory.add<object3D>(SceneObjects::Object3D);
}

void Menu::keyPressEvent(QKeyEvent *event)
{
    ui->canvas->keyPressEvent(event);
}


void Menu::on_skybox_pb_clicked()
{
    WorldEngineObject* obj = factory.pullout(SceneObjects::Skybox);

    ui->canvas->add_skybox(obj, ":/skybox0.png");
}


void Menu::on_block_pb_clicked()
{
    WorldEngineObject* obj = factory.pullout(SceneObjects::Object3D);

    ui->canvas->add_scene_object(obj, ":/cl_ppl.jpg", ":/ice1_n.jpg");
}

