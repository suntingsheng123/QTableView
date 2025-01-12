#include "tcomboxdelegate.h"
#include<QComboBox>

TComboxDelegate::TComboxDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{


}

void TComboxDelegate::setItem(QStringList items, bool editable)
{
    m_itemList=items;
    m_editable=editable;
}
QWidget *TComboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox *editor=new QComboBox(parent);
    editor->setEditable(m_editable);
    for(auto &item:m_itemList)
    {
        editor->addItem(item);
    }
    return editor;
}

void TComboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comBox=dynamic_cast<QComboBox*>(editor);
    QString str=index.model()->data(index,Qt::EditRole).toString();
    comBox->setCurrentText(str);

}

void TComboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
     QComboBox *comBox=dynamic_cast<QComboBox*>(editor);
    QString str=comBox->currentText();
     model->setData(index,str);
}

void TComboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
