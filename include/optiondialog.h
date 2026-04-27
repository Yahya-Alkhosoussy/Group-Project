// optiondialog.h
#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    // Getters to read user input from the dialog UI
    QString getName() const;
    bool isVisibleChecked() const;
    int getR() const;
    int getG() const;
    int getB() const;
    void setName(const QString& name);
    void setVisibleChecked(bool checked);
    void setRGB(int r, int g, int b);
    bool isShrinkChecked() const;
    void setShrinkChecked(bool checked);
    bool isClipChecked() const;
    void setClipChecked(bool checked);

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
