#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include <QPalette>
#include <QPixmap>
#include <QFont>

#include "widget.h"
#include "graphicspipeline.h"

enum SceneObjects {

    Skybox,

    Object3D,
};


namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:

    explicit Menu(QWidget *parent = nullptr);

    ~Menu();

    void ChangeBackgroundColor(int r, int g, int b);

protected:

    void RegisterFactories();

    virtual void keyPressEvent(QKeyEvent *event);

private:
    Ui::Menu *ui;
    Widget *w;

    GraphicsPipeline<WorldEngineObject, SceneObjects> factory;

private slots:
    void on_skybox_pb_clicked();
    void on_block_pb_clicked();
};

#endif // MENU_H
