#include "tspinboxdelegate.h"
#include<QspinBox>

TSpinBoxDelegate::TSpinBoxDelegate(QObject *parent):QStyledItemDelegate(parent)
{

}
QWidget *TSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QSpinBox *editor= new QSpinBox(parent);
    editor->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(500000);
    return editor;
}

void TSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSpinBox *spinBox=dynamic_cast<QSpinBox*>(editor);

    int value=index.model()->data(index,Qt::EditRole).toInt();
    spinBox->setValue(value);

}

void TSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinBox=dynamic_cast<QSpinBox*>(editor);
    int value=spinBox->value();
    model->setData(index,value,Qt::EditRole);
}

void TSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
