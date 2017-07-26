#include "pathdialog.h"
#include "formdisplay.h"
#include "Collect.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

pathDialog::pathDialog(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(Qt::WindowCloseButtonHint);

    this->setWindowTitle(tr(" Electrical Accessories Test Automation Program"));
    this->setWindowIcon(QIcon(WINDOW_ICON));
    this->setStyleSheet("#titleLabel {color: rgba(212,212,212, 80%);}");

    pathLabel = new QLabel(tr("Default Path:"), this);
    pathLabel->setGeometry(10, 10, 81, 22);

    defaultPathLineEdit = new QLineEdit(this);
    defaultPathLineEdit->setGeometry(10, 30, 381, 21);

    pathPushButton = new QPushButton(tr("Browse..."),this);
    pathPushButton->setGeometry(400, 29, 75, 23);
//    pathPushButton->setEnabled(false);

    pathLabel_2 = new QLabel(tr("Current Path:"),this);
    pathLabel_2->setGeometry(10, 60, 81, 22);

    currentPathLineEdit = new QLineEdit(this);
    currentPathLineEdit->setGeometry(10, 80, 381, 21);

    pathPushButton_2 = new QPushButton(tr("Browse..."),this);
    pathPushButton_2->setGeometry(400, 79, 75, 23);

    savePushButton = new QPushButton(tr("save"),this);
    savePushButton->setGeometry(295, 120, 75, 23);

    closePushButton = new QPushButton(tr("close"),this);
    closePushButton->setGeometry(380, 120, 75, 23);

    connect(pathPushButton, SIGNAL(clicked(bool)), this, SLOT(defaultClicked()));
    connect(pathPushButton_2, SIGNAL(clicked(bool)), this, SLOT(currentClicked()));
    connect(savePushButton, SIGNAL(clicked(bool)), this, SLOT(saveClicked()));
    connect(closePushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}

pathDialog::~pathDialog()
{
}

void pathDialog::defaultClicked()
{
    QString path = QFileDialog::getExistingDirectory(NULL, tr("Path"), defaultPathLineEdit->text());
    if (!path.isEmpty()) {
        defaultPathLineEdit->setText(path);
        currentPathLineEdit->setText(path);
    }
}

void pathDialog::currentClicked()
{
    QString path = QFileDialog::getExistingDirectory(NULL, tr("Path"), currentPathLineEdit->text());
    if (!path.isEmpty())
        currentPathLineEdit->setText(path);
}

void pathDialog::saveClicked()
{
    if (defaultPathLineEdit->text().isEmpty()) {
        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),tr("You didn't select any directories to Default Path."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

        msgBox.exec();
        return;
    } else if (currentPathLineEdit->text().isEmpty()) {
        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),tr("You didn't select any directories to Current Path."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

        msgBox.exec();
        return;
    } else {
        FormDisplay::dirName = currentPathLineEdit->text();
        QSettings *configIniWrite = new QSettings("EATconfig.ini", QSettings::IniFormat);
        configIniWrite->setValue("Default Path", defaultPathLineEdit->text());
        delete configIniWrite;
        this->close();
    }
}
