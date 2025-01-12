#ifndef TCOMBOXDELEGATE_H
#define TCOMBOXDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TComboxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:
    QStringList m_itemList;
    bool m_editable;
public:
    explicit TComboxDelegate(QObject *parent = nullptr);
    void setItem(QStringList items,bool editable);
    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};



#endif // TCOMBOXDELEGATE_H
