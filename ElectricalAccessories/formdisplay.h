#ifndef FORMDISPLAY_H
#define FORMDISPLAY_H

#define ITEM_AUTOSORT               true            /*!* \brief 项目自动排序 */
#define ITEM_NAUTOSORT              false
#define PROJECT_CONDITION_OLD       true            /*!* \brief 旧的工程条件 */
#define PROJECT_CONDITION_NEW       false

#define ITEM_ID_LENGTH              30              /*!* \brief 项目ID长度 */
#define ITEM_ICON_SIZE              75              /*!* \brief 项目图标长宽 */

#define IMAGE_CLAUSE19              ":/icon/icon/file6"     /*!* \brief 项目图标资源 */
#define IMAGE_CLAUSE20              ":/icon/icon/file17"
#define IMAGE_CLAUSE21              ":/icon/icon/file19"
#define IMAGE_CLAUSE22              ":/icon/icon/file7"

#define STACKWIDGET_LOGIN           0               /*!* \brief 堆栈窗口切换页数 */
#define STACKWIDGET_MAIN            1
#define STACKWIDGET_CLAUSE19_884    2
#define STACKWIDGET_CLAUSE20_884    3
#define STACKWIDGET_CLAUSE21_884    4
#define STACKWIDGET_CLAUSE22_884    5
#define STACKWIDGET_CLAUSE19_320    6
#define STACKWIDGET_CLAUSE20_320    7
#define STACKWIDGET_CLAUSE21_320    8
#define STACKWIDGET_884AUTO         9
#define STACKWIDGET_320AUTO         10

#define TABWIDGET_SERVOA            0               /*!* \brief 伺服切换页数 */
#define TABWIDGET_SERVOB            1
#define TABWIDGET_SERVOC            2
#define TABWIDGET_SETTING           3

#define REWIRABLE_FIXED_ACCESSORIES             0    /*!* \brief 样品附加类型选择页数 */
#define REWIRABLE_PORTABLE_ACCESSORIES          1
#define NON_REWIRABLE_PORTABLE_SOCKET_OUTLET    2
#define NON_REWIRABLE_PLUG                      3

#define NON_REWIRABLE_PORTABLE_SOCKET_OUTLET_TIP "0.5/0.75/1/1.5"    /*!* \brief 导线横截面积提示 */
#define NON_REWIRABLE_PLUG_TIP      "Tinsel/0.5/0.75/1/1.5"

#define RATED_CURRENT_2_5A          "2.5"           /*!* \brief 额定电流 */
#define RATED_CURRENT_6A            "6"
#define RATED_CURRENT_10A           "10"
#define RATED_CURRENT_13A           "13"
#define RATED_CURRENT_16A           "16"
#define RATED_CURRENT_25A           "25"
#define RATED_CURRENT_32A           "32"
#define RARED_VOLTAGE_130V          "130"           /*!* \brief 额定电压 */
#define RARED_VOLTAGE_250V          "250"
#define RARED_VOLTAGE_440V          "440"

#define HANDLE_RATE_15_MIN          "15"            /*!* \brief 寿命测试机操作速率 */
#define HANDLE_RATE_30_MIN          "30"
#define HANDLE_TIME_1_5_SECOND      "1.5"           /*!* \brief 样品接通时间 */
#define HANDLE_TIME_3_0_SECOND      "3.0"

#include "registrationcode.h"
#include "pathdialog.h"
#include "pieceslist.h"
#include "qmessagewidget.h"
#include <QTableWidget>
#include <QWidget>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class mHelper;
class updateController;

namespace Ui {
class FormDisplay;
}

/*!
 * \brief The mian widget class
 */
class FormDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit FormDisplay(QWidget *parent = 0);
    ~FormDisplay();

    enum PlugType { IEC60320
                    ,IEC60884
                  };                            /*!* \brief IEC模块 */
    enum LifeTesterServo { Servo_A
                           ,Servo_B
                           ,Servo_C
                         };                     /*!* \brief 工作伺服对象 */
    enum TRType { YOKOGAWA_GP10
                 ,AGILENT_34970
                };                              /*!* \brief 无纸记录仪对象 */

    static QString dirName;                     /*!* \brief 当前工作目录 */

    void enterMain();                           /*!* \brief 进入并初始化主界面 */
    void addItemIcon(QString Image, int goal);  /*!* \brief 增加项目图标 */
    bool isChecked(QTableWidgetItem *item);     /*!* \brief 判断item是否已勾选 */
    void initMPiece();                          /*!* \brief 初始化拖拉界面 */
    void uartTips();                            /*!* \brief 串口情况 */

protected:
    void clause19_out(QIODevice *device);       /*!* \brief 章节模块数据输出到本地文件 */
    void clause20_out(QIODevice *device);
    void clause21_out(QIODevice *device);
    void clause22_out(QIODevice *device);

    void clause19_in(QIODevice *device);        /*!* \brief 章节模块数据输入到界面中 */
    void clause20_in(QIODevice *device);
    void clause21_in(QIODevice *device);
    void clause22_in(QIODevice *device);

    void clause19_clear();                      /*!* \brief 初始化模块界面 */
    void clause20_clear();
    void clause21_clear();
    void clause22_clear();
    void clause19_2_clear();
    void clause20_2_clear();
    void clause21_2_clear();

    void IEC320_clear();
    void IEC884_clear();

signals:
    void isSectionFileName(QString,int);        /*!* \brief 已选择的模块名称信号 */
    void tTimerStop(int,int);                   /*!* \brief 定时器停止信号 int为未工作模块数 */
    void btn4Clicked();                         /*!* \brief 保存chua串口设置按钮按下信号 */

public slots:
    void delItem(QListWidgetItem *item);        /*!* \brief 删除项目 */
    void itemRestore(QListWidgetItem *item);    /*!* \brief 将项目移动回备选区 */
    void setTheItem(QListWidgetItem *item);     /*!* \brief 设置项目参数 */
    void moveToZone(QListWidgetItem *item);     /*!* \brief 移动项目到备选区 */
    void finishedItem(QString fileName, int number);/*!* \brief 处理已完成测试的模块 */

protected slots:
    void clickedEnglishAction();                /*!* \brief 使界面显示英语 */
    void clickedChineseAction();                /*!* \brief 使界面显示中文 */

private slots:
    void on_pushButton_login1_clicked();        /*!* \brief IEC60320选择按钮 */
    void on_pushButton_login2_clicked();        /*!* \brief IEC60884选择按钮 */
    void on_pushButton_clicked();               /*!* \brief 懒人模式按钮 */
    void on_pushButton_4_clicked();             /*!* \brief 串口号保存按钮 */
    void on_pushButton_6_clicked();             /*!* \brief 登陆界面下拉菜单按钮 */
    void on_toolButton_19_clicked();            /*!* \brief 章节19模块参数设置打开按钮 */
    void on_toolButton_20_clicked();            /*!* \brief 章节20模块参数设置打开按钮 */
    void on_toolButton_21_clicked();            /*!* \brief 章节21模块参数设置打开按钮 */
    void on_toolButton_22_clicked();            /*!* \brief 章节21.1模块参数设置打开按钮 */
    void on_toolButton_save19_clicked();        /*!* \brief 884 温升参数保存按钮 */
    void on_toolButton_save20_clicked();        /*!* \brief 884 分断参数保存按钮 */
    void on_toolButton_save21_clicked();        /*!* \brief 884 正常操作参数保存按钮 */
    void on_toolButton_save22_clicked();        /*!* \brief 884 操作后温升参数保存按钮 */
    void on_toolButton_save19_2_clicked();      /*!* \brief 320 温升参数保存按钮 */
    void on_toolButton_save20_2_clicked();      /*!* \brief 320 分断参数保存按钮 */
    void on_toolButton_save21_2_clicked();      /*!* \brief 320 正常操作参数保存按钮 */
    void on_toolButtonSave_clicked();           /*!* \brief 884 懒人模式参数保存按钮 */
    void on_toolButtonSave_2_clicked();         /*!* \brief 320 懒人模式参数保存按钮 */
    void on_toolButton_close_clicked();         /*!* \brief 项目退出按钮 */
    void onCloseEditClicked();                  /*!* \brief 退出编辑按钮 */
    void on_toolButton_start_clicked();         /*!* \brief 伺服A启动测试按钮 */
    void on_toolButton_start_2_clicked();       /*!* \brief 伺服B启动测试按钮 */
    void on_toolButton_start_3_clicked();       /*!* \brief 伺服C启动测试按钮 */
    void on_toolButton_TRCR_clicked();          /*!* \brief 显示温升参数设置界面 */
    void on_toolButton_clicked();               /*!* \brief 显示帮助界面 */
    void on_tabWidget_currentChanged(int index);/*!* \brief 改变当前伺服 */
    void on_tabWidget_tabBarClicked(int index); /*!* \brief 判断串口数据是否改变后未保存 */
    void on_tableWidget_temp_cellChanged(int row, int column);/*!* \brief 温升值正则表达式限制 */
    void on_tableWidget_normalTemp_cellChanged(int row, int column);
    void on_tableWidget_temp_2_cellChanged(int row, int column);
    void on_tableWidget_itemChanged(QTableWidgetItem *item);/*!* \brief 改变884标准界面附件类型参数 */
    void on_tableWidget_2_itemChanged(QTableWidgetItem *item);/*!* \brief 改变320标准界面附件类型参数 */
    void on_tableWidget_3_itemChanged(QTableWidgetItem *item);/*!* \brief 改变附件类型参数 */
    void on_tableWidget_4_itemChanged(QTableWidgetItem *item);
    void on_tableWidget_5_itemChanged(QTableWidgetItem *item);
    void on_tableWidget_6_itemChanged(QTableWidgetItem *item);
    void on_lineEdit_tempVn_textChanged(const QString &arg1);/*!* \brief 884标准温升额定电压参数处理 */
    void on_lineEdit_tempVn_t_textChanged(const QString &arg1);/*!* \brief 884标准温升测试电压参数处理 */
    void on_lineEdit_tempIn_textChanged(const QString &arg1);/*!* \brief 884标准温升额定电流参数处理 */
    void on_lineEdit_tempIn_t_textChanged(const QString &arg1);/*!* \brief 884标准温升测试电流参数处理 */
    void on_lineEdit_tempH_textChanged(const QString &arg1);/*!* \brief 884标准温升测试时间参数处理 */
    void on_lineEdit_tempIm_textChanged(const QString &arg1);/*!* \brief 884标准温升短路电流参数处理 */
    void on_lineEdit_tempMaxValue2_textChanged(const QString &arg1);/*!* \brief 884标准温升备用室温参数处理 */
    void on_lineEdit_tempVn_2_textChanged(const QString &arg1);/*!* \brief 320标准温升额定电压参数处理 */
    void on_lineEdit_tempVn_t_2_textChanged(const QString &arg1);/*!* \brief 320标准温升测试电压参数处理 */
    void on_lineEdit_tempIn_2_textChanged(const QString &arg1);/*!* \brief 320标准温升额定电流参数处理 */
    void on_lineEdit_tempIn_t_2_textChanged(const QString &arg1);/*!* \brief 320标准温升测试电流参数处理 */
    void on_lineEdit_tempH_2_textChanged(const QString &arg1);/*!* \brief 320标准温升测试时间参数处理 */
    void on_lineEdit_tempMaxValue2_2_textChanged(const QString &arg1);/*!* \brief 320标准温升备用室温参数处理 */
    void on_lineEdit_breakingVn_textChanged(const QString &arg1);/*!* \brief 884标准分断额定电压参数处理 */
    void on_lineEdit_breakingVn_t_textChanged(const QString &arg1);/*!* \brief 884标准分断测试电压参数处理 */
    void on_lineEdit_breakingIn_textChanged(const QString &arg1);/*!* \brief 884标准分断额定电流参数处理 */
    void on_lineEdit_breakingIn_t_textChanged(const QString &arg1);/*!* \brief 884标准分断测试电流参数处理 */
    void on_lineEdit_breaking_rate_t_textChanged(const QString &arg1);/*!* \brief 884标准分断伺服速度参数处理 */
    void on_lineEdit_breaking_position_t_textChanged(const QString &arg1);/*!* \brief 884标准分断伺服行程参数处理 */
    void on_lineEdit_breaking_handleRate_textChanged(const QString &arg1);/*!* \brief 884标准分断操作速率参数处理 */
    void on_lineEdit_breaking_tongTime_textChanged(const QString &arg1);/*!* \brief 884标准分断接通时间参数处理 */
    void on_lineEdit_breakingVn_2_textChanged(const QString &arg1);/*!* \brief 320标准分断额定电压参数处理 */
    void on_lineEdit_breakingVn_t_2_textChanged(const QString &arg1);/*!* \brief 320标准分断测试电压参数处理 */
    void on_lineEdit_breakingIn_2_textChanged(const QString &arg1);/*!* \brief 320标准分断额定电流参数处理 */
    void on_lineEdit_breakingIn_t_2_textChanged(const QString &arg1);/*!* \brief 320标准分断测试电流参数处理 */
    void on_lineEdit_breaking_rate_t_2_textChanged(const QString &arg1);/*!* \brief 320标准分断伺服速度参数处理 */
    void on_lineEdit_breaking_position_t_2_textChanged(const QString &arg1);/*!* \brief 320标准分断伺服行程参数处理 */
    void on_lineEdit_breakingRn_2_textChanged(const QString &arg1);/*!* \brief 320标准分断功率因素参数处理 */
    void on_lineEdit_normalVn_textChanged(const QString &arg1);/*!* \brief 884标准正常操作额定电压参数处理 */
    void on_lineEdit_normalVn_t_textChanged(const QString &arg1);/*!* \brief 884标准正常操作测试电压参数处理 */
    void on_lineEdit_normalIn_textChanged(const QString &arg1);/*!* \brief 884标准正常操作额定电流参数处理 */
    void on_lineEdit_normalIn_t_textChanged(const QString &arg1);/*!* \brief 884标准正常操作测试电流参数处理 */
    void on_lineEdit_normal_rate_t_textChanged(const QString &arg1);/*!* \brief 884标准正常操作伺服速度参数处理 */
    void on_lineEdit_normal_position_t_textChanged(const QString &arg1);/*!* \brief 884标准正常操作伺服行程参数处理 */
    void on_lineEdit_normal_handleRate_textChanged(const QString &arg1);/*!* \brief 884标准正常操作速率参数处理 */
    void on_lineEdit_normal_tongTime_textChanged(const QString &arg1);/*!* \brief 884标准正常操作接通时间参数处理 */
    void on_lineEdit_normalVn_2_textChanged(const QString &arg1);/*!* \brief 320标准正常操作额定电压参数处理 */
    void on_lineEdit_normalVn_t_2_textChanged(const QString &arg1);/*!* \brief 320标准正常操作测试电压参数处理 */
    void on_lineEdit_normalIn_2_textChanged(const QString &arg1);/*!* \brief 320标准正常操作额定电流参数处理 */
    void on_lineEdit_normalIn_t_2_textChanged(const QString &arg1);/*!* \brief 320标准正常操作测试电流参数处理 */
    void on_lineEdit_normal_rate_t_2_textChanged(const QString &arg1);/*!* \brief 320标准正常操作伺服速度参数处理 */
    void on_lineEdit_normal_position_t_2_textChanged(const QString &arg1);/*!* \brief 320标准正常操作伺服行程参数处理 */
    void on_lineEdit_normal_handleNum_2_textChanged(const QString &arg1);/*!* \brief 320标准正常操作速率参数处理 */
    void on_lineEdit_normalRn_2_textChanged(const QString &arg1);/*!* \brief 320标准正常操作功率因素参数处理 */
    void on_lineEdit_normalTVn_textChanged(const QString &arg1);/*!* \brief 884标准正常操作后温升额定电压参数处理 */
    void on_lineEdit_normalTVn_t_textChanged(const QString &arg1);/*!* \brief 884标准正常操作后温升测试电压参数处理 */
    void on_lineEdit_normalTIn_textChanged(const QString &arg1);/*!* \brief 884标准正常操作后温升额定电流参数处理 */
    void on_lineEdit_normalTIn_t_textChanged(const QString &arg1);/*!* \brief 884标准正常操作后温升测试电流参数处理 */
    void on_lineEdit_normalTH_textChanged(const QString &arg1);/*!* \brief 884标准正常操作后温升测试时间参数处理 */
    void on_lineEdit_normalMaxValue2_textChanged(const QString &arg1);/*!* \brief 884标准正常操作后温升备用室温参数处理 */
    void on_lineEdit_VOLTUART_textChanged(const QString &arg1);/*!* \brief 电源串口号处理 */
    void on_lineEdit_TESTUART_textChanged(const QString &arg1);/*!* \brief 寿命测试机串口号处理 */
    void on_lineEdit_TRCUART_textChanged(const QString &arg1);/*!* \brief 无纸记录仪串口号处理 */
    void on_lineEdit_LOADUART_A_textChanged(const QString &arg1);/*!* \brief 负载串口号处理 */
    void on_lineEdit_LOADUART_B_textChanged(const QString &arg1);
    void on_lineEdit_LOADUART_C_textChanged(const QString &arg1);
    void on_lineEditVn_textChanged(const QString &arg1);/*!* \brief 884标准懒人模式额定电压参数处理 */
    void on_lineEditIn_textChanged(const QString &arg1);/*!* \brief 884标准懒人模式额定电流参数处理 */
    void on_lineEditH_textChanged(const QString &arg1);/*!* \brief 884标准懒人模式测试时间参数处理 */
    void on_lineEditRate_textChanged(const QString &arg1);/*!* \brief 884标准懒人模式伺服速度参数处理 */
    void on_lineEditPosition_textChanged(const QString &arg1);/*!* \brief 884标准懒人模式伺服行程参数处理 */
    void on_lineEditIm_textChanged(const QString &arg1);/*!* \brief 884标准懒人模式短路电流参数处理 */
    void on_lineEditMaxValue_textChanged(const QString &arg1);/*!* \brief 884标准懒人模式备用室温参数处理 */
    void on_lineEditVn_2_textChanged(const QString &arg1);/*!* \brief 320标准懒人模式额定电压参数处理 */
    void on_lineEditIn_2_textChanged(const QString &arg1);/*!* \brief 320标准懒人模式额定电流参数处理 */
    void on_lineEditH_2_textChanged(const QString &arg1);/*!* \brief 320标准懒人模式测试时间参数处理 */
    void on_lineEditRate_2_textChanged(const QString &arg1);/*!* \brief 320标准懒人模式伺服速度参数处理 */
    void on_lineEditPosition_2_textChanged(const QString &arg1);/*!* \brief 320标准懒人模式伺服行程参数处理 */
    void on_lineEditMaxValue_2_textChanged(const QString &arg1);/*!* \brief 320标准懒人模式备用室温参数处理 */
    void on_checkBoxSwitch_toggled(bool checked);/*!* \brief 改变884标准界面L-N/L-E切换参数 */
    void on_checkBoxIC_toggled(bool checked);/*!* \brief 改变884标准界面短路开关参数 */
    void on_checkBox_tempIC_toggled(bool checked);/*!* \brief 884标准温升短路开关处理 */
    void on_checkBox_A_toggled(bool checked);   /*!* \brief 串口通讯情况处理 */
    void on_checkBox_B_toggled(bool checked);   /*!* \brief  */
    void on_checkBox_C_toggled(bool checked);   /*!* \brief  */
    void on_checkBox_VOLT_toggled(bool checked);/*!* \brief  */
    void on_checkBox_TRC_toggled(bool checked); /*!* \brief  */
    void on_checkBox_TEST_toggled(bool checked);/*!* \brief  */
    void on_checkBox_TEST_A_toggled(bool checked);/*!* \brief  */
    void on_checkBox_TEST_B_toggled(bool checked);/*!* \brief  */
    void on_checkBox_TEST_C_toggled(bool checked);/*!* \brief  */
    void on_radioButton_temp_toggled(bool checked);/*!* \brief 特殊模式选择框 */
    void on_radioButton_breaking_toggled(bool checked);/*!* \brief  */
    void on_radioButton_normal_toggled(bool checked);/*!* \brief  */
    void on_radioButton_normalTemp_toggled(bool checked);/*!* \brief  */
    void on_radioButton_temp_2_toggled(bool checked);/*!* \brief  */
    void on_radioButton_breaking_2_toggled(bool checked);/*!* \brief  */
    void on_radioButton_normal_2_toggled(bool checked);/*!* \brief  */

    void on_comboBox_currentIndexChanged(int index);
    void changedStackedWidget(int ind);
    void updateUartState();

private:
    Ui::FormDisplay     *ui;

    bool                isOld;                  /*!* \brief 判断项目时候为旧项目 */
    bool                isAuto;                 /*!* \brief 判断是否为懒人一键设定 */
    bool                isManual;               /*!* \brief 判断是否手动更新串口情况 */

    PlugType            mPlug;                  /*!* \brief IEC对象 */
    LifeTesterServo     mServo;                 /*!* \brief 伺服对象 */
    static TRType       mTRType;                /*!* \brief 无纸记录仪对象 */

    QPixmap             mImage;                 /*!* \brief 拖拉框项目图标 */
    QMenu*              popMenu;                /*!* \brief 下拉菜单 */
    QAction*            englishAction;          /*!* \brief 选择英语 */
    QAction*            chineseAction;          /*!* \brief 选择中文 */
    QAction*            pathAction;             /*!* \brief 打开路径编辑界面 */
    QAction*            RegistrationAction;     /*!* \brief 打开注册码编辑界面 */
    piecesList*         mPiecesViewListA;       /*!* \brief 可拖拉备选框 */
    piecesList*         mPiecesPowerListA;      /*!* \brief 可拖拉待命框 */
    piecesList*         mPiecesViewListB;       /*!* \brief  */
    piecesList*         mPiecesPowerListB;      /*!* \brief  */
    piecesList*         mPiecesViewListC;       /*!* \brief  */
    piecesList*         mPiecesPowerListC;      /*!* \brief  */
    QMessageWidget*     mMessage;               /*!* \brief 底部小提示窗口 */
    mHelper*         m_helpWidget;           /*!* \brief 帮助界面 */

    pathDialog*         mPathDialog;            /*!* \brief 路径编辑界面 */
    RegistrationCode*   mRegistration;          /*!* \brief 注册码编辑界面 */

    int                 mAttachmentType;        /*!* \brief 附件类型 */
    int                 fileSize;               /*!* \brief 工作目录初始路径长度 */

    QString             oldItemName;            /*!* \brief 旧项目名 */
    QStringList         mItemTextList;          /*!* \brief 温升数据list */
    QStringList         mFileNameListA;         /*!* \brief 备选框项目名list */
    QStringList         mFileNameListB;         /*!* \brief  */
    QStringList         mFileNameListC;         /*!* \brief  */
    QStringList         dirNameListA;           /*!* \brief 要测试的项目list */
    QStringList         dirNameListB;           /*!* \brief  */
    QStringList         dirNameListC;           /*!* \brief  */

    updateController    *mUpdateController;

    friend class mTRCRThread;
};

#endif // FORMDISPLAY_H
