#ifndef COLLECT_H
#define COLLECT_H

#define ZQWL_MODE
#define ZQWL_IP                                 "192.168.1.253"
#define ZQWL_PORT                               (1030)
#define WINDOW_ICON                             ":/icon/icon/14"    /*!* \brief 界面图标 */

#define COM_DELAY                               2000            /*!* \brief 串口命令最大延时[0,5000](0-5000s) */
#define COM_TRDELAY                             10000
#define COM_TRDELAYS                            COM_TRDELAY / 1000

#define CLAUSE_19                               19              /*!* \brief 第19章节 */
#define CLAUSE_20                               20              /*!* \brief 第20章节 */
#define CLAUSE_21                               21              /*!* \brief 第21章节 */
#define CLAUSE_22                               22              /*!* \brief 第21.1章节 */
#define CLAUSE_20_VN(char)                      QString::number(char.toFloat()*1.1) /*!* \brief 1.1倍额定电压 */
#define CLAUSE_20_IN(char)                      QString::number(char.toFloat()*1.25)/*!* \brief 1.25倍额定电流 */
#define CLAUSE_20_HANDLENUMBER                  "100"           /*!* \brief 分断标准操作次数:100 */
#define CLAUSE_21_HANDLENUMBER_884_FIRST        "4500"         /*!* \brief 884正常操作标准操作次数1:4500 */
#define CLAUSE_21_HANDLENUMBER_884_LAST         "10000"         /*!* \brief 884正常操作标准总操作次数:10000 */
#define CLAUSE_21_HANDLENUMBER_320_ONLOAD       "2000"          /*!* \brief 320正常操作标准带电操作次数:2000 */
#define CLAUSE_21_HANDLENUMBER_320_NOTONLOAD    "4000"          /*!* \brief 320正常操作标准无电操作次数:4000 */

#define SET_LPF_INDUCTIVE                       "101"           /*!* \brief 负载感性40A+阻性25A设置地址:电流[0,2200](0-220.0A) */
#define SET_INDUCTANCE                          "102"           /*!* \brief 负载感性40A+阻性25A设置地址:电感[0,255](0-255) */
#define SET_LPF_INDUCTIVE_0_6                   "104"           /*!* \brief 负载感性40A+阻性25A设置地址:PF0.6电流[0,40](0-40) */
#define SET_LPF_INDUCTIVE_0_8                   "108"           /*!* \brief 负载感性40A+阻性25A设置地址:PF0.8电流[0,40](0-40) */
#define SET_LPF_RESISTIVE_884                   "106"           /*!* \brief 负载感性40A+阻性25A设置地址:阻性电流[0,300](0-30.0A) */
#define SET_LPF_RESISTIVE_320                   "109"           /*!* \brief 自定义判断符 */

#include <QObject>

#pragma once
#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

typedef struct {
    QString Pf;     //功率因素
    QString In;     //电流
    QString Nu;     //测试次数
    QString Ra;     //测试速度
    QString Po;     //测试行程
    QString Ti;     //测试接通时间
} Option_t;

struct UART_t {
    QString data;   //串口数据
    int com;        //串口号
    Option_t value; //设置参数
};

extern QList<UART_t> devInformation;    //设备信息
extern bool connectSTAS;                //STAT连接状态
extern unsigned int comLoadA;           //伺服A对应负载串口号
extern unsigned int comLoadB;           //伺服B对应负载串口号
extern unsigned int comLoadC;           //伺服C对应负载串口号
extern unsigned int comVolt;            //电源对应串口号
extern unsigned int comTR_YOKOGAWAGP10; //横河无纸记录仪对应串口号
extern unsigned int comTR_AGILENT34970; //安捷伦无纸记录仪对应串口号
extern unsigned int comTest;            //寿命测试机对应串口号

namespace CollectControl {
 /*!* \brief 外部库函数 */
extern "C"{
    char*   GetComputerCpuAndDiskNum();                     /*!* \brief 获取设备机器码 */
    bool    HardConnet(char* RegeditNumber);                /*!* \brief 连接仪器,RegeditNumber为注册码 */
    void    InsertInstrCmd(int ComNo, char* Cmd, char Type, char haveReturn);/*!* \brief 向采集卡插入命令 */
    void    ChangeYSignal(int group, int bit);              /*!* \brief 反转O口状态 */
    void    OnBnClickedButtonHome();                        /*!* \brief 伺服回零 */
    void    OnBnClickedButtonClearlimit();                  /*!* \brief 伺服解除上限位 */
    int     SetIoStatus(int group, int bit, int nIndex);    /*!* \brief 设置O口状态 */
    int     GetIStatus(int group, int bit);                 /*!* \brief 获取I口状态 */
    int     GetOStatus(int group, int bit);                 /*!* \brief 获取O口状态 */
    void    RunServo(int nIndex=0,double MaxVol=1.0,double MaxAcc=1.0,double TargetPos=1.0);/*!* \brief 运行伺服 */
    void    HardCommand(int ComIndex,char* SendStr);        /*!* \brief 给com口发命令无返回 */
    char*   HardSend(int ComIndex,char* SendStr,int LimitTime=5000);/*!* \brief 有返回值命令，LimitTime为最大延时量（ms） */
    char*   GetReceiveMsg(int nIndex);                      /*!* \brief 获取串口返回值 */
    double  GetMotorTargetPos(int nIndex=0);                /*!* \brief 获取伺服绝对位置 */
}
}

class ZQWL;

/*!
 * \brief The Collect class
 */
class Collect : public QObject
{
    Q_OBJECT
public:
    explicit Collect(QObject *parent = 0);

    /*!* \brief PC连接STAS 返回结果 */
    bool ConnectToSTAS();

    /*!* \brief  servo[A,B,C] 对应寿命测试机伺服[A,B,C]*/
    /*!* \brief 短路切换[开] */
    static void ComponentsShortCut(const QString &servo);
    /*!* \brief 短路切换[关] */
    static void ComponentsNoShortCut(const QString &servo);
    /*!* \brief LN/LE切换[切换至LN] */
    static void CutToLN(const QString &servo);
    /*!* \brief LN/LE切换[切换至LE] */
    static void CutToLE(const QString &servo);
    /*!* \brief 设置电源电压 */
    static void SetVolt(const QString &volt);
    /*!* \brief 设置负载在275V下0.6功率因素电流 com_t:串口号 current:电流 */
    static QString setLoadLPF275_6(int com_t, const QString &current);
    /*!* \brief 设置负载在250V下0.8功率因素电流 com_t:串口号 current:电流 */
    static QString setLoadLPF250_8(int com_t, const QString &current);
    /*!* \brief 设置负载电流 LPFactor:功率因素地址 current:电流 record:保存电流 */
    static void SetLoad(const QString LPFactor, const QString current,  const QString servo, bool record = true);
    /*!* \brief 寿命测试机测试计数设置 number:数量 */
    static void SetTestNumber(const QString number, const QString servo);
    /*!* \brief 寿命测试机测试周期设置 number:数量 */
    static void SetTestCycleNumber(const QString number, const QString servo);
    /*!* \brief 寿命测试机步骤设置 number:数量 rate:速度 position:行程 time:接通时间 */
    static void SetTest(const QString number,   const QString rate,     const QString position,
                        const QString time,     const QString servo);

    /*!* \brief 启动电源 */
    static void PowerStart();
    /*!* \brief 启动负载 */
    static void LoadStart(const QString servo);
    /*!* \brief 启动寿命测试机 */
    static void TestStart(const QString servo);

    /*!* \brief 关闭电源 */
    static void PowerStop();
    /*!* \brief 关闭负载 */
    static void LoadStop(const QString servo);
    /*!* \brief 关闭寿命测试机 */
    static void TestStop(const QString servo);

    /*!* \brief 获取寿命测试机当前计数 */
    static QString GetTestNumber(const QString servo);
    /*!* \brief 获取寿命测试机当前离原点距离 */
    static QString GetTestPosition(const QString servo);
    /*!* \brief 寿命测试机回原点 */
    static void TestComeBackToOrigin(const QString servo);
    /*!* \brief 清空寿命测试机测试计数  */
    static void TestClearNumber(const QString servo);

    enum AlarmState{
        open = true,
        close = !open
    };
    /*!* \brief 打开蜂鸣器 */
    static void AlarmOpen();
    /*!* \brief 关闭蜂鸣器 */
    static void AlarmClose();
    static void TestAlarmClose(QString servo);

    /*!* \brief 寿命测试机开启触摸屏显示 */
    static void TestDisplayOpen();
    /*!* \brief 寿命测试机关闭触摸屏显示 */
    static void TestDisplayClose();

    /*!* \brief 获取正确温升数据 过滤温升外的数据 */
    static void getTemperatureRise(QStringList *list);

    /*!* \brief 转换三色状态灯状态 待机 */
    static void stateStand();
    /*!* \brief 转换三色状态灯状态 运行  */
    static void stateRunning();
    /*!* \brief 转换三色状态灯状态 出错  */
    static void stateWarning();
    /*!* \brief 转换三色状态灯状态 关闭  */
    static void stateAllStop();

    /*!* \brief 寿命测试机伺服A运行状态 */
    static bool IsTestServoA;
    /*!* \brief 寿命测试机伺服B运行状态 */
    static bool IsTestServoB;
    /*!* \brief 寿命测试机伺服C运行状态 */
    static bool IsTestServoC;


    /*!* \brief 注册码 */
    static QString registrNumber;
    /*!* \brief 电压值 */
    static QString voltageA;
    /*!* \brief 伺服A对应负载电流值 */
    static QString currentA;
    /*!* \brief 伺服B对应负载电流值 */
    static QString currentB;
    /*!* \brief 伺服C对应负载电流值 */
    static QString currentC;
    /*!* \brief 伺服A对应负载功率因素值 */
    static QString loadPowerFactorA;
    /*!* \brief 伺服B对应负载功率因数值 */
    static QString loadPowerFactorB;
    /*!* \brief 伺服C对应负载功率因素值 */
    static QString loadPowerFactorC;

    static AlarmState mAlarm;

    static ZQWL *mZQWL;
    static void writeCommon(int comIndex, const QString &text);
    static QString writeReturn(int comIndex, const QString &text, time_t msec = 5000);
};

#endif // COLLECT_H
