#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    Asks.push_back(std::pair<QLabel*, QLabel*>(ui->lblAskPr5, ui->lblAskQty5));
    Asks.push_back(std::pair<QLabel*, QLabel*>(ui->lblAskPr6, ui->lblAskQty6));
    Asks.push_back(std::pair<QLabel*, QLabel*>(ui->lblAskPr7, ui->lblAskQty7));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr1, ui->lblBidQty1));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr2, ui->lblBidQty2));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr3, ui->lblBidQty3));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr4, ui->lblBidQty4));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr5, ui->lblBidQty5));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr6, ui->lblBidQty6));
    Bids.push_back(std::pair<QLabel*, QLabel*>(ui->lblBidPr7, ui->lblBidQty7));

    int i = 0;
    int j = 0;
    QTableWidgetItem* pItem;
    for (i = 0; i < 100; ++i)
    {
        ui->tableOrders->insertRow(0);
        for (j = 0; j < 6; ++j)
        {
            pItem = ui->tableOrders->item(0, j);
            if (!pItem)
            {
                pItem = new QTableWidgetItem();
                ui->tableOrders->setItem(0, j, pItem);
            }
        }
    }

    UpdateCount = 0;
    feedCount = 0;
    lastfeedCount = 0;

    Displaytimer = new QTimer(this);
    Displaytimer->setInterval(1000);
    Displaytimer->setSingleShot(false);

    DefineStyleSheet();
    SetStyle();

    connect(Displaytimer, SIGNAL(timeout()),this, SLOT(UpdateDisplay()));

    ProcedureStarted = false;

    int InitialLogPool = 1000;
 
    Displaytimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::WriteLog(void)
{
    time_t now = time(NULL);
    struct tm* pnow = localtime(&now);
    QString strtime;
    strtime = strtime.asprintf("%02d:%02d:%02d", pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
   
    std::string msg = "";
    Enums::logType type = Enums::logType::_NONE;
    type = logWriter->getLog(msg);
   

    if (msg != "")
    {
        int count = 0;
        while (msg != "")
        {
            const char* chmsg = msg.c_str();
            QString NewLine = "";
            NewLine.append(strtime);
            NewLine.append("     ");
            NewLine.append(chmsg);
            NewLine.append("\n");
            LogText.append(NewLine);
            ++count;
            logfile << NewLine.toStdString();
            logfile.flush();
            if (count > 1000)
            {
                break;
            }
            type = logWriter->getLog(msg);
        }
        if (ui->LogTextEdit->verticalScrollBar()->value() == ui->LogTextEdit->verticalScrollBar()->maximum())
        {
            ui->LogTextEdit->setPlainText(LogText);
            ui->LogTextEdit->verticalScrollBar()->setValue(ui->LogTextEdit->verticalScrollBar()->maximum());
        }
        else
        {
            int CurrentPos = ui->LogTextEdit->verticalScrollBar()->value();
            ui->LogTextEdit->setPlainText(LogText);
            ui->LogTextEdit->verticalScrollBar()->setValue(CurrentPos);
        }
        if (ui->BTLogTextEdit->verticalScrollBar()->value() == ui->BTLogTextEdit->verticalScrollBar()->maximum())
        {
            ui->BTLogTextEdit->setPlainText(LogText);
            ui->BTLogTextEdit->verticalScrollBar()->setValue(ui->BTLogTextEdit->verticalScrollBar()->maximum());
        }
        else
        {
            int CurrentPos = ui->BTLogTextEdit->verticalScrollBar()->value();
            ui->BTLogTextEdit->setPlainText(LogText);
            ui->BTLogTextEdit->verticalScrollBar()->setValue(CurrentPos);
        }
    }
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

        //MainTab
        lastfeedCount = feedCount;
        feedCount = feedReader->feedCount;
        ui->lblFeedAll->setText(QString("%L1").arg(feedCount));
        ui->lblFeedInc->setText(QString("%L1").arg(feedCount - lastfeedCount));

        //SymbolTab
        if (displayedIns)
        {
            UpdateInsInfo(displayedIns);
        }
    }
    WriteLog();  
}

void MainWindow::InitilizeInsBox(void)
{
    if (insList)
    {
        std::map<std::string, OKExInstrument*>::iterator it;
        std::map<std::string, OKExInstrument*>::iterator it_end = insList->end();
        for (it = insList->begin(); it != it_end; ++it)
        {
            ui->BoxInstruments->addItem(QString::fromStdString(it->first));
        }
    }
}

void MainWindow::on_BoxInstruments_currentTextChanged(const QString& arg1)
{
    ui->lblSymbol->setText(arg1);
    std::string ID = arg1.toStdString();
    SetInstrunemt(ID);
}

void MainWindow::SetInstrunemt(std::string instId)
{
    std::map<std::string, OKExInstrument*>::iterator it = insList->find(instId);
    if (it != insList->end())
    {
        displayedIns = it->second;

        UpdateInsInfo(displayedIns);
    }
}

void MainWindow::UpdateInsInfo(OKExInstrument* ins)
{
    //Price
    ui->lblOpen->setText(QString("%L1").arg(ins->open, 0, 'f', 1));
    ui->lblLow->setText(QString("%L1").arg(ins->low, 0, 'f', 1));
    ui->lblHigh->setText(QString("%L1").arg(ins->high, 0, 'f', 1));
    ui->lblLast->setText(QString("%L1").arg(ins->last, 0, 'f', 1));


    //Trade Summary
    ui->lblSellQty->setText(QString("%L1").arg(ins->tradedQtySell, 0, 'f', 5));
    ui->lblBuyQty->setText(QString("%L1").arg(ins->tradedQtyBuy, 0, 'f', 5));
    if (ins->tradedQtySell > 0)
    {
        ui->lblSellPr->setText(QString("%L1").arg(ins->tradedAmtSell / ins->tradedQtySell, 0, 'f', 2));
    }
    else
    {
        ui->lblSellPr->setText(QString("%L1").arg(0, 0, 'f', 2));
    }
    if (ins->tradedQtyBuy > 0)
    {
        ui->lblBuyPr->setText(QString("%L1").arg(ins->tradedAmtBuy / ins->tradedQtyBuy, 0, 'f', 2));
    }
    else
    {
        ui->lblBuyPr->setText(QString("%L1").arg(0, 0, 'f', 2));
    }
    
    
    //PL
    ins->tradePL = (ins->tradedAmtSell - ins->tradedQtySell * ins->mid) + (ins->tradedQtyBuy * ins->mid - ins->tradedAmtBuy);
    ins->posPL = ins->prevNetPos * (ins->mid - ins->baseMid);
    ins->totalPL = ins->tradePL + ins->posPL;
    ui->lblTradePL->setText(QString("%L1").arg(ins->tradePL, 0, 'f', 2));
    ui->lblPosPL->setText(QString("%L1").arg(ins->posPL, 0, 'f', 2));
    ui->lblTotalPL->setText(QString("%L1").arg(ins->totalPL, 0, 'f', 2));

    //Books
    std::vector<std::pair<QLabel*, QLabel*>>::iterator it = Asks.begin();
    std::vector<std::pair<QLabel*, QLabel*>>::iterator bkend = Asks.end();
    std::map<int, book*>::iterator insbkit = ins->bestAsk;
    std::map<int, book*>::iterator insbkend = ins->books->end();
    while (it != bkend)
    {
        if (insbkit == insbkend)
        {
            it->first->setText("");
            it->second->setText("");
        }
        else
        {
            while (insbkit != insbkend)
            {
                if (insbkit->second->sz > 0)
                {
                    it->first->setText(QString("%L1").arg((double)insbkit->first / ins->priceUnit,0,'f',1));
                    it->second->setText(QString("%L1").arg(insbkit->second->sz,0,'f',5));
                    ++insbkit;
                    break;
                }
                else
                {
                    ++insbkit;
                    if (insbkit == insbkend)
                    {
                        it->first->setText("");
                        it->second->setText("");
                        break;
                    }
                }
            }
        }
        ++it;
    }
    it = Bids.begin();
    bkend = Bids.end();
    insbkit = ins->bestBid;
    std::map<int, book*>::iterator insbkbegin = ins->books->begin();
    while (it != bkend)
    {
        if (insbkit == insbkend)
        {
            it->first->setText("");
            it->second->setText("");
        }
        else if (insbkit == insbkbegin)
        {
            if (insbkit->second->sz > 0)
            {
                it->first->setText(QString("%L1").arg((double)insbkit->first / ins->priceUnit, 0, 'f', 1));
                it->second->setText(QString("%L1").arg(insbkit->second->sz, 0, 'f', 5));
                insbkit = insbkend;
            }
            else
            {
                it->first->setText("");
                it->second->setText("");
                insbkit = insbkend;
            }
        }
        else
        {
            while (insbkit != insbkbegin)
            {
                if (insbkit->second->sz > 0)
                {
                    it->first->setText(QString("%L1").arg((double)insbkit->first / ins->priceUnit, 0, 'f', 1));
                    it->second->setText(QString("%L1").arg(insbkit->second->sz, 0, 'f', 5));
                    --insbkit;
                    break;
                }
                else
                {
                    --insbkit;
                    if (insbkit == insbkbegin)
                    {
                        if (insbkit->second->sz > 0)
                        {
                            it->first->setText(QString("%L1").arg((double)insbkit->first / ins->priceUnit, 0, 'f', 1));
                            it->second->setText(QString("%L1").arg(insbkit->second->sz, 0, 'f', 5));
                            insbkit = insbkend;
                        }
                        else
                        {
                            it->first->setText("");
                            it->second->setText("");
                            insbkit = insbkend;
                        }
                    }
                }
            }
        }
        ++it;
    }

    //Live Orders
    bool desired = true;
    bool expected = false;
    while (true)
    {
        if (ins->lckLiveOrdList.compare_exchange_weak(expected, desired))
        {
            int i = 0;
            std::map<std::string, OKExOrder*>::iterator it;
            std::map<std::string, OKExOrder*>::iterator itend = ins->liveOrdList->end();
            for (it = ins->liveOrdList->begin(); it != itend; ++it)
            {
                if (i > 99)
                {
                    break;
                }
                ui->tableOrders->item(i, 0)->setText(QString::fromStdString(it->second->baseOrdId));
                ui->tableOrders->item(i, 1)->setText(QString::number((int)it->second->status));
                ui->tableOrders->item(i, 2)->setText(QString::number((int)it->second->side));
                ui->tableOrders->item(i, 3)->setText(QString("%L1").arg(it->second->px, 0, 'f', 1));
                ui->tableOrders->item(i, 4)->setText(QString("%L1").arg(it->second->sz, 0, 'f', 5));
                ui->tableOrders->item(i, 5)->setText(QString("%L1").arg(it->second->execSz, 0, 'f', 5));
                ++i;
            }
            ins->lckLiveOrdList = false;
            break;
        }
        else
        {
            expected = false;
        }
    }
}

void MainWindow::UpdateSettingTab(void)
{
    ui->lblOutputFilePath->setText(QString::fromStdString(GlobalVariables::OKExBacktest_GUI::outputFilePath));
    ui->lblCalendarFile->setText(QString::fromStdString(GlobalVariables::OKExBacktest_GUI::calandarFile));
    ui->lblMasterFilePath->setText(QString::fromStdString(GlobalVariables::OKExBacktest_GUI::masterFilePath));
    ui->lblFeedFilePath->setText(QString::fromStdString(GlobalVariables::OKExBacktest_GUI::feedFilePath));
}

void MainWindow::UpdatePairsTab(void)
{
    if (insList)
    {
        std::map<std::string, OKExInstrument*>::iterator ins_it;
        std::map<std::string, OKExInstrument*>::iterator ins_end = insList->end();
        std::vector<PairInfo>::iterator info_it = PairsInfo->begin();
        std::vector<PairInfo>::iterator info_end = PairsInfo->end();
        for (ins_it = insList->begin(); ins_it != ins_end; ++ins_it)
        {
            
        }
    }
}

void MainWindow::InitializeObjects(void)
{
    calendar->Initialize(GlobalVariables::OKExBacktest_GUI::calandarFile);
    feedReader->initialize();
    insList = feedReader->initializeInsList(GlobalVariables::OKExBacktest_GUI::masterFilePath + "\\master.txt");
    feedReader->readParamFile(GlobalVariables::OKExBacktest_GUI::paramFile);
}

void MainWindow::SetNewDate(int date)
{

}

void MainWindow::on_testButton_clicked(void)
{
    ui->testPushbutton->setEnabled(false);
    PrevFeedCount = 0;

    InitializeObjects();
    InitilizeInsBox();

    ProcedureStarted = true;
    boost::thread* th = new boost::thread(boost::bind(&MainWindow::test,this));
    ui->testPushbutton->setText("testing...");
}

void MainWindow::test(void)
{
    int startdate = ui->testTextEdit->toPlainText().toInt();
    int enddate = ui->testTextEdit_2->toPlainText().toInt();

    bool IsFirstDay = true;

    std::map<int, Date>::iterator it_end = calendar->end();
    std::map<int, Date>::iterator it = calendar->GetDay(startdate, 0);
    std::map<int, Date>::iterator it_prev = calendar->GetDay(startdate, -1);

    std::string feedFile = "";

    while (it->first <= enddate)
    {
        feedFile = GlobalVariables::OKExBacktest_GUI::feedFilePath + "\\OKExFeed_" + it->second.strday + ".log.gz";
        feedReader->readFeedFile(feedFile);
        ++it;
    }
    ui->testPushbutton->setText("tested.");
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

void MainWindow::PairInfo::SetInfo(OKExInstrument* ins, bool color)
{
    double sp;
    QPalette G;
    QPalette R;
    QPalette B;
    G.setColor(QPalette::WindowText, Qt::green);
    R.setColor(QPalette::WindowText, Qt::red);
    B.setColor(QPalette::WindowText, Qt::black);
   
}
