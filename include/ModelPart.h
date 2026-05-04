#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkDataSetMapper.h>
#include <vtkColor.h>
#include <vtkShrinkFilter.h>
#include <vtkClipDataSet.h>
#include <vtkPlane.h>

/** @file
* This file contains the declarations of the VRInteractorStyle class
*/

  /**
   * @class ModelPart
   * @brief Represents one model item in the TreeView.
   *
   * Stores item data, child hierarchy, visibility, colour and VTK pipeline
   * objects used to render STL geometry.
   */

class ModelPart {
public:
    /** Constructor
      *  Arguments are standard arguments for this type of class but are not used in this example.
      * @param data is not used
      * @param parent is used by the parent class constructor
      */
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);

    /** Destructor
      *  Frees root item allocated in constructor
      */
    ~ModelPart();

    /** 
    * Appends a child to the tree
    * @param item is the item that will be appended to the tree
    */
    void appendChild(ModelPart* item);

    /**
    * Gets a child object that is in a certain row of the tree
    * @param row is the row number for the part in the tree.
    * @return ModelPart object
    */
    ModelPart* child(int row);
    /**
    * Gets the amount of children objects
    * @return integer amount of children
    */
    int childCount() const;

    /**
    * Gets the amount of columns
    * @return integer amount of columns
    */
    int columnCount() const;
    /**
    * Gets the data in a certain column
    * @param column the relevant column
    * @return QVariant object representing the data in that column
    */
    QVariant data(int column) const;
    /**
    * Set the data in a certain column
    * @param value is the new value of the column
    */
    void set(int column, const QVariant& value);

    /**
    * Gets the parent object objects
    * @return ModelPart object
    */
    ModelPart* parentItem();

    /**
    * Gets the row of the parent object
    * @return the number of the row the parent object is in
    */
    int row() const;

    /**
    * Sets the colour for the part
    * @param R is the value for the Red colour
    * @param G is the value for the Green colour
    * @param B is the value for the Blue colour
    */
    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);

    /**
    * Gets the value for the colour Red
    * @return the value of the colour Red (from 0-255)
    */
    unsigned char getColourR();
    /**
    * Gets the value for the colour Green
    * @return the value of the colour Green (from 0-255)
    */
    unsigned char getColourG();
    /**
    * Gets the value for the colour Blue
    * @return the value of the colour Blue (from 0-255)
    */
    unsigned char getColourB();

    /**
    * Sets the value for the visibility of an object
    * @param isVisible is a boolean value of whether or not the object will be visible or not
    */
    void setVisible(bool isVisible);
    /**
    * Gets the value for the object's visibility
    * @return the boolean value of the visibility
    */
    bool visible();

    /**
    * This function handles the loading of an STL file
    * @param fileName is the name of the STL file
    */
    void loadSTL(QString fileName);

    // Return actor for GUI rendering (ENABLE)
    vtkActor* getActor();

    // VR actor later (keep commented until worksheet asks)
    // vtkActor* getNewActor();

    /**
    * Removed a child object
    * @param row is the row of the child object that will be removed
    */
    void removeChild(int row);
    /**
    * Sets the boolean value for the shrink filter
    * @param enabled is a boolean value of whether to turn on or off the filter
    */
    void setShrink(bool enabled);
    /**
    * Gets the boolean value for the shrink filter
    * @return the boolean value of the shrink filter
    */
    bool shrinkEnabled() const;
    void updatePipeline();

    /**
    * Sets the boolean value for the clip filter
    * @param enabled is a boolean value of whether to turn on or off the filter
    */
    void setClip(bool enabled);
    /**
    * Gets the boolean value for the clip filter
    * @return the boolean value of the clip filter
    */
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
