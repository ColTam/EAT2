#include "temperaturerisedialog.h"
#include "mLoadThread.h"
#include "Collect.h"
#include "titlebar.h"
#include "ui_temperaturerisedialog.h"
#include "countdown.h"

#include <QAxObject>
#include <QDateTime>
#include <Windows.h>

TemperatureRiseDialog::TemperatureRiseDialog(time_t hour, int num, QString Im, QWidget *parent) :
    QDialog(parent),duration(hour),flag(num),widgetCount(--num),_Im(Im),
    ui(new Ui::TemperatureRiseDialog),
    loadCount(0),
    failNumber(0),
    timeCount(0),
    m_currentIn(""),
    _worksheet(nullptr),
    savePngSecess(0)
{
    ui->setupUi(this);

    _miniDateWidget = new MiniDateWidget(this);
    _miniDateWidget->move(0, (this->height() - _miniDateWidget->height()) *5/8);
    _miniDateWidget->show();

    oldTemperatureRise_list.clear();
    setTargetTime();

    initDialogTitle();

    _mMessage = new QMessageWidget(this);
    _mMessage->setMinimumSize(718,26);
    _mMessage->setGeometry(1,510,718,26);
    _mMessage->show();

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentIndex(TRISE_STACKWIDGET_TC);
    on_toolButton_tc_clicked();
}

TemperatureRiseDialog::~TemperatureRiseDialog()
{
    delete _loadController;
    delete ui;
}

void TemperatureRiseDialog::initDialogTitle()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    _titleBar = new TitleBar(this, 1);
    this->installEventFilter(_titleBar);

    this->resize(720, 540);
    this->setWindowIcon(QIcon(WINDOW_ICON));

    QPalette mPalette(palette());
    mPalette.setColor(QPalette::Background, QColor(64,66,68));
    this->setAutoFillBackground(true);
    this->setPalette(mPalette);

    QVBoxLayout *titleVBoxLayout = new QVBoxLayout();
    titleVBoxLayout->addWidget(_titleBar);
    titleVBoxLayout->addStretch(0);
    titleVBoxLayout->setSpacing(0);
    titleVBoxLayout->setContentsMargins(0, 0, 0, 0);

    connect(_titleBar, SIGNAL(maximizeClicked()), this, SLOT(maximizedWidget()));
    connect(_titleBar, SIGNAL(restoreClicked()), this, SLOT(restoreWidget()));
    connect(_titleBar, SIGNAL(closeClicked()), this, SLOT(closeWidget()));

    ui->gridLayout_4->addLayout(titleVBoxLayout, 0, 0, Qt::AlignTop);
    ui->gridLayout_3->setGeometry(QRect(0, 26, 720, 514));

    this->setLayout(ui->gridLayout_4);

}

void TemperatureRiseDialog::setTargetTime()
{
    _currentTime = QDateTime::currentDateTime();//第一次记录时的时间，用于打印excel表格中的时间

#if 1
    int secs = duration * 60 * 60 + 3;
#else
    int secs = duration * 80 + 3; //测试用时间
#endif
    _objectTime = _currentTime.addSecs(secs);
    _miniDateWidget->overTimeLineEdit->setText(_objectTime.time().toString());
}

void TemperatureRiseDialog::initCustomPlot()
{
    ui->tableWidget->clear();
    this->loadCount = 0;

    ui->customplot->clearGraphs();

    ui->customplot->xAxis->setLabelColor(QColor("#F0F0F0"));
    ui->customplot->xAxis->setTickLabelColor(QColor("#B8B8B8"));
    ui->customplot->xAxis->setBasePen(QPen(QColor("#F0F0F0")));
    ui->customplot->xAxis->setTickPen(QPen(QColor("#B8B8B8")));
    ui->customplot->xAxis->setSubTickPen(QPen(QColor("#B8B8B8")));
    ui->customplot->xAxis->setSelectedBasePen(QPen(QColor("#F46320")));
    ui->customplot->xAxis->setSelectedTickPen(QPen(QColor("#F46320")));
    ui->customplot->xAxis->setSelectedTickLabelColor(QColor("#F46320"));
    ui->customplot->xAxis->setSelectedSubTickPen(QPen(QColor("#F46320")));

    ui->customplot->yAxis->setLabelColor(QColor("#F0F0F0"));
    ui->customplot->yAxis->setTickLabelColor(QColor("#B8B8B8"));
    ui->customplot->yAxis->setBasePen(QPen(QColor("#F0F0F0")));
    ui->customplot->yAxis->setTickPen(QPen(QColor("#B8B8B8")));
    ui->customplot->yAxis->setSubTickPen(QPen(QColor("#B8B8B8")));
    ui->customplot->yAxis->setSelectedBasePen(QPen(QColor("#F46320")));
    ui->customplot->yAxis->setSelectedTickPen(QPen(QColor("#F46320")));
    ui->customplot->yAxis->setSelectedTickLabelColor(QColor("#F46320"));
    ui->customplot->yAxis->setSelectedSubTickPen(QPen(QColor("#F46320")));

    ui->customplot->xAxis->setLabel(tr("Time ( 5s )"));
    ui->customplot->yAxis->setLabel(tr("Temperatur Rise ( °C )"));

    ui->customplot->legend->setVisible(true);
    ui->customplot->legend->setFont(QFont("Helvetica", 9));
    ui->customplot->legend->setRowSpacing(-3);
    ui->customplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

    for (int i = 0; i < temperatureRiseCount_list.count(); i++)
    {
        ui->customplot->addGraph();
        QString ch = "CH" + QString::number(temperatureRiseCount_list.at(i));
        ui->customplot->graph(i)->setName(ch);
        ui->customplot->graph(i)->setPen(QPen(QColor(qSin(i*1+1.2)*80+80,qSin(i*0.3+0)*80+80,qSin(i*0.3+1.5)*80+80)));
        ui->customplot->graph(i)->setLineStyle((QCPGraph::lsLine));
        ui->customplot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 3));
    }

    QBrush backRole;
    backRole.setColor(QColor(40,40,40));
    backRole.setStyle(Qt::SolidPattern);
    ui->customplot->setBackground(backRole);

    ui->customplot->xAxis->setRange(-2.5,79.5);
    ui->customplot->yAxis->setRange(-2.5,100.25);

    connect(ui->customplot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
    connect(ui->customplot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    connect(ui->customplot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
}

void TemperatureRiseDialog::arrivalTime()
{
    QDateTime time = QDateTime::currentDateTime();
    if (time.operator >=(_objectTime)) {
        saveCustomPlotData(time);

        if (!_Im.toInt()){
            if (widgetCount--) {
                {
                    CountDown countDown(_miniDateWidget->groupbox->title());
                    countDown.exec();
                }
                setTargetTime();
                initCustomPlot();
                Collect::CutToLE(servo);
                Collect::PowerStart();
                Collect::LoadStart(servo);
                emit loadStart(COM_TRDELAY);
            } else {
                Collect::PowerStart();
                {
                    CountDown countDown(_miniDateWidget->groupbox->title());
                    countDown.exec();
                }
                Collect::CutToLN(servo);
                closeEffect();
            }
        } else {
            if (flag == 4) {
                if (widgetCount--) {
                    {
                        CountDown countDown(_miniDateWidget->groupbox->title());
                        countDown.exec();
                    }
                    setTargetTime();
                    initCustomPlot();
                    if (!(widgetCount%2)) {
                        Collect::CutToLE(servo);
                        Collect::PowerStart();
                        Collect::LoadStart(servo);
                        emit loadStart(COM_TRDELAY);
                    } else {
                        Collect::ComponentsNoShortCut(servo);
                        Collect::SetLoad(SET_LPF_RESISTIVE_884, _Im, servo);
                        this->m_currentIn = _Im;
                        Collect::PowerStart();
                        Collect::LoadStart(servo);
                        emit loadStart(COM_TRDELAY);
                    }
                } else {
                    Collect::PowerStart();
                    {
                        CountDown countDown(_miniDateWidget->groupbox->title());
                        countDown.exec();
                    }
                    Collect::CutToLN(servo);
                    closeEffect();
                }
            } else if (flag == 2) {
                if (widgetCount--) {
                    {
                        CountDown countDown(_miniDateWidget->groupbox->title());
                        countDown.exec();
                    }
                    setTargetTime();
                    initCustomPlot();
                    Collect::ComponentsNoShortCut(servo);
                    Collect::SetLoad(SET_LPF_RESISTIVE_884, _Im, servo);
                    this->m_currentIn = _Im;
                    Collect::PowerStart();
                    Collect::LoadStart(servo);
                    emit loadStart(COM_TRDELAY);
                } else {
                    Collect::PowerStart();
                    {
                        CountDown countDown(_miniDateWidget->groupbox->title());
                        countDown.exec();
                    }
                    Collect::CutToLN(servo);
                    closeEffect();
                }
            }
        }
    }
}

void TemperatureRiseDialog::closeEffect()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(999);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();

    connect(animation, SIGNAL(finished()), this, SLOT(close()));
}

QString TemperatureRiseDialog::to26AlphabetString(int data)
{
    QChar ch = data + 0x40;//A:0x41
    return QString(ch);
}

void TemperatureRiseDialog::convertToColName(int data, QString &res)
{
    int tempData = data / 26;

    if (1 == tempData && 26 == data){
        res = to26AlphabetString(data);
        return;
    } else if (tempData > 0) {
        int mode = data % 26;
        if (0 == mode) {
            --tempData;
            mode = 26;
        }
        convertToColName(mode,res);
        convertToColName(tempData,res);
    } else {
        res = (to26AlphabetString(data) + res);
    }
}

void TemperatureRiseDialog::rangeToWrite(QString itemName, column_t column, QVariantList variantList, QString head, QColor color, bool judge)
{
    if (itemName.isEmpty() || variantList.isEmpty()) return;

    QList<QList<QVariant>> m_datas;
    QVariantList port;
    port.append(itemName);
    m_datas.append(port);

    for(int i=0;i<variantList.size();i++)
    {
        QList<QVariant> rows;
        if (judge) {
            if (variantList.at(i) > 0) {
                rows.append(QVariant("Fail"));
            } else {
                rows.append(QVariant("Pass"));
            }
        } else {
            rows.append(variantList.at(i));
        }
        m_datas.append(rows);
    }

    int col = column;
    int row = m_datas.size();
    QString rangStr1;
    QString rangStr2;
    convertToColName(col,rangStr1);
    rangStr1.replace("@", "Z");
    rangStr2 = rangStr1;
    rangStr1 += head + ":" + rangStr1 + QString::number(row+head.toInt()-1);
    QAxObject* range = _worksheet->querySubObject("Range(const QString&)",rangStr1);

    if (NULL == range || range->isNull()) {
        return;
    }

    QVariantList vars;
    for (int i=0;i<row;i++)
    {
        vars.append(QVariant(m_datas[i]));
    }

    range->setProperty("Value", vars);
    range->setProperty("HorizontalAlignment", -4108);

    QAxObject *Borders = range->querySubObject("Borders");//Interior background; Borders border; Font font;
    Borders->setProperty("Color", color);

    if (itemName == "TempR") {
        QString rangStr = "=MAX(" + rangStr2 + "2:" + rangStr2 + QString::number(row) + ")";
        QAxObject *cells = _worksheet->querySubObject("Cells(int, int)", row+1, col);
        cells->setProperty("Value", QVariant(rangStr));
        cells->setProperty("HorizontalAlignment", -4108);
    }
}

void TemperatureRiseDialog::writeExcel(const QString fileName)
{//通过 QAxObject + Excel VBA来实现操作excal
    QString name = fileName+".xlsx";
//    QDir::toNativeSeparators(name);
    name.replace("/", "\\");

    QAxObject excel("Excel.Application");
    if (excel.isNull()) excel.setControl("ET.Application");
    excel.setProperty("Visible", false);
    excel.setProperty("DisplayAlerts", false);

    QAxObject *workbooks = excel.querySubObject("WorkBooks");
    workbooks->dynamicCall("Add");
    QAxObject *workbook = excel.querySubObject("ActiveWorkBook");
    _worksheet = workbook->querySubObject("WorkSheets(int)", 1);

    this->temperatureRiseCOne_list.clear();
    QVariantList var;
    for (int graphCount = 0; graphCount < temperatureRiseCount_list.count(); graphCount++)
    {
        if (graphCount == 0) {
            QTime time = QTime(_currentTime.time().hour(), _currentTime.time().minute(), _currentTime.time().second());
            for (int i = 0; i <= this->loadCount; i++)
            {
                if (time.second() + COM_TRDELAYS > 59) {
                    if (time.minute() + 1 == 60) {
                        time.setHMS(time.hour()+1,time.minute()+1-60,time.second()+COM_TRDELAYS-60);
                    } else {
                        time.setHMS(time.hour(),time.minute()+1,time.second()+COM_TRDELAYS-60);
                    }
                } else {
                    time.setHMS(time.hour(),time.minute(),time.second()+COM_TRDELAYS);
                }
//                time.addSecs(5 * (i+1));
                var.append(QVariant(time.toString()));
            }
            rangeToWrite("Time", 1, var, "1");
            var.clear();

            for (int i = 0; i <= this->loadCount; i++)
            {
                var.append(ui->customplot->graph(graphCount)->dataMainValue(i));
            }
            rangeToWrite("CH"+QString::number(temperatureRiseCount_list.at(graphCount)), 2, var, "1", QColor(255,192,0));
            var.clear();//Room Temperature;
        } else if (temperatureRiseCount_list.at(graphCount) == roomTemperatureRiseC) {
            for (int i = 0; i <= this->loadCount; i++)
            {
                var.append(ui->customplot->graph(graphCount)->dataMainValue(i));
            }
            rangeToWrite("CH"+QString::number(temperatureRiseCount_list.at(graphCount)), 3, var, "1", QColor(255,192,0));
            var.clear();
        } else if (graphCount < temperatureRiseCount_list.indexOf(roomTemperatureRiseC)){
            for (int i = 0; i <= this->loadCount; i++)
            {
                var.append(ui->customplot->graph(graphCount)->dataMainValue(i));
            }
            rangeToWrite("CH"+QString::number(temperatureRiseCount_list.at(graphCount)), 4*graphCount+0, var, "1", QColor(68,114,196));
            var.clear();

            temperatureRiseCHxxx_list.clear();
            for (int i = 0; i <= this->loadCount; i++)
            {
                double t = ui->customplot->graph(graphCount)->dataMainValue(i) - ui->customplot->graph(temperatureRiseCount_list.indexOf(1))->dataMainValue(i);
                temperatureRiseCHxxx_list << t;
                var.append(t);
            }
            qSort(temperatureRiseCHxxx_list);
            this->temperatureRiseCOne_list << temperatureRiseCHxxx_list.at(temperatureRiseCHxxx_list.size()-1);
            rangeToWrite("TempR", 4*graphCount+1, var, "1", QColor(68,114,196));
            var.clear();

            for (int i = 0; i <= this->loadCount; i++)
            {
                var.append(temperatureRiseUpper_list.at(graphCount));
            }
            rangeToWrite("MaxTR", 4*graphCount+2, var, "1", QColor(68,114,196));
            var.clear();

            QString temperatureRiseUpper = temperatureRiseUpper_list.at(graphCount);
            for (int i = 0; i <= this->loadCount; i++)
            {
                double t = ui->customplot->graph(graphCount)->dataMainValue(i) - ui->customplot->graph(temperatureRiseCount_list.indexOf(1))->dataMainValue(i) - temperatureRiseUpper.toDouble();
                if (t > 0){
                    cellsToWrite(i+2,4*graphCount+3,QColor(174,77,80));
                } else {
                    cellsToWrite(i+2,4*graphCount+3,QColor(112,173,71));
                }
                var.append(t);

            }
            rangeToWrite("P/F", 4*graphCount+3, var, "1", QColor(112,173,71), true);
            var.clear();
        } else if (graphCount > temperatureRiseCount_list.indexOf(roomTemperatureRiseC)){
            for (int i = 0; i <= this->loadCount; i++)
            {
                var.append(ui->customplot->graph(graphCount)->dataMainValue(i));
            }
            if (roomTemperatureRiseC == 1) {
                rangeToWrite("CH"+QString::number(temperatureRiseCount_list.at(graphCount)), 4*(graphCount)+0, var, "1", QColor(68,114,196));
            } else {
                rangeToWrite("CH"+QString::number(temperatureRiseCount_list.at(graphCount)), 4*(graphCount-1)+0, var, "1", QColor(68,114,196));
            }
            var.clear();

            temperatureRiseCHxxx_list.clear();
            for (int i = 0; i <= this->loadCount; i++)
            {
                double t = ui->customplot->graph(graphCount)->dataMainValue(i) - ui->customplot->graph(temperatureRiseCount_list.indexOf(roomTemperatureRiseC))->dataMainValue(i);
                temperatureRiseCHxxx_list << t;
                var.append(t);
            }
            qSort(temperatureRiseCHxxx_list);
            this->temperatureRiseCOne_list << temperatureRiseCHxxx_list.at(temperatureRiseCHxxx_list.size()-1);
            if (roomTemperatureRiseC == 1) {
                rangeToWrite("TempR", 4*(graphCount)+1, var, "1", QColor(68,114,196));
            } else {
                rangeToWrite("TempR", 4*(graphCount-1)+1, var, "1", QColor(68,114,196));
            }
            var.clear();

            for (int i = 0; i <= this->loadCount; i++)
            {
                var.append(temperatureRiseUpper_list.at(graphCount));
            }
            if (roomTemperatureRiseC == 1) {
                rangeToWrite("MaxTR", 4*(graphCount)+2, var, "1", QColor(68,114,196));
            } else {
                rangeToWrite("MaxTR", 4*(graphCount-1)+2, var, "1", QColor(68,114,196));
            }
            var.clear();

            QString temperatureRiseUpper = temperatureRiseUpper_list.at(graphCount);
            for (int i = 0; i <= this->loadCount; i++)
            {
                double t = ui->customplot->graph(graphCount)->dataMainValue(i) - ui->customplot->graph(temperatureRiseCount_list.indexOf(roomTemperatureRiseC))->dataMainValue(i) - temperatureRiseUpper.toDouble();
                if (t > 0){
                    if (roomTemperatureRiseC == 1) {
                        cellsToWrite(i+2,4*(graphCount)+3,QColor(174,77,80));
                    } else {
                        cellsToWrite(i+2,4*(graphCount-1)+3,QColor(174,77,80));
                    }
                } else {
                    if (roomTemperatureRiseC == 1) {
                        cellsToWrite(i+2,4*(graphCount)+3,QColor(112,173,71));
                    } else {
                        cellsToWrite(i+2,4*(graphCount-1)+3,QColor(112,173,71));
                    }
                }
                var.append(t);

            }
            if (roomTemperatureRiseC == 1) {
                rangeToWrite("P/F", 4*(graphCount)+3, var, "1", QColor(112,173,71), true);
            } else {
                rangeToWrite("P/F", 4*(graphCount-1)+3, var, "1", QColor(112,173,71), true);
            }
            var.clear();
        }
    }

    for (int i = 0; i <= this->loadCount; i++)
    {
        if (ui->tableWidget->item(i, 0))
            var.append(ui->tableWidget->item(i, 0)->text());
    }
    if (roomTemperatureRiseC == 1)
        rangeToWrite("Voltage", temperatureRiseCount_list.count()*4+1, var, "1", QColor(68,114,196));
    else
        rangeToWrite("Voltage", (temperatureRiseCount_list.count()-1)*4+1, var, "1", QColor(68,114,196));
    var.clear();

    for (int i = 0; i <= this->loadCount; i++)
    {
        if (ui->tableWidget->item(i, 1))
            var.append(ui->tableWidget->item(i, 1)->text());
    }
    if (roomTemperatureRiseC == 1)
        rangeToWrite("Current", temperatureRiseCount_list.count()*4+2, var, "1", QColor(68,114,196));
    else
        rangeToWrite("Current", (temperatureRiseCount_list.count()-1)*4+2, var, "1", QColor(68,114,196));
    var.clear();

    for (int i = 0; i < temperatureRiseCount_list.count(); i++)
    {
        if (temperatureRiseCount_list.at(i) != 1 && temperatureRiseCount_list.at(i) != roomTemperatureRiseC) {
            var.append(ui->customplot->graph(i)->name());
        }
    }
    rangeToWrite("Port", 1, var, QString::number(this->loadCount+5), QColor(220,220,220));
    var.clear();

    for (int i = 0; i < temperatureRiseCount_list.count(); i++)
    {
        if (temperatureRiseCount_list.at(i) != 1 && temperatureRiseCount_list.at(i) != roomTemperatureRiseC) {
            var.append(temperatureRiseRemark_list.at(i));
        }
    }
    rangeToWrite("Remark", 2, var, QString::number(this->loadCount+5), QColor(220,220,220));
    var.clear();

    for (int i = 0; i < temperatureRiseCOne_list.size(); i++)
    {
        var.append(temperatureRiseCOne_list.at(i));
    }
    rangeToWrite("TempR", 3, var, QString::number(this->loadCount+5), QColor(220,220,220));
    var.clear();

    for (int i = 0; i < temperatureRiseCount_list.count(); i++)
    {
        if (temperatureRiseCount_list.at(i) != 1 && temperatureRiseCount_list.at(i) != roomTemperatureRiseC) {
            var.append(temperatureRiseUpper_list.at(i));
        }
    }
    rangeToWrite("MaxTR", 4, var, QString::number(this->loadCount+5), QColor(220,220,220));
    var.clear();

    temperatureRiseUpper_lists.clear();
    temperatureRiseUpper_lists = temperatureRiseUpper_list;
    temperatureRiseUpper_lists.removeAt(temperatureRiseCount_list.indexOf(roomTemperatureRiseC));
    if (roomTemperatureRiseC != 1)
        temperatureRiseUpper_lists.removeAt(temperatureRiseCount_list.indexOf(1));
    for (int i = 0; i < temperatureRiseCOne_list.size(); i++)
    {
        QString tc = temperatureRiseUpper_lists.at(i);
        double t = temperatureRiseCOne_list.at(i) - tc.toDouble();
        var.append(t);
        if (t > 0) {
            cellsToWrite(i+this->loadCount+6,6,QColor(174,77,80));
            this->failNumber++;
        }
    }
    rangeToWrite("Bad", 5, var, QString::number(this->loadCount+5), QColor(237,125,49));
    rangeToWrite("P/F", 6, var, QString::number(this->loadCount+5), QColor(112,173,71), true);
    var.clear();

    QAxObject *worksheets = workbook->querySubObject("Sheets");
    int sheetCount = worksheets->property("Count").toInt();    // GET WORKSHEETS COUNT
    QAxObject *lastSheet = worksheets->querySubObject("Item(int)", sheetCount);
    QAxObject *workSheet = worksheets->querySubObject("Add(QVariant)", lastSheet->asVariant());
    lastSheet->dynamicCall("Move(QVariant)", workSheet->asVariant());

    _worksheet = workbook->querySubObject("WorkSheets(int)", 2);
    QAxObject *cell = _worksheet->querySubObject("Shapes");

    if (savePngSecess){
        QString p = fileName+".png";
        p.replace("/", "\\");
        cell->dynamicCall("AddPicture(QString&, bool, bool, double, double, double, double",p,true,true,0,0,718,440);
    } else {
        QAxObject *range = workSheet->querySubObject("Cells(int, int)", 2, 2);
        range->dynamicCall("SetValue(const QVariant&)", QVariant(tr("The picture save failed.")));
    }

    workbook->dynamicCall("SaveAs(const QString&)", name);
    workbook->dynamicCall("Close(Boolean)", false);
    excel.dynamicCall("Quit(void)");
}

void TemperatureRiseDialog::cellsToWrite(row_t row, column_t column, QColor color)
{
    QAxObject *cell = _worksheet->querySubObject("Cells(int, int)", row, column);
    QAxObject *font = cell->querySubObject("Font");
    font->setProperty("Color", color);

    delete font;
    delete cell;
}

void TemperatureRiseDialog::saveCustomPlotData(QDateTime time)
{
    QString name = this->windowTitle().mid(this->windowTitle().indexOf("-")+1);
    name.remove(name.indexOf("cfg"), 15);
    switch (this->windowTitle().right(5).mid(0,2).toInt()) {
    case 19:
    {
        if (servo == "A") {
            name += "data/beforeTest/servoA/" + time.toString("MM_dd_hh_mm");
        } else if (servo == "B") {
            name += "data/beforeTest/servoB/" + time.toString("MM_dd_hh_mm");
        } else if (servo == "C") {
            name += "data/beforeTest/servoC/" + time.toString("MM_dd_hh_mm");
        }
        break;
    }
    case 22:
    {
        if (servo == "A") {
            name += "data/afterTest/servoA/" + time.toString("MM_dd_hh_mm");
        } else if (servo == "B") {
            name += "data/afterTest/servoB/" + time.toString("MM_dd_hh_mm");
        } else if (servo == "C") {
            name += "data/afterTest/servoC/" + time.toString("MM_dd_hh_mm");
        }
        break;
    }
    }
//        if (ui->customplot->saveJpg(name+".jpg"))
//            qDebug() << "save sucess";
//        if (ui->customplot->saveBmp(name+".bmp"))
//            qDebug() << "save sucess";
//        if (ui->customplot->savePdf(name+".pdf"))
//            qDebug() << "save sucess";
    ui->customplot->rescaleAxes(true);
    ui->customplot->xAxis->setRange(0,this->loadCount);
    ui->customplot->replot();

    savePngSecess = ui->customplot->savePng(name+".png");

    --this->loadCount;
    emit loadStop();
    Collect::PowerStop();
    Collect::LoadStop(servo);
    Collect::CutToLN(servo);

    writeExcel(name);
//        qDebug() << QDateTime::currentDateTime();
}

void TemperatureRiseDialog::maximizedWidget()
{
    _mMessage->setGeometry(1, 101, QApplication::desktop()->width()-2, 26);
}

void TemperatureRiseDialog::restoreWidget()
{
    if (_miniDateWidget->x() > 720) {
        _miniDateWidget->move(524, _miniDateWidget->y());
    }
    if (_miniDateWidget->y() > 540) {
        _miniDateWidget->move(_miniDateWidget->x(), 419);
    }
    _mMessage->setGeometry(1,510,718,26);
}

void TemperatureRiseDialog::closeWidget()
{
    QMessageBox msgBox(QMessageBox::Warning,tr("close"),tr("Is about to quit the test!\nWhether to save the data ?"));
    msgBox.setStandardButtons(QMessageBox::Save|QMessageBox::Ignore|QMessageBox::Close|QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Save,tr("Save"));
    msgBox.setButtonText(QMessageBox::Ignore,tr("Ignore"));
    msgBox.setButtonText(QMessageBox::Close,tr("Close All"));
    msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
    msgBox.setWindowIcon(QIcon(WINDOW_ICON));

    int ret = msgBox.exec();

    if (ret == QMessageBox::Save) {
        saveCustomPlotData(QDateTime::currentDateTime());
    } else if (ret == QMessageBox::Close) {
        failNumber = -1;
    } else if (ret == QMessageBox::Cancel) {
        return;
    }

    emit loadStop();

//    Collect::PowerStop();
    Collect::LoadStop(servo);
    closeEffect();
}

void TemperatureRiseDialog::updateLoad(QString volt, QString load)
{
    arrivalTime();

    if (!volt.isEmpty()) {
        QString _voltage = QString::number(volt.right(11).left(4).toInt(nullptr, 16)/10) + " V";
        QString _frequency = QString::number(volt.right(6).left(4).toInt(nullptr, 16)/10) + " Hz";

        ui->lineEdit_volt->setText(_voltage);
        ui->lineEdit_frequency->setText(_frequency);
    }

    if (!load.isEmpty()) {
        load.replace("\r\n", '\0');

        QStringList load_list;
        load_list << load.split(",");

        ui->tableWidget->setRowCount(this->loadCount-1);
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());

        QString vol = load_list.at(0);
        vol = QString::number(vol.toDouble());

        QString cur = load_list.at(1);
        cur = QString::number(cur.toDouble());

        QString lpf = load_list.at(2);
        lpf = QString::number(lpf.toDouble()/10,'f',2);

        _miniDateWidget->setText(vol,cur,lpf);

        QTableWidgetItem *volt      = new QTableWidgetItem(vol+" V");
        QTableWidgetItem *current   = new QTableWidgetItem(cur+" A");
        QTableWidgetItem *LPF       = new QTableWidgetItem(lpf+" ");

        volt->setTextAlignment(     Qt::AlignHCenter |Qt::AlignVCenter);
        LPF->setTextAlignment(      Qt::AlignHCenter |Qt::AlignVCenter);
        current->setTextAlignment(  Qt::AlignHCenter |Qt::AlignVCenter);

        ui->tableWidget->setItem(this->loadCount-1,0,volt);
        ui->tableWidget->setItem(this->loadCount-1,1,current);
        ui->tableWidget->setItem(this->loadCount-1,2,LPF);
        ui->tableWidget->scrollToBottom();
#if 1
        if (vol.toDouble() == 0 || cur.toDouble() == 0) {
            if (3599 - QDateTime::currentDateTime().msecsTo(_objectTime)/1000 < 60) {
                return;//一小时秒数 - 当前时间到目标时间的秒数 = 测试了多久......
            }

            emit loadStop();
//            disconnect(_mLoadThreadA, SIGNAL(updateData(QString,QString)), this, SLOT(updateLoad(QString,QString)));
            //            emit temperatureRiseStop();
            QDateTime nowTime = QDateTime::currentDateTime();
            Collect::stateWarning();
            Collect::LoadStop(servo);

            if (QDateTime::currentDateTime().msecsTo(_objectTime)/1000/60 >= 56){
                QMessageBox msgBox(QMessageBox::Warning,tr("Error"),tr("Not detected current!\nPlease check the sample!"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                msgBox.setWindowIcon(QIcon(WINDOW_ICON));

                msgBox.exec();
                //emit temperatureRiseStart();
                _objectTime = _objectTime.addSecs(nowTime.time().secsTo(QTime::currentTime()));

                _miniDateWidget->overTimeLineEdit->setText(_objectTime.time().toString());
                Collect::SetLoad(SET_LPF_RESISTIVE_884, m_currentIn, servo);
                Collect::LoadStart(servo);
                emit loadStart(COM_TRDELAY);
            } else {
                QMessageBox msgBox(QMessageBox::Warning,tr("Error"),tr("Not detected current!\nPlease check the sample!\nContinue test on 'Yes'!"));
                msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
                msgBox.setButtonText(QMessageBox::Yes,tr("Yes"));
                msgBox.setButtonText(QMessageBox::No,tr("No"));
                msgBox.setWindowIcon(QIcon(WINDOW_ICON));

                int ret = msgBox.exec();

                if (ret == QMessageBox::Yes) {
                    //                    emit temperatureRiseStart();
                    _objectTime = _objectTime.addSecs(nowTime.time().secsTo(QTime::currentTime()));

                    _miniDateWidget->overTimeLineEdit->setText(_objectTime.time().toString());
                    Collect::SetLoad(SET_LPF_RESISTIVE_884, m_currentIn, servo);
                    Collect::LoadStart(servo);
                    emit loadStart(COM_TRDELAY);
                } else {
                    Collect::LoadStop(servo);
                    failNumber = -1;
                    closeEffect();
                }
            }
            Collect::stateRunning();
        }
#endif
    }
}

void TemperatureRiseDialog::updateTC(QStringList list)
{
    int sec = COM_TRDELAYS * (++this->timeCount);

    QTime t(0, 0, 0);
    _mMessage->setText(tr("Test Time :") + t.addSecs(sec).toString());
    if (_mMessage->isHidden())
        _mMessage->show();

    if (QDateTime::currentDateTime().operator >=(_objectTime)) {
        oldTemperatureRise_list.clear();
        return;
    }

    if (!list.isEmpty()) {
//        qDebug() << list;
        for (int i = 0; i < temperatureRiseCount_list.count(); i++)
        {
            if (temperatureRiseCount_list.at(i) <= 10) {
                for(int j = 0; j < list.count(); j++)
                {
                    QString tc;
                    if (list.size() < 20 && !oldTemperatureRise_list.isEmpty()) {
                        tc = oldTemperatureRise_list.at(j);
                    } else {
                        tc = list.at(j);
                    }

                    if (tc.mid(3,3).toInt() == temperatureRiseCount_list.at(i)) {
                        double value = tc.mid(19).toDouble();
                        if (value < 0){
                            if (!ui->customplot->graph(i)->dataMainValue(this->loadCount))
                                ui->customplot->graph(i)->addData(this->loadCount+1, 0);
                        } else {
                            if (!ui->customplot->graph(i)->dataMainValue(this->loadCount))
                                ui->customplot->graph(i)->addData(this->loadCount+1, value);
                        }
                    }
                }
            } else {
                for(int j = 0; j < list.count(); j++)
                {
                    QString tc;
                    if (list.size() < 20 && !oldTemperatureRise_list.isEmpty()) {
                        tc = oldTemperatureRise_list.at(j);
                    } else {
                        tc = list.at(j);
                    }

                    if (tc.mid(3,3).toInt() == temperatureRiseCount_list.at(i)+90) {
                        double value = tc.mid(19).toDouble();
                        if (value < 0){
                            if (!ui->customplot->graph(i)->dataMainValue(this->loadCount)) {
                                ui->customplot->graph(i)->addData(this->loadCount+1, 0);
                            }
                        } else {
                            if (!ui->customplot->graph(i)->dataMainValue(this->loadCount)) {
                                ui->customplot->graph(i)->addData(this->loadCount+1, value);
                            }
                        }
                    }
                }
            }
        }
    }

    ui->customplot->rescaleAxes();
    if (loadCount >= 68)
        ui->customplot->xAxis->setRange(this->loadCount-64.5,82,Qt::AlignRight);
    ui->customplot->yAxis->setRange(-2.5,100.25);
    ui->customplot->replot();

    if (list.size() >= 20) {
        qDebug() << "update oldTemperature rise";
        oldTemperatureRise_list = list;
    }

    this->loadCount+=1;
}

void TemperatureRiseDialog::temprtatureRiseUpper(QStringList upper_tc_list, int roomTC)
{
    this->roomTemperatureRiseC = roomTC;
    temperatureRiseCount_list.clear();
    temperatureRiseUpper_list.clear();
    temperatureRiseRemark_list.clear();

    for (int i = 0; i < upper_tc_list.count(); i++)
    {
        QString tc = upper_tc_list.at(i);
        if (tc.left(1).toInt()) {
            temperatureRiseCount_list << i+1;
            temperatureRiseUpper_list << tc.mid(1, tc.indexOf("#")-1);
            temperatureRiseRemark_list << tc.mid(tc.indexOf("#")+1);
        }
    }

    initCustomPlot();

    servo = this->windowTitle().right(2).mid(0,1);
    QString str = this->windowTitle().right(11).left(8) + tr(" Servo-") + servo;
    _miniDateWidget->groupbox->setTitle(str);

    int uart = 0;
    if (servo == "A") {
        uart = 2;
        this->m_currentIn = devInformation.at(uart).value.In;
    } else if (servo == "B") {
        uart = 3;
        this->m_currentIn = devInformation.at(uart).value.In;
    } else if (servo == "C") {
        uart = 4;
        this->m_currentIn = devInformation.at(uart).value.In;
    }
    _loadController = new loadController(uart);

    connect(_loadController, &loadController::threadLoadData, this, &TemperatureRiseDialog::updateLoad);
    connect(this, &TemperatureRiseDialog::loadStart, _loadController, &loadController::threadLoadTimerStart);
    connect(this, &TemperatureRiseDialog::loadStop, _loadController, &loadController::threadLoadTimerStop);

    emit this->loadStart(COM_TRDELAY);
}

void TemperatureRiseDialog::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
    Q_UNUSED(legend)

    if (item) {
        QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
        bool ok;
        QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
        if (ok) {
            plItem->plottable()->setName(newName);
            ui->customplot->replot();
        }
    }
}

void TemperatureRiseDialog::selectionChanged()
{
    if (ui->customplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customplot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            ui->customplot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customplot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        ui->customplot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->customplot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }

    if (ui->customplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customplot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            ui->customplot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customplot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        ui->customplot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->customplot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }

    for (int i=0; i<ui->customplot->graphCount(); ++i)
    {
        QCPGraph *graph = ui->customplot->graph(i);
        QCPPlottableLegendItem *item = ui->customplot->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected()) {
            item->setSelected(true);
            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
    }
}

void TemperatureRiseDialog::mousePress()
{
    if (ui->customplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
        ui->customplot->axisRect()->setRangeDrag(ui->customplot->xAxis->orientation());
    } else if (ui->customplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
        ui->customplot->axisRect()->setRangeDrag(ui->customplot->yAxis->orientation());
    } else {
        ui->customplot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
    }
}

void TemperatureRiseDialog::voltageChange(QString v)
{
    emit temperatureRiseStop();
//    emit loadStop();
    Collect::SetVolt(v);

    Collect::PowerStart();
//    if (servo == "A") {
//        _mLoadThreadA->isFirst = true;
//    } else if (servo == "B") {
//        _mLoadThreadB->isFirst = true;
//    } else if (servo == "C") {
//        _mLoadThreadC->isFirst = true;
//    }
    emit temperatureRiseStart();
    Sleep(1000);
    emit loadStart(COM_TRDELAY);
}

void TemperatureRiseDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F2:
    {
        if (_miniDateWidget->isHidden()) {
            _miniDateWidget->show();
        } else {
            _miniDateWidget->hide();
        }
        break;
    }
    case Qt::Key_1:
    {
        on_toolButton_tc_clicked();
        break;
    }
    case Qt::Key_2:
    {
        on_toolButton_load_clicked();
        break;
    }
    case Qt::Key_3:
    {
        on_toolButton_volt_clicked();
        break;
    }
    default:
        QWidget::keyPressEvent(event);
    }
}

void TemperatureRiseDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    Collect::CutToLN(servo);

    if (servo == "A") {
        emit clauseFinishedA(failNumber);
    } else if (servo == "B") {
        emit clauseFinishedB(failNumber);
    } else if (servo == "C") {
        emit clauseFinishedC(failNumber);
    }

    this->~TemperatureRiseDialog();
}

void TemperatureRiseDialog::on_toolButton_tc_clicked()
{
    if (_miniDateWidget->isHidden())
        _miniDateWidget->show();

    ui->stackedWidget->setCurrentIndex(TRISE_STACKWIDGET_TC);

    ui->toolButton_tc->setStyleSheet(TRISE_BUTTON_RESET);
    ui->toolButton_volt->setStyleSheet(TRISE_BUTTON_INIT);
    ui->toolButton_load->setStyleSheet(TRISE_BUTTON_INIT);
}

void TemperatureRiseDialog::on_toolButton_load_clicked()
{
    if (!_miniDateWidget->isHidden())
        _miniDateWidget->hide();

    ui->stackedWidget->setCurrentIndex(TRISE_STACKWIDGET_LOAD);

    ui->toolButton_load->setStyleSheet(TRISE_BUTTON_RESET);
    ui->toolButton_volt->setStyleSheet(TRISE_BUTTON_INIT);
    ui->toolButton_tc->setStyleSheet(TRISE_BUTTON_INIT);
}

void TemperatureRiseDialog::on_toolButton_volt_clicked()
{
    if (_miniDateWidget->isHidden())
        _miniDateWidget->show();

    ui->stackedWidget->setCurrentIndex(TRISE_STACKWIDGET_VOLT);

    ui->toolButton_volt->setStyleSheet(TRISE_BUTTON_RESET);
    ui->toolButton_load->setStyleSheet(TRISE_BUTTON_INIT);
    ui->toolButton_tc->setStyleSheet(TRISE_BUTTON_INIT);
}
