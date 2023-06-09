#include "QtGuiClassifire.h"

QtGuiClassifire::QtGuiClassifire(const QString& classifireType, QWidget *parent):
	classifireType_{ classifireType },
	QDialog{parent}
{
	ui.setupUi(this);

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
	setWindowTitle(QString::fromLocal8Bit("Классификатор \"") + classifireType_ + "\"" );

	connect(ui.pb_addValue, &QPushButton::clicked, this, &QtGuiClassifire::slot_addValue);
	connect(ui.pb_removeValue, &QPushButton::clicked, this, &QtGuiClassifire::slot_removeValue);
	connect(ui.pb_save, &QPushButton::clicked, this, &QtGuiClassifire::slot_saveClassifire);

}

QtGuiClassifire::~QtGuiClassifire()
{}

void QtGuiClassifire::slot_removeValue()
{
	int activItem{ ui.listWithValues->currentRow() };
	ui.listWithValues->removeItemWidget(ui.listWithValues->takeItem(activItem));
	if (ui.listWithValues->count() == 0)
	{
		ui.pb_removeValue->setDisabled(true);
		ui.pb_save->setDisabled(true);
	}
}

void QtGuiClassifire::slot_saveClassifire()
{
	bool saveClassifire{ true };
	if (checkNotUnuqueValues())
	{
		QMessageBox* msgBox{ new QMessageBox{} };
		msgBox->setText(QString::fromLocal8Bit("В классификаторе имеются идентичные наименовани.\n"
											"При сохранении повторяющиеся элементы будут удалены."));
		msgBox->setInformativeText(QString::fromLocal8Bit("Желаете продолжить?"));
		msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		msgBox->setDefaultButton(QMessageBox::Ok);
		int answer{ msgBox->exec() };
		if (answer == QMessageBox::Cancel)
			saveClassifire = false;
	}
	if (saveClassifire)
	{

		close();
	}
}

void QtGuiClassifire::slot_canel()
{
	close();
}

void QtGuiClassifire::slot_addValue()
{
	int positionNumber{ ui.listWithValues->count() };
	ui.listWithValues->addItem(classifireType_ + QString::number(positionNumber + 1));
	ui.listWithValues->item(positionNumber)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
	if (positionNumber == 0)
	{
		ui.pb_removeValue->setEnabled(true);
		ui.pb_save->setEnabled(true);
	}
}

bool QtGuiClassifire::checkNotUnuqueValues()
{
	int quantityValues{ ui.listWithValues->count() };
	QList<size_t> notUnuque{};
	for (size_t i{}; i < quantityValues; ++i)
	{
		ui.listWithValues->item(i)->setBackgroundColor(QColor{ 255,255,255 });
		int oldListSize{ notUnuque.size() };
		for (size_t j{ i + 1 }; j < quantityValues; ++j)
		{
			if (ui.listWithValues->item(i)->text() == ui.listWithValues->item(j)->text())
				notUnuque.push_back(j);
		}
		if (notUnuque.size() > oldListSize)
			notUnuque.push_back(i);
	}
	for (auto index : notUnuque)
		ui.listWithValues->item(index)->setBackgroundColor(QColor{ 255,0,0 });
	if (notUnuque.size() > 0)
		return true;
	else
		return false;
}
