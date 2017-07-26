#ifndef REGISTRATIONCODE_H
#define REGISTRATIONCODE_H

#include <QDialog>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QLabel;
class QLineEdit;

/*!
 * \brief The registration code dialog class
 */
class RegistrationCode : public QDialog
{
    Q_OBJECT
public:
    explicit RegistrationCode(QWidget *parent = 0);
    ~RegistrationCode();

signals:

public slots:
    void saveClicked();             /*!* \brief 保存注册码到本地.ini */

public:
    QLabel *m_computerNumberLabel;  /*!* \brief 机器码便签 */
    QLabel *m_label;                /*!* \brief 注册码便签 */
    QLineEdit *m_computerNumberLineEdit;/*!* \brief 机器码显示框 */
    QLineEdit *m_lineEdit;          /*!* \brief 注册码输入框 */
    QPushButton *m_pushButton1;     /*!* \brief 保存按钮 */
    QPushButton *m_pushButton2;     /*!* \brief 关闭按钮 */
};

#endif // REGISTRATIONCODE_H
