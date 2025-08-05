#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QPoint>
#include <QPixmap>
#include <QString>
#include "QMenu"
#include "QAction"
#include "ui_titlebar.h"
namespace Ui {
class TitleBar;
}

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

    void setWindowTitle(const QString& title);

    void SetTitleBarIcon(const QString& iconPath);

    Ui::TitleBar *ui;

public slots:
    void receiver(int width);

signals:
    void send();

    void filesend();
   void settingsend();
   void helpsend();
   void file_import_send();
   void file_export_send();
   void ParamSetsend();
   void  IPSetsend();

   void chinesesend();
   void englishsend();

protected:


    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    /**
     * @brief 鼠标按下事件处理函数
     * @param event，参数，事件
     * @note 按下鼠标左键
     */
    virtual void mousePressEvent(QMouseEvent *event);

    /**
     * @brief 鼠标移动事件处理函数
     * @param event，参数，事件
     * @note 移动鼠标
     */
    virtual void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief 鼠标释放事件处理函数
     * @param event，参数，事件
     * @note 释放鼠标
     */
    virtual void mouseReleaseEvent(QMouseEvent *event);

    /**
     * @brief 事件过滤处理器
     * @param obj，参数
     * @param event，参数，事件
     * @return 成功返回true，失败返回false
     * @note 设置标题、图标
     */
    virtual bool eventFilter(QObject *obj, QEvent *event);

    /**
     * @brief 最大化/还原
     */
    void updateMaximize();

public slots:
    /**
     * @brief 最小化、最大化/还原、关闭按钮点击时响应的槽函数
     */
    void onClicked();

     void actionfileOneSlot();
     void actionsettingOneSlot();
     void actionhelpOneSlot();
     void actionfile_import_Slot();
     void actionfile_export_Slot();
     void actionchineseOneSlot();
     void actionenglishOneSlot();
     void actionParamSetSlot();
     void actionIPSetSlot();

//    void actionTwoSlot();
//    void actionThreeSlot();
    void fileshowMenu(const QPoint &point);
    void settingshowMenu(const QPoint &point);
    void helpshowMenu(const QPoint &point);
    void languageshowMenu(const QPoint &point);
//    void onButtonHover(QEvent *event);
//    void onButtonLeave(QEvent *event);
    void updateStyles();
private slots:

    void on_setting_clicked();

    void on_file_clicked();

    void on_help_clicked();

    void on_translation_clicked();

    void on_dropdownbox_clicked();

private:
    QLabel* m_iconLabel;
    QLabel* m_titleLabel;
    QPushButton* m_minimizeButton;
    QPushButton* m_maximizeButton;
    QPushButton* m_closeButton;
    QPoint m_start;//起始点
    QPoint m_end;//结束点


    bool m_leftButtonPressed;//鼠标左键按下标记

    QMenu *file_menu;
    QAction *file_actionOne;
    QAction *file_action_import;
    QAction *file_action_export;

    QMenu *setting_menu;
    QAction *setting_actionOne;
    QAction *action_Param_Set;
    QAction *action_IP_Set;

    QMenu *help_menu;
    QAction *help_actionOne;

    QMenu *language_menu;
    QAction *chinese_actionOne;
    QAction *english_actionOne;
 QPoint m_windowStart;


};



#endif // TITLEBAR_H
