#ifndef MINIDATEWIDGET_H
#define MINIDATEWIDGET_H

#include <QWidget>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QLineEdit;
class QGroupBox;

/*!
 * \brief The data mini widget class
 */
class MiniDateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MiniDateWidget(QWidget *parent = 0);
    ~MiniDateWidget();

    void setText(QString volt, QString current, QString LPF);/*!* \brief 显示数据 */

    QGroupBox*  groupbox;                   /*!* \brief  */
    QLineEdit*  overTimeLineEdit;           /*!* \brief 结束时间 */

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    bool        isPressed;                  /*!* \brief 判断左键按下 */
    QPoint      mPoint;                     /*!* \brief 当前坐标点 */

    QLineEdit*  voltLineEdit;               /*!* \brief 电压编辑框 */
    QLineEdit*  LPFLineEdit;                /*!* \brief 功率因素编辑框 */
    QLineEdit*  currentLineEdit;            /*!* \brief 电流编辑框 */
};

#endif // MINIDATEWIDGET_H
