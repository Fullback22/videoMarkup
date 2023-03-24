#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_videoMarkup.h"

#include "Display/QtGuiDisplay.h"

class videoMarkup : public QMainWindow
{
    Q_OBJECT

public:
    videoMarkup(QWidget *parent = nullptr);
    ~videoMarkup();

private:
    Ui::videoMarkupClass ui;
};
