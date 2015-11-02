#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void connect_updates_to_widgets();
    void edit_vertex_state();
    void add_vertex_state();
    void remove_vertex_state();
    void add_camara_views();
signals:
    void place_node(); // allows a node to be placed
    void node_links(); // allow nodes to be linkeds
    void change_view_top_left(int type);
    void change_view_top_right(int type);
    void change_view_bottom_left(int type);
    void change_view_bottom_right(int type);

};

#endif // MAINWINDOW_H
