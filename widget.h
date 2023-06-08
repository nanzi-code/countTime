#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QPoint>
#include <QFont>
#include <QPalette>
#include <QImage>
#include <QBrush>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QSettings>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QFile>

#include "set.h"

class Widget : public QWidget
{
    Q_OBJECT

private:
    QTimer *setDayTime;
    QLabel *begin;
    QLabel *time;
    QLabel *day;

    QPushButton *menuBtn;

    QWidget *innerWidget;
    QPoint m_poin;

    QAction *setting;
    QAction *setImg;
    QAction *setDefaultImg;
    QAction *setClose;

    set *setWindow;

    QSystemTrayIcon *sysTray;

    void initUI();
    void initSlots();
    void initSysTray();

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void closeEvent(QCloseEvent *event);

protected:

private slots:
    void on_settingClick();
    void on_setImgClick();
    void on_setDefaultImg();
    void on_setCloseClick();

    void onTimerAction();


public:
    void settimeString(QDate timeString);
    void setbeginString(QString beginString);
    void setTranValue(int value);
    void setbackgroundImg();

    void setAutoRun(const QString &appPath, bool flag);
    bool isAutoRun(const QString &appPath);

    static void onCallBack(int tranValue, bool powerOn, QDate timeString, QString beginString, void *context);

    Widget(QWidget *parent = 0);
    ~Widget();
};

#endif // WIDGET_H
