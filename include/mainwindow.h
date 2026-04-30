// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QModelIndex>

#include "ModelPart.h"
#include "ModelPartList.h"
#include "VRManager.h"

// ===== VTK =====
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVector>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void handleTreeClicked(const QModelIndex &index);

signals:
    void statusUpdateMessage(const QString &message, int timeout);

public slots:
    void handleLoadButton();
    void handleClearButton();
    void handleToggleVRButton();

private slots:
    void on_actionOpenFile_triggered();
    void on_actionItem_Options_triggered();
    void handleItemChanged(const QModelIndex &topLeft,
                           const QModelIndex &bottomRight,
                           const QVector<int> &roles);
    void on_actionRemove_Selected_triggered();
    void on_actionExit_triggered();
private:
    Ui::MainWindow *ui;
    ModelPartList* partList;
    std::unique_ptr<VRManager> m_vr;

    // ===== Exercise 3.3.2  =====
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);
    
    void pushActorsToVR(const QModelIndex& index);
};

#endif // MAINWINDOW_H
