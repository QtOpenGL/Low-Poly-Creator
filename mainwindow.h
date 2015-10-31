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

private slots:
    void on_button_node_clicked();
    void on_button_link_clicked();

private:
    Ui::MainWindow *ui;
signals:
    void place_node(); // allows a node to be placed
    void node_links(); // allow nodes to be linkeds
};

#endif // MAINWINDOW_H
