#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#define MACHINE_COOL_MINUTE         30    /*!* \brief 倒计时时长 */
#define MACHINE_COOL_SECOND         0     /*!* \brief  */

#include "titlebar.h"

#include <QDialog>
#include <QTime>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QTimer;
class QLabel;
class QLCDNumber;

/*!
 * \brief The CountDown class
 */
class CountDown : public QDialog
{
    Q_OBJECT
public:
    explicit CountDown(const QString &object, QWidget *parent = 0);
    ~CountDown();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

public slots:
    void onTimeUpdate();    /*!* \brief 更新计时 */
    void closeWidget();     /*!* \brief 关闭窗口 */

private:
    TitleBar*       titleBar;   /*!* \brief 标题栏 */
    QLabel*         m_label;    /*!* \brief 对象名标签 */
    QLCDNumber*     _lcdNumber; /*!* \brief 时间显示屏 */
    QTimer*         _timer;     /*!* \brief 定时器 */
    QTime           duration;   /*!* \brief 倒计时时长 */

    int             minute;     /*!* \brief 分 */
    int             second;     /*!* \brief 秒 */
    QString         m_object;   /*!* \brief 对象名 */
};

#endif // COUNTDOWN_H
