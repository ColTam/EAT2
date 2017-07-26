#ifndef TEMPERATURERISEDIALOG_H
#define TEMPERATURERISEDIALOG_H

/*!* \brief 对应按钮切换界面页数 */
#define TRISE_STACKWIDGET_TC        0
#define TRISE_STACKWIDGET_LOAD      1
#define TRISE_STACKWIDGET_VOLT      2
/*!* \brief 按钮样式表 */
#define TRISE_BUTTON_RESET          "QToolButton { border: none;border-radius: 0px;border-bottom-style: solid;border-bottom-color: #F46320;border-bottom-width: 2px;color: #F0F0F0;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #383838, stop:1 #282828);}QToolButton:hover {color: #F46320;}QToolButton:pressed {color: #C84E19;border-bottom-color: #C84E19;}"
#define TRISE_BUTTON_INIT           "QToolButton { border: none;border-radius: 0px;color: #F0F0F0;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #383838, stop:1 #282828);}QToolButton:hover {color: #F46320;border-bottom-style: solid;border-bottom-color: #F46320;border-bottom-width: 2px;}QToolButton:pressed {color: #C84E19;border-bottom-color: #C84E19;}"

typedef int row_t;
typedef int column_t;

#include "minidatewidget.h"
#include "qcustomplot.h"
#include "mloadthread.h"
#include "titlebar.h"
#include "qmessagewidget.h"

#include <QDialog>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QAxObject;
class QDateTime;
class QThread;
namespace Ui {
class TemperatureRiseDialog;
}

/*!
 * \brief The temperature rise dialog class
 */
class TemperatureRiseDialog : public QDialog
{
    Q_OBJECT

public:             /*!* \brief hour:单次测试时长;num:测试次数;Im:短路后电流 */
    explicit TemperatureRiseDialog(time_t hour  = 1, int num = 1, QString Im = "0", QWidget *parent = 0);
    ~TemperatureRiseDialog();

    int loadCount;                                  /*!* \brief 数据计数器 */
    QString servo;                                  /*!* \brief 伺服号 */

    void initDialogTitle();                         /*!* \brief 初始化标题栏 */
    void setTargetTime();                           /*!* \brief 设置目标时间 */
    void initCustomPlot();                          /*!* \brief 初始化曲线图界面 */
    void arrivalTime();                             /*!* \brief 判断测试完成 流程功能处理 */
    void closeEffect();                             /*!* \brief 界面关闭处理 */
    void writeExcel(QString fileName);              /*!* \brief 新建并写入数据到excel fileName:需要保存的文件名称*/
    /*!* \brief 保存一列数据到excel itemName:端口号;column:列;variantList:保存的数据;head:列名称;color:数据颜色;judge:是否保存判断结果 */
    void rangeToWrite(QString itemName, column_t column, QVariantList variantList, QString head,
                      QColor color = QColor(255,255,255), bool judge = false);
    /*!* \brief 保存单个数据到excel row:行;column:列;color:数据颜色 */
    void cellsToWrite(row_t row, column_t column,
                      QColor color = Qt::white);

    void saveCustomPlotData(QDateTime time);        /*!* \brief 保存曲线图数据 */

signals:
    void temperatureRisePortA(QString);             /*!* \brief 发送伺服号 */
    void temperatureRisePortB(QString);             /*!* \brief  */
    void temperatureRisePortC(QString);             /*!* \brief  */
    void loadStart(int);                            /*!* \brief 开始读取负载数据 */
    void loadStop();                                /*!* \brief 停止读取负载数据 */
    void temperatureRiseStart();                    /*!* \brief 开始读取温升数据 */
    void temperatureRiseStop();                     /*!* \brief 停止读取温升数据 */
    void clauseFinishedA(int);                      /*!* \brief 测试完成信号 int:完成方式 */
    void clauseFinishedB(int);                      /*!* \brief  */
    void clauseFinishedC(int);                      /*!* \brief  */

public slots:
    void maximizedWidget();                         /*!* \brief 最大化窗口 */
    void restoreWidget();                           /*!* \brief 还原窗口 */
    void closeWidget();                             /*!* \brief 关闭窗口 */
    void updateLoad(QString volt, QString load);    /*!* \brief 更新负载数据 */
    void updateTC(QStringList list);                /*!* \brief 更新温升数据 */
    /*!* \brief 热电偶端口处理 */
    void temprtatureRiseUpper(QStringList upper_tc_list, int roomTC);

    /*!* \brief 曲线图图例被双击处理 */
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void selectionChanged();                        /*!* \brief 曲线图选择改变处理 */
    void mousePress();                              /*!* \brief 曲线图中有鼠标按下处理 */

    void voltageChange(QString v);                  /*!* \brief 电压微调处理 */

protected:                                          /*!* \brief 重载函数 */
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void on_toolButton_tc_clicked();                /*!* \brief 界面切换功能 */
    void on_toolButton_load_clicked();
    void on_toolButton_volt_clicked();

private:
    Ui::TemperatureRiseDialog *ui;

    TitleBar*           _titleBar;                  /*!* \brief 标题栏对象 */
    QMessageWidget*     _mMessage;                  /*!* \brief 底部提示信息对象 */
    MiniDateWidget*     _miniDateWidget;            /*!* \brief 迷你信息对象 */

    loadController*     _loadController;
    QList<int>          temperatureRiseCount_list;  /*!* \brief 温升端口号list */
    QStringList         temperatureRiseUpper_list;  /*!* \brief 温升端口号对应上限值list */
    QStringList         temperatureRiseRemark_list; /*!* \brief 温升端口号对应备注list */
    QStringList         temperatureRiseUpper_lists; /*!* \brief 无室温温升对应端口号上限值list */
    QStringList         oldTemperatureRise_list;    /*!* \brief 旧温升数据 */

    QList<double>       temperatureRiseCHxxx_list;  /*!* \brief CHXXX温升值 */
    QList<double>       temperatureRiseCOne_list;   /*!* \brief 各端口最大温升值 */

    QDateTime           _currentTime;               /*!* \brief 测试启动时间 */
    QDateTime           _objectTime;                /*!* \brief 测试目标时间 */
    QAxObject*          _worksheet;                 /*!* \brief excel工作簿对象 */

    time_t duration;                                /*!* \brief 测试总时长 */
    time_t timeCount;                               /*!* \brief 已测试时长 */
    int flag;                                       /*!* \brief 短路开关切换标识 */
    int widgetCount;                                /*!* \brief 测试次数 */
    int roomTemperatureRiseC;                       /*!* \brief 备用室温变量 */
    int failNumber;                                 /*!* \brief 温升结果fail计数器 fail>0: fail;fail=0: normal;fail<0: user close test;*/

    bool savePngSecess;                             /*!* \brief png保存结果 */

    QString _Im;                                    /*!* \brief 短路后测试电流 */
    QString m_currentIn;                            /*!* \brief 当前测试电流 */

    static QString to26AlphabetString(int data);    /*!* \brief 返回data对应字母 */
    static void convertToColName(int data, QString &res);/*!* \brief 返回excel专用列数符号 */
};

#endif // TEMPERATURERISEDIALOG_H
