#ifndef NOFOCUSDELEGATE_H
#define NOFOCUSDELEGATE_H

#include <QObject>
#include <QPainter>
#include <QStyleOption>
#include <QStyledItemDelegate>

class NoFocusDelegate : public QStyledItemDelegate
{
public:
    NoFocusDelegate();
    ~NoFocusDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const;
};

#endif // NOFOCUSDELEGATE_H

NoFocusDelegate::NoFocusDelegate()
{
}

NoFocusDelegate::~NoFocusDelegate()
{
}

void NoFocusDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus)
        itemOption.state = itemOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, itemOption, index);
}
