#ifndef MSPLASHSCREEN_H
#define MSPLASHSCREEN_H

#include <QSplashScreen>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QLabel;
class QProgressBar;

/*!
 * \brief The splash screen class
 */
class mSplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit mSplashScreen(const QPixmap& pixmap);/*!* \brief 界面显示pixmap图片 */
    ~mSplashScreen();

    void setProgress(int value);        /*!* \brief 设置进度条属性 */
    void setLabelTest(QString str);     /*!* \brief 设置便签文本 */

private:
    QProgressBar *mProgressBar;         /*!* \brief 进度条对象 */
    QLabel *mLabel;                     /*!* \brief 便签对象 */
};

#endif // MSPLASHSCREEN_H
