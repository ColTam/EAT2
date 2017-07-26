#ifndef LIFETESTERDIALOG_H
#define LIFETESTERDIALOG_H

#define TEST_JUDGEMENT_TIME         12  /*!* \brief 判断常通/常断时长(s) */
#define TEST_READTIME               1000/*!* \brief 读取负载数据间隔 */

#include "mloadthread.h"
#include "qmessagewidget.h"
#include "titlebar.h"

#include <QDialog>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QThread;

namespace Ui {
class LifeTesterDialog;
}

/*!
 * \brief The lifetester dialog class
 */
class LifeTesterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LifeTesterDialog(QWidget *parent = 0);
    ~LifeTesterDialog();


    void initDialogTitle();         /*!* \brief 初始化标题栏 */
    void closeEffect();             /*!* \brief 界面关闭处理 */
    bool judgement(QString newCurrent);/*!* \brief 电流常通常断判断 流程功能处理 */
    void resetTest(QString servo);  /*!* \brief 重设置设备参数 */
    /*!* \brief 十六进制数转换十进制数 */
    QString HexTodec(QString Hex) {return QString::number(Hex.toInt(nullptr, 16));}

signals:
    void loadStart(int);            /*!* \brief 开始读取负载数据 */
    void loadStop();                /*!* \brief 停止读取负载数据 */
    void testTimerStart();          /*!* \brief 开始读取寿命测试机数据 */
    void testTimerStop();           /*!* \brief 停止读取寿命测试机数据 */
    void clauseFinishedA(int);      /*!* \brief 测试完成信号 int:完成方式 */
    void clauseFinishedB(int);      /*!* \brief  */
    void clauseFinishedC(int);      /*!* \brief  */
    void testServoA(QString);       /*!* \brief 发送伺服号 */
    void testServoB(QString);       /*!* \brief  */
    void testServoC(QString);       /*!* \brief  */
    void tRcrTimerStart();          /*!* \brief 开始读取温升数据 */
    void tRcrTimerStop();           /*!* \brief 停止读取温升数据 */

    void errorStart(QString);       /*!* \brief 启动出错信号 停止其他正在工作的伺服 */
    void errorStop(QString);        /*!* \brief 停止出错信号 恢复工作伺服 */

public slots:
    void closeDialog();             /*!* \brief 关闭界面处理 */
    void isOrigin();                /*!* \brief 判断机器是否已回原点 */
    void updateLoad(QString volt, QString load);/*!* \brief 更新负载数据 */
    void updateTest(const QString &list1, const QString &list2, const QString &list3);  /*!* \brief 更新测试数据 */

    void STOPTEST();                /*!* \brief 停止工作伺服 */
    void voltageChange(QString v);  /*!* \brief 电压微调功能 */
    void errorLoadStart();          /*!* \brief 样品出错 停止工作 */
    void errorLoadStop();           /*!* \brief 样品出错 恢复工作 */

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void on_lineEdit_W_textChanged(const QString &arg1);/*!* \brief 显示样品出错次数 */

private:
    Ui::LifeTesterDialog *ui;

    QMessageWidget* mMessage;       /*!* \brief 底部提示信息 */
    TitleBar*       _titleBar;      /*!* \brief 标题栏 */
    QTimer*         _timer;         /*!* \brief 判断机器回原点定时器 */
    loadController* _loadController;

    int     loadCount;              /*!* \brief 计数器 */
    QString oldCurrent;             /*!* \brief 旧电流数据 */
    QString servo;                  /*!* \brief 当前测试伺服号 */
    int             judgeCount1;    /*!* \brief 电流常通计数器 */
    int             judgeCount2;    /*!* \brief 电流常断计数器 */
    int             errorCount;     /*!* \brief 样品出错计数器 */

    QString         oldPosition;    /*!* \brief 旧机器离原点距离 */
};

#endif // LIFETESTERDIALOG_H
