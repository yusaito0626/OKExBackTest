#include "mainwindow_temp.h"
#include "../build-OKExBacktest_GUI-Desktop_Qt_6_4_0_MSVC2019_64bit-Release/ui_mainwindow.h""

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stbarStatus = new QLabel();
    stbarStatus->setFixedWidth(85);
    stbarStatus->setAlignment(Qt::AlignCenter);
    stbarMsg = new QLabel();
    stbarMsg->setFixedWidth(380);
    stbarMsg->setAlignment(Qt::AlignCenter);
    stbarTime = new QLabel();
    stbarTime->setFixedWidth(85);
    stbarTime->setAlignment(Qt::AlignCenter);
    stbarDate = new QLabel();
    stbarDate->setFixedWidth(85);
    stbarDate->setAlignment(Qt::AlignCenter);

    ui->statusbar->addWidget(stbarStatus);
    ui->statusbar->addWidget(stbarMsg);
    ui->statusbar->addWidget(stbarTime);
    ui->statusbar->addWidget(stbarDate);

    Asks.push_back(std::pair<QLabel*, QLabel*>(ui->lblAskPr1,ui->lblAskQty1));
    Asks.push_back(std::pair<QLabel*, QLabel*>(ui->lblAskPr2, ui->lblAskQty2));
    Asks.push_back(std::pair<QLabel*, QLabel*>(ui->lblAskPr3, ui->lblAskQty3));
    Asks.push_back(std::pair<QLabel*, QLabel*>(ui->lblAskPr4, ui->lblAskQty4));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr1, ui->lblBidQty1));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr2, ui->lblBidQty2));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr3, ui->lblBidQty3));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr4, ui->lblBidQty4));

    UpdateCount = 0;

    Displaytimer = new QTimer(this);
    Displaytimer->setInterval(1000);
    Displaytimer->setSingleShot(false);

    DefineStyleSheet();
    SetStyle();

    connect(Displaytimer, SIGNAL(timeout()),this, SLOT(UpdateDisplay()));

    ProcedureStarted = false;

    Displaytimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::WriteLog(void)
{

}

void MainWindow::UpdateDisplay(void)
{
    if (ProcedureStarted)
    {
        if (UpdateCount > 1)
        {
            AddColor = true;
            UpdateCount = 0;
        }
        else
        {
            AddColor = false;
            ++UpdateCount;
        }
    }
    WriteLog();  
}

void MainWindow::InitilizeInsBox(void)
{

}

void MainWindow::on_BoxInstruments_currentTextChanged(const QString& arg1)
{
    ui->lblSymbol->setText(arg1);
    std::string ID = arg1.toStdString();
    SetInstrunemt(ID);
}

void MainWindow::SetInstrunemt(std::string instId)
{

}

void MainWindow::UpdateSettingTab(void)
{

}

void MainWindow::UpdatePairsTab(void)
{

}

void MainWindow::InitializeObjects(void)
{

}

void MainWindow::SetNewDate(int date)
{

}

void MainWindow::on_testButton_clicked(void)
{

}

void MainWindow::test(void)
{

}

void MainWindow::DefineStyleSheet(void)
{
    MainWindowStyle = "QMainWindow{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(223,227,232,0.9) stop: 1 rgba(223,227,232,1));\
                        }";
    MenuBarStyle = "QMenuBar{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(197,206,216,0.9) stop: 1 rgba(197,206,216,1));\
                        }";
    MenuStyle = "QMenu{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(223,227,232,0.9) stop: 1 rgba(223,227,232,1));\
                        }";
    MainTabStyle = "QTabWidget::pane{\
                                border-top: 1px solid #C2C7CB;\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.4 rgba(223,227,232,0.9) stop: 1 rgba(223,227,232,1));\
                        }";
    TabBarStyle = "QTabBar::tab{\
                                background-color : rgba(223,227,232,1);\
                                min-width: 10ex;\
                                border-top-left-radius: 2px;\
                                border-top-right-radius: 2px;\
                                padding: 3px;\
                        }\
                    QTabBar::tab:selected{\
                                background-color : rgba(223,227,232,0.5); \
                        }\
                    QTabBar::tab:hover{\
                                background-color : rgba(223,227,232,0.5); \
                        }";
    ScrollAreaStyle = "QLabel{\
                                background-color : rgba(137,114,150,1);\
                        }";
    TableStyle = "QTableView QTableCornerButton::section{\
                                background-color : rgba(197,206,216,1);\
                        }";
    msgBoxStyle = "QMessageBox{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(223,227,232,0.9) stop: 1 rgba(223,227,232,1));\
                        }";
    GeneralButtonStyle = "QPushButton{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(197,206,216,0.9) stop: 1 rgba(197,206,216,1));\
                                border: ridge;\
                                border-width: 2px;\
                                border-radius: 5;\
                                border-right-color: rgba(0,0,0,0.8);\
                                border-bottom-color: rgba(0,0,0,0.8);\
                                border-top-color: rgba(255,255,255,0.8);\
                                border-left-color: rgba(255,255,255,0.8);\
                        }\
                         QPushButton:checked{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(197,206,216,0.9) stop: 1 rgba(197,206,216,1));\
                                border: ridge;\
                                border-width: 2px;\
                                border-radius: 5;\
                                border-right-color: rgba(255,255,255,0.8);\
                                border-bottom-color: rgba(255,255,255,0.8);\
                                border-top-color: rgba(0,0,0,0.8);\
                                border-left-color: rgba(0,0,0,0.8);\
                        }\
                        QPushButton:pressed{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(197,206,216,0.9) stop: 1 rgba(197,206,216,1));\
                                border: ridge;\
                                border-width: 2px;\
                                border-radius: 5;\
                                border-right-color: rgba(255,255,255,0.8);\
                                border-bottom-color: rgba(255,255,255,0.8);\
                                border-top-color: rgba(0,0,0,0.8);\
                                border-left-color: rgba(0,0,0,0.8);\
                        }";
    StatusButtonStyle = "QPushButton{\
                                color : rgba(255,255,255,1)\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,0,0,0.3), stop: 0.5 rgba(255,0,0,0.9) stop: 1 rgba(255,0,0,1));\
                                border: ridge;\
                                border-width: 2px;\
                                border-radius: 5px;\
                                border-right-color: rgba(0,0,0,0.8);\
                                border-bottom-color: rgba(0,0,0,0.8);\
                                border-top-color: rgba(255,255,255,0.8);\
                                border-left-color: rgba(255,255,255,0.8);\
                        }\
                          QPushButton:checked{\
                                color : rgba(60,60,60,1)\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(0,255,0,1), stop: 0.3 rgba(0,255,0,0.9) stop: 1 rgba(0,255,0,1));\
                                border: ridge;\
                                border-width: 2px;\
                                border-radius: 5px;\
                                border-right-color: rgba(255,255,255,0.8);\
                                border-bottom-color: rgba(255,255,255,0.8);\
                                border-top-color: rgba(0,0,0,0.8);\
                                border-left-color: rgba(0,0,0,0.8);\
                        }\
                        QPushButton:pressed{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(197,206,216,0.9) stop: 1 rgba(197,206,216,1));\
                                border: ridge;\
                                border-width: 2px;\
                                border-radius: 5;\
                                border-right-color: rgba(255,255,255,0.8);\
                                border-bottom-color: rgba(255,255,255,0.8);\
                                border-top-color: rgba(0,0,0,0.8);\
                                border-left-color: rgba(0,0,0,0.8);\
                        }";
    LockButtonStyle = "QPushButton{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(197,206,216,0.9) stop: 1 rgba(197,206,216,1));\
                                border: ridge;\
                                border-width: 2px;\
                                border-radius: 10;\
                                border-right-color: rgba(0,0,0,0.8);\
                                border-bottom-color: rgba(0,0,0,0.8);\
                                border-top-color: rgba(255,255,255,0.8);\
                                border-left-color: rgba(255,255,255,0.8);\
                        }\
                          QPushButton::checked{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(197,206,216,0.9) stop: 1 rgba(197,206,216,1));\
                                border: ridge;\
                                border-width: 2px;\
                                border-radius: 10;\
                                border-right-color: rgba(255,255,255,0.8);\
                                border-bottom-color: rgba(255,255,255,0.8);\
                                border-top-color: rgba(0,0,0,0.8);\
                                border-left-color: rgba(0,0,0,0.8);\
                        }\
                        QPushButton:pressed{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(197,206,216,0.9) stop: 1 rgba(197,206,216,1));\
                                border: ridge;\
                                border-width: 2px;\
                                border-radius: 5;\
                                border-right-color: rgba(255,255,255,0.8);\
                                border-bottom-color: rgba(255,255,255,0.8);\
                                border-top-color: rgba(0,0,0,0.8);\
                                border-left-color: rgba(0,0,0,0.8);\
                        }";
    StatusBarStyle = "QStatusBar{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(223,227,232,0.9) stop: 1 rgba(223,227,232,1));\
                        }\
                        QStatusBar::item{\
                                background-color : QLinearGradient(x1: 0, y1: 0,x2: 1, y2: 1 stop: 0 rgba(255,255,255,1), stop: 0.3 rgba(223,227,232,0.9) stop: 1 rgba(223,227,232,1));\
                                border: ridge;\
                                border-width: 1.5px;\
                                border-right-color: rgba(255,255,255,0.8);\
                                border-bottom-color: rgba(255,255,255,0.8);\
                                border-top-color: rgba(0,0,0,0.8);\
                                border-left-color: rgba(0,0,0,0.8);\
                        }";

    GeneralLabelFont.setFamily(QStringLiteral("Calibri"));
    GeneralLabelFont.setPointSize(10);
    GeneralLabelFont.setBold(true);

    GeneralButtonFont.setFamily(QStringLiteral("Calibri"));
    GeneralButtonFont.setPointSize(10);
    GeneralButtonFont.setBold(true);

    QBrush TableButtonBrush(QColor(197,206,216,255));
    QBrush TableWindowBrush(QColor(223,227,232,255));

    TablePalette.setBrush(QPalette::Button,TableButtonBrush);
    TablePalette.setBrush(QPalette::Window,TableWindowBrush);

    Green.setColor(QPalette::WindowText,Qt::green);
    Red.setColor(QPalette::WindowText, Qt::red);

    QBrush ComboBoxBrush(QColor(197,206,216,255));

    //ComboBoxPalette.setStyle(Qt::SolidPattern);
    TablePalette.setBrush(QPalette::Button,ComboBoxBrush);
}

void MainWindow::SetStyle(void)
{
    this->setStyleSheet(MainWindowStyle);
    ui->menubar->setStyleSheet(MenuBarStyle);
    ui->statusbar->setStyleSheet(StatusBarStyle);
    ui->tabWidget->setStyleSheet(MainTabStyle);
    ui->tabWidget->tabBar()->setStyleSheet(TabBarStyle);
    ui->tabMain->setStyleSheet(MenuStyle);
    this->setFont(GeneralLabelFont);
    ui->testPushbutton->setStyleSheet(GeneralButtonStyle);
    ui->testPushbutton->setFont(GeneralButtonFont);
}


void MainWindow::PairInfo::Clear(void)
{
    if (Symbol)
    {
        Symbol->setText("");
    }
    if (SellAmt)
    {
        SellAmt->setText("");
    }
    if (SellPr)
    {
        SellPr->setText("");
    }
    if (Spread)
    {
        Spread->setText("");
    }
    if (BuyPr)
    {
        BuyPr->setText("");
    }
    if (BuyAmt)
    {
        BuyAmt->setText("");
    }
}
