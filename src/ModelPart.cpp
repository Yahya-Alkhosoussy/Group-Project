/**     @file ModelPart.cpp
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"
#include <vtkShrinkFilter.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkSTLReader.h>
#include <vtkDataSetMapper.h>
#include <vtkClipDataSet.h>
#include <vtkPlane.h>

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent )
    : m_itemData(data), m_parentItem(parent)
{
    isVisible = true;
    colourR = 255; colourG = 255; colourB = 255;

    useShrinkFilter = false;
    useClipFilter = false;

    // VTK objects start null until loadSTL called
}

ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart* item) {
    item->m_parentItem = this;
    m_childItems.append(item);
}

ModelPart* ModelPart::child(int row) {
    if (row < 0 || row >= m_childItems.size()) return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const {
    return m_childItems.count();
}

int ModelPart::columnCount() const {
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const {
    if (column < 0 || column >= m_itemData.size()) return QVariant();
    return m_itemData.at(column);
}

void ModelPart::set(int column, const QVariant &value) {
    if (column < 0 || column >= m_itemData.size()) return;
    m_itemData.replace(column, value);
}

ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}

int ModelPart::row() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    colourR = R; colourG = G; colourB = B;

    // If actor exists, update its property immediately
    if (actor) {
        actor->GetProperty()->SetColor(
            colourR / 255.0,
            colourG / 255.0,
            colourB / 255.0
            );
    }
}

unsigned char ModelPart::getColourR() { return colourR; }
unsigned char ModelPart::getColourG() { return colourG; }
unsigned char ModelPart::getColourB() { return colourB; }

void ModelPart::setVisible(bool v) {
    isVisible = v;
    if (actor) actor->SetVisibility(isVisible ? 1 : 0);
}

bool ModelPart::visible() { return isVisible; }

// STL loading

void ModelPart::loadSTL(QString fileName) {
    // 1) Reader
    file = vtkSmartPointer<vtkSTLReader>::New();
    file->SetFileName(fileName.toStdString().c_str());
    file->Update();

    // 2) Mapper
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();

    if (useShrinkFilter) {
        shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
        shrinkFilter->SetInputConnection(file->GetOutputPort());
        shrinkFilter->SetShrinkFactor(0.8);
        shrinkFilter->Update();

        mapper->SetInputConnection(shrinkFilter->GetOutputPort());
    }
    else {
        mapper->SetInputConnection(file->GetOutputPort());
    }

    mapper->Update();

    // 3) Actor
    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // Apply current properties to actor
    actor->SetVisibility(isVisible ? 1 : 0);
    actor->GetProperty()->SetColor(
        colourR / 255.0,
        colourG / 255.0,
        colourB / 255.0
        );
}

vtkActor* ModelPart::getActor() {
    // Return raw pointer (renderer wants vtkActor*)
    return actor.GetPointer();
}
void ModelPart::removeChild(int row)
{
    if (row < 0 || row >= m_childItems.size()) return;

    delete m_childItems.takeAt(row);
}
void ModelPart::setShrink(bool enabled)
{
    useShrinkFilter = enabled;
}

bool ModelPart::shrinkEnabled() const
{
    return useShrinkFilter;
}
void ModelPart::updatePipeline()
{
    if (!file || !mapper) return;

    vtkAlgorithmOutput* currentOutput = file->GetOutputPort();

    // Apply clip filter first if enabled
    if (useClipFilter) {
        clipPlane = vtkSmartPointer<vtkPlane>::New();
        clipPlane->SetOrigin(0.0, 0.0, 0.0);
        clipPlane->SetNormal(1.0, 0.0, 0.0);

        clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
        clipFilter->SetInputConnection(currentOutput);
        clipFilter->SetClipFunction(clipPlane);
        clipFilter->Update();

        currentOutput = clipFilter->GetOutputPort();
    }

    // Apply shrink filter after clip if enabled
    if (useShrinkFilter) {
        shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
        shrinkFilter->SetInputConnection(currentOutput);
        shrinkFilter->SetShrinkFactor(0.8);
        shrinkFilter->Update();

        currentOutput = shrinkFilter->GetOutputPort();
    }

    mapper->SetInputConnection(currentOutput);
    mapper->Update();
}
void ModelPart::setClip(bool enabled)
{
    useClipFilter = enabled;
}

bool ModelPart::clipEnabled() const
{
    return useClipFilter;
}
