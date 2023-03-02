/*
Copyright (C) 2022 Paolo Gaboardi

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3.0 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <QDesktopServices>
#include <QScreen>
#include <QSize>
#include <QQuickItem>
#include "widgetgetto.h"
#include "widget_stato.h"
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <qscroller.h>
#endif

#define SOFTWARE_VERSION "2.2"

/**
  revisioni
  0.1   03-10-18    Prima versione con base grafica da ECU app
  0.2   15-10-18    Attivazione schermata Stato e Programmazione
  0.3   15-11-18    Modifiche viste in fiera
  0.4   05-12-18    Aggiunta gestione metri di rientro per settori
  1.0   21-01-19    Modifiche grafica su Iphone e pubblicazione su Play Store
  1.01  26-02-19    Limitazione inserimento angoli (minimo settore 15 gradi e massimo 360-15 gradi)
  1.02  27-08-19    Resa possibile modifica con angoli a zero
  2.0   03-12-19    MQTT + WIFI
  2.1   11-06-21    Bordo Campo
  2.2   10-11-21    Modifiche Bordo Campo

TODO:

 * Costanti QT per i vari sistemi operativi:
    * Q_OS_ANDROID
    * Q_OS_CYGWIN
    * Q_OS_FREEBSD
    * Q_OS_IOS
    * Q_OS_LINUX
    * Q_OS_MAC
    * Q_OS_OSX
    * Q_OS_QNX
    * Q_OS_UNIX
    * Q_OS_WIN32
    * Q_OS_WIN64
    * Q_OS_WIN
    * Q_OS_WINCE
    * Q_OS_WINPHONE
    * Q_OS_WINRT

********************************************************
 Main
*******************************************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->frame_Mappa->setVisible(false);

    connect(qApp,  SIGNAL(applicationStateChanged(Qt::ApplicationState)),this, SLOT(stateChanged(Qt::ApplicationState)));

    connect(&tcpClient, SIGNAL(readyRead()),this, SLOT(IncomingData()));
    connect(&tcpClient, SIGNAL(connected()),this, SLOT(TCPConnesso()));
    connect(&tcpClient, SIGNAL(disconnected()),this, SLOT(TCPDisconnesso()));

    connect( timerInitTCP, SIGNAL(timeout()), this, SLOT( TCPDisconnesso() ));
    timerInitTCP->setInterval( 10000 );

    connect( timerDatoTCP, SIGNAL(timeout()), this, SLOT( TCPDisconnesso() ));
    timerDatoTCP->setInterval( 30000 );

//    connect( timerKeepAlive, SIGNAL(timeout()), this, SLOT( InvioKeepAlive() ));
//    timerKeepAlive->setInterval( 30000 );

    connect( timerHTML, SIGNAL(timeout()), this, SLOT( HTMLTimeout() ));
    timerHTML->setInterval( 10000 );

    // Segnali widget stato
    connect(ui->W_Stato, SIGNAL(update_stato()),this, SLOT(UpdateStato()));
    connect(ui->W_Stato, SIGNAL(change_prog()),this, SLOT(ChangeProg()));
    connect(ui->W_Stato, SIGNAL(change_manaut()),this, SLOT(ChangeManAut()));
    connect(ui->W_Stato, SIGNAL(reverse_save()),this, SLOT(ReverseSave()));
    connect(ui->W_Stato, SIGNAL(reverse_notsave()),this, SLOT(ReverseNotSave()));
    connect(ui->W_Stato, SIGNAL(pagina_rompigetto()),this, SLOT(PaginaRompigetto()));
    connect(ui->W_Stato, SIGNAL(pagina_auto()),this, SLOT(PaginaAuto()));

    // Segnali widget prog
    connect(ui->W_Prog, SIGNAL(update_prog()),this, SLOT(UpdateProg()));
    connect(this, SIGNAL(default_page_prog()),ui->W_Prog, SLOT(Default_Page()));

    // Segnali widget bordo
    connect(ui->W_Bordo, SIGNAL(update_bordo()),this, SLOT(UpdateBordo()));
    connect(ui->W_Bordo, SIGNAL(update_dati()),this, SLOT(UpdateDati()));

    // Caricamento widget mappa (pagina vuota)
    Mappa= new MapWidget("", "", "", "", false, 0, 0, 0, 0, 0, 0, 0, 0, ui->W_Bordo->ListaGPS_Np0, ui->W_Bordo->ListaGPS_Np0, ui->W_Bordo->ListaKML);
    //set layout programatically
    ui->page_Mappa->layout()->addWidget(Mappa);

    ContatoreResize=0;

    ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Elenco));


    ui->label_Titolo->setText(tr("Elektrorain ")+tr(SOFTWARE_VERSION));
    ui->pushButton_Settings->setFocus();

#ifdef Q_OS_ANDROID
    keep_screen_on(true);
#endif

#ifdef Q_OS_ANDROID
    //definizione scroller
    QScroller* scroller=QScroller::scroller(ui->listWidget_Getti);
    scroller->grabGesture(ui->listWidget_Getti,QScroller::LeftMouseButtonGesture);
    QScrollerProperties ActualProperties;
    ActualProperties=scroller->scrollerProperties();
    QVariant VerticalOvershoot=QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    ActualProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, VerticalOvershoot);
    scroller->setScrollerProperties(ActualProperties);
#endif

    //---------- FILE INI contenente la lista getti -------------------------------------------------------------------------
    //Setta la cartella di lavoro
    QString   WorkingDirectory;
    WorkingDirectory=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/DATI"));

    qDebug() << "Cartella settings:" << WorkingDirectory;

    ProjectPath.setPath(WorkingDirectory);

    if (!ProjectPath.exists()) {
        ProjectPath.mkpath(WorkingDirectory);
    }

    QFile FileIni(ProjectPath.absolutePath()+QString("/Elenco.ini"));

    if (FileIni.exists()){
       QSettings settings (ProjectPath.absolutePath()+QString("/Elenco.ini"), QSettings::IniFormat);
       settings.beginGroup("ElencoGetti");
       ui->lineEdit_Nome1->setText(settings.value("Nome1").toString());
       ui->lineEdit_Nome2->setText(settings.value("Nome2").toString());
       ui->lineEdit_Nome3->setText(settings.value("Nome3").toString());
       ui->lineEdit_Nome4->setText(settings.value("Nome4").toString());
       ui->lineEdit_Nome5->setText(settings.value("Nome5").toString());
       ui->lineEdit_Nome6->setText(settings.value("Nome6").toString());
       ui->lineEdit_Nome7->setText(settings.value("Nome7").toString());
       ui->lineEdit_Nome8->setText(settings.value("Nome8").toString());
       ui->lineEdit_Nome9->setText(settings.value("Nome9").toString());
       ui->lineEdit_Nome10->setText(settings.value("Nome10").toString());
       ui->lineEdit_ID1->setText(settings.value("IP1").toString());
       ui->lineEdit_ID2->setText(settings.value("IP2").toString());
       ui->lineEdit_ID3->setText(settings.value("IP3").toString());
       ui->lineEdit_ID4->setText(settings.value("IP4").toString());
       ui->lineEdit_ID5->setText(settings.value("IP5").toString());
       ui->lineEdit_ID6->setText(settings.value("IP6").toString());
       ui->lineEdit_ID7->setText(settings.value("IP7").toString());
       ui->lineEdit_ID8->setText(settings.value("IP8").toString());
       ui->lineEdit_ID9->setText(settings.value("IP9").toString());
       ui->lineEdit_ID10->setText(settings.value("IP10").toString());
       settings.endGroup();
//    }else{
//        ui->stackedWidget_Core->setCurrentIndex(ui->stackedWidget_Core->indexOf(ui->page_Core));
//        emit ui->pushButton_Settings->click();
    }
    //-----------------------------------------------------------------------------------------------------------------------

    for(int i=0; i<10; i++) PaginaBonus[i]=false;

    Crea_Lista_Getti();
    InvioHTTP(StepHTTP);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event){

    QMainWindow::resizeEvent(event);

#ifdef Q_OS_ANDROID
    if (ContatoreResize==1){

    //ridimensiona tasto logo
        QSize IconSizes;
        IconSizes.setHeight(ui->pushButton_Logo->height()*0.9);
        IconSizes.setWidth(ui->pushButton_Logo->width()*0.9);
        ui->pushButton_Logo->setIconSize(IconSizes);

    //ridimensiona tasto wifi/dati
        IconSizes.setHeight(ui->pushButton_ConnType->height()*0.95);
        IconSizes.setWidth(ui->pushButton_ConnType->height()*0.95);
        ui->pushButton_ConnType->setIconSize(IconSizes);

    //ridimensiona tasto settings
        int Height;
        Height=ui->pushButton_Settings->height();
        ui->pushButton_Settings->setFixedWidth(Height);
        IconSizes.setHeight(ui->pushButton_Settings->height()*0.9);
        IconSizes.setWidth(ui->pushButton_Settings->height()*0.9);
        ui->pushButton_Settings->setIconSize(IconSizes);

        int Width;
        Width=ui->pushButton_Elenco->width();
        ui->pushButton_Elenco->setFixedHeight((int)(0.8*Width));
        ui->pushButton_Elenco->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
        ui->pushButton_Stato->setFixedHeight((int)(0.8*Width));
        ui->pushButton_Stato->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
        ui->pushButton_Prog->setFixedHeight((int)(0.8*Width));
        ui->pushButton_Prog->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
        ui->pushButton_Mappa->setFixedHeight((int)(0.8*Width));
        ui->pushButton_Mappa->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));

        IconSizes.setHeight(ui->pushButton_Indietro->height()*0.9);
        IconSizes.setWidth(ui->pushButton_Indietro->width()*0.9);
        ui->pushButton_Indietro->setIconSize(IconSizes);
        IconSizes.setHeight(ui->pushButton_Info->height()*0.9);
        IconSizes.setWidth(ui->pushButton_Info->width()*0.9);
        ui->pushButton_Info->setIconSize(IconSizes);

        Refresh_Lista_Getti();
    }
#endif

#ifdef Q_OS_IOS
    if (ContatoreResize==2){

    //ridimensiona tasto logo
        QSize IconSizes;
        IconSizes.setHeight((int)(ui->pushButton_Logo->height()*0.9));
        IconSizes.setWidth((int)(ui->pushButton_Logo->width()*0.9));
        ui->pushButton_Logo->setIconSize(IconSizes);

     //ridimensiona tasto wifi/dati
        IconSizes.setHeight(ui->pushButton_ConnType->height()*0.95);
        IconSizes.setWidth(ui->pushButton_ConnType->height()*0.95);
        ui->pushButton_ConnType->setIconSize(IconSizes);

    //ridimensiona tasto settings
        setFixedWidth(ui->pushButton_Settings->height());
 //       ui->pushButton_User->setGeometry(0,0,ui->pushButton_User->height(),ui->pushButton_User->height());
        IconSizes.setHeight((int)(ui->pushButton_Settings->height()*0.9));
        IconSizes.setWidth((int)(ui->pushButton_Settings->height()*0.9));
        ui->pushButton_Settings->setIconSize(IconSizes);

        int Width;
        Width=ui->pushButton_Elenco->width();
        ui->pushButton_Elenco->setFixedHeight((int)(0.8*Width));
        ui->pushButton_Elenco->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
        ui->pushButton_Stato->setFixedHeight((int)(0.8*Width));
        ui->pushButton_Stato->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
        ui->pushButton_Prog->setFixedHeight((int)(0.8*Width));
        ui->pushButton_Prog->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));
        ui->pushButton_Mappa->setFixedHeight((int)(0.8*Width));
        ui->pushButton_Mappa->setIconSize(QSize((int)(0.7*Width),(int)(0.7*Width)));

        Refresh_Lista_Getti();
    }
#endif

if (ContatoreResize<10) ContatoreResize++;
}

void MainWindow::on_pushButton_ConnType_clicked()
{
    if(MQTTprotocol){
        MQTTprotocol=false;
        QPixmap Img=QPixmap(tr(":/Resources/wifi.png"));
        ui->pushButton_ConnType->setIcon(Img);
    }
    else {
        MQTTprotocol=true;
        QPixmap Img=QPixmap(tr(":/Resources/mobile_data.png"));
        ui->pushButton_ConnType->setIcon(Img);
    }
}

void MainWindow::on_pushButton_Settings_clicked()
{
    // Da Core a Settings
    if(ui->stackedWidget_Core->currentIndex()==ui->stackedWidget_Core->indexOf(ui->page_Core)){
        ui->stackedWidget_Core->setCurrentIndex(ui->stackedWidget_Core->indexOf(ui->page_Settings));
        QPixmap Img=QPixmap(tr(":/Resources/Check.png"));
        ui->pushButton_Settings->setIcon(Img);
    }
    // Da Settings a Core
    else if(ui->stackedWidget_Core->currentIndex()==ui->stackedWidget_Core->indexOf(ui->page_Settings)){
        QSettings settings (ProjectPath.absolutePath()+QString("/Elenco.ini"), QSettings::IniFormat);
        settings.beginGroup("ElencoGetti");
        settings.setValue("Nome1",ui->lineEdit_Nome1->text());
        settings.setValue("Nome2",ui->lineEdit_Nome2->text());
        settings.setValue("Nome3",ui->lineEdit_Nome3->text());
        settings.setValue("Nome4",ui->lineEdit_Nome4->text());
        settings.setValue("Nome5",ui->lineEdit_Nome5->text());
        settings.setValue("Nome6",ui->lineEdit_Nome6->text());
        settings.setValue("Nome7",ui->lineEdit_Nome7->text());
        settings.setValue("Nome8",ui->lineEdit_Nome8->text());
        settings.setValue("Nome9",ui->lineEdit_Nome9->text());
        settings.setValue("Nome10",ui->lineEdit_Nome10->text());
        settings.setValue("IP1",ui->lineEdit_ID1->text());
        settings.setValue("IP2",ui->lineEdit_ID2->text());
        settings.setValue("IP3",ui->lineEdit_ID3->text());
        settings.setValue("IP4",ui->lineEdit_ID4->text());
        settings.setValue("IP5",ui->lineEdit_ID5->text());
        settings.setValue("IP6",ui->lineEdit_ID6->text());
        settings.setValue("IP7",ui->lineEdit_ID7->text());
        settings.setValue("IP8",ui->lineEdit_ID8->text());
        settings.setValue("IP9",ui->lineEdit_ID9->text());
        settings.setValue("IP10",ui->lineEdit_ID10->text());
        settings.endGroup();

        Crea_Lista_Getti();
        InvioHTTP(StepHTTP);
        Refresh_Lista_Getti();

        ui->stackedWidget_Core->setCurrentIndex(ui->stackedWidget_Core->indexOf(ui->page_Core));
        QPixmap Img=QPixmap(tr(":/Resources/Settings.png"));
        ui->pushButton_Settings->setIcon(Img);
    }

}

void MainWindow::Crea_Lista_Getti(void){

    ListaGetti.clear();

    // Aggiungi gli elementi della lista
    if(ui->lineEdit_ID1->text()!=""){
        Getto_t* NewGetto=new Getto_t;
        NewGetto->Nome=ui->lineEdit_Nome1->text();
        NewGetto->NumeroID=ui->lineEdit_ID1->text();
        NewGetto->Status=OFF_LINE;
        ListaGetti.append(NewGetto);
    }
    if(ui->lineEdit_ID2->text()!=""){
        Getto_t* NewGetto=new Getto_t;
        NewGetto->Nome=ui->lineEdit_Nome2->text();
        NewGetto->NumeroID=ui->lineEdit_ID2->text();
        NewGetto->Status=OFF_LINE;
        ListaGetti.append(NewGetto);
    }
    if(ui->lineEdit_ID3->text()!=""){
        Getto_t* NewGetto=new Getto_t;
        NewGetto->Nome=ui->lineEdit_Nome3->text();
        NewGetto->NumeroID=ui->lineEdit_ID3->text();
        NewGetto->Status=OFF_LINE;
        ListaGetti.append(NewGetto);
    }
    if(ui->lineEdit_ID4->text()!=""){
        Getto_t* NewGetto=new Getto_t;
        NewGetto->Nome=ui->lineEdit_Nome4->text();
        NewGetto->NumeroID=ui->lineEdit_ID4->text();
        NewGetto->Status=OFF_LINE;
        ListaGetti.append(NewGetto);
    }
    if(ui->lineEdit_ID5->text()!=""){
        Getto_t* NewGetto=new Getto_t;
        NewGetto->Nome=ui->lineEdit_Nome5->text();
        NewGetto->NumeroID=ui->lineEdit_ID5->text();
        NewGetto->Status=OFF_LINE;
        ListaGetti.append(NewGetto);
    }
    if(ui->lineEdit_ID6->text()!=""){
        Getto_t* NewGetto=new Getto_t;
        NewGetto->Nome=ui->lineEdit_Nome6->text();
        NewGetto->NumeroID=ui->lineEdit_ID6->text();
        NewGetto->Status=OFF_LINE;
        ListaGetti.append(NewGetto);
    }
    if(ui->lineEdit_ID7->text()!=""){
        Getto_t* NewGetto=new Getto_t;
        NewGetto->Nome=ui->lineEdit_Nome7->text();
        NewGetto->NumeroID=ui->lineEdit_ID7->text();
        NewGetto->Status=OFF_LINE;
        ListaGetti.append(NewGetto);
    }
    if(ui->lineEdit_ID8->text()!=""){
        Getto_t* NewGetto=new Getto_t;
        NewGetto->Nome=ui->lineEdit_Nome8->text();
        NewGetto->NumeroID=ui->lineEdit_ID8->text();
        NewGetto->Status=OFF_LINE;
        ListaGetti.append(NewGetto);
    }
    if(ui->lineEdit_ID9->text()!=""){
        Getto_t* NewGetto=new Getto_t;
        NewGetto->Nome=ui->lineEdit_Nome9->text();
        NewGetto->NumeroID=ui->lineEdit_ID9->text();
        NewGetto->Status=OFF_LINE;
        ListaGetti.append(NewGetto);
    }
    if(ui->lineEdit_ID10->text()!=""){
        Getto_t* NewGetto=new Getto_t;
        NewGetto->Nome=ui->lineEdit_Nome10->text();
        NewGetto->NumeroID=ui->lineEdit_ID10->text();
        NewGetto->Status=OFF_LINE;
        ListaGetti.append(NewGetto);
    }

    StepHTTP=0;
}

void MainWindow::Refresh_Lista_Getti(void){

    // Svuota il listview
    ui->listWidget_Getti->clear();

    foreach(Getto_t* Elemento, ListaGetti){
        QListWidgetItem* item;
        item = new QListWidgetItem(ui->listWidget_Getti);
        WidgetGetto* NewWidgetGetto=new WidgetGetto(this,Elemento);

        QSize Size;
        Size.setHeight(NewWidgetGetto->height);
        Size.setWidth(ui->listWidget_Getti->width()-4);
        item->setSizeHint(Size);

        if(VisualizzaSIM==false) NewWidgetGetto->SimNum=false; else NewWidgetGetto->SimNum=true;
        NewWidgetGetto->Refresh();

        ui->listWidget_Getti->setItemWidget(item, NewWidgetGetto);
        ui->listWidget_Getti->addItem(item);
    }

}

void MainWindow::on_listWidget_Getti_itemClicked(QListWidgetItem *item)
{
    //-------- Verifica abbonamento ---------------------------------------------------------------
    quint8 IndiceG = ui->listWidget_Getti->currentIndex().row();
    if(PaginaBonus[IndiceG]==false){
        if(ListaGetti.at(IndiceG)->AbbScaduto){
            QDate oggi = QDate::currentDate();
            if(ListaGetti.at(IndiceG)->DataBonus == "01/01/0001"){
                QDate bonus = oggi.addDays(30);
                ListaGetti.at(IndiceG)->DataBonus = bonus.toString("dd/MM/yyyy");
                InvioHTTPdata(IndiceG);
            }
            ui->label_Abb1->setStyleSheet("QLabel {font-size: 20pt;}");
            ui->label_Abb2->setStyleSheet("QLabel {font-size: 20pt;}");
            ui->label_Giorni->setStyleSheet("QLabel {font-size: 50pt; font-weight: bold;}");
            ui->label_Abb1->setText("Abbonamento scaduto\r\nSubscription expired\r\nAbonnement expiré");
            ui->label_Abb2->setText("giorni rimanenti\r\nremaining days\r\njours restants");
            QDate scadenza = QDate::fromString(ListaGetti.at(IndiceG)->DataBonus,"dd/MM/yyyy");
            int giorni = oggi.daysTo(scadenza);
            if(giorni>0){
                ui->label_Giorni->setText(tr("%1").arg(giorni));
                PaginaBonus[IndiceG]=true;
            }
            else {
                ui->label_Giorni->setText("0");
            }
            ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Abbonamento));
            return;
        }
    }
    //---------------------------------------------------------------------------------------------

    // Se ho cliccato un getto diverso dal precedente
    if(ui->listWidget_Getti->currentIndex().row()!=IndiceGetto){
        IndiceGetto=ui->listWidget_Getti->currentIndex().row();
        for(int i=0; i<ListaGetti.count(); i++){
            ListaGetti.at(i)->Status=OFF_LINE;
        }
        // Se non c'è nessun collegamento attivo
        if(TCPconnected==OFF_LINE){
            emit on_pushButton_Connect_clicked();
        }
        // Se c'è un collegamento attivo va prima chiuso
        else {
            tcpClient.abort();
            emit on_pushButton_Connect_clicked();
        }
    }
    // Se ho cliccato lo stesso getto commuto lo stato
    else {
        // Se il getto non era collegato
        if(TCPconnected==OFF_LINE){
            emit on_pushButton_Connect_clicked();
        }
        // Se il getto era collegato chiudo la connessione
        else {
            tcpClient.abort();
            ListaGetti.at(IndiceGetto)->Status=OFF_LINE;
            Refresh_Lista_Getti();
        }
    }


}

void MainWindow::on_pushButton_Indietro_clicked()
{
    ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Elenco));
}

void MainWindow::on_pushButton_Info_clicked()
{
    QString link = "https://sito.info";
    QDesktopServices::openUrl(QUrl(link));
}

//-------------------------------------------------------------------------------------------------------------------------------
//--------------------- HTTP ----------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------

void MainWindow::InvioHTTP(int IndiceMacchina)
{
    QString StringaComando;
    StringaComando=tr("http://sitoweb.com/Ekrain.php?id=%1").arg(ListaGetti.at(IndiceMacchina)->NumeroID);

    QUrl url=QUrl::fromUserInput(StringaComando);
    startRequest(url);
}

void MainWindow::InvioHTTPdata(int IndiceMacchina)
{
    BypassHTTP = true;   // evita chiamate http successive
    StepHTTP=IndiceMacchina;

    QString StringaComando;
    QString DataBonus = ListaGetti.at(IndiceMacchina)->DataBonus.mid(0,2) + ListaGetti.at(IndiceMacchina)->DataBonus.mid(3,2) +ListaGetti.at(IndiceMacchina)->DataBonus.mid(6,4);
    StringaComando=tr("http://sitoweb.com/Ekrain.php?id=%1&data=%2").arg(ListaGetti.at(IndiceMacchina)->NumeroID).arg(DataBonus);

    QUrl url=QUrl::fromUserInput(StringaComando);
    startRequest(url);
}

void MainWindow::startRequest(const QUrl &requestedUrl)
{
    url = requestedUrl;

    if (!url.isValid()) {
        ShowTerminalErrorMessage(ui->textEdit,tr("Invalid URL: %1: %2").arg(url.toString(), url.errorString()));
        return;
    }

    ShowTerminalMessage(ui->textEdit,tr("Request URL: %1").arg(url.toString()));

    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));

    timerHTML->start();
}

void MainWindow::httpFinished()
{
    timerHTML->stop();

    if (reply->error()) {
        ShowTerminalErrorMessage(ui->textEdit,tr("URL ERROR: %1").arg(reply->errorString()));
        reply->deleteLater();
        reply = Q_NULLPTR;
        emit HTTPTimeout();
        return;
    }

    reply->deleteLater();
    reply = Q_NULLPTR;

    Refresh_Lista_Getti();
    if(BypassHTTP==false){
        if(StepHTTP<(ListaGetti.count()-1)){
            StepHTTP++;
            InvioHTTP(StepHTTP);
        }
    }
    else BypassHTTP = false;

}

void MainWindow::httpReadyRead()
{
    QString replyString=QString::fromStdString(reply->readAll().toStdString());

    ShowTerminalMessage(ui->textEdit,tr("answer: %1").arg(replyString));

    if((replyString.left(1))==QString("+")){
        ListaGetti.at(StepHTTP)->NumeroSIM=replyString.section(';', 0, 0, QString::SectionSkipEmpty);
        ListaGetti.at(StepHTTP)->DataScadenza=replyString.section(';', 1, 1, QString::SectionSkipEmpty);
        ListaGetti.at(StepHTTP)->DataBonus=replyString.section(';', 2, 2, QString::SectionSkipEmpty);
    }

}

void MainWindow::HTTPTimeout()
{


}

//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------


void MainWindow::on_pushButton_Connect_clicked()
{    
    if(MQTTprotocol){
        // Server MQTT
        ui->lineEdit_IP->setText("servermqtt.com");
        ui->lineEdit_Port->setText("1883");
    }
    else {
        // WIFI
        ui->lineEdit_IP->setText("192.168.4.1");
        ui->lineEdit_Port->setText("333");
    }
    ShowTerminalMessage(ui->textEdit,QString("Connecting to %1:%2 Server").arg(ui->lineEdit_IP->text()).arg(ui->lineEdit_Port->text().toInt()));
    tcpClient.connectToHost(ui->lineEdit_IP->text(), ui->lineEdit_Port->text().toInt());

    ListaGetti.at(IndiceGetto)->Status=CONNECTING;
    Refresh_Lista_Getti();

    timerInitTCP->start();

}

void MainWindow::on_pushButton_Back_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Finestra di debug
void MainWindow::on_pushButton_Logo_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::on_pushButton_Elenco_clicked()
{
    if(ui->stackedWidget_App->currentIndex()!=ui->stackedWidget_App->indexOf(ui->page_Elenco)){
        ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Elenco));
    }
    else {
        if(VisualizzaSIM==false) VisualizzaSIM=true; else VisualizzaSIM=false;
        Refresh_Lista_Getti();
    }
}

void MainWindow::on_pushButton_Stato_clicked()
{
//    timerKeepAlive->stop();
    timerDatoTCP->start();
    ui->W_Stato->ProgInCorso=0;      // pulizia campi precedenti
    ui->W_Stato->RefreshStato();
    ComandoTCP = "$STATE";
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
    ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Stato));
    ui->pushButton_Stato->setEnabled(false);
    ui->pushButton_Prog->setEnabled(false);
    ui->pushButton_Mappa->setEnabled(false);
    ui->W_Stato->RefreshStato();

}

void MainWindow::on_pushButton_Prog_clicked()
{
    if(ui->W_Prog->PageLoadSave){
        emit default_page_prog();
        ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Prog));
        return;
    }

//    timerKeepAlive->stop();
    timerDatoTCP->start();
    ui->W_Prog->ProgSel=0;      // pulizia campi precedenti
    ui->W_Prog->RefreshProg();
    ComandoTCP = "$READ";
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
    ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Prog));
    ui->pushButton_Stato->setEnabled(false);
    ui->pushButton_Prog->setEnabled(false);
    ui->pushButton_Mappa->setEnabled(false);
    ui->W_Prog->RefreshProg();
}

void MainWindow::on_pushButton_Mappa_clicked()
{
    // ignorato se sono già sulla mappa
    if(ui->stackedWidget_App->currentIndex()==ui->stackedWidget_App->indexOf(ui->page_Mappa)) return;

//    timerKeepAlive->stop();
    timerDatoTCP->start();
    ComandoTCP = "$GPS";
    Publish_MQTT();
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
    ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Mappa));
    ui->pushButton_Stato->setEnabled(false);
    ui->pushButton_Prog->setEnabled(false);
    ui->pushButton_Mappa->setEnabled(false);

}

void MainWindow::UpdateStato()
{
//    timerKeepAlive->stop();
    timerDatoTCP->start();
    ComandoTCP = "$ANGLE," + QString("%1").arg(ui->W_Stato->AngI, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Stato->AngF, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Stato->VelRot, 3, 10, QChar('0'));
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
}

void MainWindow::ChangeProg()
{
//    timerKeepAlive->stop();
    timerDatoTCP->start();
    ui->W_Stato->ProgInCorso=0;      // pulizia campi precedenti
    ui->W_Stato->RefreshStato();
    ComandoTCP = "$PROGS";
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
}

void MainWindow::ChangeManAut()
{
//    timerKeepAlive->stop();
    timerDatoTCP->start();
    ui->W_Stato->ProgInCorso=0;      // pulizia campi precedenti
    ui->W_Stato->RefreshStato();
    ComandoTCP = "$MANAUT";
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
}

void MainWindow::ReverseSave()
{
//    timerKeepAlive->stop();
    timerDatoTCP->start();
    ui->W_Stato->ProgInCorso=0;      // pulizia campi precedenti
    ui->W_Stato->RefreshStato();
    ComandoTCP = "$REVERSES";
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
}

void MainWindow::ReverseNotSave()
{
//    timerKeepAlive->stop();
    timerDatoTCP->start();
    ui->W_Stato->ProgInCorso=0;      // pulizia campi precedenti
    ui->W_Stato->RefreshStato();
    ComandoTCP = "$REVERSEN";
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
}

void MainWindow::PaginaRompigetto()
{
    ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Rompigetto));

    QSize IconSizes;
    IconSizes.setHeight(ui->pushButton_Alza->height()*0.6);
    IconSizes.setWidth(ui->pushButton_Alza->width()*0.6);
    ui->pushButton_Alza->setIconSize(IconSizes);

    IconSizes.setHeight(ui->pushButton_Abbassa->height()*0.6);
    IconSizes.setWidth(ui->pushButton_Abbassa->width()*0.6);
    ui->pushButton_Abbassa->setIconSize(IconSizes);

}

void MainWindow::PaginaAuto()
{
    timerDatoTCP->start();
    ui->W_Bordo->DatiAgg=false;      // pulizia campi precedenti
    ui->W_Bordo->RefreshBordo();
    ComandoTCP = "$BORDER";
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
    ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Auto));
    ui->pushButton_Stato->setEnabled(false);
    ui->pushButton_Prog->setEnabled(false);
    ui->pushButton_Mappa->setEnabled(false);
    ui->W_Bordo->RefreshBordo();
}

void MainWindow::on_pushButton_Alza_clicked()
{
    timerDatoTCP->start();
    ComandoTCP = "$RGUP";
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);

    ui->pushButton_Stato->setEnabled(false);
    ui->pushButton_Prog->setEnabled(false);
    ui->pushButton_Mappa->setEnabled(false);
    ui->pushButton_Alza->setEnabled(false);
    ui->pushButton_Abbassa->setEnabled(false);
}

void MainWindow::on_pushButton_Abbassa_clicked()
{
    timerDatoTCP->start();
    ComandoTCP = "$RGDOWN";
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);

    ui->pushButton_Stato->setEnabled(false);
    ui->pushButton_Prog->setEnabled(false);
    ui->pushButton_Mappa->setEnabled(false);
    ui->pushButton_Alza->setEnabled(false);
    ui->pushButton_Abbassa->setEnabled(false);
}

void MainWindow::UpdateProg()
{
//    timerKeepAlive->stop();
    timerDatoTCP->start();
    ComandoTCP = "$WRITE," + QString("%1").arg(ui->W_Prog->SelTM_P1, 1, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang1_P1, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang2_P1, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Tempo12_P1, 4, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Metri12_P1, 3, 10, QChar('0')) + ",";
    if(ui->W_Prog->Rot1234_P1==1 ) ComandoTCP = ComandoTCP + ">,";
    else if(ui->W_Prog->Rot1234_P1==2 ) ComandoTCP = ComandoTCP + "<,";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang3_P1, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang4_P1, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Tempo34_P1, 4, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Metri34_P1, 3, 10, QChar('0')) + ",";
    if(ui->W_Prog->Rot3456_P1==1 ) ComandoTCP = ComandoTCP + ">,";
    else if(ui->W_Prog->Rot3456_P1==2 ) ComandoTCP = ComandoTCP + "<,";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang5_P1, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang6_P1, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Tempo56_P1, 4, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Metri56_P1, 3, 10, QChar('0')) + ",";
    if(ui->W_Prog->Rot5678_P1==1 ) ComandoTCP = ComandoTCP + ">,";
    else if(ui->W_Prog->Rot5678_P1==2 ) ComandoTCP = ComandoTCP + "<,";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang7_P1, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang8_P1, 3, 10, QChar('0')) + ",";

    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->SelTM_P2, 1, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang1_P2, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang2_P2, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Tempo12_P2, 4, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Metri12_P2, 3, 10, QChar('0')) + ",";
    if(ui->W_Prog->Rot1234_P2==1 ) ComandoTCP = ComandoTCP + ">,";
    else if(ui->W_Prog->Rot1234_P2==2 ) ComandoTCP = ComandoTCP + "<,";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang3_P2, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang4_P2, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Tempo34_P2, 4, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Metri34_P2, 3, 10, QChar('0')) + ",";
    if(ui->W_Prog->Rot3456_P2==1 ) ComandoTCP = ComandoTCP + ">,";
    else if(ui->W_Prog->Rot3456_P2==2 ) ComandoTCP = ComandoTCP + "<,";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang5_P2, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang6_P2, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Tempo56_P2, 4, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Metri56_P2, 3, 10, QChar('0')) + ",";
    if(ui->W_Prog->Rot5678_P2==1 ) ComandoTCP = ComandoTCP + ">,";
    else if(ui->W_Prog->Rot5678_P2==2 ) ComandoTCP = ComandoTCP + "<,";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang7_P2, 3, 10, QChar('0')) + ",";
    ComandoTCP = ComandoTCP + QString("%1").arg(ui->W_Prog->Ang8_P2, 3, 10, QChar('0'));

    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
}

void MainWindow::UpdateBordo()
{
    timerDatoTCP->start();
    ComandoTCP = "$BC," + QString("%1").arg(ui->W_Bordo->Boccaglio, 2, 10) + "," +
            QString("%1").arg(ui->W_Bordo->OffsetDir, 3, 10) + "," +
            QString("%1").arg(ui->W_Bordo->OffsetM, 3, 10) + "," +
            QString("%1").arg(ui->W_Bordo->GradiAper, 2, 10) + "," +
            QString("%1").arg(ui->W_Bordo->IncNord, 10, 'f', 8) + "," +
            QString("%1").arg(ui->W_Bordo->IncEst, 10, 'f', 8) + "," +
            QString("%1").arg(ui->W_Bordo->NPunti, 2, 10);
    for (quint8 i=0; i<ui->W_Bordo->NPunti; i++) {
        ComandoTCP = ComandoTCP + "," + QString("%1").arg(ui->W_Bordo->GPS_Np[i], 10, 'f', 6) + "," + QString("%1").arg(ui->W_Bordo->GPS_Ep[i], 10, 'f', 6);
    }
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
}

void MainWindow::UpdateDati()
{
    timerDatoTCP->start();
    ComandoTCP = "$WBORDER," + QString("%1").arg(ui->W_Bordo->Boccaglio, 2, 10) + "," +
            QString("%1").arg(ui->W_Bordo->OffsetDir, 3, 10) + "," +
            QString("%1").arg(ui->W_Bordo->OffsetM, 3, 10) + "," +
            QString("%1").arg(ui->W_Bordo->GradiAper, 2, 10);
    if(MQTTprotocol){
        Publish_MQTT();
    }
    else {
        tcpClient.write(ComandoTCP.toLatin1());
    }
    ShowTerminalMessage(ui->textEdit, ComandoTCP);
}

void MainWindow::Publish_MQTT()
{
    quint16 RL=0, i=0;

    RL = 20 + ComandoTCP.length();		// Remaining Lenght = 2 TPLEN + 18 TOPIC ("GETTO/" + numero ID + "rx") + Payload

    BufferTX_MQTT[0]  = 0x30;		// PUBLISH
    if(RL<128){
        BufferTX_MQTT[1] = RL;			// Remaining Lenght
        BufferTX_MQTT[2] = 0x00;		// TPLEN MSB
        BufferTX_MQTT[3] = 0x12;		// TPLEN LSB
        BufferTX_MQTT[4] = 0x47;		// 'G'
        BufferTX_MQTT[5] = 0x45;		// 'E'
        BufferTX_MQTT[6] = 0x54;		// 'T'
        BufferTX_MQTT[7] = 0x54;		// 'T'
        BufferTX_MQTT[8] = 0x4F;		// 'O'
        BufferTX_MQTT[9] = 0x2F;		// '/'
//        QString IDnumber="4118839749";
        BufferTX_MQTT.insert(10,ListaGetti.at(IndiceGetto)->NumeroID);
        BufferTX_MQTT[20] = 0x72;		// 'r'
        BufferTX_MQTT[21] = 0x78;		// 'x'
        BufferTX_MQTT.insert(22,ComandoTCP);		// payload
        tcpClient.write(BufferTX_MQTT,RL+2);
    }
    else {
        BufferTX_MQTT[1] = (RL%128)+128;		// Remaining Lenght LSB
        BufferTX_MQTT[2] = (RL/128);				// Remaining Lenght MSB
        BufferTX_MQTT[3] = 0x00;		// TPLEN MSB
        BufferTX_MQTT[4] = 0x12;		// TPLEN LSB
        BufferTX_MQTT[5] = 0x47;		// 'G'
        BufferTX_MQTT[6] = 0x45;		// 'E'
        BufferTX_MQTT[7] = 0x54;		// 'T'
        BufferTX_MQTT[8] = 0x54;		// 'T'
        BufferTX_MQTT[9] = 0x4F;		// 'O'
        BufferTX_MQTT[10] = 0x2F;		// '/'
//        QString IDnumber="4118839749";
        BufferTX_MQTT.insert(11,ListaGetti.at(IndiceGetto)->NumeroID);
        BufferTX_MQTT[21] = 0x72;		// 'r'
        BufferTX_MQTT[22] = 0x78;		// 'x'
        BufferTX_MQTT.insert(23,ComandoTCP);		// payload
        tcpClient.write(BufferTX_MQTT,RL+3);
    }

}

void MainWindow::IncomingData(void)
{        
    timerDatoTCP->stop();

    if(MQTTprotocol){
        BufferRX_MQTT.clear();
        BufferRX_MQTT = tcpClient.readAll();

        // CONNACK corretta
        if(BufferRX_MQTT.toHex()=="20020000"){
            ShowTerminalMessage(ui->textEdit, "CONNACK OK!");

            // SUBSCRIBE PACKET
            BufferTX_MQTT.clear();
            BufferTX_MQTT[0]  = 0x82;		// SUBSCRIBE
            BufferTX_MQTT[1]  = 0x17;		// Remaining Lenght (lunghezza fissa) = 23
                                            // 2 PKTID + 2 TPLEN + 18 TOPIC ("GETTO/" + numero ID + "rx") + 1 QS
            BufferTX_MQTT[2]  = 0x00;		// PKTID MSB
            BufferTX_MQTT[3]  = 0x01;		// PKTID LSB
            BufferTX_MQTT[4]  = 0x00;		// TPLEN MSB
            BufferTX_MQTT[5]  = 0x12;		// TPLEN LSB
            BufferTX_MQTT[6]  = 0x47;		// 'G'
            BufferTX_MQTT[7]  = 0x45;		// 'E'
            BufferTX_MQTT[8]  = 0x54;		// 'T'
            BufferTX_MQTT[9]  = 0x54;		// 'T'
            BufferTX_MQTT[10] = 0x4F;		// 'O'
            BufferTX_MQTT[11] = 0x2F;		// '/'
//            QString IDnumber="4118839749";
            BufferTX_MQTT.insert(12,ListaGetti.at(IndiceGetto)->NumeroID);
            BufferTX_MQTT[22] = 0x74;		// 't'
            BufferTX_MQTT[23] = 0x78;		// 'x'
            BufferTX_MQTT[24] = 0x00;		// QS
            tcpClient.write(BufferTX_MQTT,25);
            timerDatoTCP->start();
            ShowTerminalMessage(ui->textEdit, "MQTT SUBSCRIBE PACKET ...");

            return;
        }
        // SUBSCRIBE ACK
        else if(BufferRX_MQTT.toHex()=="9003000100"){
            ShowTerminalMessage(ui->textEdit, "SUBSCRIBE OK!");

            timerDatoTCP->start();
            ComandoTCP = "$CK," + ListaGetti.at(IndiceGetto)->NumeroID;
            Publish_MQTT();

            return;
        }
        // PUBLISH PACKET received
        else if(BufferRX_MQTT.toHex().left(2)=="30"){
            quint16 RL=0, TPLEN=0;
            if(static_cast<quint8>(BufferRX_MQTT[1])<128) {
                RL = static_cast<quint8>(BufferRX_MQTT[1]);
                TPLEN = ((static_cast<quint16>(BufferRX_MQTT[2]))<<8) + static_cast<quint8>(BufferRX_MQTT[3]);
                RispostaTCP = BufferRX_MQTT.right(RL-TPLEN-2);
            }
            else {
                RL = (BufferRX_MQTT[1]&0x7F) + ((static_cast<quint16>(BufferRX_MQTT[2]))*128);
                TPLEN = ((static_cast<quint16>(BufferRX_MQTT[3]))<<8) + static_cast<quint8>(BufferRX_MQTT[4]);
                RispostaTCP = BufferRX_MQTT.right(RL-TPLEN-2);
            }
        }
    }
    else {
        RispostaTCP = QString::fromStdString(tcpClient.readAll().toStdString());
    }

    ShowTerminalAssertMessage(ui->textEdit, RispostaTCP);

    QString Intestazione=RispostaTCP.section(',', 0, 0, QString::SectionSkipEmpty);

    if(Intestazione=="$SA"){
        ui->W_Stato->AngAttuale = RispostaTCP.section(',', 1, 1, QString::SectionSkipEmpty).toInt();
        ui->W_Stato->StatoMotore = RispostaTCP.section(',', 2, 2, QString::SectionSkipEmpty).toInt();
        ui->W_Stato->ProgInCorso = RispostaTCP.section(',', 3, 3, QString::SectionSkipEmpty).toInt();
        ui->W_Stato->AngI = RispostaTCP.section(',', 4, 4, QString::SectionSkipEmpty).toInt();
        ui->W_Stato->AngF = RispostaTCP.section(',', 5, 5, QString::SectionSkipEmpty).toInt();
        ui->W_Stato->Settore = RispostaTCP.section(',', 6, 6, QString::SectionSkipEmpty).toInt();
        ui->W_Stato->SelTM = RispostaTCP.section(',', 7, 7, QString::SectionSkipEmpty).toInt();
        ui->W_Stato->TempoRes = RispostaTCP.section(',', 8, 8, QString::SectionSkipEmpty).toInt();
        ui->W_Stato->MetriRes = RispostaTCP.section(',', 9, 9, QString::SectionSkipEmpty).toInt();
        ui->W_Stato->VelRot = RispostaTCP.section(',', 10, 10, QString::SectionSkipEmpty).toInt();
        ui->W_Stato->Pressione = RispostaTCP.section(',', 11, 11, QString::SectionSkipEmpty).toFloat();
        if(RispostaTCP.section(',', 12, 12, QString::SectionSkipEmpty)==NULL) ui->W_Stato->Auto=0;
        else ui->W_Stato->Auto = RispostaTCP.section(',', 12, 12, QString::SectionSkipEmpty).toInt();

        ui->W_Stato->RefreshStato();
    }
    else if(Intestazione=="$OKAN"){
        ui->W_Stato->Update_OK();
    }
    else if(Intestazione=="$PG"){
        ui->W_Prog->ProgSel = RispostaTCP.section(',', 1, 1, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->SelTM_P1 = RispostaTCP.section(',', 2, 2, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Ang1_P1 = RispostaTCP.section(',', 3, 3, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Ang2_P1 = RispostaTCP.section(',', 4, 4, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Tempo12_P1 = RispostaTCP.section(',', 5, 5, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Metri12_P1 = RispostaTCP.section(',', 6, 6, QString::SectionSkipEmpty).toInt();
        QString SimboloRot = RispostaTCP.section(',', 7, 7, QString::SectionSkipEmpty);
        if(SimboloRot==">") ui->W_Prog->Rot1234_P1 = 1;
        else if(SimboloRot=="<") ui->W_Prog->Rot1234_P1 = 2;
        ui->W_Prog->Ang3_P1 = RispostaTCP.section(',', 8, 8, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Ang4_P1 = RispostaTCP.section(',', 9, 9, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Tempo34_P1 = RispostaTCP.section(',', 10, 10, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Metri34_P1 = RispostaTCP.section(',', 11, 11, QString::SectionSkipEmpty).toInt();
        SimboloRot = RispostaTCP.section(',', 12, 12, QString::SectionSkipEmpty);
        if(SimboloRot==">") ui->W_Prog->Rot3456_P1 = 1;
        else if(SimboloRot=="<") ui->W_Prog->Rot3456_P1 = 2;
        ui->W_Prog->Ang5_P1 = RispostaTCP.section(',', 13, 13, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Ang6_P1 = RispostaTCP.section(',', 14, 14, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Tempo56_P1 = RispostaTCP.section(',', 15, 15, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Metri56_P1 = RispostaTCP.section(',', 16, 16, QString::SectionSkipEmpty).toInt();
        SimboloRot = RispostaTCP.section(',', 17, 17, QString::SectionSkipEmpty);
        if(SimboloRot==">") ui->W_Prog->Rot5678_P1 = 1;
        else if(SimboloRot=="<") ui->W_Prog->Rot5678_P1 = 2;
        ui->W_Prog->Ang7_P1 = RispostaTCP.section(',', 18, 18, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Ang8_P1 = RispostaTCP.section(',', 19, 19, QString::SectionSkipEmpty).toInt();

        ui->W_Prog->SelTM_P2 = RispostaTCP.section(',', 20, 20, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Ang1_P2 = RispostaTCP.section(',', 21, 21, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Ang2_P2 = RispostaTCP.section(',', 22, 22, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Tempo12_P2 = RispostaTCP.section(',', 23, 23, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Metri12_P2 = RispostaTCP.section(',', 24, 24, QString::SectionSkipEmpty).toInt();
        SimboloRot = RispostaTCP.section(',', 25, 25, QString::SectionSkipEmpty);
        if(SimboloRot==">") ui->W_Prog->Rot1234_P2 = 1;
        else if(SimboloRot=="<") ui->W_Prog->Rot1234_P2 = 2;
        ui->W_Prog->Ang3_P2 = RispostaTCP.section(',', 26, 26, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Ang4_P2 = RispostaTCP.section(',', 27, 27, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Tempo34_P2 = RispostaTCP.section(',', 28, 28, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Metri34_P2 = RispostaTCP.section(',', 29, 29, QString::SectionSkipEmpty).toInt();
        SimboloRot = RispostaTCP.section(',', 30, 30, QString::SectionSkipEmpty);
        if(SimboloRot==">") ui->W_Prog->Rot3456_P2 = 1;
        else if(SimboloRot=="<") ui->W_Prog->Rot3456_P2 = 2;
        ui->W_Prog->Ang5_P2 = RispostaTCP.section(',', 31, 31, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Ang6_P2 = RispostaTCP.section(',', 32, 32, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Tempo56_P2 = RispostaTCP.section(',', 33, 33, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Metri56_P2 = RispostaTCP.section(',', 34, 34, QString::SectionSkipEmpty).toInt();
        SimboloRot = RispostaTCP.section(',', 35, 35, QString::SectionSkipEmpty);
        if(SimboloRot==">") ui->W_Prog->Rot5678_P2 = 1;
        else if(SimboloRot=="<") ui->W_Prog->Rot5678_P2 = 2;
        ui->W_Prog->Ang7_P2 = RispostaTCP.section(',', 36, 36, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->Ang8_P2 = RispostaTCP.section(',', 37, 37, QString::SectionSkipEmpty).toInt();
        ui->W_Prog->RefreshProg();
    }
    else if(Intestazione=="$OKWR"){
        ui->W_Prog->Update_OK();
    }
    else if(Intestazione=="$NE"){
        GPS_Nord = RispostaTCP.section(',', 1, 1, QString::SectionSkipEmpty);
        GPS_Est = RispostaTCP.section(',', 2, 2, QString::SectionSkipEmpty);
        GPS_Nord_1 = RispostaTCP.section(',', 3, 3, QString::SectionSkipEmpty);
        GPS_Est_1 = RispostaTCP.section(',', 4, 4, QString::SectionSkipEmpty);
        MappaAuto = false;
        CaricaMappa();
    }
    else if(Intestazione=="$NB"){
        GPS_Nord = RispostaTCP.section(',', 1, 1, QString::SectionSkipEmpty);
        GPS_Est = RispostaTCP.section(',', 2, 2, QString::SectionSkipEmpty);
        GPS_Nord_1 = RispostaTCP.section(',', 3, 3, QString::SectionSkipEmpty);
        GPS_Est_1 = RispostaTCP.section(',', 4, 4, QString::SectionSkipEmpty);
        MappaAuto = true;
        AngI = RispostaTCP.section(',', 5, 5, QString::SectionSkipEmpty).toInt();
        AngF = RispostaTCP.section(',', 6, 6, QString::SectionSkipEmpty).toInt();
        DirezioneGPS = RispostaTCP.section(',', 7, 7, QString::SectionSkipEmpty).toInt();
        raggio = RispostaTCP.section(',', 8, 8, QString::SectionSkipEmpty).toInt();
        IncNord = RispostaTCP.section(',', 9, 9, QString::SectionSkipEmpty).toFloat();
        IncEst = RispostaTCP.section(',', 10, 10, QString::SectionSkipEmpty).toFloat();
        GPS_Np0 = RispostaTCP.section(',', 11, 11, QString::SectionSkipEmpty);
        GPS_Ep0 = RispostaTCP.section(',', 12, 12, QString::SectionSkipEmpty);
        CaricaMappa();
    }
    else if(Intestazione=="$READY"){
        ListaGetti.at(IndiceGetto)->Status=ON_LINE;
        Refresh_Lista_Getti();
        ui->pushButton_Settings->setEnabled(false);
    }
    else if(Intestazione=="$ERROR"){
        TCPDisconnesso();
    }
    else if(Intestazione=="$OKRG"){
        ui->pushButton_Alza->setEnabled(true);
        ui->pushButton_Abbassa->setEnabled(true);
    }
    else if(Intestazione=="$DB"){
        ui->W_Bordo->Boccaglio = RispostaTCP.section(',', 1, 1, QString::SectionSkipEmpty).toInt();
        ui->W_Bordo->OffsetDir = RispostaTCP.section(',', 2, 2, QString::SectionSkipEmpty).toInt();
        ui->W_Bordo->OffsetM = RispostaTCP.section(',', 3, 3, QString::SectionSkipEmpty).toInt();
        ui->W_Bordo->GradiAper = RispostaTCP.section(',', 4, 4, QString::SectionSkipEmpty).toInt();
        ui->W_Bordo->GPS_Np0 = RispostaTCP.section(',', 5, 5, QString::SectionSkipEmpty);
        ui->W_Bordo->GPS_Ep0 = RispostaTCP.section(',', 6, 6, QString::SectionSkipEmpty);

        ui->W_Bordo->DatiAgg=true;
        ui->W_Bordo->RefreshBordo();
    }
    else if(Intestazione=="$OKBC"){
        ui->W_Bordo->Update_OK();
    }
    else if(Intestazione=="$OKWB"){
        ui->W_Bordo->Update_OK();
    }

    ui->pushButton_Stato->setEnabled(true);
    ui->pushButton_Prog->setEnabled(true);
    if(MQTTprotocol) ui->pushButton_Mappa->setEnabled(true);

//    timerKeepAlive->start();

}

//----------- MAPPA --------------------------------------------------------------------------
void MainWindow::on_stackedWidget_App_currentChanged(int arg1)
{
//    if (arg1!=3){
        //distruggi il widget mappa
        if (Mappa!=nullptr){
            delete Mappa;
            Mappa=nullptr;
        }
//    }
//    else {
//        //crea e visualizza il widget
////        Mappa= new MapWidget(GPS_Nord, GPS_Est, GPS_Nord_1, GPS_Est_1);
//        Mappa= new WidgetMappa();
//        //set layout programatically
//        ui->page_Mappa->layout()->addWidget(Mappa);
//    }
}

void MainWindow::CaricaMappa()
{
    //crea e visualizza il widget
    Mappa= new MapWidget(GPS_Nord, GPS_Est, GPS_Nord_1, GPS_Est_1, MappaAuto, AngI, AngF, DirezioneGPS, raggio, IncNord, IncEst,
                         GPS_Np0, GPS_Ep0, ui->W_Bordo->ListaGPS_Np0, ui->W_Bordo->ListaGPS_Ep0, ui->W_Bordo->ListaKML, this);
    //set layout programatically
    ui->page_Mappa->layout()->addWidget(Mappa);

}

//----------- Controlli su IP immessi -------------------------------------------------------

void MainWindow::on_lineEdit_ID1_editingFinished()
{
    if ((ui->lineEdit_ID1->text().length()!=10)&&(ui->lineEdit_ID1->text()!=""))
    {
       ui->lineEdit_ID1->setStyleSheet(Stile_ERR);
       ui->pushButton_Settings->setEnabled(false);
    }
}
void MainWindow::on_lineEdit_ID2_editingFinished()
{
    if ((ui->lineEdit_ID2->text().length()!=10)&&(ui->lineEdit_ID2->text()!=""))
    {
       ui->lineEdit_ID2->setStyleSheet(Stile_ERR);
       ui->pushButton_Settings->setEnabled(false);
    }
}
void MainWindow::on_lineEdit_ID3_editingFinished()
{
    if ((ui->lineEdit_ID3->text().length()!=10)&&(ui->lineEdit_ID3->text()!=""))
    {
       ui->lineEdit_ID3->setStyleSheet(Stile_ERR);
       ui->pushButton_Settings->setEnabled(false);
    }
}
void MainWindow::on_lineEdit_ID4_editingFinished()
{
    if ((ui->lineEdit_ID4->text().length()!=10)&&(ui->lineEdit_ID4->text()!=""))
    {
       ui->lineEdit_ID4->setStyleSheet(Stile_ERR);
       ui->pushButton_Settings->setEnabled(false);
    }
}
void MainWindow::on_lineEdit_ID5_editingFinished()
{
    if ((ui->lineEdit_ID5->text().length()!=10)&&(ui->lineEdit_ID5->text()!=""))
    {
       ui->lineEdit_ID5->setStyleSheet(Stile_ERR);
       ui->pushButton_Settings->setEnabled(false);
    }
}
void MainWindow::on_lineEdit_ID6_editingFinished()
{
    if ((ui->lineEdit_ID6->text().length()!=10)&&(ui->lineEdit_ID6->text()!=""))
    {
       ui->lineEdit_ID6->setStyleSheet(Stile_ERR);
       ui->pushButton_Settings->setEnabled(false);
    }
}
void MainWindow::on_lineEdit_ID7_editingFinished()
{
    if ((ui->lineEdit_ID7->text().length()!=10)&&(ui->lineEdit_ID7->text()!=""))
    {
       ui->lineEdit_ID7->setStyleSheet(Stile_ERR);
       ui->pushButton_Settings->setEnabled(false);
    }
}
void MainWindow::on_lineEdit_ID8_editingFinished()
{
    if ((ui->lineEdit_ID8->text().length()!=10)&&(ui->lineEdit_ID8->text()!=""))
    {
       ui->lineEdit_ID8->setStyleSheet(Stile_ERR);
       ui->pushButton_Settings->setEnabled(false);
    }
}
void MainWindow::on_lineEdit_ID9_editingFinished()
{
    if ((ui->lineEdit_ID9->text().length()!=10)&&(ui->lineEdit_ID9->text()!=""))
    {
       ui->lineEdit_ID9->setStyleSheet(Stile_ERR);
       ui->pushButton_Settings->setEnabled(false);
    }
}
void MainWindow::on_lineEdit_ID10_editingFinished()
{
    if ((ui->lineEdit_ID10->text().length()!=10)&&(ui->lineEdit_ID10->text()!=""))
    {
       ui->lineEdit_ID10->setStyleSheet(Stile_ERR);
       ui->pushButton_Settings->setEnabled(false);
    }
}

void MainWindow::on_lineEdit_ID1_textEdited(const QString &arg1)
{
    ui->lineEdit_ID1->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    ui->lineEdit_ID1->setStyleSheet(Stile_OK);
    ui->pushButton_Settings->setEnabled(true);
}
void MainWindow::on_lineEdit_ID2_textEdited(const QString &arg1)
{
    ui->lineEdit_ID2->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    ui->lineEdit_ID2->setStyleSheet(Stile_OK);
    ui->pushButton_Settings->setEnabled(true);
}
void MainWindow::on_lineEdit_ID3_textEdited(const QString &arg1)
{
    ui->lineEdit_ID3->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    ui->lineEdit_ID3->setStyleSheet(Stile_OK);
    ui->pushButton_Settings->setEnabled(true);
}
void MainWindow::on_lineEdit_ID4_textEdited(const QString &arg1)
{
    ui->lineEdit_ID4->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    ui->lineEdit_ID4->setStyleSheet(Stile_OK);
    ui->pushButton_Settings->setEnabled(true);
}
void MainWindow::on_lineEdit_ID5_textEdited(const QString &arg1)
{
    ui->lineEdit_ID5->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    ui->lineEdit_ID5->setStyleSheet(Stile_OK);
    ui->pushButton_Settings->setEnabled(true);
}
void MainWindow::on_lineEdit_ID6_textEdited(const QString &arg1)
{
    ui->lineEdit_ID6->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    ui->lineEdit_ID6->setStyleSheet(Stile_OK);
    ui->pushButton_Settings->setEnabled(true);
}
void MainWindow::on_lineEdit_ID7_textEdited(const QString &arg1)
{
    ui->lineEdit_ID7->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    ui->lineEdit_ID7->setStyleSheet(Stile_OK);
    ui->pushButton_Settings->setEnabled(true);
}
void MainWindow::on_lineEdit_ID8_textEdited(const QString &arg1)
{
    ui->lineEdit_ID8->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    ui->lineEdit_ID8->setStyleSheet(Stile_OK);
    ui->pushButton_Settings->setEnabled(true);
}
void MainWindow::on_lineEdit_ID9_textEdited(const QString &arg1)
{
    ui->lineEdit_ID9->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    ui->lineEdit_ID9->setStyleSheet(Stile_OK);
    ui->pushButton_Settings->setEnabled(true);
}
void MainWindow::on_lineEdit_ID10_textEdited(const QString &arg1)
{
    ui->lineEdit_ID10->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    ui->lineEdit_ID10->setStyleSheet(Stile_OK);
    ui->pushButton_Settings->setEnabled(true);
}

void MainWindow::TCPConnesso()
{
    timerInitTCP->stop();
//    timerKeepAlive->start();
    TCPconnected=ON_LINE;
    ShowTerminalAssertMessage(ui->textEdit,"Connected!");
//    ListaGetti.at(IndiceGetto)->Status=ON_LINE;
//    Refresh_Lista_Getti();
//    ui->pushButton_Stato->setEnabled(true);
//    ui->pushButton_Prog->setEnabled(true);
//    ui->pushButton_Mappa->setEnabled(true);
//    ui->pushButton_Settings->setEnabled(false);

    if(MQTTprotocol){
        // CONNECT PACKET
        BufferTX_MQTT.clear();
        BufferTX_MQTT[0]  = 0x10;		// CONNECT
        BufferTX_MQTT[1]  = 0x2B;		// Remaining Lenght (lunghezza fissa) = 43
                                        // 2 PLEN + 4 "MQTT" + 1 LVL + 1 FL + 2 KA + 2 CIDLEN + 14 ID (numero ID+"_app") + 2 ULEN + 7 USER + 2 PWLEN + 6 PASSWORD
        BufferTX_MQTT[2]  = 0x00;		// PLEN MSB
        BufferTX_MQTT[3]  = 0x04;		// PLEN LSB
        BufferTX_MQTT[4]  = 0x4D;		// 'M'
        BufferTX_MQTT[5]  = 0x51;		// 'Q'
        BufferTX_MQTT[6]  = 0x54;		// 'T'
        BufferTX_MQTT[7]  = 0x54;		// 'T'
        BufferTX_MQTT[8]  = 0x04;		// LVL
        BufferTX_MQTT[9]  = 0xC2;		// FL
        BufferTX_MQTT[10] = 0x00;		// KA MSB
        BufferTX_MQTT[11] = 0xB4;		// KA LSB (180 sec)
        BufferTX_MQTT[12] = 0x00;		// CIDLEN MSB
        BufferTX_MQTT[13] = 0x0E;		// CIDLEN LSB
//        QString IDnumber="4118839749_app";
        BufferTX_MQTT.insert(14,ListaGetti.at(IndiceGetto)->NumeroID + "_app");
        BufferTX_MQTT[28] = 0x00;		// ULEN MSB
        BufferTX_MQTT[29] = 0x07;		// ULEN LSB
        BufferTX_MQTT[30] = 0x00;
        BufferTX_MQTT[31] = 0x00;
        BufferTX_MQTT[32] = 0x00;
        BufferTX_MQTT[33] = 0x00;
        BufferTX_MQTT[34] = 0x00;
        BufferTX_MQTT[35] = 0x00;
        BufferTX_MQTT[36] = 0x00;
        BufferTX_MQTT[37] = 0x00;		// PWLEN MSB
        BufferTX_MQTT[38] = 0x06;		// PWLEN LSB
        BufferTX_MQTT[39] = 0x00;
        BufferTX_MQTT[40] = 0x00;
        BufferTX_MQTT[41] = 0x00;
        BufferTX_MQTT[42] = 0x00;
        BufferTX_MQTT[43] = 0x00;
        BufferTX_MQTT[44] = 0x00;
        tcpClient.write(BufferTX_MQTT,45);
        timerDatoTCP->start();
        ShowTerminalMessage(ui->textEdit, "MQTT CONNECT PACKET ...");
    }
    else {
        ComandoTCP = "$CK," + ListaGetti.at(IndiceGetto)->NumeroID;
        tcpClient.write(ComandoTCP.toLatin1());
        ShowTerminalMessage(ui->textEdit, ComandoTCP);
    }
}

void MainWindow::TCPDisconnesso()
{
    tcpClient.abort();
    timerInitTCP->stop();
    timerKeepAlive->stop();
    TCPconnected=OFF_LINE;
    ShowTerminalErrorMessage(ui->textEdit,"Not Connected!");
    if(ListaGetti.at(IndiceGetto)->Status!=OFF_LINE) ListaGetti.at(IndiceGetto)->Status=ERROR_TCP;
    Refresh_Lista_Getti();
    ui->pushButton_Stato->setEnabled(false);
    ui->pushButton_Prog->setEnabled(false);
    ui->pushButton_Mappa->setEnabled(false);
    ui->pushButton_Settings->setEnabled(true);
    ui->stackedWidget_App->setCurrentIndex(ui->stackedWidget_App->indexOf(ui->page_Elenco));
    ui->pushButton_Elenco->setChecked(true);
}

//void MainWindow::InvioKeepAlive()
//{
//    ComandoTCP = "&";
//    tcpClient.write(ComandoTCP.toLatin1());
//    ShowTerminalMessage(ui->textEdit, ComandoTCP);
//}

void MainWindow::closeEvent(QCloseEvent *event){
    tcpClient.abort();
}

void MainWindow::ShowTerminalMessage(QTextEdit* Edit, QString Message){
    //Set Log Window color
    Edit->setStyleSheet("QTextEdit {background-color: black;}");
    Edit->setTextColor(Qt::white);
    //Output to log window
    Edit->append(Message);
}

void MainWindow::ShowTerminalErrorMessage(QTextEdit* Edit, QString Message){
    //Set Log Window color
    Edit->setStyleSheet("QTextEdit {background-color: black;}");
    Edit->setTextColor(Qt::red);
    //Output to log window
    Edit->append(Message);
}

void MainWindow::ShowTerminalAssertMessage(QTextEdit* Edit, QString Message){
    //Set Log Window color
    Edit->setStyleSheet("QTextEdit {background-color: black;}");
    Edit->setTextColor(Qt::green);
    //Output to log window
    Edit->append(Message);
}

void MainWindow::stateChanged(Qt::ApplicationState newState) {
    if ((newState==Qt::ApplicationSuspended)||(newState==Qt::ApplicationInactive)){
//        qApp->exit();
    }
    QApplication::processEvents();
}

#ifdef Q_OS_ANDROID
void MainWindow::keep_screen_on(bool on) {
  QtAndroid::runOnAndroidThread([on]{
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
      QAndroidJniObject window =
          activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

      if (window.isValid()) {
        const int FLAG_KEEP_SCREEN_ON = 128;
        if (on) {
          window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        } else {
          window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        }
      }
    }
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
      env->ExceptionClear();
    }
  });
}
#endif


