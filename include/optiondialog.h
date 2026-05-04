#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QString>
#include <QColor>

/**
 * @class OptionDialog
 * @brief Dialog for editing selected model part properties.
 *
 * Allows the user to change the selected item's name, visibility,
 * colour and filter options.
 */

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:

    /** 
    * Option Dialog constructor
    * - Sets up the UI
    * - sets up the boxes for RGB
    */
    explicit OptionDialog(QWidget *parent = nullptr);

    /**
    * Option Dialog Deconstructor
    * - Deleted the UI object
    */
    ~OptionDialog();

    // Getters to read user input from the dialog UI

    /**
    * Gets the name of a line
    * @return the name of the line
    */
    QString getName() const;

    /**
    * Gets the visibility of an object
    * @return the visibility (true or false)
    */
    bool isVisibleChecked() const;
    
    /**
    * Gets the red value for the object
    * @return a value between 0 and 255
    */
    int getR() const;

    /**
    * Gets the green value for the object
    * @return a value between 0 and 255
    */
    int getG() const;

    /**
    * Gets the blue value for the object
    * @return a value between 0 and 255
    */
    int getB() const;

    /**
    * Sets the name of a line
    * @param name is the new name of the line
    */
    void setName(const QString& name);
    
    /**
    * Sets the visibility of an object
    * @param checked is the visibility of the object (true or false)
    */
    void setVisibleChecked(bool checked);

    /**
    * Sets the RGB values for the object
    * @param r is the value for red   (between 0-255)
    * @param g is the value for green (between 0-255)
    * @param b is the value for blue  (between 0-255)
    */
    void setRGB(int r, int g, int b);
    
    /**
    * Gets if the shrink filter is active for the object
    * @return true or false
    */
    bool isShrinkChecked() const;

    /**
    * Sets the shrink filter to active or not for the object
    * @param checked is a true or false value
    */
    void setShrinkChecked(bool checked);

    /**
    * Gets if the clip filter is active for the object
    * @return true or false
    */
    bool isClipChecked() const;

    /**
    * Sets the clip filter to active or not for the object
    * @param checked is a true or false value
    */
    void setClipChecked(bool checked);

private slots:
    /**
    * handles what happens when the okay button is clicked on the UI after opening options
    */
    void on_pushButtonColour_clicked();

private:
    Ui::OptionDialog *ui;
    QColor selectedColour;
};

#endif // OPTIONDIALOG_H
