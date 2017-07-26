#ifndef TITLEBAR_H
#define TITLEBAR_H

/*!* \brief  */
#define TITLE_HEIGHT                26
#define TITLE_ICON_WIDTH            33
#define TITLE_BUTTON_WIDTH          45
/*!* \brief  */

#include <QWidget>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QLabel;
class QToolButton;

/*!
 * \brief The windows titleBar class
 */
class TitleBar : public QWidget
{
    Q_OBJECT
public:             /*!* \brief flags:显示最大化按钮标识 */
    explicit TitleBar(QWidget *parent, int flags = 0);
    ~TitleBar();

    const int flag;                   /*!* \brief 显示最大化按钮标识 */

signals:
    void closeClicked();        /*!* \brief 窗口关闭信号 */
    void maximizeClicked();     /*!* \brief 窗口最大化信号 */
    void restoreClicked();      /*!* \brief 窗口还原信号 */

protected:                      /*!* \brief 重载函数 */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event)  Q_DECL_OVERRIDE;

private slots:
    void onClicked();           /*!* \brief 按钮点击处理槽函数 */

private:
    QLabel      *mIconLabel;    /*!* \brief 图标 */
    QLabel      *mTitleLabel;   /*!* \brief 标题栏 */
    QToolButton *mMinimizeButton;/*!* \brief 最小化按钮 */
    QToolButton *mMaximizeButton;/*!* \brief 最大化按钮 */
    QToolButton *mRestoreButton;/*!* \brief 还原按钮 */
    QToolButton *mCloseButton;  /*!* \brief 关闭按钮 */
};

#endif // TITLEBAR_H
