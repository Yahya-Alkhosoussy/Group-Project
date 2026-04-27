// optiondialog.cpp
#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    // Optional but recommended: constrain RGB inputs
    ui->spinBoxR->setRange(0, 255);
    ui->spinBoxG->setRange(0, 255);
    ui->spinBoxB->setRange(0, 255);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

QString OptionDialog::getName() const
{
    return ui->lineEditName->text();
}

bool OptionDialog::isVisibleChecked() const
{
    return ui->checkBoxVisible->isChecked();
}

int OptionDialog::getR() const
{
    return ui->spinBoxR->value();
}

int OptionDialog::getG() const
{
    return ui->spinBoxG->value();
}

int OptionDialog::getB() const
{
    return ui->spinBoxB->value();
}
void OptionDialog::setName(const QString& name)
{
    ui->lineEditName->setText(name);
}

void OptionDialog::setVisibleChecked(bool checked)
{
    ui->checkBoxVisible->setChecked(checked);
}

void OptionDialog::setRGB(int r, int g, int b)
{
    ui->spinBoxR->setValue(r);
    ui->spinBoxG->setValue(g);
    ui->spinBoxB->setValue(b);
}
bool OptionDialog::isShrinkChecked() const
{
    return ui->checkBoxShrink->isChecked();
}

void OptionDialog::setShrinkChecked(bool checked)
{
    ui->checkBoxShrink->setChecked(checked);
}
bool OptionDialog::isClipChecked() const
{
    return ui->checkBoxClip->isChecked();
}

void OptionDialog::setClipChecked(bool checked)
{
    ui->checkBoxClip->setChecked(checked);
}