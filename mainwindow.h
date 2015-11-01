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

signals:
    void place_node(); // allows a node to be placed
    void node_links(); // allow nodes to be linkeds
    void change_view(int type);
};

#endif // MAINWINDOW_H
