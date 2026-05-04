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
    /**
    * MainWindow constructor
    * - Sets up the UI
    * - Creates the VTK render window and renderer
    * - Connects buttons and signals
    * - Creates the model-based TreeView
    * - Builds a demo tree structure
    * - Detects TreeView item selection
    */
    explicit MainWindow(QWidget *parent = nullptr);
    
    /**
    * Destructor
    * Releases allocated UI resources
    */
    ~MainWindow();

    /**
    * Slot triggered when the user clicks an item in the TreeView
    * Displays the selected item name in the status bar
    * @param index is the index of the actor that is in the partList object.
    */
    void handleTreeClicked(const QModelIndex &index);

signals:
    
    void statusUpdateMessage(const QString &message, int timeout);

public slots:
    
    /**
    * Handling the clear button
    * This function handles what the clear button should do like clearing all the props from the renderers
    */
    void handleClearButton();
    
    /**
    * Handling the reset view button
    * This function handles what the reset view button does.
    */
    void handleResetViewButton();
    
    /**
    * Handling the toggle VR button
    * This function handles what the Toggle VR button does.
    */
    void handleToggleVRButton();
    
    /**
    * handling the transparency slider
    * This function handles how the app reacts to changes in the transparency Slider.
    * @param value is the transparency value given by the slider.
    */
    void handleTransparencySlider(int value);
    
    /**
    * handling the show all button
    * This function handles the show all button.
    */
    void handleShowAllButton();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    
    /**
    * Handling the open file action being triggered
    * This function handles what the app should do when the open file action is triggered.
    */
    void on_actionOpenFile_triggered();

    /**
    * Handling the item options action being triggered
    * This function handles what the app should do when the item options action is triggered.
    */
    void on_actionItem_Options_triggered();
    
    /**
    * handle when the item changed
    * This function handles how a change in an item is done.
    */
    void handleItemChanged(const QModelIndex &topLeft,
                           const QModelIndex &bottomRight,
                           const QVector<int> &roles);
    
    /**
    * handling the remove action being triggered
    * This function handles what the app does when the remove selected action gets triggered.
    */
    void on_actionRemove_Selected_triggered();
    
    /**
    * handling the exit action being triggered
    * This function handles how the app reacts to the exit action being triggered.
    */
    void on_actionExit_triggered();

private slots:
    
    /**
    * handling the wireframe action being triggered
    * This function handles how the app reacts to the wireframe action being triggered.
    */
    void on_actionWireframe_triggered();
    
    /**
    * handling the open folder action being triggered
    * This function handles how the app reacts to the open folder action being triggered.
    */
    void on_actionOpenFolder_triggered();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;
    std::unique_ptr<VRManager> m_vr;

    // ===== Exercise 3.3.2  =====
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    
    /**
    * Updating the Render
    * This function handles how a new frame is rendered.
    */
    void updateRender();
    
    /**
    * Updating the Render from the tree
    * This function adds the objects from the tree to the new rendered frame.
    */
    void updateRenderFromTree(const QModelIndex& index);
    bool wireframeEnabled = false;
    
    /**
    * Pushing Actors to VR
    * This function pushes all loaded actors into VR.
    * @param index is the index of the actor that is in the partList object.
    */
    void pushActorsToVR(const QModelIndex& index);
};

#endif // MAINWINDOW_H
