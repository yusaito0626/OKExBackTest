/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tabMain;
    QGroupBox *groupBox;
    QPlainTextEdit *LogTextEdit;
    QGroupBox *groupBox_5;
    QFrame *line_33;
    QFrame *line_36;
    QFrame *line_37;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_34;
    QLabel *lblFeedAll;
    QLabel *lblFeedInc;
    QLabel *lblFeedQueue;
    QGroupBox *groupBox_7;
    QLabel *label_18;
    QLabel *label_17;
    QFrame *line_15;
    QFrame *line_20;
    QFrame *line_16;
    QLabel *label_10;
    QFrame *line_21;
    QLabel *label_15;
    QFrame *line_22;
    QLabel *label_14;
    QLabel *label_16;
    QFrame *line_23;
    QLabel *lblSmryInsSell;
    QLabel *lblSmryInsBuy;
    QLabel *lblSmryOdrSell;
    QLabel *lblSmryOdrBuy;
    QLabel *lblSmryExeSell;
    QLabel *lblSmryExeBuy;
    QLabel *lblSmryExeAmtSell;
    QLabel *lblSmryExeAmtBuy;
    QGroupBox *groupBox_8;
    QLabel *label_24;
    QLabel *label_28;
    QFrame *line_30;
    QFrame *line_31;
    QFrame *line_32;
    QLabel *label_29;
    QLabel *label_30;
    QFrame *line_34;
    QLabel *label_31;
    QFrame *line_35;
    QLabel *lblOdrLiveInsSell;
    QLabel *lblOdrLiveInsBuy;
    QLabel *lblOdrLiveSell;
    QLabel *lblOdrLiveBuy;
    QLabel *lblOdrLiveAmtSell;
    QLabel *lblOdrLiveAmtBuy;
    QWidget *tabPair;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *line_4;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QFrame *line_5;
    QFrame *line_6;
    QFrame *line_7;
    QFrame *line_8;
    QFrame *line_9;
    QFrame *line_10;
    QWidget *tabInstrument;
    QComboBox *BoxInstruments;
    QLabel *lblSymbol;
    QGroupBox *groupBox_2;
    QFrame *line_11;
    QFrame *line_12;
    QLabel *lblBidPr1;
    QLabel *lblBidPr2;
    QLabel *lblBidPr3;
    QLabel *lblBidPr4;
    QLabel *lblAskPr1;
    QLabel *lblAskPr2;
    QLabel *lblAskPr3;
    QLabel *lblAskPr4;
    QLabel *lblBidQty1;
    QLabel *lblBidQty2;
    QLabel *lblBidQty3;
    QLabel *lblBidQty4;
    QLabel *lblAskQty1;
    QLabel *lblAskQty2;
    QLabel *lblAskQty4;
    QLabel *lblAskQty3;
    QTableView *tableOrders;
    QGroupBox *groupBox_3;
    QCheckBox *chkBoxEnableTrade;
    QCheckBox *chkBoxNewOrder;
    QCheckBox *chkBoxCanMod;
    QPushButton *pushButtonStopTradeSymbol;
    QGroupBox *groupBox_4;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *lblOpen;
    QLabel *lblHigh;
    QLabel *lblLow;
    QLabel *lblTime;
    QWidget *tabSetting;
    QWidget *tab;
    QPushButton *testPushbutton;
    QPlainTextEdit *testTextEdit;
    QPlainTextEdit *testTextEdit_2;
    QPlainTextEdit *BTLogTextEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(675, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(5, 9, 665, 680));
        QFont font;
        font.setBold(true);
        tabWidget->setFont(font);
        tabWidget->setLayoutDirection(Qt::LeftToRight);
        tabMain = new QWidget();
        tabMain->setObjectName("tabMain");
        groupBox = new QGroupBox(tabMain);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(490, 120, 161, 171));
        LogTextEdit = new QPlainTextEdit(tabMain);
        LogTextEdit->setObjectName("LogTextEdit");
        LogTextEdit->setGeometry(QRect(0, 299, 660, 281));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Calibri")});
        font1.setPointSize(8);
        font1.setBold(false);
        LogTextEdit->setFont(font1);
        groupBox_5 = new QGroupBox(tabMain);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setGeometry(QRect(10, 210, 471, 80));
        line_33 = new QFrame(groupBox_5);
        line_33->setObjectName("line_33");
        line_33->setGeometry(QRect(10, 35, 451, 21));
        line_33->setFrameShape(QFrame::HLine);
        line_33->setFrameShadow(QFrame::Sunken);
        line_36 = new QFrame(groupBox_5);
        line_36->setObjectName("line_36");
        line_36->setGeometry(QRect(140, 20, 20, 51));
        line_36->setFrameShape(QFrame::VLine);
        line_36->setFrameShadow(QFrame::Sunken);
        line_37 = new QFrame(groupBox_5);
        line_37->setObjectName("line_37");
        line_37->setGeometry(QRect(310, 20, 20, 51));
        line_37->setFrameShape(QFrame::VLine);
        line_37->setFrameShadow(QFrame::Sunken);
        label_32 = new QLabel(groupBox_5);
        label_32->setObjectName("label_32");
        label_32->setGeometry(QRect(70, 25, 21, 16));
        QFont font2;
        font2.setPointSize(8);
        font2.setBold(true);
        label_32->setFont(font2);
        label_33 = new QLabel(groupBox_5);
        label_33->setObjectName("label_33");
        label_33->setGeometry(QRect(200, 25, 101, 16));
        label_33->setFont(font2);
        label_34 = new QLabel(groupBox_5);
        label_34->setObjectName("label_34");
        label_34->setGeometry(QRect(380, 25, 61, 16));
        label_34->setFont(font2);
        lblFeedAll = new QLabel(groupBox_5);
        lblFeedAll->setObjectName("lblFeedAll");
        lblFeedAll->setGeometry(QRect(40, 55, 101, 16));
        lblFeedAll->setFont(font2);
        lblFeedAll->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblFeedInc = new QLabel(groupBox_5);
        lblFeedInc->setObjectName("lblFeedInc");
        lblFeedInc->setGeometry(QRect(210, 55, 101, 16));
        lblFeedInc->setFont(font2);
        lblFeedInc->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblFeedQueue = new QLabel(groupBox_5);
        lblFeedQueue->setObjectName("lblFeedQueue");
        lblFeedQueue->setGeometry(QRect(360, 55, 101, 16));
        lblFeedQueue->setFont(font2);
        lblFeedQueue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_7 = new QGroupBox(tabMain);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setGeometry(QRect(10, 10, 641, 95));
        label_18 = new QLabel(groupBox_7);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(10, 48, 41, 16));
        label_18->setFont(font2);
        label_17 = new QLabel(groupBox_7);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(10, 73, 41, 16));
        label_17->setFont(font2);
        line_15 = new QFrame(groupBox_7);
        line_15->setObjectName("line_15");
        line_15->setGeometry(QRect(10, 60, 621, 21));
        line_15->setFrameShape(QFrame::HLine);
        line_15->setFrameShadow(QFrame::Sunken);
        line_20 = new QFrame(groupBox_7);
        line_20->setObjectName("line_20");
        line_20->setGeometry(QRect(10, 35, 621, 21));
        line_20->setFrameShape(QFrame::HLine);
        line_20->setFrameShadow(QFrame::Sunken);
        line_16 = new QFrame(groupBox_7);
        line_16->setObjectName("line_16");
        line_16->setGeometry(QRect(100, 20, 20, 70));
        line_16->setFrameShape(QFrame::VLine);
        line_16->setFrameShadow(QFrame::Sunken);
        label_10 = new QLabel(groupBox_7);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(130, 20, 71, 16));
        label_10->setFont(font2);
        line_21 = new QFrame(groupBox_7);
        line_21->setObjectName("line_21");
        line_21->setGeometry(QRect(200, 20, 20, 70));
        line_21->setFrameShape(QFrame::VLine);
        line_21->setFrameShadow(QFrame::Sunken);
        label_15 = new QLabel(groupBox_7);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(520, 20, 101, 16));
        label_15->setFont(font2);
        line_22 = new QFrame(groupBox_7);
        line_22->setObjectName("line_22");
        line_22->setGeometry(QRect(490, 20, 20, 70));
        line_22->setFrameShape(QFrame::VLine);
        line_22->setFrameShadow(QFrame::Sunken);
        label_14 = new QLabel(groupBox_7);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(380, 20, 91, 16));
        label_14->setFont(font2);
        label_16 = new QLabel(groupBox_7);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(250, 20, 71, 16));
        label_16->setFont(font2);
        line_23 = new QFrame(groupBox_7);
        line_23->setObjectName("line_23");
        line_23->setGeometry(QRect(340, 20, 20, 70));
        line_23->setFrameShape(QFrame::VLine);
        line_23->setFrameShadow(QFrame::Sunken);
        lblSmryInsSell = new QLabel(groupBox_7);
        lblSmryInsSell->setObjectName("lblSmryInsSell");
        lblSmryInsSell->setGeometry(QRect(130, 50, 71, 20));
        lblSmryInsSell->setFont(font2);
        lblSmryInsSell->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblSmryInsBuy = new QLabel(groupBox_7);
        lblSmryInsBuy->setObjectName("lblSmryInsBuy");
        lblSmryInsBuy->setGeometry(QRect(130, 70, 71, 20));
        lblSmryInsBuy->setFont(font2);
        lblSmryInsBuy->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblSmryOdrSell = new QLabel(groupBox_7);
        lblSmryOdrSell->setObjectName("lblSmryOdrSell");
        lblSmryOdrSell->setGeometry(QRect(270, 50, 71, 20));
        lblSmryOdrSell->setFont(font2);
        lblSmryOdrSell->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblSmryOdrBuy = new QLabel(groupBox_7);
        lblSmryOdrBuy->setObjectName("lblSmryOdrBuy");
        lblSmryOdrBuy->setGeometry(QRect(270, 70, 71, 20));
        lblSmryOdrBuy->setFont(font2);
        lblSmryOdrBuy->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblSmryExeSell = new QLabel(groupBox_7);
        lblSmryExeSell->setObjectName("lblSmryExeSell");
        lblSmryExeSell->setGeometry(QRect(420, 50, 71, 20));
        lblSmryExeSell->setFont(font2);
        lblSmryExeSell->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblSmryExeBuy = new QLabel(groupBox_7);
        lblSmryExeBuy->setObjectName("lblSmryExeBuy");
        lblSmryExeBuy->setGeometry(QRect(420, 70, 71, 20));
        lblSmryExeBuy->setFont(font2);
        lblSmryExeBuy->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblSmryExeAmtSell = new QLabel(groupBox_7);
        lblSmryExeAmtSell->setObjectName("lblSmryExeAmtSell");
        lblSmryExeAmtSell->setGeometry(QRect(550, 50, 71, 20));
        lblSmryExeAmtSell->setFont(font2);
        lblSmryExeAmtSell->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblSmryExeAmtBuy = new QLabel(groupBox_7);
        lblSmryExeAmtBuy->setObjectName("lblSmryExeAmtBuy");
        lblSmryExeAmtBuy->setGeometry(QRect(550, 70, 71, 20));
        lblSmryExeAmtBuy->setFont(font2);
        lblSmryExeAmtBuy->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_8 = new QGroupBox(tabMain);
        groupBox_8->setObjectName("groupBox_8");
        groupBox_8->setGeometry(QRect(10, 110, 471, 95));
        label_24 = new QLabel(groupBox_8);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(10, 48, 41, 16));
        label_24->setFont(font2);
        label_28 = new QLabel(groupBox_8);
        label_28->setObjectName("label_28");
        label_28->setGeometry(QRect(10, 73, 41, 16));
        label_28->setFont(font2);
        line_30 = new QFrame(groupBox_8);
        line_30->setObjectName("line_30");
        line_30->setGeometry(QRect(10, 60, 451, 21));
        line_30->setFrameShape(QFrame::HLine);
        line_30->setFrameShadow(QFrame::Sunken);
        line_31 = new QFrame(groupBox_8);
        line_31->setObjectName("line_31");
        line_31->setGeometry(QRect(10, 35, 451, 21));
        line_31->setFrameShape(QFrame::HLine);
        line_31->setFrameShadow(QFrame::Sunken);
        line_32 = new QFrame(groupBox_8);
        line_32->setObjectName("line_32");
        line_32->setGeometry(QRect(90, 20, 20, 70));
        line_32->setFrameShape(QFrame::VLine);
        line_32->setFrameShadow(QFrame::Sunken);
        label_29 = new QLabel(groupBox_8);
        label_29->setObjectName("label_29");
        label_29->setGeometry(QRect(110, 20, 101, 16));
        label_29->setFont(font2);
        label_30 = new QLabel(groupBox_8);
        label_30->setObjectName("label_30");
        label_30->setGeometry(QRect(380, 23, 101, 16));
        label_30->setFont(font2);
        line_34 = new QFrame(groupBox_8);
        line_34->setObjectName("line_34");
        line_34->setGeometry(QRect(330, 20, 20, 70));
        line_34->setFrameShape(QFrame::VLine);
        line_34->setFrameShadow(QFrame::Sunken);
        label_31 = new QLabel(groupBox_8);
        label_31->setObjectName("label_31");
        label_31->setGeometry(QRect(250, 20, 91, 16));
        label_31->setFont(font2);
        line_35 = new QFrame(groupBox_8);
        line_35->setObjectName("line_35");
        line_35->setGeometry(QRect(210, 20, 20, 70));
        line_35->setFrameShape(QFrame::VLine);
        line_35->setFrameShadow(QFrame::Sunken);
        lblOdrLiveInsSell = new QLabel(groupBox_8);
        lblOdrLiveInsSell->setObjectName("lblOdrLiveInsSell");
        lblOdrLiveInsSell->setGeometry(QRect(140, 50, 71, 20));
        lblOdrLiveInsSell->setFont(font2);
        lblOdrLiveInsSell->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblOdrLiveInsBuy = new QLabel(groupBox_8);
        lblOdrLiveInsBuy->setObjectName("lblOdrLiveInsBuy");
        lblOdrLiveInsBuy->setGeometry(QRect(140, 70, 71, 20));
        lblOdrLiveInsBuy->setFont(font2);
        lblOdrLiveInsBuy->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblOdrLiveSell = new QLabel(groupBox_8);
        lblOdrLiveSell->setObjectName("lblOdrLiveSell");
        lblOdrLiveSell->setGeometry(QRect(260, 50, 71, 20));
        lblOdrLiveSell->setFont(font2);
        lblOdrLiveSell->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblOdrLiveBuy = new QLabel(groupBox_8);
        lblOdrLiveBuy->setObjectName("lblOdrLiveBuy");
        lblOdrLiveBuy->setGeometry(QRect(260, 70, 71, 20));
        lblOdrLiveBuy->setFont(font2);
        lblOdrLiveBuy->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblOdrLiveAmtSell = new QLabel(groupBox_8);
        lblOdrLiveAmtSell->setObjectName("lblOdrLiveAmtSell");
        lblOdrLiveAmtSell->setGeometry(QRect(390, 50, 71, 20));
        lblOdrLiveAmtSell->setFont(font2);
        lblOdrLiveAmtSell->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblOdrLiveAmtBuy = new QLabel(groupBox_8);
        lblOdrLiveAmtBuy->setObjectName("lblOdrLiveAmtBuy");
        lblOdrLiveAmtBuy->setGeometry(QRect(390, 70, 71, 20));
        lblOdrLiveAmtBuy->setFont(font2);
        lblOdrLiveAmtBuy->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        tabWidget->addTab(tabMain, QString());
        tabPair = new QWidget();
        tabPair->setObjectName("tabPair");
        line = new QFrame(tabPair);
        line->setObjectName("line");
        line->setGeometry(QRect(3, 20, 20, 621));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(tabPair);
        line_2->setObjectName("line_2");
        line_2->setGeometry(QRect(730, 20, 20, 1241));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(tabPair);
        line_3->setObjectName("line_3");
        line_3->setGeometry(QRect(20, 40, 1241, 16));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_4 = new QFrame(tabPair);
        line_4->setObjectName("line_4");
        line_4->setGeometry(QRect(460, 10, 20, 631));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);
        label = new QLabel(tabPair);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 20, 91, 16));
        label_2 = new QLabel(tabPair);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(110, 20, 121, 16));
        label_3 = new QLabel(tabPair);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(210, 20, 91, 16));
        label_4 = new QLabel(tabPair);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(300, 20, 141, 20));
        label_5 = new QLabel(tabPair);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(400, 20, 131, 20));
        label_6 = new QLabel(tabPair);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(480, 20, 141, 20));
        label_6->setFont(font);
        label_7 = new QLabel(tabPair);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(580, 20, 141, 16));
        label_7->setFont(font);
        label_8 = new QLabel(tabPair);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(680, 20, 161, 16));
        label_8->setFont(font);
        line_5 = new QFrame(tabPair);
        line_5->setObjectName("line_5");
        line_5->setGeometry(QRect(90, 20, 20, 621));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);
        line_6 = new QFrame(tabPair);
        line_6->setObjectName("line_6");
        line_6->setGeometry(QRect(560, 10, 20, 631));
        line_6->setFrameShape(QFrame::VLine);
        line_6->setFrameShadow(QFrame::Sunken);
        line_7 = new QFrame(tabPair);
        line_7->setObjectName("line_7");
        line_7->setGeometry(QRect(180, 20, 20, 621));
        line_7->setFrameShape(QFrame::VLine);
        line_7->setFrameShadow(QFrame::Sunken);
        line_8 = new QFrame(tabPair);
        line_8->setObjectName("line_8");
        line_8->setGeometry(QRect(380, 10, 20, 631));
        line_8->setFrameShape(QFrame::VLine);
        line_8->setFrameShadow(QFrame::Sunken);
        line_9 = new QFrame(tabPair);
        line_9->setObjectName("line_9");
        line_9->setGeometry(QRect(270, 10, 20, 631));
        line_9->setFrameShape(QFrame::VLine);
        line_9->setFrameShadow(QFrame::Sunken);
        line_10 = new QFrame(tabPair);
        line_10->setObjectName("line_10");
        line_10->setGeometry(QRect(650, 10, 20, 631));
        line_10->setFrameShape(QFrame::VLine);
        line_10->setFrameShadow(QFrame::Sunken);
        tabWidget->addTab(tabPair, QString());
        tabInstrument = new QWidget();
        tabInstrument->setObjectName("tabInstrument");
        BoxInstruments = new QComboBox(tabInstrument);
        BoxInstruments->setObjectName("BoxInstruments");
        BoxInstruments->setGeometry(QRect(10, 10, 221, 22));
        lblSymbol = new QLabel(tabInstrument);
        lblSymbol->setObjectName("lblSymbol");
        lblSymbol->setEnabled(false);
        lblSymbol->setGeometry(QRect(10, 40, 191, 19));
        QFont font3;
        font3.setPointSize(14);
        font3.setBold(true);
        lblSymbol->setFont(font3);
        groupBox_2 = new QGroupBox(tabInstrument);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setEnabled(false);
        groupBox_2->setGeometry(QRect(417, 9, 211, 181));
        line_11 = new QFrame(groupBox_2);
        line_11->setObjectName("line_11");
        line_11->setGeometry(QRect(60, 10, 20, 161));
        line_11->setFrameShape(QFrame::VLine);
        line_11->setFrameShadow(QFrame::Sunken);
        line_12 = new QFrame(groupBox_2);
        line_12->setObjectName("line_12");
        line_12->setGeometry(QRect(130, 10, 20, 161));
        line_12->setFrameShape(QFrame::VLine);
        line_12->setFrameShadow(QFrame::Sunken);
        lblBidPr1 = new QLabel(groupBox_2);
        lblBidPr1->setObjectName("lblBidPr1");
        lblBidPr1->setEnabled(false);
        lblBidPr1->setGeometry(QRect(80, 100, 50, 12));
        lblBidPr2 = new QLabel(groupBox_2);
        lblBidPr2->setObjectName("lblBidPr2");
        lblBidPr2->setEnabled(false);
        lblBidPr2->setGeometry(QRect(80, 120, 50, 12));
        lblBidPr3 = new QLabel(groupBox_2);
        lblBidPr3->setObjectName("lblBidPr3");
        lblBidPr3->setEnabled(false);
        lblBidPr3->setGeometry(QRect(80, 140, 50, 12));
        lblBidPr4 = new QLabel(groupBox_2);
        lblBidPr4->setObjectName("lblBidPr4");
        lblBidPr4->setEnabled(false);
        lblBidPr4->setGeometry(QRect(80, 160, 50, 12));
        lblAskPr1 = new QLabel(groupBox_2);
        lblAskPr1->setObjectName("lblAskPr1");
        lblAskPr1->setEnabled(false);
        lblAskPr1->setGeometry(QRect(80, 80, 50, 12));
        lblAskPr2 = new QLabel(groupBox_2);
        lblAskPr2->setObjectName("lblAskPr2");
        lblAskPr2->setEnabled(false);
        lblAskPr2->setGeometry(QRect(80, 60, 50, 12));
        lblAskPr3 = new QLabel(groupBox_2);
        lblAskPr3->setObjectName("lblAskPr3");
        lblAskPr3->setEnabled(false);
        lblAskPr3->setGeometry(QRect(80, 40, 50, 12));
        lblAskPr4 = new QLabel(groupBox_2);
        lblAskPr4->setObjectName("lblAskPr4");
        lblAskPr4->setEnabled(false);
        lblAskPr4->setGeometry(QRect(80, 20, 50, 12));
        lblBidQty1 = new QLabel(groupBox_2);
        lblBidQty1->setObjectName("lblBidQty1");
        lblBidQty1->setGeometry(QRect(150, 100, 50, 12));
        lblBidQty2 = new QLabel(groupBox_2);
        lblBidQty2->setObjectName("lblBidQty2");
        lblBidQty2->setGeometry(QRect(150, 120, 50, 12));
        lblBidQty3 = new QLabel(groupBox_2);
        lblBidQty3->setObjectName("lblBidQty3");
        lblBidQty3->setGeometry(QRect(150, 140, 50, 12));
        lblBidQty4 = new QLabel(groupBox_2);
        lblBidQty4->setObjectName("lblBidQty4");
        lblBidQty4->setGeometry(QRect(150, 160, 50, 12));
        lblAskQty1 = new QLabel(groupBox_2);
        lblAskQty1->setObjectName("lblAskQty1");
        lblAskQty1->setEnabled(false);
        lblAskQty1->setGeometry(QRect(10, 80, 50, 12));
        lblAskQty2 = new QLabel(groupBox_2);
        lblAskQty2->setObjectName("lblAskQty2");
        lblAskQty2->setEnabled(false);
        lblAskQty2->setGeometry(QRect(10, 60, 50, 12));
        lblAskQty4 = new QLabel(groupBox_2);
        lblAskQty4->setObjectName("lblAskQty4");
        lblAskQty4->setEnabled(false);
        lblAskQty4->setGeometry(QRect(10, 20, 50, 12));
        lblAskQty3 = new QLabel(groupBox_2);
        lblAskQty3->setObjectName("lblAskQty3");
        lblAskQty3->setEnabled(false);
        lblAskQty3->setGeometry(QRect(10, 40, 50, 12));
        tableOrders = new QTableView(tabInstrument);
        tableOrders->setObjectName("tableOrders");
        tableOrders->setEnabled(false);
        tableOrders->setGeometry(QRect(0, 340, 661, 291));
        groupBox_3 = new QGroupBox(tabInstrument);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(460, 200, 171, 131));
        chkBoxEnableTrade = new QCheckBox(groupBox_3);
        chkBoxEnableTrade->setObjectName("chkBoxEnableTrade");
        chkBoxEnableTrade->setGeometry(QRect(20, 30, 111, 18));
        chkBoxNewOrder = new QCheckBox(groupBox_3);
        chkBoxNewOrder->setObjectName("chkBoxNewOrder");
        chkBoxNewOrder->setGeometry(QRect(40, 50, 111, 18));
        chkBoxCanMod = new QCheckBox(groupBox_3);
        chkBoxCanMod->setObjectName("chkBoxCanMod");
        chkBoxCanMod->setGeometry(QRect(40, 70, 111, 18));
        pushButtonStopTradeSymbol = new QPushButton(groupBox_3);
        pushButtonStopTradeSymbol->setObjectName("pushButtonStopTradeSymbol");
        pushButtonStopTradeSymbol->setGeometry(QRect(10, 90, 131, 21));
        groupBox_4 = new QGroupBox(tabInstrument);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setEnabled(false);
        groupBox_4->setGeometry(QRect(160, 240, 211, 81));
        label_25 = new QLabel(groupBox_4);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(20, 20, 50, 12));
        label_26 = new QLabel(groupBox_4);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(20, 40, 50, 12));
        label_27 = new QLabel(groupBox_4);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(20, 60, 50, 12));
        lblOpen = new QLabel(groupBox_4);
        lblOpen->setObjectName("lblOpen");
        lblOpen->setGeometry(QRect(70, 20, 50, 12));
        lblHigh = new QLabel(groupBox_4);
        lblHigh->setObjectName("lblHigh");
        lblHigh->setGeometry(QRect(70, 40, 50, 12));
        lblLow = new QLabel(groupBox_4);
        lblLow->setObjectName("lblLow");
        lblLow->setGeometry(QRect(70, 60, 50, 12));
        lblTime = new QLabel(tabInstrument);
        lblTime->setObjectName("lblTime");
        lblTime->setEnabled(false);
        lblTime->setGeometry(QRect(220, 40, 81, 16));
        tabWidget->addTab(tabInstrument, QString());
        tabSetting = new QWidget();
        tabSetting->setObjectName("tabSetting");
        tabWidget->addTab(tabSetting, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        testPushbutton = new QPushButton(tab);
        testPushbutton->setObjectName("testPushbutton");
        testPushbutton->setGeometry(QRect(420, 130, 181, 41));
        testTextEdit = new QPlainTextEdit(tab);
        testTextEdit->setObjectName("testTextEdit");
        testTextEdit->setGeometry(QRect(460, 40, 151, 31));
        testTextEdit_2 = new QPlainTextEdit(tab);
        testTextEdit_2->setObjectName("testTextEdit_2");
        testTextEdit_2->setGeometry(QRect(460, 80, 151, 31));
        BTLogTextEdit = new QPlainTextEdit(tab);
        BTLogTextEdit->setObjectName("BTLogTextEdit");
        BTLogTextEdit->setEnabled(false);
        BTLogTextEdit->setGeometry(QRect(0, 240, 660, 340));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Calibri")});
        font4.setPointSize(8);
        font4.setBold(true);
        BTLogTextEdit->setFont(font4);
        tabWidget->addTab(tab, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 675, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(testPushbutton, SIGNAL(clicked()), MainWindow, SLOT(on_testButton_clicked()));
        QObject::connect(BoxInstruments, SIGNAL(currentTextChanged(QString)), MainWindow, SLOT(on_BoxInstruments_currentTextChanged()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QString());
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "Feed Info", nullptr));
        label_32->setText(QCoreApplication::translate("MainWindow", "All", nullptr));
        label_33->setText(QCoreApplication::translate("MainWindow", "Increment", nullptr));
        label_34->setText(QCoreApplication::translate("MainWindow", "Queue", nullptr));
        lblFeedAll->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblFeedInc->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblFeedQueue->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "Summary", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Sell", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Buy", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Instruments", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Executed Amount", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Executed Orders", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "# of Orders", nullptr));
        lblSmryInsSell->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblSmryInsBuy->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblSmryOdrSell->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblSmryOdrBuy->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblSmryExeSell->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblSmryExeBuy->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblSmryExeAmtSell->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblSmryExeAmtBuy->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("MainWindow", "Orders", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "Sell", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "Buy", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "Live Instruments", nullptr));
        label_30->setText(QCoreApplication::translate("MainWindow", "Live Amount", nullptr));
        label_31->setText(QCoreApplication::translate("MainWindow", "Live Orders", nullptr));
        lblOdrLiveInsSell->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblOdrLiveInsBuy->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblOdrLiveSell->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblOdrLiveBuy->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblOdrLiveAmtSell->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblOdrLiveAmtBuy->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMain), QCoreApplication::translate("MainWindow", "Main", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Symbol", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Ask Amt", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Ask Qty", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Ask AvgPrice", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "MidPrice", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Bid AvgPrice", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Bid Qty", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Bid Amt", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabPair), QCoreApplication::translate("MainWindow", "Pairs", nullptr));
        lblSymbol->setText(QCoreApplication::translate("MainWindow", "Symbol", nullptr));
        groupBox_2->setTitle(QString());
        lblBidPr1->setText(QCoreApplication::translate("MainWindow", "BPrice1", nullptr));
        lblBidPr2->setText(QCoreApplication::translate("MainWindow", "BPrice2", nullptr));
        lblBidPr3->setText(QCoreApplication::translate("MainWindow", "BPrice3", nullptr));
        lblBidPr4->setText(QCoreApplication::translate("MainWindow", "BPrice4", nullptr));
        lblAskPr1->setText(QCoreApplication::translate("MainWindow", "APrice1", nullptr));
        lblAskPr2->setText(QCoreApplication::translate("MainWindow", "APrice2", nullptr));
        lblAskPr3->setText(QCoreApplication::translate("MainWindow", "APrice3", nullptr));
        lblAskPr4->setText(QCoreApplication::translate("MainWindow", "APrice4", nullptr));
        lblBidQty1->setText(QCoreApplication::translate("MainWindow", "BQty1", nullptr));
        lblBidQty2->setText(QCoreApplication::translate("MainWindow", "BQty2", nullptr));
        lblBidQty3->setText(QCoreApplication::translate("MainWindow", "BQty3", nullptr));
        lblBidQty4->setText(QCoreApplication::translate("MainWindow", "BQty4", nullptr));
        lblAskQty1->setText(QCoreApplication::translate("MainWindow", "AQty1", nullptr));
        lblAskQty2->setText(QCoreApplication::translate("MainWindow", "AQty2", nullptr));
        lblAskQty4->setText(QCoreApplication::translate("MainWindow", "AQty4", nullptr));
        lblAskQty3->setText(QCoreApplication::translate("MainWindow", "AQty3", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Status", nullptr));
        chkBoxEnableTrade->setText(QCoreApplication::translate("MainWindow", "Enable Trade", nullptr));
        chkBoxNewOrder->setText(QCoreApplication::translate("MainWindow", "New Order", nullptr));
        chkBoxCanMod->setText(QCoreApplication::translate("MainWindow", "Can / Mod Order", nullptr));
        pushButtonStopTradeSymbol->setText(QCoreApplication::translate("MainWindow", "Stop Trading", nullptr));
        groupBox_4->setTitle(QString());
        label_25->setText(QCoreApplication::translate("MainWindow", "Open :", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "High :", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "Low :", nullptr));
        lblOpen->setText(QCoreApplication::translate("MainWindow", "Price", nullptr));
        lblHigh->setText(QCoreApplication::translate("MainWindow", "Price", nullptr));
        lblLow->setText(QCoreApplication::translate("MainWindow", "Price", nullptr));
        lblTime->setText(QCoreApplication::translate("MainWindow", "Time", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabInstrument), QCoreApplication::translate("MainWindow", "Symbol", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSetting), QCoreApplication::translate("MainWindow", "Setting", nullptr));
        testPushbutton->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        testTextEdit->setPlainText(QString());
        testTextEdit_2->setPlainText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Backtest", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
