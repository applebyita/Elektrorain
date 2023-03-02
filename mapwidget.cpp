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

#include "mapwidget.h"
#include "ui_mapwidget.h"
#include <QObject>
#include <QQuickItem>
#include <QDir>
#include <QStandardPaths>
#include <QtMath>

MapWidget::MapWidget(QString Latitude, QString Longitude, QString Latitude1, QString Longitude1,
                     bool MappaAuto, qint16 AngI, qint16 AngF, quint16 DirezioneGPS, quint16 raggio, float IncNord, float IncEst,
                     QString GPS_Np0, QString GPS_Ep0, QStringList ListaGPS_Np0, QStringList ListaGPS_Ep0, QStringList ListaKML, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapWidget)
{
    ui->setupUi(this);

    //MAP QML
    ContextMappa=ui->quickWidget_Mappa->rootContext();
    ContextMappa->setContextProperty("mainWidget", this);
    QQuickItem* QML_Mappa_Item = qobject_cast<QQuickItem *>(ui->quickWidget_Mappa->rootObject());
    // connect our C++ signal to our QML slot
    // NOTE: if we want to pass an parameter to our QML slot, it has to be
    // a QVariant.
    QObject::connect(this, SIGNAL(ReloadMap()), QML_Mappa_Item, SLOT(reloadMap()));
    QObject::connect(this, SIGNAL(EraseMap()), QML_Mappa_Item, SLOT(eraseMap()));
    QObject::connect(this, SIGNAL(TurnOffMap()), QML_Mappa_Item, SLOT(turnOff()));
    QObject::connect(this, SIGNAL(TurnOnMap()), QML_Mappa_Item, SLOT(turnOn()));

//    // Mappa normale ----------------------------------------------------------------------------------------
//    if(MappaAuto==false){
//        //Inserisci le coordinate (se presenti) nel file html altrimenti pagina html vuota
//        FileHTML.clear();
//        if(Latitude!=""){
//            FileHTML=FileHTMLModel;
//            QString Coordinate=tr("(%1,%2)").arg(Latitude).arg(Longitude);
//            FileHTML.replace("(44.917457,9.774474)",Coordinate);
//            if(Latitude1.toDouble()!=0.0){
//              QString Punto1=tr("{lat: %1, lng: %2}").arg(Latitude1).arg(Longitude1);
//              QString Punto2=tr("{lat: %1, lng: %2}").arg(Latitude).arg(Longitude);
//              FileHTML.replace("{lat: 44.000000, lng: 9.000000}",Punto1);
//              FileHTML.replace("{lat: 44.000001, lng: 9.000001}",Punto2);
//              FileHTML.replace("SprinklerPath.setMap(null);","SprinklerPath.setMap(map);");
//            }
//        }
//        else {
//            FileHTML=FileHTMLBlank;
//            qDebug()<<"PAGINA VUOTA!";
//        }
//    }
//    // Mappa bordo campo -------------------------------------------------------------------------------------
//    else {
//        //Inserisci le coordinate (se presenti) nel file html altrimenti pagina html vuota
//        FileHTML.clear();
//        if(Latitude!=""){
//            FileHTML=FileHTMLModelA;

//            qint16 Angolo=0;
//            Angolo = 270-(DirezioneGPS+AngI);
//            float metri_x=0, metri_y=0;
//            metri_x = qCos(qDegreesToRadians((float)Angolo))*raggio;
//            metri_y = qSin(qDegreesToRadians((float)Angolo))*raggio;
//            double LatI=0, LonI=0;
//            LonI = Longitude.toDouble()+(metri_x*IncEst);
//            LatI = Latitude.toDouble()+(metri_y*IncNord);

//            Angolo = 270-(DirezioneGPS+AngF);
//            metri_x = qCos(qDegreesToRadians((float)Angolo))*raggio;
//            metri_y = qSin(qDegreesToRadians((float)Angolo))*raggio;
//            double LatF=0, LonF=0;
//            LonF = Longitude.toDouble()+(metri_x*IncEst);
//            LatF = Latitude.toDouble()+(metri_y*IncNord);

//            // centerPoint
//            QString Coordinate=tr("(%1,%2)").arg(Latitude).arg(Longitude);
//            FileHTML.replace("(44.917457, 9.774474)",Coordinate);
//            // initPoint
//            if(Latitude1.toDouble()!=0.0){
//                Coordinate=tr("(%1,%2)").arg(Latitude1).arg(Longitude1);
//                FileHTML.replace("(44.917057, 9.774174)",Coordinate);
//                FileHTML.replace("SprinklerPath.setMap(null);","SprinklerPath.setMap(map);");
//            }
//            // startPoint
//            Coordinate=tr("(%1,%2)").arg(LatI, 10, 'f', 6).arg(LonI, 10, 'f', 6);
//            FileHTML.replace("(44.917457, 9.774074)",Coordinate);
//            // endPoint
//            Coordinate=tr("(%1,%2)").arg(LatF, 10, 'f', 6).arg(LonF, 10, 'f', 6);
//            FileHTML.replace("(44.917457, 9.774874)",Coordinate);

//            // POLIGONO
//            // Ricerca campo tramite il primo punto restituito dal getto
//            qint8 indiceN=-1;
//            qint8 indiceE=-1;
//            foreach (QString var, ListaGPS_Np0) {
//              if (var == GPS_Np0) {indiceN = ListaGPS_Np0.indexOf(var); break;}
//            }
//            foreach (QString var, ListaGPS_Ep0) {
//              if (var == GPS_Ep0) {indiceE = ListaGPS_Ep0.indexOf(var); break;}
//            }
//            qint8 SelectedField=-1;
//            if((indiceN>=0)&&(indiceN==indiceE)) SelectedField = indiceN;

//            if(SelectedField>=0){
//                FieldDirectory=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/MAPPE"));
//                FileKML.setFileName(FieldDirectory + "/" + ListaKML.at(SelectedField));
//                FileKML.open(QFile::ReadOnly);
//                QString line = "";
//                while(!(line.contains("<coordinates>"))){
//                    line = FileKML.readLine();
//                }
//                line = FileKML.readLine();
//                FileKML.close();

//                QString CoordPoly = "";
//                QStringList splitP = line.split(" ");
//                quint8 NPunti = splitP.length()-1;
//                for (quint8 i=0; i<NPunti; i++) {
//                    QStringList splitC = splitP[i].split(",");
//                    CoordPoly = CoordPoly + "{ lat: " + splitC[1] + ", lng: " + splitC[0] + " },\n";
//                }
//                FileHTML.replace("{ lat: 0.00000000000000, lng: 0.000000000000000 },",CoordPoly);
//                FileHTML.replace("BordoPoly.setMap(null);","BordoPoly.setMap(map);");
//            }
//        }
//        else {
//            FileHTML=FileHTMLBlank;
//            qDebug()<<"PAGINA VUOTA!";
//        }
//    }

//    // PROBLEMA WEBVIEW --> Apertura HTML da file
//    QDir ProjectPath=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
//    QFile FileMap;
//    FileMap.setFileName(ProjectPath.absolutePath()+QString("/Map.html"));
//    FileMap.open(QFile::WriteOnly|QFile::Truncate);
//    FileMap.write(FileHTML.toUtf8());
//    FileMap.close();

//    PercorsoMap = QString("file:///"+QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)+"/Map.html");


    //*******************************************************************************************************
    //******* Chiamata al file Map.php online ***************************************************************
    //*******************************************************************************************************

    // Mappa normale ----------------------------------------------------------------------------------------
    if(MappaAuto==false){
        PercorsoMap = QString("http://sitoweb.com/Map.php");
        if(Latitude!=""){
            PercorsoMap = PercorsoMap + QString("?Lat=%1").arg(Latitude) + QString("&Lon=%1").arg(Longitude);
            if(Latitude1.toDouble()!=0.0){
              PercorsoMap = PercorsoMap + QString("&Lat1=%1").arg(Latitude1) + QString("&Lon1=%1").arg(Longitude1);
            }
            else {
              PercorsoMap = PercorsoMap + QString("&Lat1=0") + QString("&Lon1=0");
            }
        }
        PercorsoMap = PercorsoMap + QString("&Auto=0");
    }
    // Mappa bordo campo -------------------------------------------------------------------------------------
    else {
        PercorsoMap = QString("http://sitoweb.com/Map.php");
        if(Latitude!=""){
            qint16 Angolo=0;
            Angolo = 270-(DirezioneGPS+AngI);
            float metri_x=0, metri_y=0;
            metri_x = qCos(qDegreesToRadians((float)Angolo))*raggio;
            metri_y = qSin(qDegreesToRadians((float)Angolo))*raggio;
            double LatI=0, LonI=0;
            LonI = Longitude.toDouble()+(metri_x*IncEst);
            LatI = Latitude.toDouble()+(metri_y*IncNord);

            Angolo = 270-(DirezioneGPS+AngF);
            metri_x = qCos(qDegreesToRadians((float)Angolo))*raggio;
            metri_y = qSin(qDegreesToRadians((float)Angolo))*raggio;
            double LatF=0, LonF=0;
            LonF = Longitude.toDouble()+(metri_x*IncEst);
            LatF = Latitude.toDouble()+(metri_y*IncNord);

            // centerPoint
            PercorsoMap = PercorsoMap + QString("?Lat=%1").arg(Latitude) + QString("&Lon=%1").arg(Longitude);
            // initPoint
            if(Latitude1.toDouble()!=0.0){
              PercorsoMap = PercorsoMap + QString("&Lat1=%1").arg(Latitude1) + QString("&Lon1=%1").arg(Longitude1);
            }
            else {
              PercorsoMap = PercorsoMap + QString("&Lat1=0") + QString("&Lon1=0");
            }
            PercorsoMap = PercorsoMap + QString("&Auto=1");
            // startPoint
            PercorsoMap = PercorsoMap + QString("&start=(%1,%2)").arg(LatI, 10, 'f', 6).arg(LonI, 10, 'f', 6);
            // endPoint
            PercorsoMap = PercorsoMap + QString("&end=(%1,%2)").arg(LatF, 10, 'f', 6).arg(LonF, 10, 'f', 6);

            // POLIGONO
            // Ricerca campo tramite il primo punto restituito dal getto
            qint8 indiceN=-1;
            qint8 indiceE=-1;
            foreach (QString var, ListaGPS_Np0) {
              if (var == GPS_Np0) {indiceN = ListaGPS_Np0.indexOf(var); break;}
            }
            foreach (QString var, ListaGPS_Ep0) {
              if (var == GPS_Ep0) {indiceE = ListaGPS_Ep0.indexOf(var); break;}
            }
            qint8 SelectedField=-1;
            if((indiceN>=0)&&(indiceN==indiceE)) SelectedField = indiceN;

            if(SelectedField>=0){
                PercorsoMap = PercorsoMap + QString("&Bordo=1");

                FieldDirectory=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/MAPPE"));
                FileKML.setFileName(FieldDirectory + "/" + ListaKML.at(SelectedField));
                FileKML.open(QFile::ReadOnly);
                QString line = "";
                while(!(line.contains("<coordinates>"))){
                    line = FileKML.readLine();
                }
                // Google Earth
                if(line.length()<50){
                    line = FileKML.readLine();
                }
                // Fields Area Measure
                else {
                    QStringList splitL1 = line.split(">");
                    line = splitL1[1];
                    QStringList splitL2 = line.split("<");
                    line = splitL2[0];
                }

                FileKML.close();

                QString CoordPoly = "";
                QStringList splitP = line.split(" ");
                quint8 NPunti = splitP.length()-1;
                for (quint8 i=0; i<NPunti; i++) {
                    QStringList splitC = splitP[i].split(",");
                    CoordPoly = CoordPoly + "{ lat: " + splitC[1] + ", lng: " + splitC[0] + " },\n";
                    PercorsoMap = PercorsoMap + QString("&Np[]=%1").arg(splitC[1].toDouble(), 10, 'f', 6) + QString("&Ep[]=%1").arg(splitC[0].toDouble(), 10, 'f', 6);
                }
            }
            else {
                PercorsoMap = PercorsoMap + QString("&Bordo=0");
            }
        }
    }

    PercorsoMap.replace(" ","");
    qDebug()<<PercorsoMap;

    ContextMappa->setContextProperty("mainWidget", this);
    emit FileHTMLChanged();
    QApplication::processEvents();
    emit ReloadMap();

}

MapWidget::~MapWidget()
{
    delete ui;
}
