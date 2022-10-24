#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/qscrollbar.h>
#include <qtimer.h>
#include <QtWidgets/qabstractscrollarea.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qtablewidget.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    struct PairInfo
    {
        QLabel* Symbol;
        QLabel* SellAmt;
        QLabel* SellPr;
        QLabel* Spread;
        QLabel* BuyPr;
        QLabel* BuyAmt;
        PairInfo(QLabel* symbol,QLabel* sellamt,
            QLabel* sellpr,QLabel* spread,QLabel* buypr,QLabel* buyamt) :Symbol(symbol), SellAmt(sellamt), SellPr(sellpr), Spread(spread), BuyPr(buypr), BuyAmt(buyamt) {};
        PairInfo() {};
        ~PairInfo() {};
        void Clear(void);
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void WriteLog(void);
    void UpdateSettingTab(void);
    void UpdatePairsTab(void);
    

    int PrevFeedCount;
    bool ProcedureStarted;

    void InitializeObjects(void);
    void SetNewDate(int date);

    void InitilizeInsBox(void);
    void SetInstrunemt(std::string instId);

    void test(void);

private:
    Ui::MainWindow *ui;
    QTimer* Displaytimer;
    std::vector<std::pair<QLabel*, QLabel*>> Asks;
    std::vector<std::pair<QLabel*, QLabel*>> Bids;
    std::vector<PairInfo>* PairsInfo;
    QString LogText;

    int UpdateCount;
    bool AddColor;

    QLabel* stbarDate;
    QLabel* stbarMsg;
    QLabel* stbarStatus;
    QLabel* stbarTime;

    QString MainWindowStyle;
    QString MenuBarStyle;
    QString MenuStyle;
    QString MainTabStyle;
    QString TabBarStyle;
    QString ScrollAreaStyle;
    QString TableStyle;
    QString msgBoxStyle;
    QString GeneralButtonStyle;
    QString StatusButtonStyle;
    QString LockButtonStyle;
    QString Stripes;
    QString StatusBarStyle;

    QFont GeneralLabelFont;
    QFont GeneralButtonFont;

    QPalette TablePalette;
    QPalette ComboBoxPalette;
    QPalette Green;
    QPalette Red;

    void DefineStyleSheet(void);
    void SetStyle(void);

private slots:
    void on_testButton_clicked(void);
    void on_BoxInstruments_currentTextChanged(const QString& arg1);
    void UpdateDisplay(void);
};
#endif // MAINWINDOW_H