#pragma once

#include <QDialog>
#include <qtextcodec.h>
#include <qstring.h>
#include "ui_QtGuiClassifire.h"

#include "Classifire.h"

class QtGuiClassifire : public QDialog
{
	Q_OBJECT
	QString classifireType_{};
	Classifire classifire_{};
public:
	QtGuiClassifire(const QString& classifireType, QWidget *parent = nullptr);
	~QtGuiClassifire();

private:
	Ui::QtGuiClassifireClass ui;

protected slots:
	void slot_addValue();
};
