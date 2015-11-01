#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
  connect_updates_to_widgets();
  connect(this, SIGNAL(change_view(int)),
          this->ui->openGLWidget_left_bottom, SLOT(change_camara(int)));
  emit change_view(0);
}

void  MainWindow::connect_updates_to_widgets() {
  // connect updates of opengl widgets
  connect(this->ui->openGLWidget_left_bottom, SIGNAL(update_frame()),
          this->ui->openGLWidget_left_top, SLOT(update_frame_from_extern()));
  connect(this->ui->openGLWidget_left_bottom, SIGNAL(update_frame()),
          this->ui->openGLWidget_right_top, SLOT(update_frame_from_extern()));
  connect(this->ui->openGLWidget_left_bottom, SIGNAL(update_frame()),
          this->ui->openGLWidget_right_bottom, SLOT(update_frame_from_extern()));

  connect(this->ui->openGLWidget_left_top, SIGNAL(update_frame()),
          this->ui->openGLWidget_left_bottom, SLOT(update_frame_from_extern()));
  connect(this->ui->openGLWidget_left_top, SIGNAL(update_frame()),
          this->ui->openGLWidget_right_top, SLOT(update_frame_from_extern()));
  connect(this->ui->openGLWidget_left_top, SIGNAL(update_frame()),
          this->ui->openGLWidget_right_bottom, SLOT(update_frame_from_extern()));

  connect(this->ui->openGLWidget_right_bottom, SIGNAL(update_frame()),
          this->ui->openGLWidget_left_top, SLOT(update_frame_from_extern()));
  connect(this->ui->openGLWidget_right_bottom, SIGNAL(update_frame()),
          this->ui->openGLWidget_right_top, SLOT(update_frame_from_extern()));
  connect(this->ui->openGLWidget_right_bottom, SIGNAL(update_frame()),
          this->ui->openGLWidget_left_bottom, SLOT(update_frame_from_extern()));

  connect(this->ui->openGLWidget_right_top, SIGNAL(update_frame()),
          this->ui->openGLWidget_left_top, SLOT(update_frame_from_extern()));
  connect(this->ui->openGLWidget_right_top, SIGNAL(update_frame()),
          this->ui->openGLWidget_right_bottom, SLOT(update_frame_from_extern()));
  connect(this->ui->openGLWidget_right_top, SIGNAL(update_frame()),
          this->ui->openGLWidget_left_bottom, SLOT(update_frame_from_extern()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_button_node_clicked() {
}

void MainWindow::on_button_link_clicked(){

}
