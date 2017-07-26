#ifndef PATHDIALOG_H
#define PATHDIALOG_H

#include <QDialog>
#include <QLabel>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QLineEdit;

/*!
 * \brief The path dialog class
 */
class pathDialog : public QDialog
{
    Q_OBJECT
public:
    explicit pathDialog(QWidget *parent = 0);
    ~pathDialog();

signals:

public slots:
    void defaultClicked();              /*!* \brief 按键点击槽函数 */
    void currentClicked();              /*!* \brief  */
    void saveClicked();                 /*!* \brief  */

public:
    QLabel*         pathLabel;          /*!* \brief 默认路径便签 */
    QLineEdit*      defaultPathLineEdit;/*!* \brief 默认路径输入框 */
    QPushButton*    pathPushButton;     /*!* \brief 默认路径浏览按钮 */
    QLabel*         pathLabel_2;        /*!* \brief 当前路径便签 */
    QLineEdit*      currentPathLineEdit;/*!* \brief 当前路径输入框 */
    QPushButton*    pathPushButton_2;   /*!* \brief 当前路径浏览按钮 */
    QPushButton*    savePushButton;     /*!* \brief 保存按钮 */
    QPushButton*    closePushButton;    /*!* \brief 界面关闭按钮 */
};

#endif // PATHDIALOG_H
