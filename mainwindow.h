#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void frameUpdate();
    void updateSceneTree();
    void updateMeshView();
    void updateDetails();
    void showEntityDetails(class Entity *ent);

private slots:

    void on_overviewType_activated(const QString &arg1);

    void on_meshComboBox_activated(const QString &arg1);

    void on_sceneTree_itemClicked(class QTreeWidgetItem *item, int column);

    void on_locationX_valueChanged(double arg1);

    void on_locationY_valueChanged(double arg1);

    void on_locationZ_valueChanged(double arg1);

    void on_rotationX_valueChanged(double arg1);

    void on_rotationY_valueChanged(double arg1);

    void on_rotationZ_valueChanged(double arg1);

    void on_scaleX_valueChanged(double arg1);

    void on_scaleY_valueChanged(double arg1);

    void on_scaleZ_valueChanged(double arg1);

    void on_playButton_clicked();

    void on_stopButton_clicked();

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
};

#endif // MAINWINDOW_H
