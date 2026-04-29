#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QModelIndex>
#include "ModelPart.h"
#include "ModelPartList.h"
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVector>
#include <QKeyEvent>
#include <QShortcut>
#include <QKeySequence>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE  

/**
 * @brief Main application window for the GroupProject Viewer.
 *
 * Handles the Qt GUI, TreeView interaction, STL loading,
 * rendering updates, model management and VTK display.
 */

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
    void handleResetViewButton();
    void handleStartVRButton();
    void handleStopVRButton();
    void handleTransparencySlider(int value);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void on_actionOpenFile_triggered();
    void on_actionItem_Options_triggered();
    void handleItemChanged(const QModelIndex &topLeft,
                           const QModelIndex &bottomRight,
                           const QVector<int> &roles);
    void on_actionRemove_Selected_triggered();
    void on_actionExit_triggered();

private slots:
    void on_actionWireframe_triggered();
    void on_actionOpenFolder_triggered();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;

    // ===== Exercise 3.3.2  =====
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);
    bool wireframeEnabled = false;
};

#endif // MAINWINDOW_H
