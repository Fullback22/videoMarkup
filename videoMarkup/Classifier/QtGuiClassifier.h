#pragma once

#include <QDialog>
#include <qtextcodec.h>
#include <qstring.h>
#include <qmessagebox.h>
#include "ui_QtGuiClassifire.h"

#include "Classifier.h"

class QtGuiClassifier : public QDialog
{
	Q_OBJECT
	QString classifierType_{};
	
public:
	QtGuiClassifier(const QString& classifierType, QWidget *parent = nullptr);
	~QtGuiClassifier();

	void updateClassifier(Classifier& updetedClassifier) const;
	void setClassifier(const Classifier& classifier);
private:
	Ui::QtGuiClassifireClass ui;

protected slots:
	void slot_addValue();
	void slot_removeValue();
	void slot_saveClassifier();
	void slot_canel();

signals:
	void readyToUpdateClassifier();

private:
	bool checkNotUnuqueValues();

};
