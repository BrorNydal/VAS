#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QHBoxLayout>

#include "ECS/coreengine.h"
#include "resourcemanager.h"
#include "renderwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    //this sets up what's in the mainwindow.ui
    ui->setupUi(this);
    //this->setStyleSheet("background-color: rgb(80, 80, 80); border: 3px solid rgb(60, 60, 60);");
    //ui->CentralWidget->setLayout(new QHBoxLayout());
    init();
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::frameUpdate()
{    
    updateDetails();
}

void MainWindow::updateSceneTree()
{
    ui->sceneTree->clear();

    for(unsigned int w = 0; w < Engine::getWorlds().size(); w++)
    {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->sceneTree);
        treeItem->setText(w, Engine::getWorlds().at(w).getTag().c_str());

        for(unsigned int e = 0; e < Engine::getWorlds().at(w).getEntityManager()->getEntities().size(); e++)
        {
            QTreeWidgetItem *childItem = new QTreeWidgetItem();
            childItem->setText(1, QString::number(Engine::getWorlds().at(w).getEntityManager()->getEntities().at(e).ID));
            treeItem->addChild(childItem);
        }
    }
}

void MainWindow::updateMeshView()
{
    ui->meshList->clear();
    ui->meshComboBox->clear();

    for(unsigned int m = 0; m < ResourceManager::getMeshComponents().size(); m++)
    {
        QListWidgetItem *listItem = new QListWidgetItem(ui->meshList);
        auto text = ResourceManager::getMesh(m)->tag.c_str();
        listItem->setText(text);

        ui->meshComboBox->addItem(text);
    }
}

void MainWindow::updateDetails()
{
    if(Engine::getSelectedEntity() == nullptr)
    {
        ui->detailsDock->setEnabled(false);
        ui->detailsDock->setStyleSheet("color: rgb(40,40,40);");
    }
    else
    {
        ui->detailsDock->setEnabled(true);
        ui->detailsDock->setStyleSheet("color: rgb(255,255,255);");

        TransformComponent *tc = static_cast<TransformComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::transform));
        MeshComponent *mc = static_cast<MeshComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::mesh));

        assert(tc != nullptr && mc != nullptr);

        ui->locationX->setValue(tc->location.x());
        ui->locationY->setValue(tc->location.y());
        ui->locationZ->setValue(tc->location.z());

        ui->rotationX->setValue(tc->rotation.x());
        ui->rotationY->setValue(tc->rotation.y());
        ui->rotationZ->setValue(tc->rotation.z());

        ui->scaleX->setValue(tc->scale.x());
        ui->scaleY->setValue(tc->scale.y());
        ui->scaleZ->setValue(tc->scale.z());

        int i = ui->meshComboBox->findText(mc->tag.c_str());
        ui->meshComboBox->setCurrentIndex(i);
    }
}

void MainWindow::showEntityDetails(Entity *ent)
{

}

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //OpenGL v 4.1 - (Ole Flatens Mac does not support higher than this - sorry!)
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    format.setVersion(4, 1);
    //Using the main profile for OpenGL - no legacy code permitted
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types than OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have some OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The renderer will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Just prints out what OpenGL format we try to get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    //OpenGLLayout is made in the .ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();

    //ui->sceneDock->setFixedSize(QSize(100, 200));

    qDebug() << "(mainwindow) mainwindow initialized!";
}

void MainWindow::on_overviewType_activated(const QString &arg1)
{
    if(arg1 == "Meshes")
    {
        qDebug() << "Show meshes.";


        ui->overviewWidgets->setCurrentIndex(1);
    }
    else if(arg1 == "Scenes")
    {
        qDebug() << "Show scenes.";

        ui->overviewWidgets->setCurrentIndex(0);
    }
    else if(arg1 == "Entities")
    {
        qDebug() << "Show entities.";

        ui->overviewWidgets->setCurrentIndex(2);
    }
    else{
        qDebug() << "Combo box non-existent!";
    }

    qDebug() << "Combobox activated : " << arg1;
}

void MainWindow::on_meshComboBox_activated(const QString &arg1)
{
    if(Engine::getSelectedEntity() != nullptr)
    {
        int mci = ResourceManager::getMeshComponentIndex(arg1.toStdString());

        if(mci < 0)
            qDebug() << "invalid mesh";

        if(mci >= 0)
        {
            Engine::getSelectedEntity()->attachComponent(ResourceManager::getMesh(mci));
        }
        else
        {
            qDebug() << "(mainwindow) No such mesh.";
        }
    }
    else
    {
        qDebug() << "(mainwindow) Selected object nullptr.";
    }
}

void MainWindow::on_sceneTree_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item != nullptr)
    {
        qDebug() << item->text(column);

        bool isInt;
        int ent = item->text(column).toInt(&isInt);

        qDebug() << "Entity index = " << ent;

        if(isInt == false)
        {
            qDebug() << "Selected item is a world object.";
            return;
        }
        else
        {
            qDebug() << "Selected item is an entity.";
        }

        Engine::selectEntity(&Engine::getEditorWorld()->getEntityManager()->getEntities().at(ent));
    }
    else
        qDebug() << "Selected item is pointing to null.";

}

void MainWindow::on_locationX_valueChanged(double arg1)
{
    if(Engine::getSelectedEntity()!=nullptr)
    {
        TransformComponent *tc = static_cast<TransformComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::transform));

        tc->location.setX(arg1);
    }
}

void MainWindow::on_locationY_valueChanged(double arg1)
{
    if(Engine::getSelectedEntity()!=nullptr)
    {
        TransformComponent *tc = static_cast<TransformComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::transform));

        tc->location.setY(arg1);
    }
}

void MainWindow::on_locationZ_valueChanged(double arg1)
{
    if(Engine::getSelectedEntity()!=nullptr)
    {
        TransformComponent *tc = static_cast<TransformComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::transform));

        tc->location.setZ(arg1);
    }
}

void MainWindow::on_rotationX_valueChanged(double arg1)
{
    if(Engine::getSelectedEntity()!=nullptr)
    {
        TransformComponent *tc = static_cast<TransformComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::transform));

        tc->rotation.setX(arg1);
    }
}

void MainWindow::on_rotationY_valueChanged(double arg1)
{
    if(Engine::getSelectedEntity()!=nullptr)
    {
        TransformComponent *tc = static_cast<TransformComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::transform));

        tc->rotation.setY(arg1);
    }
}

void MainWindow::on_rotationZ_valueChanged(double arg1)
{
    if(Engine::getSelectedEntity()!=nullptr)
    {
        TransformComponent *tc = static_cast<TransformComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::transform));

        tc->rotation.setZ(arg1);
    }
}

void MainWindow::on_scaleX_valueChanged(double arg1)
{
    if(Engine::getSelectedEntity()!=nullptr)
    {
        TransformComponent *tc = static_cast<TransformComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::transform));

        tc->scale.setX(arg1);
    }
}

void MainWindow::on_scaleY_valueChanged(double arg1)
{
    if(Engine::getSelectedEntity()!=nullptr)
    {
        TransformComponent *tc = static_cast<TransformComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::transform));

        tc->scale.setY(arg1);
    }
}

void MainWindow::on_scaleZ_valueChanged(double arg1)
{
    if(Engine::getSelectedEntity()!=nullptr)
    {
        TransformComponent *tc = static_cast<TransformComponent*>(Engine::getSelectedEntity()->getComponent(EComponentType::transform));

        tc->scale.setZ(arg1);
    }
}

void MainWindow::on_playButton_clicked()
{
    Engine::play();
}

void MainWindow::on_stopButton_clicked()
{
    Engine::stop();
}
