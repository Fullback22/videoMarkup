#include "QtGuiClassifire.h"

QtGuiClassifire::QtGuiClassifire(const QString& classifireType, QWidget *parent):
	classifireType_{ classifireType },
	QDialog{parent}
{
	ui.setupUi(this);

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
	setWindowTitle(QString::fromLocal8Bit("Классификатор \"") + classifireType_ + "\"" );

}

QtGuiClassifire::~QtGuiClassifire()
{}
