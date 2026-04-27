/**     @file ModelPart.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>

// VTK headers (ENABLE)
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkDataSetMapper.h>
#include <vtkColor.h>
#include <vtkShrinkFilter.h>
#include <vtkClipDataSet.h>
#include <vtkPlane.h>

class ModelPart {
public:
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);
    ~ModelPart();

    void appendChild(ModelPart* item);
    ModelPart* child(int row);
    int childCount() const;

    int columnCount() const;
    QVariant data(int column) const;
    void set(int column, const QVariant& value);

    ModelPart* parentItem();
    int row() const;

    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);
    unsigned char getColourR();
    unsigned char getColourG();
    unsigned char getColourB();

    void setVisible(bool isVisible);
    bool visible();

    void loadSTL(QString fileName);

    // Return actor for GUI rendering (ENABLE)
    vtkActor* getActor();

    // VR actor later (keep commented until worksheet asks)
    // vtkActor* getNewActor();

    void removeChild(int row);
    void setShrink(bool enabled);
    bool shrinkEnabled() const;
    void updatePipeline();
    void setClip(bool enabled);
    bool clipEnabled() const;

private:
    QList<ModelPart*> m_childItems;
    QList<QVariant>   m_itemData;
    ModelPart*        m_parentItem;

    bool isVisible;
    unsigned char colourR, colourG, colourB;
    bool useShrinkFilter;
    vtkSmartPointer<vtkShrinkFilter> shrinkFilter;

    // VTK data (ENABLE)
    vtkSmartPointer<vtkSTLReader>      file;    // STL reader
    vtkSmartPointer<vtkDataSetMapper> mapper;
    vtkSmartPointer<vtkActor>          actor;   // actor

    vtkColor3<unsigned char> colour;
    bool useClipFilter;
    vtkSmartPointer<vtkClipDataSet> clipFilter;
    vtkSmartPointer<vtkPlane> clipPlane;

};

#endif
