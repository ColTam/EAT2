#include "pieceslist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
#include <QMenu>

piecesList::piecesList(int pieceSize, bool isSort, QWidget *parent)
    : QListWidget(parent),
      mPieceSize(pieceSize),
      mSort(isSort),
      m_item(nullptr)
{
    setDragEnabled(true);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(mPieceSize, mPieceSize));
    setSpacing(3);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    setSortingEnabled(isSort);
}

piecesList::~piecesList()
{
    delete m_item;
}

void piecesList::addPiece(QPixmap pixmap, QString name)
{
    QListWidgetItem *pieceItem = new QListWidgetItem(this);
    pieceItem->setIcon(QIcon(pixmap));
    pieceItem->setText(name);
    pieceItem->setData(Qt::UserRole, QVariant(pixmap));
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
}

void piecesList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(piecesList::pMimeType())) {
        event->accept();
    } else {
        event->ignore();
    }
}

void piecesList::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(piecesList::pMimeType())) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void piecesList::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(piecesList::pMimeType())) {
        QByteArray pieceData = event->mimeData()->data(piecesList::pMimeType());
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QString name;
        QPixmap pixmap;
        dataStream >> name >> pixmap;

        addPiece(pixmap, name);

        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void piecesList::startDrag(Qt::DropActions supportedDropActions)
{
    Q_UNUSED(supportedDropActions);

    m_item = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QString name = m_item->text();
    QPixmap pixmap = qvariant_cast<QPixmap>(m_item->data(Qt::UserRole));

    dataStream << name << pixmap;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(piecesList::pMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width() * 0.15, pixmap.height() * 0.15));    //the pixmap with mouse_key.pos
    drag->setPixmap(pixmap.scaled(pixmap.width() * 0.3, pixmap.height() * 0.3));//pixmap.w&h
                                                                        // 1 : 2

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
        delete takeItem(row(m_item));
    }
}

void piecesList::contextMenuEvent(QContextMenuEvent *event)
{
    if (!mSort || itemAt(event->pos()) == NULL) {
        return;
    }

    QMenu *popMenu = new QMenu(this);

    QAction *MI = new QAction();
    MI->setText(tr("Move To Waiting Zone"));
    connect(MI, SIGNAL(triggered(bool)), this, SLOT(clickedMI()));

    QAction *CI = new QAction();
    CI->setText(tr("Change The Item"));
    connect(CI, SIGNAL(triggered(bool)), this, SLOT(clickedCI()));

    QAction *DI = new QAction();
    DI->setText(tr("Delete The Item"));
    connect(DI, SIGNAL(triggered(bool)), this, SLOT(clickedDI()));

    if (itemAt(event->pos()) != NULL) {
        popMenu->addAction(MI);
        popMenu->addAction(CI);
        popMenu->addAction(DI);
        popMenu->exec(QCursor::pos());
    }

    delete DI;
    delete CI;
    delete MI;
    delete popMenu;
}

void piecesList::clickedMI()
{
    emit moveItem(currentItem());
}

void piecesList::clickedCI()
{
    emit changeItem(currentItem());
}

void piecesList::clickedDI()
{
    emit deleteItem(currentItem());
}
