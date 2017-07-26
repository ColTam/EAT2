#ifndef WIDGET_H
#define WIDGET_H

#define WINDOW_WIDTH            960
#define WINDOW_HEIGHT           720

#include "Collect.h"
#include "formdisplay.h"
#include "titlebar.h"
#include "mtestthread.h"
#include "mtrcrthread.h"

#include <QWidget>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QPropertyAnimation;
class QThread;

/*!
 * \brief The widget class
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void initWidgetTitle();             /*!* \brief 初始化标题栏 */
    void initWidget();                  /*!* \brief 初始化界面 */
    /*!* \brief 新建温升测试界面 hours:单次测试时长;fileName:文件名;tc_list:温升端口数据;roomT2:第二室温;servo:伺服号;num:测试次数;Im:短路后电流 */
    void newTemperatureDialog(time_t hours, QString fileName, QStringList tc_list, int roomT2,
                              QString servo, int num, QString Im = "0");
    /*!* \brief 新建寿命测试机界面 fileName:文件名; servo:伺服号 */
    void newLifeTesterDialog(QString fileName, QString servo);

signals:
    void temperatureRiseUpperA(QStringList, int);/*!* \brief 发送温升端口号、上限值、备注及第二室温 */
    void temperatureRiseUpperB(QStringList, int);/*!* \brief  */
    void temperatureRiseUpperC(QStringList, int);/*!* \brief  */
    void finishedItem(QString, int);    /*!* \brief 发送测试完成的项目 */

    void testData(const QString &, const QString &, const QString &);/*!* \brief 寿命测试机数据 */
    void testTimerStart();              /*!* \brief 开始读取寿命测试机数据 */
    void testTimerStop();               /*!* \brief 停止读取寿命测试机数据 */
    void tRCRData(const QStringList &);         /*!* \brief 温升数据 */
    void tRCRTimerStart();              /*!* \brief 开始读取无纸记录仪数据 */
    void tRCRTimerStop();               /*!* \brief 停止读取无纸记录仪数据 */

    void testAThreadStart();            /*!* \brief 使寿命测试机伺服A启动 */
    void testAThreadStop();             /*!* \brief 使寿命测试机伺服A停止 */
    void testBThreadStart();            /*!* \brief  */
    void testBThreadStop();             /*!* \brief  */
    void testCThreadStart();            /*!* \brief  */
    void testCThreadStop();             /*!* \brief  */

public slots:
    void closeWidget();                 /*!* \brief 关闭窗口 */
    void selectClause(QString fileName, int iec);/*!* \brief 已选择章节处理 */
    /*!* \brief 测试完成处理 */
    void finishedClauseA(int number) { emit finishedItem(mFileNameA, number); }
    void finishedClauseB(int number) { emit finishedItem(mFileNameB, number); }
    void finishedClauseC(int number) { emit finishedItem(mFileNameC, number); }

    void tTimerStart();                 /*!* \brief 开始读取无纸记录仪数据 */
    void tTimerStop(int r, int e);      /*!* \brief 停止读取无纸记录仪、寿命测试机数据 */
    void testStartRead();               /*!* \brief 开始读取寿命测试机数据 */
    void testStopRead();                /*!* \brief 停止读取寿命测试机数据 */

    void testError(QString servo);      /*!* \brief 寿命测试机样品出错处理 */
    void testNoError(QString servo);    /*!* \brief 寿命测试机样品出错处理后 恢复处理 */

protected:
    void temperatureRise(QString fileName);         /*!* \brief 884标准 温升模块参数设置 */
    void breakingCapacityTest(QString fileName, int iec = 1);/*!* \brief 分断模块参数设置 */
    void normalOperation(QString fileName);         /*!* \brief 884标准 正常操作参数设置 */
    void normalOperationTemperatureRise(QString fileName);/*!* \brief 884标准 正常操作后温升参数设置 */
    void linkerTemperature(QString fileName);       /*!* \brief 320标准 温升参数设置 */
    void linkerNormalOperation(QString fileName);   /*!* \brief 320标准 正常操作参数设置 */

private:
    Collect*            _collect;       /*!* \brief  */
    TitleBar*           _titleBar;      /*!* \brief 标题栏 */
    FormDisplay*        _formDisplay;   /*!* \brief 主界面 */
    QPropertyAnimation* _animation;     /*!* \brief 界面关闭效果对象 */

    QString             mServo;         /*!* \brief 伺服号 */
    QString             mFileNameA;     /*!* \brief 文件名 */
    QString             mFileNameB;     /*!* \brief  */
    QString             mFileNameC;     /*!* \brief  */

    testController*     _testController;/*!* \brief 寿命测试机通讯线程 */
    trController*       _trController;

    bool                _isTR;          /*!* \brief 判断启动的是否为温升测试 */
    bool                _isIngA;        /*!* \brief 判断寿命测试机伺服A是否工作 */
    bool                _isIngB;        /*!* \brief  */
    bool                _isIngC;        /*!* \brief  */

private:    /*!* \brief d:;pf:功率因素;In:电流;Nu:测试总数;Ra:测试速率;Po:测试行程;Ti:接通时间 */
    void recordValue(int d, QString pf, QString In, QString Nu, QString Ra, QString Po, QString Ti);
};

#endif // WIDGET_H
