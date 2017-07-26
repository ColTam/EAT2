#include "registrationcode.h"
#include "Collect.h"

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>

RegistrationCode::RegistrationCode(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(Qt::WindowCloseButtonHint);

    this->setWindowTitle(tr(" Electrical Accessories Test Automation Program"));
    this->setWindowIcon(QIcon(WINDOW_ICON));
    this->setStyleSheet("#titleLabel {color: rgba(212,212,212, 80%);}");

    m_computerNumberLabel = new QLabel(this);
    m_computerNumberLabel->setText(tr("Cpu And Disk Number:"));
    m_computerNumberLabel->setGeometry(10, 10, 131, 22);

    m_label = new QLabel(this);
    m_label->setText(tr("Registration Code:"));
    m_label->setGeometry(10, 37, 131, 22);

    m_computerNumberLineEdit = new QLineEdit(this);
    m_computerNumberLineEdit->setGeometry(140, 10, 131, 21);
    m_computerNumberLineEdit->setText(CollectControl::GetComputerCpuAndDiskNum());
    m_computerNumberLineEdit->setReadOnly(true);

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setGeometry(140, 37, 131, 21);
    m_lineEdit->setMaxLength(20);

    QSettings *configIniRead = new QSettings("EATconfig.ini", QSettings::IniFormat);
    QString registrN = configIniRead->value("Registration Code").toString();
    delete configIniRead;
    m_lineEdit->setText(registrN);

    m_pushButton1 = new QPushButton(this);
    m_pushButton1->setText(tr("save"));
    m_pushButton1->setGeometry(110, 70, 75, 23);

    m_pushButton2 = new QPushButton(this);
    m_pushButton2->setText(tr("close"));
    m_pushButton2->setGeometry(190, 70, 75, 23);

    connect(m_pushButton1, SIGNAL(clicked(bool)), this, SLOT(saveClicked()));
    connect(m_pushButton2, SIGNAL(clicked(bool)), this, SLOT(close()));
}

RegistrationCode::~RegistrationCode()
{
}

void RegistrationCode::saveClicked()
{
    if (m_lineEdit->text().size() < 20) {
        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),tr("Please enter the correct registration code."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

        msgBox.exec();
        return;
    }

    QSettings *configIniWrite = new QSettings("EATconfig.ini", QSettings::IniFormat);
    configIniWrite->setValue("Registration Code", m_lineEdit->text());
    delete configIniWrite;
    this->close();
}
