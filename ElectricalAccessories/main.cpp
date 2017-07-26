/***************************************************************************
**                                                                        **
**  email: jason0x4@outlook.com                                           **
**  address: https://github.com/jason0x4/tonstep.git                      **
**  单样品、完整流程运行成功                                              **
**  同类型多样品流程成功运行                                              **
**  不同类型多样品测试待优化                                              **
**                                                                        **
****************************************************************************/

/* 2017-05-02 18:01:07 +0200 */

#include "widget.h"
#include "msplashscreen.h"
#include "Collect.h"

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QDesktopWidget>
#include <Windows.h>
#include <QSettings>
#include <QDebug>
#include <QFile>

#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    QFile qss(":/other/dark.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    FormDisplay::dirName = QCoreApplication::applicationDirPath();//获取当前程序执行目录路径
    //read setting .ini
    QSettings *configIniRead = new QSettings("EATconfig.ini", QSettings::IniFormat);//获取本地配置文件

    QString voltageU    = configIniRead->value("Voltage Uart").toString();
    QString TRU1        = configIniRead->value("Temperature Rise Uart1").toString();
    QString TRU2        = configIniRead->value("Temperature Rise Uart2").toString();
    QString LoadAU      = configIniRead->value("Load(ServoA) Uart").toString();
    QString LoadBU      = configIniRead->value("Load(ServoB) Uart").toString();
    QString LoadCU      = configIniRead->value("Load(ServoC) Uart").toString();
    QString lifeTestU   = configIniRead->value("Life Tester Uart").toString();
    QString path        = configIniRead->value("Default Path").toString();
    int language        = configIniRead->value("Language").toInt();

    //load language  cn/en
    QTranslator translator;
    translator.load( ":/other/zh_CN.qm" );//加载汉化包
    if (language == 1) {//判断是否安装汉化包
        a.installTranslator( &translator );
    }
    if (!voltageU.isEmpty()){//更新串口号
        comVolt  = voltageU.toInt();
    }
    if (!TRU1.isEmpty()){
        comTR_YOKOGAWAGP10 = TRU1.toInt();
    }
    if (!TRU2.isEmpty()){
        comTR_AGILENT34970 = TRU2.toInt();
    }
    if (!LoadAU.isEmpty()){
        comLoadA = LoadAU.toInt();
    }
    if (!LoadBU.isEmpty()){
        comLoadB = LoadBU.toInt();
    }
    if (!LoadCU.isEmpty()){
        comLoadC = LoadCU.toInt();
    }
    if (!lifeTestU.isEmpty()){
        comTest  = lifeTestU.toInt();
    }
    if (!path.isEmpty()) {//更新保存路径
        FormDisplay::dirName = path;
    }
    delete configIniRead;//释放加载的本地文件

    Widget w;//初始化界面
    mSplashScreen *splash = new mSplashScreen(QPixmap(":/image/image/SGS01.jpg"));//设置加载界面

    splash->show();//显示加载界面
    splash->setGeometry((QApplication::desktop()->width()-480)/2,(QApplication::desktop()->height()-270)/2,480,270);
    splash->setLabelTest(QObject::tr("Is connecting STAS and Configure the local file"));
    splash->setProgress(1);     //

    Sleep(2000);//停留两秒表示前面加载工作 然后开始对STAS的各个串口连接情况进行判断
    //voltage               0
    splash->setLabelTest(QObject::tr("Checking the Power Supply UART..."));//设置加载界面进度条文字 下同
    splash->setProgress(100/10+(qrand()%10-5));//设置进度条进度 下同

    Collect::writeCommon(comVolt, "RDW VF");
    QString volt = Collect::writeReturn(comVolt, "RDW VF", 1100);

    UART_t voltUart;
    if (!volt.isEmpty() && volt.size() == 14) {
        Sleep(700);
        voltUart.data = volt;
    } else {
        voltUart.data = "";
    }
    voltUart.com = comVolt;
    devInformation << voltUart;//保留判断结果 下同
    //temperature rise      1       yokogawa Gp10
    splash->setLabelTest(QObject::tr("Checking the Yokogawa Paperless Recorder UART...."));
    splash->setProgress(200/10+(qrand()%10-5));

    QString trcr = Collect::writeReturn(comTR_YOKOGAWAGP10, "FData,0,0001,0110", 1100);

    UART_t trcrUart;
    if (!trcr.isEmpty() && trcr.size() == 723) {
        Sleep(700);
        trcrUart.data = trcr;
    } else {
        trcrUart.data = "";
    }
    trcrUart.com = comTR_YOKOGAWAGP10;
    devInformation << trcrUart;
    //loadA                 2
    splash->setLabelTest(QObject::tr("Checking the Load(ServoA) UART..."));
    splash->setProgress(300/10+(qrand()%10-5));

    QString loadA = Collect::writeReturn(comLoadA, "RDM%101%102%\r", 1100);

    UART_t loadAUart;
    if (!loadA.isEmpty() && loadA.size() > 15) {//load.size() == [16, 19]
        Sleep(700);
        loadAUart.data = loadA;
    } else {
        loadAUart.data = "";
    }
    loadAUart.com = comLoadA;
    devInformation << loadAUart;
    //loadB                 3
    splash->setLabelTest(QObject::tr("Checking the Load(ServoB) UART...."));
    splash->setProgress(400/10+(qrand()%10-5));

    QString loadB = Collect::writeReturn(comLoadB, "RDM%101%102%\r", 1100);

    UART_t loadBUart;
    if (!loadB.isEmpty() && loadB.size() > 15) {//load.size() == [16, 19]
        Sleep(700);
        loadBUart.data = loadB;
    } else {
        loadBUart.data = "";
    }
    loadBUart.com = comLoadB;
    devInformation << loadBUart;
    //loadC                 4
    splash->setLabelTest(QObject::tr("Checking the Load(ServoC) UART..."));
    splash->setProgress(500/10+(qrand()%10-5));

    QString loadC = Collect::writeReturn(comLoadC, "RDM%101%102%\r", 1100);

    UART_t loadCUart;
    if (!loadC.isEmpty() && loadC.size() > 15) {//load.size() == [16, 19]
        Sleep(700);
        loadCUart.data = loadC;
    } else {
        loadCUart.data = "";
    }
    loadCUart.com = comLoadC;
    devInformation << loadCUart;
    //test servoA           5
    splash->setLabelTest(QObject::tr("Checking the Life Tester ServoA UART...."));
    splash->setProgress(600/10+(qrand()%10-5));

    QString testA = Collect::writeReturn(comTest, "RDWD020603", 1100);

    UART_t testAUart;
    if (!testA.isEmpty() && testA.size() == 13) {
        Sleep(700);
        testAUart.data = testA;
    } else {
        testAUart.data = "";
    }
    testAUart.com = comTest;
    devInformation << testAUart;
    //test servoB           6
    splash->setLabelTest(QObject::tr("Checking the Life Tester ServoB UART..."));
    splash->setProgress(700/10+(qrand()%10-5));

    QString testB = Collect::writeReturn(comTest, "RDWD030603", 1100);

    UART_t testBUart;
    if (!testB.isEmpty() && testB.size() == 13) {
        Sleep(700);
        testBUart.data = testB;
    } else {
        testBUart.data = "";
    }
    testBUart.com = comTest;
    devInformation << testBUart;
    //test servoC           7
    splash->setLabelTest(QObject::tr("Checking the Life Tester ServoC UART...."));
    splash->setProgress(800/10+(qrand()%10-5));

    QString testC = Collect::writeReturn(comTest, "RDWD040603", 1100);

    UART_t testCUart;
    if (!testC.isEmpty() && testC.size() == 13) {
        Sleep(700);
        testCUart.data = testC;
    } else {
        testCUart.data = "";
    }
    testCUart.com = comTest;
    devInformation << testCUart;

    //temperature rise      8       //agilent 34970a  !!!!!待测试 命令 返回数据大小 判断正常条件
    splash->setLabelTest(QObject::tr("Checking the Agilent Paperless Recorder UART...."));
    splash->setProgress(900/10+(qrand()%10-5));

    QString trcr2 = Collect::writeReturn(comTR_AGILENT34970, "FData,0,0001,0110",1100);

    UART_t trcrUart2;
    if (!trcr2.isEmpty() && trcr2.size() == 723) {
        Sleep(700);
        trcrUart2.data = trcr2;
    } else {
        trcrUart2.data = "";
    }
    trcrUart2.com = comTR_AGILENT34970;
    devInformation << trcrUart2;

    if (!connectSTAS) {//对STAS连接函数的返回结果进行判断
        splash->setLabelTest(QObject::tr("Connect STAS Sucess! Entering the login screen..."));
        splash->setProgress(100);
    } else {

        splash->setLabelTest(QObject::tr("Connect STAS failed! Entering the login screen...."));
        splash->setProgress(100);
    }
    Sleep(1600);

    w.show();//显示主界面
    w.move((QApplication::desktop()->width()-w.width())/2,
           (QApplication::desktop()->height()-w.height())/2);

    splash->finish(&w); //释放加载界面
    delete splash;

    return a.exec();
}
