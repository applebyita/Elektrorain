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

#include "widget_bordo.h"
#include "ui_widget_bordo.h"
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QStandardPaths>
#include <QtMath>
#include <QtAndroid>
#include <QFileDialog>

WidgetBordo::WidgetBordo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetBordo)
{
    ui->setupUi(this);

    ui->lineEdit_Field->installEventFilter(this);
    ui->lineEdit_Boccaglio->installEventFilter(this);
    ui->lineEdit_OffsetM->installEventFilter(this);
    ui->lineEdit_DeltaAng->installEventFilter(this);
    ui->label_DrawWind->installEventFilter(this);

    CaricaField();

}

WidgetBordo::~WidgetBordo()
{
    delete ui;
}

void WidgetBordo::resizeEvent(QResizeEvent *event){

    QWidget::resizeEvent(event);

    // Dimensioni font e icone
    int Height;
#ifndef Q_OS_IOS
    Height = this->fontMetrics().height();
#else
    Height = this->fontMetrics().height()*0.85;
#endif
    QFont FontLabel=qApp->font();
    FontLabel.setPixelSize(Height);
    FontLabel.setBold(false);
    ui->label_Boccaglio->setFont(FontLabel);
    ui->label_Field->setFont(FontLabel);

//    FontLabel.setPixelSize(Height*0.8);
//    FontLabel.setBold(false);
//    ui->label_VelUnit->setFont(FontLabel);

    FontLabel.setPixelSize(Height*1.2);
    FontLabel.setBold(true);
    ui->lineEdit_Field->setFont(FontLabel);
    ui->lineEdit_Boccaglio->setFont(FontLabel);
    ui->lineEdit_OffsetM->setFont(FontLabel);
    ui->lineEdit_DeltaAng->setFont(FontLabel);

    qDebug() << "Height:" << ui->pushButton_Piu->height();

    QTimer::singleShot(10,this,SLOT(ResizeButtons()));

}

void WidgetBordo::ResizeButtons()
{
    qDebug() << "Height:" << ui->pushButton_Piu->height();

    QSize IconSizes;
    IconSizes.setHeight(ui->pushButton_Import->height());
    IconSizes.setWidth(ui->pushButton_Import->width());
    ui->pushButton_Import->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_Erase->height());
    IconSizes.setWidth(ui->pushButton_Erase->width());
    ui->pushButton_Erase->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_Meno->height());
    IconSizes.setWidth(ui->pushButton_Meno->width());
    ui->pushButton_Meno->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_Update->height());
    IconSizes.setWidth(ui->pushButton_Update->width());
    ui->pushButton_Update->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_Piu->height());
    IconSizes.setWidth(ui->pushButton_Piu->width());
    ui->pushButton_Piu->setIconSize(IconSizes);

}

void WidgetBordo::RefreshBordo(){

    // Pagina senza dati
    if(!DatiAgg){
        ui->label_Boccaglio->setFocus();
        ui->lineEdit_Boccaglio->setText("");
        ui->lineEdit_OffsetM->setText("");
        ui->lineEdit_DeltaAng->setText("");
        ui->pushButton_Import->setEnabled(false);
        ui->pushButton_Erase->setEnabled(false);
        ui->pushButton_Meno->setEnabled(false);
        ui->pushButton_Piu->setEnabled(false);
        ui->pushButton_Update->setEnabled(false);

        ui->label_Field->setText(tr("Field Name  0/0"));
        ui->lineEdit_Field->setText("");
    }
    // Pagina con dati
    else {
        ui->label_Boccaglio->setFocus();
        ui->lineEdit_Boccaglio->setText(tr("%1").arg(Boccaglio));
        ui->lineEdit_OffsetM->setText(tr("%1").arg(OffsetM));
        ui->lineEdit_DeltaAng->setText(tr("%1").arg(GradiAper));

        ui->pushButton_Import->setEnabled(true);
        ui->pushButton_Erase->setEnabled(true);
        ui->pushButton_Meno->setEnabled(true);
        ui->pushButton_Piu->setEnabled(true);
        ui->pushButton_Update->setEnabled(true);

        // Disegno settori
        if(ui->label_DrawDeltaAng->width()>=ui->label_DrawDeltaAng->height()) lato_arco=ui->label_DrawDeltaAng->height();
        else lato_arco=ui->label_DrawDeltaAng->width();

        int DimLine;
#ifndef Q_OS_IOS
        DimLine = 3;
#else
        DimLine = 1;
#endif

        static QImage ArcoGetto12(lato_arco, lato_arco, QImage::Format_ARGB32);
        ArcoGetto12.fill(Qt::transparent);
        QPainter painter12 (&ArcoGetto12);
        QPen pen12(Qt::blue, DimLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter12.setRenderHint(QPainter::Antialiasing);
        painter12.setPen(pen12);
        painter12.setBrush(Qt::cyan);
        QRectF rectangle12(2, 2, ArcoGetto12.width()-4, ArcoGetto12.height()-4);
        int startAngle12=0;
        int spanAngle12=0;
        startAngle12=(270-270-GradiAper) * 16;
        spanAngle12 = (270-90+(GradiAper*2)) * 16;

        painter12.drawPie(rectangle12, startAngle12, spanAngle12);
        QPixmap Img12(ui->label_DrawDeltaAng->width(),ui->label_DrawDeltaAng->height());
        Img12.fill(Qt::transparent);
        QPainter painterTot12 (&Img12);
        painterTot12.drawPixmap((ui->label_DrawDeltaAng->width()-ArcoGetto12.width())/2, (ui->label_DrawDeltaAng->height()-ArcoGetto12.height())/2, QPixmap::fromImage(ArcoGetto12));
        ui->label_DrawDeltaAng->setPixmap(Img12);

        // Disegno offset
        QPixmap ImgBack=QPixmap(tr(":/Resources/wind_back.png"));
        float scala = ((float)ui->label_DrawWind->height())/ImgBack.height();
        int heightB = ui->label_DrawWind->height();
        int widthB = ImgBack.width()*scala;

        QPixmap ImgCursor=QPixmap(tr(":/Resources/wind_cursor.png"));
        //rotate pixmap
        QMatrix rm;
        rm.rotate(OffsetDir);
        int pxw = ImgCursor.width(), pxh = ImgCursor.height();
        ImgCursor = ImgCursor.transformed(rm);
        ImgCursor = ImgCursor.copy((ImgCursor.width() - pxw)/2, (ImgCursor.height() - pxh)/2, pxw, pxh);
        int heightC = ImgCursor.height()*scala;
        int widthC = ImgCursor.width()*scala;

        QPixmap ImgTotal(ui->label_DrawWind->width(),ui->label_DrawWind->height());
        ImgTotal.fill(Qt::transparent);
        QPainter painterTot (&ImgTotal);
        painterTot.drawPixmap((ui->label_DrawWind->width()-widthB)/2,0, ImgBack.scaled(widthB, heightB));
        painterTot.drawPixmap((ui->label_DrawWind->width()-widthC)/2,(ui->label_DrawWind->height()-heightC)/2,ImgCursor.scaled(widthC, heightC));
        ui->label_DrawWind->setPixmap(ImgTotal);

        if(ListaField.count()!=0){
            if(!Field_changed){
                // Ricerca campo tramite il primo punto restituito dal getto
                quint8 indiceN=0;
                quint8 indiceE=0;
                foreach (QString var, ListaGPS_Np0) {
                  if (var == GPS_Np0) {indiceN = ListaGPS_Np0.indexOf(var); break;}
                }
                foreach (QString var, ListaGPS_Ep0) {
                  if (var == GPS_Ep0) {indiceE = ListaGPS_Ep0.indexOf(var); break;}
                }
                if(indiceN==indiceE) SelectedField = indiceN; else SelectedField=0;
            }

            ui->label_Field->setText(tr("Field Name  %1/%2").arg(SelectedField+1).arg(ListaField.count()));
            ui->lineEdit_Field->setText(ListaField.at(SelectedField));
            ui->pushButton_Erase->setEnabled(true);
        }
        else {
            ui->label_Field->setText(tr("Field Name  0/0"));
            ui->lineEdit_Field->setText("");
            ui->pushButton_Erase->setEnabled(false);
        }
    }

}

void WidgetBordo::Update_OK(){

    Field_sel=false;
    Boccaglio_sel=false;
    OffsetM_sel=false;
    OffsetDir_sel=false;
    GradiAper_sel=false;
    Field_changed=false;
    Boccaglio_changed=false;
    OffsetM_changed=false;
    OffsetDir_changed=false;
    GradiAper_changed=false;
    ui->lineEdit_Field->setStyleSheet(Stile_Normale);
    ui->lineEdit_Boccaglio->setStyleSheet(Stile_Normale);
    ui->lineEdit_OffsetM->setStyleSheet(Stile_Normale);
    ui->lineEdit_DeltaAng->setStyleSheet(Stile_Normale);
    ui->label_DrawWind->setStyleSheet(StileL_Normale);

    ui->pushButton_Update->setStyleSheet(StileB_Normale);
}

bool WidgetBordo::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        Field_sel=false;
        if(Field_changed) ui->lineEdit_Field->setStyleSheet(Stile_Mod);
        else ui->lineEdit_Field->setStyleSheet(Stile_Normale);
        Boccaglio_sel=false;
        if(Boccaglio_changed) ui->lineEdit_Boccaglio->setStyleSheet(Stile_Mod);
        else ui->lineEdit_Boccaglio->setStyleSheet(Stile_Normale);
        OffsetM_sel=false;
        if(OffsetM_changed) ui->lineEdit_OffsetM->setStyleSheet(Stile_Mod);
        else ui->lineEdit_OffsetM->setStyleSheet(Stile_Normale);
        OffsetDir_sel=false;
        if(OffsetDir_changed) ui->label_DrawWind->setStyleSheet(StileL_Mod);
        else ui->label_DrawWind->setStyleSheet(StileL_Normale);
        GradiAper_sel=false;
        if(GradiAper_changed) ui->lineEdit_DeltaAng->setStyleSheet(Stile_Mod);
        else ui->lineEdit_DeltaAng->setStyleSheet(Stile_Normale);

        if (object == ui->lineEdit_Field)
        {
            Field_sel=true; Field_changed=true;
            ui->lineEdit_Field->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_Boccaglio)
        {
            Boccaglio_sel=true;
            ui->lineEdit_Boccaglio->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_OffsetM)
        {
            OffsetM_sel=true;
            ui->lineEdit_OffsetM->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->label_DrawWind)
        {
            OffsetDir_sel=true;
            ui->label_DrawWind->setStyleSheet(StileL_Selez);
        }
        else if (object == ui->lineEdit_DeltaAng)
        {
            GradiAper_sel=true;
            ui->lineEdit_DeltaAng->setStyleSheet(Stile_Selez);
        }
    }
    else  if (event->type() == QEvent::FocusOut)
    {

    }

    return false;
}


void WidgetBordo::on_pushButton_Meno_clicked()
{
    if(Field_sel){
        if(SelectedField>0) SelectedField--;
        Field_changed=true;
    }
    else if(Boccaglio_sel){
        if(Boccaglio>14) Boccaglio-=2;
        Boccaglio_changed=true;
    }
    else if(OffsetM_sel){
        if(OffsetM>0) OffsetM--;
        OffsetM_changed=true;
    }
    else if(OffsetDir_sel){
        OffsetDir-=5;
        if(OffsetDir<0) OffsetDir=OffsetDir+360;
        OffsetDir_changed=true;
    }
    else if(GradiAper_sel){
        if(GradiAper>0) GradiAper--;
        GradiAper_changed=true;
    }
    RefreshBordo();
}

void WidgetBordo::on_pushButton_Piu_clicked()
{
    if(Field_sel){
        if(SelectedField<(ListaField.count()-1)) SelectedField++;
        Field_changed=true;
    }
    else if(Boccaglio_sel){
        if(Boccaglio<42) Boccaglio+=2;
        Boccaglio_changed=true;
    }
    else if(OffsetM_sel){
        if(OffsetM<50) OffsetM++;
        OffsetM_changed=true;
    }
    else if(OffsetDir_sel){
        OffsetDir+=5;
        if(OffsetDir>360) OffsetDir=OffsetDir-360;
        OffsetDir_changed=true;
    }
    else if(GradiAper_sel){
        if(GradiAper<60) GradiAper++;
        GradiAper_changed=true;
    }
    RefreshBordo();
}

void WidgetBordo::on_pushButton_Update_clicked()
{
    ui->pushButton_Update->setStyleSheet(StileB_Mod);
    if((Field_changed)||(Boccaglio_changed)) {

        // Estraggo il poligono dal file KML
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

        QStringList splitP = line.split(" ");
        NPunti = splitP.length()-1;

        // Ricerca min e max poligono
        double MinX = 180;
        double MaxX = 0;
        double MinY = 180;
        double MaxY = 0;
        for (quint8 i=0; i<NPunti; i++) {
            QStringList splitC = splitP[i].split(",");
            double VertXj = splitC[0].toDouble();
            double VertYj = splitC[1].toDouble();
            if(VertXj < MinX) MinX = VertXj;
            if(VertXj > MaxX) MaxX = VertXj;
            if(VertYj < MinY) MinY = VertYj;
            if(VertYj > MaxY) MaxY = VertYj;

            GPS_Np[i]=VertYj;
            GPS_Ep[i]=VertXj;
        }

        // Calcolo i gradi corrispondenti a 1 metro a quelle coordinate
        double deltam;
        deltam = distance(MinY, MinX, MinY+0.0001, MinX);
        IncNord = 0.0001/deltam;
        deltam = distance(MinY, MinX, MinY, MinX+0.0001);
        IncEst = 0.0001/deltam;

        emit update_bordo();
    }
    else emit update_dati();
}

// Calcolo distanza in metri tra due coordinate GPS
double WidgetBordo::distance(double lat1, double lon1, double lat2, double lon2){
    double theta, dist;
    if((lat1 == lat2)&&(lon1 == lon2)) return 0;
    else {
        theta = lon1 - lon2;
        dist = qSin(qDegreesToRadians(lat1)) * qSin(qDegreesToRadians(lat2)) + qCos(qDegreesToRadians(lat1)) * qCos(qDegreesToRadians(lat2)) * qCos(qDegreesToRadians(theta));
        dist = qAcos(dist);
        dist = qRadiansToDegrees(dist);
        dist = dist * 60 * 1.1515;
        dist = dist * 1.609344 * 1000;
        return dist;
    }

}

//------------------------------------------------------------------------------------------------------------------------------------------

void WidgetBordo::CaricaField()
{
    ListaKML.clear();
    ListaField.clear();
    ListaGPS_Np0.clear();
    ListaGPS_Ep0.clear();

    // Carico la lista campi e i primi punti di ogni campo
    FieldDirectory=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/MAPPE"));
    ProjectPath.setPath(FieldDirectory);
    if (!ProjectPath.exists()) {
        ProjectPath.mkpath(FieldDirectory);
    }

    ListaKML = QDir(FieldDirectory).entryList(QStringList() << "*.kml" << "*.KML",QDir::Files);

    if(ListaKML.count()==0) return;

    foreach(QString filename, ListaKML) {
        QString NomeF = filename.section('.', 0, 0, QString::SectionSkipEmpty);
        ListaField.append(NomeF);

        FieldDirectory=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/MAPPE"));
        FileKML.setFileName(FieldDirectory + "/" + filename);
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

        QStringList splitP = line.split(" ");
        double coord;
        coord = splitP[0].section(',', 0, 0, QString::SectionSkipEmpty).toDouble();
        ListaGPS_Ep0.append(QString("%1").arg(coord, 10, 'f', 6));
        coord = splitP[0].section(',', 1, 1, QString::SectionSkipEmpty).toDouble();
        ListaGPS_Np0.append(QString("%1").arg(coord, 10, 'f', 6));
    }

}

void WidgetBordo::on_pushButton_Import_clicked()
{
    ListaKML.clear();

#ifdef Q_OS_ANDROID

    // Fino ad Android 10 (SDK 29)
    if(QtAndroid::androidSdkVersion()<30){
        FieldDirectory = QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    }
    // Da Android 11 (SDK 30)
    else {
        FieldDirectory = QFileDialog::getExistingDirectory(this);
    }

#elif defined Q_OS_IOS

    FieldDirectory = QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

#endif

    ListaKML = QDir(FieldDirectory).entryList(QStringList() << "*.kml" << "*.KML",QDir::Files);

    if(ListaKML.count()!=0){
        foreach(QString filename, ListaKML) {
            FileKML.setFileName(FieldDirectory + "/" + filename);
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

            QStringList splitP = line.split(" ");
            NPunti = splitP.length()-1;

            if(NPunti<=30){
                if (QFile::exists(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/MAPPE/") + filename))
                {
                    QFile::remove(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/MAPPE/") + filename);
                }
 //               FileKML.rename(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/MAPPE/") + filename);
                QFile::copy(FileKML.fileName(), QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/MAPPE/") + filename);
                FileKML.remove();
            }
            else FileKML.remove();
        }
    }

    SelectedField=0;
    CaricaField();
    RefreshBordo();
}

void WidgetBordo::on_pushButton_Erase_clicked()
{
    FieldDirectory=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/MAPPE"));
    FileKML.setFileName(FieldDirectory + "/" + ListaKML.at(SelectedField));

    FileKML.remove();

    SelectedField=0;
    CaricaField();
    RefreshBordo();
}
