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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QTcpSocket>
#include <QTextEdit>
#include <QDir>
#include <QListWidget>
#include <QTimer>
#include <QQmlContext>
#include "getto.h"
#include "mapwidget.h"

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow() override;

signals:
    void default_page_prog();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

public slots:
    void stateChanged(Qt::ApplicationState newState);

private slots:
    void on_listWidget_Getti_itemClicked(QListWidgetItem *item);

    void on_pushButton_Connect_clicked();
    void on_pushButton_Back_clicked();
    void on_pushButton_Logo_clicked();
    void on_pushButton_Settings_clicked();
    void on_pushButton_Elenco_clicked();
    void on_pushButton_Stato_clicked();
    void on_pushButton_Prog_clicked();
    void on_pushButton_Mappa_clicked();

    void IncomingData();
    void TCPConnesso();
    void TCPDisconnesso();
    void UpdateStato();
    void ChangeProg();
    void ChangeManAut();
    void ReverseSave();
    void ReverseNotSave();
    void UpdateProg();
    void CaricaMappa();
//    void InvioKeepAlive();
    void PaginaRompigetto();
    void PaginaAuto();
    void UpdateBordo();
    void UpdateDati();

    void on_lineEdit_ID1_editingFinished();
    void on_lineEdit_ID2_editingFinished();
    void on_lineEdit_ID3_editingFinished();
    void on_lineEdit_ID4_editingFinished();
    void on_lineEdit_ID5_editingFinished();
    void on_lineEdit_ID6_editingFinished();
    void on_lineEdit_ID7_editingFinished();
    void on_lineEdit_ID8_editingFinished();
    void on_lineEdit_ID9_editingFinished();
    void on_lineEdit_ID10_editingFinished();

    void on_lineEdit_ID1_textEdited(const QString &arg1);
    void on_lineEdit_ID2_textEdited(const QString &arg1);
    void on_lineEdit_ID3_textEdited(const QString &arg1);
    void on_lineEdit_ID4_textEdited(const QString &arg1);
    void on_lineEdit_ID5_textEdited(const QString &arg1);
    void on_lineEdit_ID6_textEdited(const QString &arg1);
    void on_lineEdit_ID7_textEdited(const QString &arg1);
    void on_lineEdit_ID8_textEdited(const QString &arg1);
    void on_lineEdit_ID9_textEdited(const QString &arg1);
    void on_lineEdit_ID10_textEdited(const QString &arg1);

    void on_stackedWidget_App_currentChanged(int arg1);

    void on_pushButton_ConnType_clicked();

    void on_pushButton_Alza_clicked();

    void on_pushButton_Abbassa_clicked();

    void InvioHTTP(int IndiceMacchina);
    void InvioHTTPdata(int IndiceMacchina);
    void startRequest(const QUrl &requestedUrl);
    void httpReadyRead();
    void httpFinished();
    void HTTPTimeout();

    void on_pushButton_Indietro_clicked();

    void on_pushButton_Info_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *timerInitTCP = new QTimer(this);
    QTimer *timerDatoTCP = new QTimer(this);
    QTimer *timerKeepAlive = new QTimer(this);
    QTimer *timerHTML = new QTimer(this);

    QList<Getto_t*> ListaGetti;

    QUrl                  url;
    QNetworkAccessManager qnam;
    QNetworkReply*        reply;
    quint8                StepHTTP=0;
    bool                  BypassHTTP=false;

    unsigned char ContatoreResize=0;
    int IndiceGetto=-1;
    unsigned char TCPconnected=OFF_LINE;
    bool MQTTprotocol=true, VisualizzaSIM=false, PaginaBonus[10];

    QString GPS_Nord, GPS_Est, GPS_Nord_1, GPS_Est_1, GPS_Np0, GPS_Ep0;
    bool        MappaAuto=false;
    qint16      AngI=0;
    qint16      AngF=0;
    quint16     DirezioneGPS=0;
    quint16     raggio=0;
    float       IncNord=0, IncEst=0;

    // variabili uso ini
    QDir            ProjectPath;

    // Client TCP
    QTcpSocket tcpClient;

    QString ComandoTCP, RispostaTCP;

    QByteArray BufferTX_MQTT, BufferRX_MQTT;

    void Publish_MQTT();

    // Stili per check IP
    QString Stile_OK  = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: white;}";
    QString Stile_ERR = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: red;}";


    void Crea_Lista_Getti();
    void Refresh_Lista_Getti();

    void ShowTerminalMessage(QTextEdit* Edit, QString Message);
    void ShowTerminalErrorMessage(QTextEdit* Edit, QString Message);
    void ShowTerminalAssertMessage(QTextEdit* Edit, QString Message);

    MapWidget*  Mappa=nullptr;

#ifdef Q_OS_ANDROID
    void keep_screen_on(bool on);
#endif

};

#endif // MAINWINDOW_H
