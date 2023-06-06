#include "QtGuiClassifire.h"

QtGuiClassifire::QtGuiClassifire(const QString& classifireType, QWidget *parent):
	classifireType_{ classifireType },
	QDialog{parent}
{
	ui.setupUi(this);

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
	setWindowTitle(QString::fromLocal8Bit("Классификатор \"") + classifireType_ + "\"" );

	connect(ui.pb_addValue, &QPushButton::clicked, this, &QtGuiClassifire::slot_addValue);

}

QtGuiClassifire::~QtGuiClassifire()
{}

void QtGuiClassifire::slot_addValue()
{
	ui.listWithValues->addItem(classifireType_ + QString::number(1));
}