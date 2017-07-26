#ifndef MHELPER_H
#define MHELPER_H

#define CHILDQSS    "QStackedWidget {background: transparent; border-image:none;}" \
                    "QPushButton {min-height: 26px; min-width: 26px; font-weight: bold; border: none; background: transparent; outline: none;}"

#include <QFrame>
#include <QWidget>

class QPushButton;
class QStackedWidget;
class mHelperWidget1;
class mHelperWidget2;
class mHelperWidget3;
class mHelperWidget4;

class mHelper : public QFrame
{
    Q_OBJECT
public:
    explicit mHelper(QWidget *parent = 0);
    ~mHelper();

signals:
    void changeStackedWidget(int);

public slots:

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void initForm();
    void initWidget();
    void initConnect();
    void updateBackground();

private:
    QPushButton *m_prevBtn;
    QPushButton *m_nextBtn;

    QStackedWidget *m_mainWidget;
    mHelperWidget1 *m_stack1;
    mHelperWidget2 *m_stack2;
    mHelperWidget3 *m_stack3;
    mHelperWidget4 *m_stack4;
};

/*!
 * \brief The mHelperWidget1 class
 * \brief mHelper's stackedWidget 1
 */
class mHelperWidget1 : public QWidget
{
    Q_OBJECT
public:
    explicit mHelperWidget1(QWidget *parent = 0);
    ~mHelperWidget1(){}

Q_SIGNALS:
    void buttonClicked(int);

private:
    void initConnect();

private:
    QPushButton *m_helpBtn;
    QPushButton *m_helpBtn1;
    QPushButton *m_helpBtn2;
    QPushButton *m_helpBtn3;
    QPushButton *m_helpBtn4;
    QPushButton *m_helpBtn5;
    QPushButton *m_helpBtn6;
    QPushButton *m_helpBtn7;
};

/*!
 * \brief The mHelperWidget2 class
 * \brief mHelper's stackedWidget 2
 */
class mHelperWidget2 : public QWidget
{
    Q_OBJECT
public:
    explicit mHelperWidget2(QWidget *parent = 0);
    ~mHelperWidget2(){}

Q_SIGNALS:
    void buttonClicked(int);

private:
    void initConnect();

private:
    QPushButton *m_helpBtn;
    QPushButton *m_helpBtn1;
    QPushButton *m_helpBtn2;
    QPushButton *m_helpBtn3;
    QPushButton *m_helpBtn4;
    QPushButton *m_helpBtn5;
    QPushButton *m_helpBtn6;
    QPushButton *m_helpBtn7;
    QPushButton *m_helpBtn8;
};

/*!
 * \brief The mHelperWidget3 class
 * \brief mHelper's stackedWidget 3
 */
class mHelperWidget3 : public QWidget
{
    Q_OBJECT
public:
    explicit mHelperWidget3(QWidget *parent = 0);
    ~mHelperWidget3() {}

Q_SIGNALS:
    void buttonClicked(int);

private:
    void initConnect();

private:
    QPushButton *m_helpBtn;
    QPushButton *m_helpBtn1;
    QPushButton *m_helpBtn2;
    QPushButton *m_helpBtn3;
    QPushButton *m_helpBtn4;
    QPushButton *m_helpBtn5;
    QPushButton *m_helpBtn6;
    QPushButton *m_helpBtn7;
    QPushButton *m_helpBtn8;
    QPushButton *m_helpBtn9;
    QPushButton *m_helpBtn10;
};

/*!
 * \brief The mHelperWidget4 class
 * \brief mHelper's stackedWidget 4
 */
class mHelperWidget4 : public QWidget
{
    Q_OBJECT
public:
    explicit mHelperWidget4(QWidget *parent = 0);
    ~mHelperWidget4(){}

Q_SIGNALS:
    void buttonClicked(int);

private:
    void initConnect();

private:
};

#endif // MHELPER_H
