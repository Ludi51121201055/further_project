#include "mainwindow.h"
#include "ui_mainwindow.h"
#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*-----------日志设计-----------*/
    qDebug("Welcome to enter Debug mode");
    qWarning("Welcome to enter Warning mode");
    qCritical("Welcome to enter Critical mode");
    /*-----------窗口设计-----------*/
    setWindowTitle(tr("Sample Program V2.1"));
    //    Qt::WindowFlags flags = 0;
    //    flags |= Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint;
    //    setWindowFlags(flags);
    /*-----------创建自定义菜单栏和标题栏-----------*/
    InitTitleBar();
    //    /*-----------创建菜单栏-----------*/
    //    BuildMenuBar();
    /*-----------创建状态栏-----------*/
    BuildStatusBar();
    /*-----------创建应用程序小图标-----------*/
    BuildApplicationIcon();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitTitleBar()
{
    setWindowFlags(Qt::FramelessWindowHint);

    // 创建窗口大小调整器
    m_windowResizer = new WindowResizer(this, this);
    m_titleBar = new TitleBar(this);
    QWidget* centralWidget = new QWidget(this);
    ui->menubar->setVisible(false);
    // 创建主布局（垂直布局：标题栏 + 中央区域）
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_titleBar);
    mainLayout->addWidget(ui->centralwidget);
    // 设置主布局到中央窗口
    setCentralWidget(centralWidget);

    connect(m_titleBar->ui->minimum, &QPushButton::clicked, this, &MainWindow::showMinimized);
    connect(m_titleBar->ui->maximum, &QPushButton::clicked, this, &MainWindow::windowShw);
    connect(m_titleBar->ui->close, &QPushButton::clicked, this, &MainWindow::Exitsoftware);
    connect(m_titleBar, SIGNAL(send()), this, SLOT(titleReceive()));
    connect(m_titleBar, SIGNAL(filesend()), this, SLOT(fileReceive()));
    connect(m_titleBar, SIGNAL(file_import_send()), this, SLOT(file_import_receive()));
    connect(m_titleBar, SIGNAL(file_export_send()), this, SLOT(file_export_receive()));
    connect(m_titleBar, SIGNAL(settingsend()), this, SLOT(settingReceive()));
    connect(m_titleBar, SIGNAL(ParamSetsend()), this, SLOT(ParamSetReceive()));
    connect(m_titleBar, SIGNAL(IPSetsend()), this, SLOT(IPSetReceive()));
    connect(m_titleBar, SIGNAL(helpsend()), this, SLOT(helpReceive()));
    connect(m_titleBar, SIGNAL(chinesesend()), this, SLOT(chineseReceive()));
    connect(m_titleBar, SIGNAL(englishsend()), this, SLOT(englishReceive()));
    //窗口长度对齐
    connect(m_titleBar, SIGNAL(send()), this, SLOT(receive()));
    connect(this, SIGNAL(sendWid(int)), m_titleBar, SLOT(receiver(int)));

    transparentSpinBoxStyle = R"(
        QSpinBox, QDoubleSpinBox,QLabel,QPushButton {
            background: transparent;
            color: white;
            border: none;
            padding: 0px;
            selection-background-color: #0081FF;
        }
        QSpinBox:hover, QDoubleSpinBox,QLabel,QPushButton:hover {
            background: transparent;
            color: rgb(0,85,255);
        }
        QSpinBox:pressed, QDoubleSpinBox,QLabel,QPushButton:pressed {
            background: transparent;
            border-style: none;
        }
        QSpinBox::up-button, QDoubleSpinBox,::up-button,
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            width: 0px;
        }
    )";

    // 1. 先设置所有列自适应拉伸
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 2. 设置拉伸因子（权重）：第一列和第四列权重1，其他列权重2（比例1:2:2:1）
    // 注意：setStretch(列索引, 权重)，权重越大，宽度占比越大
    // 1. 所有列先自适应拉伸
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 2. 将第一列和第四列设置为固定宽度（比其他列窄）
    int narrowWidth = 120;   // 窄列宽度（根据需求调整）
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->setColumnWidth(0, narrowWidth);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableWidget->setColumnWidth(3, narrowWidth);

    // 3. 其他列保持Stretch（自动分配剩余宽度）
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    //

    // 创建调试窗口
    debugDockWidget = new QDockWidget("调试输出", this);
    debugDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);

    // 创建文本编辑区域
    debugTextEdit = new QTextEdit(debugDockWidget);
    debugTextEdit->setReadOnly(true);
    debugTextEdit->setMinimumHeight(100);
    debugTextEdit->setLineWrapMode(QTextEdit::NoWrap);   // 禁用自动换行
    debugTextEdit->setFrameShape(QFrame::NoFrame);       // 无边框
    debugDockWidget->setStyleSheet(
        // 标题栏样式
        "QDockWidget::title {"
        "    background-color: rgb(120,120,120);"
        "    color: white;"
        "    font-family: 'Source Han Sans', 'Microsoft YaHei', 'SimHei', monospace;"
        "    font-size: 14px;"
        "    padding: 4px 0px 4px 8px;" /* 上下内边距一致 */
        "}"
        // 关闭按钮样式

        // 关闭和浮动按钮样式
        "QDockWidget::close-button, QDockWidget::float-button {"
        "    background-color: transparent;"
        "    border: none;"
        "    width: 15px;"  /* 明确设置按钮宽度 */
        "    height: 15px;" /* 明确设置按钮高度 */
        "}"
        "QDockWidget::close-button:hover, QDockWidget::float-button:hover {"
        "    background-color:rgb(88,88,88);"
        "}"
        "QDockWidget::close-button:pressed, QDockWidget::float-button:pressed {"
        "    padding: 5px 3px 3px 5px;" /* 点击时轻微偏移增强交互感 */
        "}");
    // 设置样式（深色主题）
    debugTextEdit->setStyleSheet(
        "QTextEdit {"
        "    background-color: #1E1E1E;"
        "    color: white;"
        "    font-family: 'Source Han Sans', 'Microsoft YaHei', 'SimHei', monospace;"
        "    font-size: 14px;"
        "    padding: 8px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar:vertical {"
        "    background: #2D2D30;"
        "    width: 12px;"
        "    margin: 16px 0 16px 0;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #525256;"
        "    min-height: 20px;"
        "    border-radius: 6px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    background: #2D2D30;"
        "    height: 16px;"
        "    subcontrol-position: bottom;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
        "    border: 2px solid #525256;"
        "    width: 3px;"
        "    height: 3px;"
        "    background: #525256;"
        "}");

    // 将文本编辑区域设置为调试窗口的小部件
    debugDockWidget->setWidget(debugTextEdit);

    // 将调试窗口添加到主窗口底部
    addDockWidget(Qt::BottomDockWidgetArea, debugDockWidget);

    // 创建状态栏按钮
    debugWindowButton = new QPushButton(this);
    debugWindowButton->setCheckable(true);
    debugWindowButton->setChecked(true);
    debugWindowButton->setFixedWidth(54);
    debugWindowButton->setFixedHeight(25);
    debugWindowButton->setStyleSheet(
        "QPushButton{background-color:rgb(76, 76, 76);font-family: 'Source Han Sans','Microsoft "
        "YaHei','SimHei';  border:none;font-size: 14px; text-align: center;color: white;icon-size: "
        "16px;   }"

        "QPushButton:hover{background-color:rgb(90, 90, 90);}"
        "QPushButton:pressed{background-color:rgb(60, 60, 60);}");
    // 设置按钮图标（示例：使用Qt内置图标）
    debugWindowButton->setIcon(QIcon(":/images/resources/debug.png"));   // 替换为你的图标路径
    debugWindowButton->setIconSize(QSize(16, 16));   // 设置图标大小（根据需要调整）

    debugDockWidget->hide();

    //        // 连接按钮点击信号到槽函数
    connect(debugWindowButton, &QPushButton::toggled, this, &MainWindow::toggleDebugWindow);

    // 初始日志
    logMessage("调试窗口已初始化");
}

void MainWindow::receive()
{
    isMaximized() ? showNormal() : showMaximized();
}

void MainWindow::windowShw()
{
    isMaximized() ? showNormal() : showMaximized();
}

//自定义标题栏成员函数
void MainWindow::titleReceive()
{
    isMaximized() ? showNormal() : showMaximized();
}

void MainWindow::fileReceive()
{
    QDir directory = QDir::current();
    if (!directory.cd("document"))
    {
        CustomDialog::showMessage(this, tr("文件夹找不到！"),
                                  tr("抱歉，没有找到存放软件操作说明文档的文件夹"),
                                  CustomDialog::Warning, {"关闭"}, 0);

        return;
    }
    directory.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    QFileInfo doc(directory, QStringLiteral("软件操作说明文档.pdf"));
    if (!doc.exists())
    {
        CustomDialog::showMessage(this, tr("文档找不到！"),
                                  tr("抱歉，没有找到相关软件操作说明文档"), CustomDialog::Warning,
                                  {"关闭"}, 0);
        return;
    }
    QUrl pathUrl = QUrl::fromLocalFile(doc.filePath());
    bool isopened = QDesktopServices::openUrl(pathUrl);
    if (!isopened)
    {
        CustomDialog::showMessage(this, tr("文档打不开！"), tr("抱歉，软件操作说明文档打不开！"),
                                  CustomDialog::Warning, {"关闭"}, 0);
        return;
    }
}

void MainWindow::file_import_receive()
{
    //增加文件导入逻辑
}

void MainWindow::file_export_receive()
{
    //增加文件导出逻辑
}

void MainWindow::ParamSetReceive()
{
    //增加参数设置逻辑
}

void MainWindow::IPSetReceive()
{
    //增加IP设置逻辑
}

void MainWindow::settingReceive()
{
    this->close();
}

void MainWindow::helpReceive()
{
    //    if(LanguageManager::instance()->currentLanguage()=="Chinese"){

    //        chinese_son->setWindowTitle("软件信息");
    //        chinese_son->show();
    //        qDebug()<<"helpsend";
    //    }else{
    //        english_son->setWindowTitle("Software information");
    //        english_son->show();
    //    }
}

void MainWindow::chineseReceive()
{
    //LanguageManager::instance()->loadLanguage("Chinese");
}

void MainWindow::englishReceive()
{
    //  LanguageManager::instance()->loadLanguage("English");
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    int wid = this->size().width();
    qDebug() << "wid:" << wid;
    emit sendWid(wid);
}

void MainWindow::BuildStatusBar()
{
    // 创建状态栏
    auto statusbar = ui->statusbar;
    statusbar->setStyleSheet(
        "QStatusBar{background-color:rgb(76,76,76);} QStatusBar::item{border:0px;}");   // 去除边框
    statusbar->setMaximumHeight(25);              // 固定状态栏高度
    statusbar->setContentsMargins(20, 0, 0, 0);   // 图标距离窗口左边20px（左内边距）

    // 1. 图标控件（ztlabel1）：固定大小，不拉伸
    auto ztlabel1 = new QLabel(statusbar);
    // 加载图标并固定大小（按图标实际尺寸，不拉伸）
    QImage img(":/images/resources/logo1.png");
    QPixmap scaledPix = QPixmap::fromImage(
        img.scaledToHeight(14, Qt::SmoothTransformation));   // 按高度14px缩放（保持比例）

    ztlabel1->setPixmap(scaledPix);
    ztlabel1->setFixedSize(scaledPix.size());   // 固定为图标大小（不扩展）
    ztlabel1->setStyleSheet("background:transparent;");
    ztlabel1->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ztlabel1->setToolTip("Software copyright company");
    statusbar->addWidget(ztlabel1);   // 添加到状态栏

    // 2. 左侧弹簧（推动标签向右）
    QWidget* leftSpacer = new QWidget(statusbar);
    leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    statusbar->addWidget(leftSpacer);
    // 2. 左侧弹簧（推动标签向右）
    QWidget* leftSpacer2 = new QWidget(statusbar);
    leftSpacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    statusbar->addWidget(leftSpacer2);
    // 2. 信息标签（ztlabel3）：可扩展，占满剩余空间（核心）
    auto ztlabel3 = new QLabel("显示状态信息.......", statusbar);
    ztlabel3->setAlignment(Qt::AlignCenter);
    ztlabel3->setStyleSheet(
        "QLabel{background:transparent; color:white;font-size:14px;font-family:'Microsoft "
        "YaHei','SimHei';border-top-left-radius:10px;border-top-right-radius:10px;border-bottom-"
        "left-radius:10px;border-bottom-right-radius:10px;}"
        "QLabel:hover{background-color:#0081FF;color:white;}"
        "QLabel:pressed{background-color:#004DB3;border-style: inset; }");
    // 关键：设置尺寸策略为“水平可扩展”，占据剩余空间
    ztlabel3->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);   // 水平扩展，垂直固定
    ztlabel3->setFixedHeight(25);
    ztlabel3->setFixedWidth(500);     // 设置适当宽度
    statusbar->addWidget(ztlabel3);   // 添加到状态栏（此控件会占满中间剩余空间）
    // 4. 右侧弹簧（推动标签向左）
    QWidget* rightSpacer = new QWidget(statusbar);
    rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    statusbar->addWidget(rightSpacer);

    // 3. 按钮（debugWindowButton）：保持原大小，不拉伸
    // 假设debugWindowButton已创建，设置为固定大小（或保持默认大小）
    debugWindowButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);   // 不拉伸
    // 如需固定大小，可手动设置（根据按钮实际内容）

    statusbar->addPermanentWidget(debugWindowButton);   // 最后添加按钮（在最右侧，保持原大小）

    statusbar->setFixedHeight(25);
}

void MainWindow::BuildApplicationIcon()
{
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(QIcon(":/images/resources/example.ico"));
    systemTray->setToolTip("Company Name: Yanding\r\nSoftware Name: Sample Program\r\nVersion: "
                           "2.1\r\nSupport Email: Support@rdbuy.com");
    minimumAct = new QAction("   Minimum", this);
    minimumAct->setIcon(QIcon(":/images/resources/min.png"));
    connect(minimumAct, SIGNAL(triggered()), this, SLOT(hide()));
    maximumAct = new QAction("   Maximum", this);
    maximumAct->setIcon(QIcon(":/images/resources/max.png"));
    connect(maximumAct, SIGNAL(triggered()), this, SLOT(showMaximized()));
    restoreAct = new QAction("   Restore Windows", this);
    restoreAct->setIcon(QIcon(":/images/resources/undo.png"));
    connect(restoreAct, SIGNAL(triggered()), this, SLOT(showNormal()));
    quitAct = new QAction("   Exit", this);
    quitAct->setIcon(QIcon(":/images/resources/exit.png"));
    QObject::connect(quitAct, SIGNAL(triggered()), this, SLOT(Exitsoftware()));
    pContextMenu = new QMenu(this);
    pContextMenu->setToolTipsVisible(true);
    pContextMenu->addAction(minimumAct);
    minimumAct->setToolTip(tr("Minimize the current window."));
    pContextMenu->addAction(maximumAct);
    maximumAct->setToolTip(tr("Maximize the current window."));
    pContextMenu->addAction(restoreAct);
    restoreAct->setToolTip(tr("Restore the current window."));
    pContextMenu->addSeparator();
    pContextMenu->addAction(quitAct);
    quitAct->setToolTip(tr("Close the current window."));
    pContextMenu->setStyleSheet(""
                                "QMenuBar{"
                                "background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 "
                                "lightgray, stop:1 darkgray);"
                                "color:black;"
                                "}"
                                "QMenuBar::item{"
                                "spacing: 3px;"
                                "padding: 1px 10px;"
                                "background: transparent;"
                                "}"
                                "QMenuBar::item:selected{"
                                "background: rgb(178,178,178);"
                                "}"
                                "QMenuBar::item:pressed{"
                                "background: rgb(178,178,178);"
                                "}"
                                "QMenu{"
                                "border: 1px solid rgb(220,220,220);"
                                "background-color: rgb(255,255,255);"
                                "color:black;"
                                "margin:0px"
                                "}"
                                "QMenu::item{"
                                "padding: 2px 20px 2px 20px;"
                                "border: 1px solid transparent;"
                                "color:black;"
                                "}"
                                "QMenu::item:!checked{"
                                "color:rgb(50, 50, 50);"
                                "}"
                                "QMenu::item:!enabled{"
                                "color:rgb(120, 120, 120);"
                                "}"
                                "QMenu::item:selected{"
                                "background-color: rgb(0,160,233);"
                                "border: 1px solid rgb(180,180,180);"
                                "}");
    systemTray->setContextMenu(pContextMenu);
    connect(systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
    systemTray->show();
}

void MessWriteLog(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    //定义日志文件存放路径
    QString logDir = QApplication::applicationDirPath() + "\\Log\\"
                     + QDateTime::currentDateTime().toString("yyyy_MM_dd") + "\\";
    QString logFile;
    QDir curDir(logDir);
    if (!curDir.exists(logDir))   //如果路径不存在则创建
    {
        curDir.mkpath(logDir);
    }
    static QMutex mutex;   //互斥量
    mutex.lock();
    QString contextType;   //日志类型
    switch (type)
    {
    case QtDebugMsg:
        logFile = logDir + "Debug";
        contextType = QString("Debug");
        break;
    case QtWarningMsg:
        logFile = logDir + "Warning";
        contextType = QString("Warning");
        break;
    case QtCriticalMsg:
        logFile = logDir + "Critical";
        contextType = QString("Critical");
        break;
    case QtFatalMsg:
        logFile = logDir + "Fatal";
        contextType = QString("Fatal");
    }
    QString contextInfo
        = QString("[%1: %2]").arg(QString(context.file)).arg(context.line);   //代码所在文件及行数
    QString contextTime = QDateTime::currentDateTime().toString(
        "yyyy-MM-dd hh:mm:ss.mmm");   //当前时间

    QString mess = QString("%1: [%2] %3: %4")
                       .arg(contextTime)
                       .arg(contextType)
                       .arg(contextInfo)
                       .arg(msg);   //一行日志信息
    QFile contextFile(logFile + ".log");
    contextFile.open(QIODevice::WriteOnly | QIODevice::Append);   //以只写或追加方式打开文件
    QTextStream contextStream(&contextFile);                      //定义文件流
    contextStream << mess << "\r\n";                              //将文件流写入文件
    contextFile.flush();
    contextFile.close();
    mutex.unlock();   //释放互斥量
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    int result = CustomDialog::showMessage(this, "确认关闭", "请问您确认要关闭当前窗口？",
                                           CustomDialog::Question, {"确定", "取消"},
                                           0   // 默认按钮为"确定"
    );

    if (result == 0)
    {
        event->accept();
        delete systemTray;
        qApp->exit(0);
        return;
    }
    else
    {
        event->ignore();
    }


}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() != QEvent::WindowStateChange)
        return;
    if (this->windowState() == Qt::WindowMaximized)
    {

    }
    if (this->windowState() == Qt::WindowMinimized)
    {

    }
    if (this->windowState() == Qt::WindowNoState)
    {

    }
}

void MainWindow::Exitsoftware()
{
    int result = CustomDialog::showMessage(this, "确认关闭", "请问您确认要关闭当前窗口？",
                                           CustomDialog::Question, {"确定", "取消"},
                                           0   // 默认按钮为"确定"
    );

    if (result == 0)
    {
        delete systemTray;
        qApp->exit(0);
    }
    else
    {
    }

}

void MainWindow::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    //点击托盘显示窗口
    case QSystemTrayIcon::Trigger:
        {
            showNormal();
            break;
        }
    //双击托盘显示窗口
    case QSystemTrayIcon::DoubleClick:
        {
            showNormal();
            break;
        }
    default:
        break;
    }
}

void MainWindow::on_reconnection_triggered()
{
    //加入设备重新连接功能代码
}

void MainWindow::on_quit_triggered()
{
    Exitsoftware();
}

void MainWindow::on_chinese_triggered()
{
    //加入切换成中文界面的功能代码
}

void MainWindow::on_english_triggered()
{
    //加入切换成英文界面的功能代码
}

void MainWindow::on_manual_triggered()
{
    QDir directory = QDir::current();
    if (!directory.cd("document"))
    {
        QMessageBox msg;
        msg.setWindowTitle(QStringLiteral("文件夹找不到！"));
        msg.setText(QStringLiteral("抱歉，没有找到存放软件操作说明文档的文件夹！"));
        msg.addButton(QStringLiteral("关闭"), QMessageBox::ActionRole);
        msg.setIcon(QMessageBox::Warning);
        msg.setStyleSheet("color:white;background-color: rgb(98,98,98)");
        msg.move(this->x() + this->width() / 2.5, this->y() + this->height() / 3);
        msg.exec();
        return;
    }
    directory.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    QFileInfo doc(directory, QStringLiteral("软件操作说明文档.pdf"));
    if (!doc.exists())
    {
        QMessageBox msg;
        msg.setWindowTitle(QStringLiteral("文档找不到！"));
        msg.setText(QStringLiteral("抱歉，没有找到相关软件操作说明文档！"));
        msg.addButton(QStringLiteral("关闭"), QMessageBox::ActionRole);
        msg.setIcon(QMessageBox::Warning);
        msg.setStyleSheet("color:white;background-color: rgb(98,98,98)");
        msg.move(this->x() + this->width() / 2.5, this->y() + this->height() / 3);
        msg.exec();
        return;
    }
    QUrl pathUrl = QUrl::fromLocalFile(doc.filePath());
    bool isopened = QDesktopServices::openUrl(pathUrl);
    if (!isopened)
    {
        QMessageBox msg;
        msg.setWindowTitle(QStringLiteral("文档打不开！"));
        msg.setText(QStringLiteral("抱歉，软件操作说明文档打不开！"));
        msg.addButton(QStringLiteral("关闭"), QMessageBox::ActionRole);
        msg.setIcon(QMessageBox::Warning);
        msg.setStyleSheet("color:white;background-color: rgb(98,98,98)");
        msg.move(this->x() + this->width() / 2.5, this->y() + this->height() / 3);
        msg.exec();
        return;
    }
}

void MainWindow::on_about_triggered()
{
    Ui::aboutDialog uiAbout;
    QDialog* about = new QDialog(this);
    uiAbout.setupUi(about);
    about->setWindowFlags(about->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    about->setWindowTitle(QStringLiteral("软件信息"));
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    about->resize(QSize(deskRect.width() / 3.5, deskRect.height() / 3.5));
    about->setStyleSheet("background-color: rgb(62,62,62);");
    about->show();
}

void MainWindow::on_disconnection_triggered()
{
    //加入设备断开连接功能代码
}

void MainWindow::on_pushButton_clicked()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();   // 格式化为字符串
    QString currentTimeString = currentDateTime.toString("hh:mm:ss");
    ui->textEdit->append("·" + currentTimeString + tr("设备连接失败"));
}

void MainWindow::on_pushButton_96_clicked()
{
    int RowCont;
    QString name = "示例";
    RowCont = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(RowCont);
    QTableWidgetItem* List_item = new QTableWidgetItem(QString::number(RowCont + 1));
    List_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    // 正确写法：不可编辑，但可选中
    List_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->tableWidget->setItem(RowCont, 0, List_item);

    // 创建容器 widget
    QWidget* container = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(container);
    layout->setContentsMargins(30, 11, 30, 11);   // 设置边距，控制与单元格边缘的距离
    layout->setSpacing(5);                        // 设置图标和文本之间的间距

    // 创建图标 label
    QLabel* iconLabel = new QLabel(container);
    QPixmap pixmap(":/images/resources/file.png");
    iconLabel->setPixmap(pixmap);
    iconLabel->setAlignment(Qt::AlignRight);
    //iconLabel->setFixedSize(18, 18);  // 设置图标大小

    // 创建文本 label
    QLabel* textLabel = new QLabel("示例", container);
    textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);   // 文本左对齐并垂直居中

    QString Style = R"(
       QLabel{
            background: transparent;
            color: white;
            border: none;
            padding: 0px;
            selection-background-color: #0081FF;
        }


    )";
    QString Style2 = R"(
       QWidget{
            background: transparent;
            color: white;
            border: none;
            padding: 0px;
            selection-background-color: #0081FF;
                    }


                    )";
    container->setStyleSheet(Style2);
    textLabel->setStyleSheet(Style);
    iconLabel->setStyleSheet(Style);
    // 将图标和文本添加到布局
    layout->addWidget(iconLabel);
    layout->addWidget(textLabel);

    // 确保平均分配空间
    layout->setStretch(0, 1);
    layout->setStretch(1, 1);
    // 将容器添加到表格
    ui->tableWidget->setCellWidget(RowCont, 1, container);

    QPushButton* PB_file = new QPushButton(name);
    PB_file->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    PB_file->setStyleSheet(transparentSpinBoxStyle);
    PB_file->setIcon(QIcon(":/images/resources/编辑.png"));
    QWidget* widget_file = new QWidget;
    QHBoxLayout* hLayout_file;
    hLayout_file = new QHBoxLayout();
    hLayout_file->addWidget(PB_file);
    hLayout_file->setMargin(0);
    widget_file->setStyleSheet("background:transparent;");
    //    hLayout_file->setSizeConstraint(QLayout::SetFixedSize);
    hLayout_file->setAlignment(widget_file, Qt::AlignCenter);
    hLayout_file->setContentsMargins(0, 0, 0, 0);
    widget_file->setLayout(hLayout_file);
    ui->tableWidget->setCellWidget(RowCont, 2, widget_file);

    QPushButton* PB = new QPushButton("");
    PB->setStyleSheet(R"(
        QPushButton {
            border: none;                  /* 移除边框 */
            background: transparent;       /* 背景透明 */
            image: url(:/images/resources/错.png); /* 使用image属性 */
            width: 24px;                   /* 设置按钮宽度 */
            height: 24px;                  /* 设置按钮高度 */
        }
        QPushButton:hover {
            image: url(:/images/resources/错 (1).png); /* 悬停状态图标 */
        }
        QPushButton:pressed {
            image: url(:/style/resource/错 (1).png); /* 按下状态图标 */
            background-color: rgba(0, 128, 255, 30); /* 半透明背景效果 */
        }

    )");
    QWidget* widget = new QWidget;
    QHBoxLayout* hLayout;
    hLayout = new QHBoxLayout();
    hLayout->addWidget(PB);
    hLayout->setMargin(0);
    hLayout->setAlignment(widget, Qt::AlignCenter);
    hLayout->setContentsMargins(20, 0, 20, 0);
    widget->setLayout(hLayout);
    widget->setStyleSheet("background:transparent;");
    ui->tableWidget->setCellWidget(RowCont, 3, widget);
    QTableWidget* table = ui->tableWidget;
    connect(PB, &QPushButton::clicked, this, [this, table]() { deleteRow(table); });
}

void MainWindow::deleteRow(QTableWidget* table)
{
    QWidget* w = qobject_cast<QWidget*>(sender()->parent());
    if (w)
    {
        int row = table->indexAt(w->pos()).row();
        table->removeRow(row);
        table->setCurrentCell(0, 0);
        table->clearSelection();
        int totalrow = table->rowCount();
        for (int i = row; i < totalrow; i++)
        {
            QTableWidgetItem* List_item = new QTableWidgetItem(QString::number(i + 1));
            List_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            QFont font("Microsoft YaHei", 9, 75);
            List_item->setFont(font);
            List_item->setFlags(Qt::NoItemFlags);
            table->setItem(i, 0, List_item);
        }
    }
}

void MainWindow::on_pushButton_97_clicked()
{
    int result = CustomDialog::showMessage(this, tr("确认清空时序"),
                                           tr("您确定要清空时序表格吗？此操作不可撤销。"),
                                           CustomDialog::Question, {tr("确定"), tr("取消")},
                                           0   // 默认按钮为"确定"
    );

    if (result == 0)
    {
        for (int row = ui->tableWidget->rowCount() - 1; row >= 0; --row)
        {
            ui->tableWidget->removeRow(row);
        }
    }
}

void MainWindow::on_pushButton_99_clicked() {}

void MainWindow::on_pushButton_103_clicked()
{
    int result = CustomDialog::showMessage(this, tr("确认删除时序文件"),
                                           tr("您确定要删除时序文件并清空表格吗？"),
                                           CustomDialog::Question, {tr("确定"), tr("取消")},
                                           0   // 默认按钮为"确定"
    );

    if (result == 0)
    {
        for (int row = ui->tableWidget->rowCount() - 1; row >= 0; --row)
        {
            ui->tableWidget->removeRow(row);
        }
    }
}

void MainWindow::on_Slider_Downcct_valueChanged(int value)
{
    ui->spinBox_cct_2->setValue(value);
}

void MainWindow::on_spinBox_cct_2_valueChanged(int arg1)
{
    ui->Slider_Downcct->setValue(arg1);
}

void MainWindow::on_Slider_Downill_valueChanged(int value)
{
    ui->spinBox_ill_2->setValue(value);
}

void MainWindow::on_spinBox_ill_2_valueChanged(int arg1)
{
    ui->Slider_Downill->setValue(arg1);
}

void MainWindow::on_Slider_nm1_actionTriggered(int action) {}

void MainWindow::on_Slider_nm1_valueChanged(int value) {}

void MainWindow::on_pushButton_clicked(bool checked) {}

void MainWindow::on_pushButton_pressed()
{
    ui->pushButton->setIcon(QIcon(":/images/resources/disconnect2.png"));
}

void MainWindow::on_pushButton_released()
{
    ui->pushButton->setIcon(QIcon(":/images/resources/disconnect.png"));
}

void MainWindow::logMessage(const QString& message, bool isError)
{
    // 获取当前时间
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 根据消息类型设置不同的颜色
    QString color = isError ? "#FF0000" : "#FFFFFF";

    // 构建HTML格式的消息
    QString htmlMessage = QString("<div style=\"color:%1;\">[%2] %3</div>")
                              .arg(color)
                              .arg(timestamp)
                              .arg(message.toHtmlEscaped());

    // 将消息添加到文本编辑区域
    debugTextEdit->append(htmlMessage);

    // 自动滚动到底部
    debugTextEdit->moveCursor(QTextCursor::End);
}

void MainWindow::toggleDebugWindow(bool checked)
{
    if (checked)
    {
        debugWindowButton->setIcon(QIcon(":/images/resources/debug_down.png"));
        debugDockWidget->show();
    }
    else
    {
        debugWindowButton->setIcon(QIcon(":/images/resources/debug.png"));
        debugDockWidget->hide();
    }
}
