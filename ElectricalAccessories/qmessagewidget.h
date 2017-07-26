#ifndef QMESSAGEWIDGET_H
#define QMESSAGEWIDGET_H

#define MESSAGE_HEIGHT                  26
#define MESSAGE_ICON_WIDTH              33
#define MESSAGE_BUTTON_WIDTH            42

#include <QHBoxLayout>
#include <QToolButton>
#include <QWidget>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QLabel;
class QPushButton;

/*!
 * \brief The message widget class
 */
class QMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QMessageWidget(QWidget *parent = 0);
    ~QMessageWidget();

    void setText(const QString &text);/*!* \brief 设置界面显示文本 */

private:
    QToolButton *pCloseButton;      /*!* \brief 界面关闭按钮 */
    QLabel      *mMessageLabel;     /*!* \brief 文本显示便签 */
    QHBoxLayout *pLayout;           /*!* \brief 水平布局对象 */
};

#endif // QMESSAGEWIDGET_H
