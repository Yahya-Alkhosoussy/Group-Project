#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QFileDialog>
#include <QFileInfo>
#include <QItemSelectionModel>
#include <QDialog>

#include <QModelIndex>
#include "optiondialog.h"
#include "ModelPart.h"
#include <QString>

#include <vtkNew.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <QDir>
#include <QFileInfoList>

/** @file
* This file contains the definitions of the MainWindow class
*/



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Initialise widgets defined in the .ui file
    ui->setupUi(this);

    // ===== Link VTK render window with Qt widget =====
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    // Add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    // ===== Create a cylinder actor =====
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    // Configure Camera
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
    ui->treeView->addAction(ui->actionItem_Options);
    ui->treeView->addAction(ui->actionWireframe);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeView->addAction(ui->actionRemove_Selected);

    QShortcut* deleteShortcut = new QShortcut(QKeySequence::Delete, ui->treeView);

    connect(deleteShortcut, &QShortcut::activated,
        this,
        &MainWindow::on_actionRemove_Selected_triggered);

    // =====================================================
    // Button handling + status bar messages
    // =====================================================

    connect(ui->pushButtonClear,
            &QPushButton::released,
            this,
            &MainWindow::handleClearButton);

    connect(ui->pushButtonResetView,
        &QPushButton::released,
        this,
        &MainWindow::handleResetViewButton);

    connect(ui->pushButtonToggleVR,
        &QPushButton::released,
        this,
        &MainWindow::handleToggleVRButton);

    connect(ui->horizontalSliderOpacity,
        &QSlider::valueChanged,
        this,
        &MainWindow::handleTransparencySlider);

    connect(ui->pushButtonShowAll,
        &QPushButton::released,
        this,
        &MainWindow::handleShowAllButton);

    // Connect custom signal to status bar
    connect(this,
            &MainWindow::statusUpdateMessage,
            ui->statusbar,
            &QStatusBar::showMessage);



    // =====================================================
    // Model Based TreeView
    // =====================================================

    // Create the model list
    this->partList = new ModelPartList("Parts List");

    // Attach the model to the TreeView widget
    ui->treeView->setModel(this->partList);
    connect(this->partList, &QAbstractItemModel::dataChanged,
            this, &MainWindow::handleItemChanged);

    // Create a demo tree structure
    ModelPart* rootItem = this->partList->getRootItem();

    // Add top-level items
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({name, visible});
        rootItem->appendChild(childItem);

        // Add child items
        for (int j = 0; j < 5; j++) {
            QString childName = QString("Item %1,%2").arg(i).arg(j);
            QString childVisible("true");

            ModelPart* subItem = new ModelPart({childName, childVisible});
            childItem->appendChild(subItem);
        }
    }

    // =====================================================
    // Detect TreeView selection
    // =====================================================
    connect(ui->treeView,
            &QTreeView::clicked,
            this,
            &MainWindow::handleTreeClicked);

  
    m_vr = std::make_unique<VRManager>(this);
    connect(m_vr.get(), &VRManager::vrError, this, [this](const QString& msg) {
        emit statusUpdateMessage("VR error: " + msg, 5000);
    });

    connect(m_vr.get(), &VRManager::vrStarted, this,
        [this](const QString& msg) {
            emit statusUpdateMessage(msg, 3000);
        });

    connect(m_vr.get(), &VRManager::vrStopped, this,
        [this](const QString& msg) {
            emit statusUpdateMessage(msg, 3000);
        });
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::handleClearButton()
{
    if (renderer) {
        renderer->RemoveAllViewProps();
        if (!m_vr->isActive())
            renderWindow->Render();
        else m_vr->clearActors();
    }

    emit statusUpdateMessage("Cleared renderer", 3000);
}


void MainWindow::handleResetViewButton()
{
    if (!renderer || !renderWindow) {
        emit statusUpdateMessage("Renderer not available", 3000);
        return;
    }

    renderer->ResetCamera();
    renderer->ResetCameraClippingRange();
    renderWindow->Render();

    emit statusUpdateMessage("View reset", 3000);
}


void MainWindow::handleToggleVRButton()
{
    if (m_vr->isActive()) {
        m_vr->stop();
        ui->pushButtonToggleVR->setText("Start VR");
        return;
    }

    handleClearButton();

    QString manifestDir = QCoreApplication::applicationDirPath() + "/vrbindings/";
    bool started = m_vr->start(manifestDir);
    if (started) { 
        for (int i = 0; i < partList->rowCount(QModelIndex()); i++) {
            pushActorsToVR(partList->index(i, 0, QModelIndex()));
        }
        emit statusUpdateMessage("Toggling VR", 20); 
        ui->pushButtonToggleVR->setText("Stop VR");
    }

}


void MainWindow::pushActorsToVR(const QModelIndex& index) {
    if (!index.isValid()) return; // do nothing
    QModelIndex idx0 = index.sibling(index.row(), 0);
    ModelPart* part = static_cast<ModelPart*>(idx0.internalPointer());
    if (!part) return;

    vtkActor* actor = part->getActor();
    if (actor && part->visible()) {
        m_vr->addActor(actor);
    }

    int rows = partList->rowCount(idx0);
    for (int i = 0; i < rows; i++) {
        pushActorsToVR(partList->index(i, 0, idx0)); // run the function again for any siblings or children.
    }

}


void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    // Convert the QModelIndex into the underlying ModelPart object
    ModelPart* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    // Retrieve the name stored in column 0
    QString text = selectedPart->data(0).toString();

    // Display the selected item name
    emit statusUpdateMessage(
        QString("The selected item is: ") + text, 0);
}


void MainWindow::on_actionItem_Options_triggered()
{
    emit statusUpdateMessage(QString("Context menu: Options clicked"), 0);
    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) {
        emit statusUpdateMessage("No item selected", 0);
        return;
    }

    // Always use column 0 index to access the item pointer
    QModelIndex idx0 = idx.sibling(idx.row(), 0);
    ModelPart* part = static_cast<ModelPart*>(idx0.internalPointer());
    if (!part) {
        emit statusUpdateMessage("Invalid item pointer", 0);
        return;
    }

    // 1) Open dialog and preload from selected item
    OptionDialog dialog(this);

    dialog.setName(part->data(0).toString());
    dialog.setVisibleChecked(part->visible());
    dialog.setRGB(part->getColourR(), part->getColourG(), part->getColourB());
    dialog.setShrinkChecked(part->shrinkEnabled());
    dialog.setClipChecked(part->clipEnabled());

    // 2) If accepted, save back into the item
    if (dialog.exec() == QDialog::Accepted) {

        part->set(0, dialog.getName());

        // Update visible
        bool vis = dialog.isVisibleChecked();
        part->setVisible(vis);
        part->set(1, vis ? "true" : "false");

        // Update colour
        part->setColour(
            static_cast<unsigned char>(dialog.getR()),
            static_cast<unsigned char>(dialog.getG()),
            static_cast<unsigned char>(dialog.getB())
            );
        part->setShrink(dialog.isShrinkChecked());
        part->setClip(dialog.isClipChecked());
        part->updatePipeline();
        ui->treeView->viewport()->update();

        updateRender();   

        emit statusUpdateMessage("Item updated from dialog", 0);
    }
}


void MainWindow::on_actionOpenFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open STL File"),
        tr("C:\\"),
        tr("STL Files (*.stl);;All Files (*)")
    );

    if (fileName.isEmpty()) {
        emit statusUpdateMessage("Open file cancelled", 2000);
        return;
    }

    QModelIndex parentIndex = ui->treeView->currentIndex();

    // If user selected column 1, force it back to column 0
    if (parentIndex.isValid()) {
        parentIndex = parentIndex.sibling(parentIndex.row(), 0);
    }

    QString shortName = QFileInfo(fileName).fileName();

    // Add new item properly through the model
    QModelIndex newIndex = partList->appendChild(parentIndex, { shortName, "true" });

    ModelPart* newPart = static_cast<ModelPart*>(newIndex.internalPointer());
    if (!newPart) {
        emit statusUpdateMessage("Failed to create tree item", 3000);
        return;
    }

    newPart->loadSTL(fileName);

    if (parentIndex.isValid()) {
        ui->treeView->expand(parentIndex);
    }

    ui->treeView->setCurrentIndex(newIndex);
    updateRender();

    emit statusUpdateMessage("Loaded STL: " + shortName, 3000);
}


void MainWindow::updateRender()
{
    if (m_vr->isActive()) m_vr->clearActors();
    renderer->RemoveAllViewProps();
    

    for (int i = 0; i < partList->rowCount(QModelIndex()); ++i)
        updateRenderFromTree(partList->index(i,0,QModelIndex()));

    if (!m_vr->isActive())
        ui->vtkWidget->renderWindow()->Render();
}


void MainWindow::updateRenderFromTree(const QModelIndex &index)
{
    if (!index.isValid()) return;

    QModelIndex idx0 = index.sibling(index.row(), 0);
    ModelPart* part = static_cast<ModelPart*>(idx0.internalPointer());
    if (!part) return;

    // if actor and part visible = true then add into renderer
    vtkActor* a = part->getActor();   // or part->getActor().GetPointer() ตามที่เธอทำไว้
    if (a) {
        a->SetVisibility(part->visible() ? 1 : 0);
        if (part->visible()) {
            if (m_vr->isActive()) {
                m_vr->addActor(a);
            }
            else {
                renderer->AddActor(a);
            }
        }
    }

    int rows = partList->rowCount(idx0);
    for (int i = 0; i < rows; ++i) {
        updateRenderFromTree(partList->index(i, 0, idx0));
    }
}


void MainWindow::handleItemChanged(const QModelIndex &topLeft,
                                   const QModelIndex &bottomRight,
                                   const QVector<int> &roles)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)
    Q_UNUSED(roles)

    updateRender();
}


void MainWindow::on_actionRemove_Selected_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected to remove", 3000);
        return;
    }

    QModelIndex parent = index.parent();

    partList->removeRow(index.row(), parent);

    updateRender();

    emit statusUpdateMessage("Selected item removed", 3000);
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionWireframe_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid())
        return;

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    if (!part)
        return;

    vtkActor* actor = part->getActor();

    if (!actor)
        return;

    wireframeEnabled = !wireframeEnabled;

    if (wireframeEnabled)
    {
        actor->GetProperty()->SetRepresentationToWireframe();
        emit statusUpdateMessage("Wireframe enabled", 3000);
    }
    else
    {
        actor->GetProperty()->SetRepresentationToSurface();
        emit statusUpdateMessage("Surface enabled", 3000);
    }

    renderWindow->Render();
}


void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete)
    {
        QModelIndex index = ui->treeView->currentIndex();

        if (!index.isValid())
            return;

        ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

        if (!part)
            return;

        QModelIndex parentIndex = index.parent();

        partList->removeRow(index.row(), parentIndex);

        updateRender();

        emit statusUpdateMessage("Model deleted", 3000);
    }

    QMainWindow::keyPressEvent(event);
}


void MainWindow::on_actionOpenFolder_triggered()
{
    QString folderPath = QFileDialog::getExistingDirectory(
        this,
        tr("Select STL Folder"),
        tr("C:\\")
    );

    if (folderPath.isEmpty()) {
        emit statusUpdateMessage("Folder selection cancelled", 2000);
        return;
    }

    QModelIndex current = ui->treeView->currentIndex();

    if (!current.isValid()) {
        emit statusUpdateMessage("Select parent item first", 3000);
        return;
    }

    QModelIndex idx0 = current.sibling(current.row(), 0);
    ModelPart* parentPart = static_cast<ModelPart*>(idx0.internalPointer());

    if (!parentPart)
        return;

    QDir dir(folderPath);

    QStringList filters;
    filters << "*.stl";

    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

    for (const QFileInfo& fileInfo : files)
    {
        QString fileName = fileInfo.absoluteFilePath();
        QString shortName = fileInfo.fileName();

        ModelPart* newPart = new ModelPart({ shortName, "true" });

        parentPart->appendChild(newPart);

        newPart->loadSTL(fileName);
    }

    partList->layoutChanged();

    ui->treeView->expand(idx0);

    updateRender();

    emit statusUpdateMessage("Folder loaded successfully", 3000);
}


void MainWindow::handleTransparencySlider(int value)
{
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid()) {
        emit statusUpdateMessage("Select a model part first", 3000);
        return;
    }

    index = index.sibling(index.row(), 0);

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    if (!part || !part->getActor()) {
        emit statusUpdateMessage("No actor found for selected item", 3000);
        return;
    }

    // Transparency slider:
    // 0 = solid
    // 100 = invisible
    double opacity = 1.0 - (value / 100.0);

    part->getActor()->GetProperty()->SetOpacity(opacity);

    renderWindow->Render();

    emit statusUpdateMessage(
        QString("Transparency: %1%").arg(value),
        1000);
}


void MainWindow::handleShowAllButton()
{
    updateRender();
    renderer->ResetCamera();
    renderWindow->Render();

    emit statusUpdateMessage("All visible models restored", 3000);
}
