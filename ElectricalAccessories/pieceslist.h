#ifndef PIECESLIST_H
#define PIECESLIST_H

#include <QListWidget>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

/*!
 * \brief The piece list widget class
 */
class piecesList : public QListWidget
{
    Q_OBJECT
public:             /*!* \brief piecesize:item图标大小;issort:自动排序 */
    explicit piecesList(int pieceSize, bool isSort = true, QWidget *parent = 0);
    ~piecesList();

    void addPiece(QPixmap pixmap, QString name);/*!* \brief 窗口增加item */
    static QString pMimeType() { return QStringLiteral("/icon/Icon/x-png"); }

signals:
    void moveItem(QListWidgetItem*);    /*!* \brief item移动信号 */
    void changeItem(QListWidgetItem*);  /*!* \brief item编辑信号 */
    void deleteItem(QListWidgetItem*);  /*!* \brief item删除信号 */

protected:                              /*!* \brief 重载功能函数 */
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void startDrag(Qt::DropActions supportedDropActions) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private slots:                          /*!* \brief item右键按钮槽函数 */
    void clickedMI();
    void clickedCI();
    void clickedDI();

public:
    int                 mPieceSize;     /*!* \brief item图标大小 */
    bool                mSort;          /*!* \brief 判断是否需要自动排序功能 */
    QListWidgetItem*    m_item;         /*!* \brief item对象 */
};

#endif // PIECESLIST_H
