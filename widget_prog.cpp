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

#include "widget_prog.h"
#include "ui_widget_prog.h"
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QStandardPaths>
#include <QMessageBox>
#include <QtAndroid>
#include <QFileDialog>


WidgetProg::WidgetProg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetProg)
{
    ui->setupUi(this);

    ui->lineEdit_Ang1->installEventFilter(this);
    ui->lineEdit_Ang2->installEventFilter(this);
    ui->lineEdit_Ang3->installEventFilter(this);
    ui->lineEdit_Ang4->installEventFilter(this);
    ui->lineEdit_Ang5->installEventFilter(this);
    ui->lineEdit_Ang6->installEventFilter(this);
    ui->lineEdit_Ang7->installEventFilter(this);
    ui->lineEdit_Ang8->installEventFilter(this);
    ui->lineEdit_T12hh->installEventFilter(this);
    ui->lineEdit_T12mm->installEventFilter(this);
    ui->lineEdit_T34hh->installEventFilter(this);
    ui->lineEdit_T34mm->installEventFilter(this);
    ui->lineEdit_T56hh->installEventFilter(this);
    ui->lineEdit_T56mm->installEventFilter(this);

    CaricaProg();

}

WidgetProg::~WidgetProg()
{
    delete ui;
}

void WidgetProg::resizeEvent(QResizeEvent *event){

    QWidget::resizeEvent(event);

    // Dimensioni font e icone
    int Height;
#ifndef Q_OS_IOS
    Height = this->fontMetrics().height();
#else
    Height = this->fontMetrics().height()*0.85;
#endif
    FontLabel=qApp->font();
    FontLabel.setPixelSize(Height);
    FontLabel.setBold(false);
    ui->label_Ang1->setFont(FontLabel);
    ui->label_Ang2->setFont(FontLabel);
    ui->label_T12->setFont(FontLabel);
    ui->label_Ang3->setFont(FontLabel);
    ui->label_Ang4->setFont(FontLabel);
    ui->label_T34->setFont(FontLabel);
    ui->label_Ang5->setFont(FontLabel);
    ui->label_Ang6->setFont(FontLabel);
    ui->label_T56->setFont(FontLabel);
    ui->label_Ang7->setFont(FontLabel);
    ui->label_Ang8->setFont(FontLabel);

    FontLineEdit.setPixelSize(Height*1.2);
    FontLineEdit.setBold(true);
    ui->lineEdit_Ang1->setFont(FontLineEdit);
    ui->lineEdit_Ang2->setFont(FontLineEdit);
    ui->lineEdit_T12hh->setFont(FontLineEdit);
    ui->lineEdit_T12mm->setFont(FontLineEdit);
    ui->lineEdit_Ang3->setFont(FontLineEdit);
    ui->lineEdit_Ang4->setFont(FontLineEdit);
    ui->lineEdit_T34hh->setFont(FontLineEdit);
    ui->lineEdit_T34mm->setFont(FontLineEdit);
    ui->lineEdit_Ang5->setFont(FontLineEdit);
    ui->lineEdit_Ang6->setFont(FontLineEdit);
    ui->lineEdit_T56hh->setFont(FontLineEdit);
    ui->lineEdit_T56mm->setFont(FontLineEdit);
    ui->lineEdit_Ang7->setFont(FontLineEdit);
    ui->lineEdit_Ang8->setFont(FontLineEdit);

//    QSize IconSizes;
//    IconSizes.setHeight(ui->pushButton_P1->height());
//    IconSizes.setWidth(ui->pushButton_P1->width());
//    ui->pushButton_P1->setIconSize(IconSizes);
//    IconSizes.setHeight(ui->pushButton_P2->height());
//    IconSizes.setWidth(ui->pushButton_P2->width());
//    ui->pushButton_P2->setIconSize(IconSizes);
//    IconSizes.setHeight(ui->pushButton_TM->height());
//    IconSizes.setWidth(ui->pushButton_TM->width());
//    ui->pushButton_TM->setIconSize(IconSizes);
//    IconSizes.setHeight(ui->pushButton_LoadSave->height());
//    IconSizes.setWidth(ui->pushButton_LoadSave->width());
//    ui->pushButton_LoadSave->setIconSize(IconSizes);
//    IconSizes.setHeight(ui->pushButton_Rot1234->height());
//    IconSizes.setWidth(ui->pushButton_Rot1234->width());
//    ui->pushButton_Rot1234->setIconSize(IconSizes);
//    IconSizes.setHeight(ui->pushButton_Rot3456->height());
//    IconSizes.setWidth(ui->pushButton_Rot3456->width());
//    ui->pushButton_Rot3456->setIconSize(IconSizes);
//    IconSizes.setHeight(ui->pushButton_Rot5678->height());
//    IconSizes.setWidth(ui->pushButton_Rot5678->width());
//    ui->pushButton_Rot5678->setIconSize(IconSizes);
//    IconSizes.setHeight(ui->pushButton_Meno->height());
//    IconSizes.setWidth(ui->pushButton_Meno->width());
//    ui->pushButton_Meno->setIconSize(IconSizes);
//    IconSizes.setHeight(ui->pushButton_Update->height());
//    IconSizes.setWidth(ui->pushButton_Update->width());
//    ui->pushButton_Update->setIconSize(IconSizes);
//    IconSizes.setHeight(ui->pushButton_Piu->height());
//    IconSizes.setWidth(ui->pushButton_Piu->width());
//    ui->pushButton_Piu->setIconSize(IconSizes);

    qDebug() << "Height:" << ui->pushButton_Piu->height();

    QTimer::singleShot(10,this,SLOT(ResizeButtons()));

}

void WidgetProg::RefreshProg(){

    // Pagina senza dati
    if(ProgSel==0){
        ui->label_Ang1->setFocus();

        QPixmap ImgP1=QPixmap(tr(":/Resources/P1grey.png"));
        ui->pushButton_P1->setIcon(ImgP1);
        QPixmap ImgP2=QPixmap(tr(":/Resources/P2grey.png"));
        ui->pushButton_P2->setIcon(ImgP2);
        QPixmap ImgTM=QPixmap(tr(":/Resources/tempo.png"));
        ui->pushButton_TM->setIcon(ImgTM);

        ui->label_T12->setText("T1-2 (hh:mm)");
        ui->label_T12->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->label_T34->setText("T3-4 (hh:mm)");
        ui->label_T34->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->label_T56->setText("T5-6 (hh:mm)");
        ui->label_T56->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->lineEdit_Ang1->setText("");
        ui->lineEdit_Ang2->setText("");
        ui->lineEdit_T12hh->setText("");
        ui->lineEdit_T12mm->setText(""); ui->lineEdit_T12mm->setVisible(true);
        ui->lineEdit_Ang3->setText("");
        ui->lineEdit_Ang4->setText("");
        ui->lineEdit_T34hh->setText("");
        ui->lineEdit_T34mm->setText(""); ui->lineEdit_T34mm->setVisible(true);
        ui->lineEdit_Ang5->setText("");
        ui->lineEdit_Ang6->setText("");
        ui->lineEdit_T56hh->setText("");
        ui->lineEdit_T56mm->setText(""); ui->lineEdit_T56mm->setVisible(true);
        ui->lineEdit_Ang7->setText("");
        ui->lineEdit_Ang8->setText("");

        QPixmap ImgRot=QPixmap(tr(":/Resources/orario.png"));
        ui->pushButton_Rot1234->setIcon(ImgRot);
        ui->pushButton_Rot3456->setIcon(ImgRot);
        ui->pushButton_Rot5678->setIcon(ImgRot);

        ui->pushButton_Meno->setEnabled(false);
        ui->pushButton_Piu->setEnabled(false);
        ui->pushButton_Update->setEnabled(false);
        ui->pushButton_LoadSave->setEnabled(false);
    }
    // Pagina con dati
    else {
        if(ProgSel==1){
            QPixmap ImgP1=QPixmap(tr(":/Resources/P1.png"));
            ui->pushButton_P1->setIcon(ImgP1);
            QPixmap ImgP2=QPixmap(tr(":/Resources/P2grey.png"));
            ui->pushButton_P2->setIcon(ImgP2);
            if(SelTM_P1==0){
                QPixmap ImgTM=QPixmap(tr(":/Resources/tempo.png"));
                ui->pushButton_TM->setIcon(ImgTM);
                ui->label_T12->setText("T1-2 (hh:mm)");
                ui->label_T12->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                ui->label_T34->setText("T3-4 (hh:mm)");
                ui->label_T34->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                ui->label_T56->setText("T5-6 (hh:mm)");
                ui->label_T56->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                ui->lineEdit_T12mm->setVisible(true);
                ui->lineEdit_T34mm->setVisible(true);
                ui->lineEdit_T56mm->setVisible(true);
            }
            else {
                QPixmap ImgTM=QPixmap(tr(":/Resources/distanza.png"));
                ui->pushButton_TM->setIcon(ImgTM);
                ui->label_T12->setText("M1-2 (m)");
                ui->label_T12->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                ui->label_T34->setText("M3-4 (m)");
                ui->label_T34->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                ui->label_T56->setText("M5-6 (m)");
                ui->label_T56->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                ui->lineEdit_T12mm->setVisible(false);
                ui->lineEdit_T34mm->setVisible(false);
                ui->lineEdit_T56mm->setVisible(false);
            }
            ui->lineEdit_Ang1->setText(tr("%1").arg(Ang1_P1));
            ui->lineEdit_Ang2->setText(tr("%1").arg(Ang2_P1));
            if(SelTM_P1==0){
                ui->lineEdit_T12hh->setText(tr("%1").arg((Tempo12_P1/60), 2, 10, QChar('0')));
                ui->lineEdit_T12mm->setText(tr("%1").arg((Tempo12_P1%60), 2, 10, QChar('0')));
            }
            else {
                ui->lineEdit_T12hh->setText(tr("%1").arg(Metri12_P1));
            }
            ui->lineEdit_Ang3->setText(tr("%1").arg(Ang3_P1));
            ui->lineEdit_Ang4->setText(tr("%1").arg(Ang4_P1));
            if(SelTM_P1==0){
                ui->lineEdit_T34hh->setText(tr("%1").arg((Tempo34_P1/60), 2, 10, QChar('0')));
                ui->lineEdit_T34mm->setText(tr("%1").arg((Tempo34_P1%60), 2, 10, QChar('0')));
            }
            else {
                ui->lineEdit_T34hh->setText(tr("%1").arg(Metri34_P1));
            }
            ui->lineEdit_Ang5->setText(tr("%1").arg(Ang5_P1));
            ui->lineEdit_Ang6->setText(tr("%1").arg(Ang6_P1));
            if(SelTM_P1==0){
                ui->lineEdit_T56hh->setText(tr("%1").arg((Tempo56_P1/60), 2, 10, QChar('0')));
                ui->lineEdit_T56mm->setText(tr("%1").arg((Tempo56_P1%60), 2, 10, QChar('0')));
            }
            else {
                ui->lineEdit_T56hh->setText(tr("%1").arg(Metri56_P1));
            }
            ui->lineEdit_Ang7->setText(tr("%1").arg(Ang7_P1));
            ui->lineEdit_Ang8->setText(tr("%1").arg(Ang8_P1));

            QPixmap ImgO=QPixmap(tr(":/Resources/orario.png"));
            QPixmap ImgA=QPixmap(tr(":/Resources/antiorario.png"));
            if(Rot1234_P1==1) ui->pushButton_Rot1234->setIcon(ImgO);
            else if(Rot1234_P1==2) ui->pushButton_Rot1234->setIcon(ImgA);
            if(Rot3456_P1==1) ui->pushButton_Rot3456->setIcon(ImgO);
            else if(Rot3456_P1==2) ui->pushButton_Rot3456->setIcon(ImgA);
            if(Rot5678_P1==1) ui->pushButton_Rot5678->setIcon(ImgO);
            else if(Rot5678_P1==2) ui->pushButton_Rot5678->setIcon(ImgA);
        }
        else if(ProgSel==2){
            QPixmap ImgP1=QPixmap(tr(":/Resources/P1grey.png"));
            ui->pushButton_P1->setIcon(ImgP1);
            QPixmap ImgP2=QPixmap(tr(":/Resources/P2.png"));
            ui->pushButton_P2->setIcon(ImgP2);
            if(SelTM_P2==0){
                QPixmap ImgTM=QPixmap(tr(":/Resources/tempo.png"));
                ui->pushButton_TM->setIcon(ImgTM);
                ui->label_T12->setText("T1-2 (hh:mm)");
                ui->label_T12->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                ui->label_T34->setText("T3-4 (hh:mm)");
                ui->label_T34->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                ui->label_T56->setText("T5-6 (hh:mm)");
                ui->label_T56->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                ui->lineEdit_T12mm->setVisible(true);
                ui->lineEdit_T34mm->setVisible(true);
                ui->lineEdit_T56mm->setVisible(true);
            }
            else {
                QPixmap ImgTM=QPixmap(tr(":/Resources/distanza.png"));
                ui->pushButton_TM->setIcon(ImgTM);
                ui->label_T12->setText("M1-2 (m)");
                ui->label_T12->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                ui->label_T34->setText("M3-4 (m)");
                ui->label_T34->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                ui->label_T56->setText("M5-6 (m)");
                ui->label_T56->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                ui->lineEdit_T12mm->setVisible(false);
                ui->lineEdit_T34mm->setVisible(false);
                ui->lineEdit_T56mm->setVisible(false);
            }
            ui->lineEdit_Ang1->setText(tr("%1").arg(Ang1_P2));
            ui->lineEdit_Ang2->setText(tr("%1").arg(Ang2_P2));
            if(SelTM_P2==0){
                ui->lineEdit_T12hh->setText(tr("%1").arg((Tempo12_P2/60), 2, 10, QChar('0')));
                ui->lineEdit_T12mm->setText(tr("%1").arg((Tempo12_P2%60), 2, 10, QChar('0')));
            }
            else {
                ui->lineEdit_T12hh->setText(tr("%1").arg(Metri12_P2));
            }
            ui->lineEdit_Ang3->setText(tr("%1").arg(Ang3_P2));
            ui->lineEdit_Ang4->setText(tr("%1").arg(Ang4_P2));
            if(SelTM_P2==0){
                ui->lineEdit_T34hh->setText(tr("%1").arg((Tempo34_P2/60), 2, 10, QChar('0')));
                ui->lineEdit_T34mm->setText(tr("%1").arg((Tempo34_P2%60), 2, 10, QChar('0')));
            }
            else {
                ui->lineEdit_T34hh->setText(tr("%1").arg(Metri34_P2));
            }
            ui->lineEdit_Ang5->setText(tr("%1").arg(Ang5_P2));
            ui->lineEdit_Ang6->setText(tr("%1").arg(Ang6_P2));
            if(SelTM_P2==0){
                ui->lineEdit_T56hh->setText(tr("%1").arg((Tempo56_P2/60), 2, 10, QChar('0')));
                ui->lineEdit_T56mm->setText(tr("%1").arg((Tempo56_P2%60), 2, 10, QChar('0')));
            }
            else {
                ui->lineEdit_T56hh->setText(tr("%1").arg(Metri56_P2));
            }
            ui->lineEdit_Ang7->setText(tr("%1").arg(Ang7_P2));
            ui->lineEdit_Ang8->setText(tr("%1").arg(Ang8_P2));

            QPixmap ImgO=QPixmap(tr(":/Resources/orario.png"));
            QPixmap ImgA=QPixmap(tr(":/Resources/antiorario.png"));
            if(Rot1234_P2==1) ui->pushButton_Rot1234->setIcon(ImgO);
            else if(Rot1234_P2==2) ui->pushButton_Rot1234->setIcon(ImgA);
            if(Rot3456_P2==1) ui->pushButton_Rot3456->setIcon(ImgO);
            else if(Rot3456_P2==2) ui->pushButton_Rot3456->setIcon(ImgA);
            if(Rot5678_P2==1) ui->pushButton_Rot5678->setIcon(ImgO);
            else if(Rot5678_P2==2) ui->pushButton_Rot5678->setIcon(ImgA);
        }

        ui->pushButton_Meno->setEnabled(true);
        ui->pushButton_Piu->setEnabled(true);
        ui->pushButton_Update->setEnabled(true);
        ui->pushButton_LoadSave->setEnabled(true);

        // Disegno settori
        if(ui->label_Draw12->width()>=ui->label_Draw12->height()) lato_arco=ui->label_Draw12->height();
        else lato_arco=ui->label_Draw12->width();

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
        if(ProgSel==1){
            startAngle12=(270-Ang2_P1) * 16;
            if(Ang2_P1>Ang1_P1) spanAngle12 = (Ang2_P1-Ang1_P1) * 16;
            else spanAngle12=(Ang2_P1-Ang1_P1+360) * 16;
        }
        else if(ProgSel==2){
            startAngle12=(270-Ang2_P2) * 16;
            if(Ang2_P2>Ang1_P2) spanAngle12 = (Ang2_P2-Ang1_P2) * 16;
            else spanAngle12=(Ang2_P2-Ang1_P2+360) * 16;
        }
        painter12.drawPie(rectangle12, startAngle12, spanAngle12);
        QPixmap Img12(ui->label_Draw12->width(),ui->label_Draw12->height());
        Img12.fill(Qt::transparent);
        QPainter painterTot12 (&Img12);
        painterTot12.drawPixmap((ui->label_Draw12->width()-ArcoGetto12.width())/2, (ui->label_Draw12->height()-ArcoGetto12.height())/2, QPixmap::fromImage(ArcoGetto12));
        ui->label_Draw12->setPixmap(Img12);

        static QImage ArcoGetto34(lato_arco, lato_arco, QImage::Format_ARGB32);
        ArcoGetto34.fill(Qt::transparent);
        QPainter painter34 (&ArcoGetto34);
        QPen pen34(Qt::blue, DimLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter34.setRenderHint(QPainter::Antialiasing);
        painter34.setPen(pen34);
        painter34.setBrush(Qt::cyan);
        QRectF rectangle34(2, 2, ArcoGetto12.width()-4, ArcoGetto12.height()-4);
        int startAngle34=0;
        int spanAngle34=0;
        if(ProgSel==1){
            startAngle34=(270-Ang4_P1) * 16;
            if(Ang4_P1>Ang3_P1) spanAngle34 = (Ang4_P1-Ang3_P1) * 16;
            else spanAngle34=(Ang4_P1-Ang3_P1+360) * 16;
        }
        else if(ProgSel==2){
            startAngle34=(270-Ang4_P2) * 16;
            if(Ang4_P2>Ang3_P2) spanAngle34 = (Ang4_P2-Ang3_P2) * 16;
            else spanAngle34=(Ang4_P2-Ang3_P2+360) * 16;
        }
        painter34.drawPie(rectangle34, startAngle34, spanAngle34);
        QPixmap Img34(ui->label_Draw34->width(),ui->label_Draw34->height());
        Img34.fill(Qt::transparent);
        QPainter painterTot34 (&Img34);
        painterTot34.drawPixmap((ui->label_Draw34->width()-ArcoGetto34.width())/2, (ui->label_Draw34->height()-ArcoGetto34.height())/2, QPixmap::fromImage(ArcoGetto34));
        ui->label_Draw34->setPixmap(Img34);

        static QImage ArcoGetto56(lato_arco, lato_arco, QImage::Format_ARGB32);
        ArcoGetto56.fill(Qt::transparent);
        QPainter painter56 (&ArcoGetto56);
        QPen pen56(Qt::blue, DimLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter56.setRenderHint(QPainter::Antialiasing);
        painter56.setPen(pen56);
        painter56.setBrush(Qt::cyan);
        QRectF rectangle56(2, 2, ArcoGetto56.width()-4, ArcoGetto56.height()-4);
        int startAngle56=0;
        int spanAngle56=0;
        if(ProgSel==1){
            startAngle56=(270-Ang6_P1) * 16;
            if(Ang6_P1>Ang5_P1) spanAngle56 = (Ang6_P1-Ang5_P1) * 16;
            else spanAngle56=(Ang6_P1-Ang5_P1+360) * 16;
        }
        else if(ProgSel==2){
            startAngle56=(270-Ang6_P2) * 16;
            if(Ang6_P2>Ang5_P2) spanAngle56 = (Ang6_P2-Ang5_P2) * 16;
            else spanAngle56=(Ang6_P2-Ang5_P2+360) * 16;
        }
        painter56.drawPie(rectangle56, startAngle56, spanAngle56);
        QPixmap Img56(ui->label_Draw56->width(),ui->label_Draw56->height());
        Img56.fill(Qt::transparent);
        QPainter painterTot56 (&Img56);
        painterTot56.drawPixmap((ui->label_Draw56->width()-ArcoGetto56.width())/2, (ui->label_Draw56->height()-ArcoGetto56.height())/2, QPixmap::fromImage(ArcoGetto56));
        ui->label_Draw56->setPixmap(Img56);

        static QImage ArcoGetto78(lato_arco, lato_arco, QImage::Format_ARGB32);
        ArcoGetto78.fill(Qt::transparent);
        QPainter painter78 (&ArcoGetto78);
        QPen pen78(Qt::blue, DimLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter78.setRenderHint(QPainter::Antialiasing);
        painter78.setPen(pen78);
        painter78.setBrush(Qt::cyan);
        QRectF rectangle78(2, 2, ArcoGetto78.width()-4, ArcoGetto78.height()-4);
        int startAngle78=0;
        int spanAngle78=0;
        if(ProgSel==1){
            startAngle78=(270-Ang8_P1) * 16;
            if(Ang8_P1>Ang7_P1) spanAngle78 = (Ang8_P1-Ang7_P1) * 16;
            else spanAngle78=(Ang8_P1-Ang7_P1+360) * 16;
        }
        else if(ProgSel==2){
            startAngle78=(270-Ang8_P2) * 16;
            if(Ang8_P2>Ang7_P2) spanAngle78 = (Ang8_P2-Ang7_P2) * 16;
            else spanAngle78=(Ang8_P2-Ang7_P2+360) * 16;
        }
        painter78.drawPie(rectangle78, startAngle78, spanAngle78);
        QPixmap Img78(ui->label_Draw78->width(),ui->label_Draw78->height());
        Img78.fill(Qt::transparent);
        QPainter painterTot78 (&Img78);
        painterTot78.drawPixmap((ui->label_Draw78->width()-ArcoGetto78.width())/2, (ui->label_Draw78->height()-ArcoGetto78.height())/2, QPixmap::fromImage(ArcoGetto78));
        ui->label_Draw78->setPixmap(Img78);
    }
}

void WidgetProg::Update_OK(){

    Ang1_sel=false;
    Ang2_sel=false;
    Ang3_sel=false;
    Ang4_sel=false;
    Ang5_sel=false;
    Ang6_sel=false;
    Ang7_sel=false;
    Ang8_sel=false;
    Tempo12hh_sel=false;
    Tempo12mm_sel=false;
    Tempo34hh_sel=false;
    Tempo34mm_sel=false;
    Tempo56hh_sel=false;
    Tempo56mm_sel=false;
    Ang1_P1_changed=false; Ang2_P1_changed=false; Ang3_P1_changed=false; Ang4_P1_changed=false;
    Ang5_P1_changed=false; Ang6_P1_changed=false; Ang7_P1_changed=false; Ang8_P1_changed=false;
    Tempo12hh_P1_changed=false; Tempo12mm_P1_changed=false;
    Tempo34hh_P1_changed=false; Tempo34mm_P1_changed=false;
    Tempo56hh_P1_changed=false; Tempo56mm_P1_changed=false;
    Metri12_P1_changed=false; Metri34_P1_changed=false; Metri56_P1_changed=false;
    Rot1234_P1_changed=false; Rot3456_P1_changed=false; Rot5678_P1_changed=false;
    Ang1_P2_changed=false; Ang2_P2_changed=false; Ang3_P2_changed=false; Ang4_P2_changed=false;
    Ang5_P2_changed=false; Ang6_P2_changed=false; Ang7_P2_changed=false; Ang8_P2_changed=false;
    Tempo12hh_P2_changed=false; Tempo12mm_P2_changed=false;
    Tempo34hh_P2_changed=false; Tempo34mm_P2_changed=false;
    Tempo56hh_P2_changed=false; Tempo56mm_P2_changed=false;
    Metri12_P2_changed=false; Metri34_P2_changed=false; Metri56_P2_changed=false;
    Rot1234_P2_changed=false; Rot3456_P2_changed=false; Rot5678_P2_changed=false;
    ui->lineEdit_Ang1->setStyleSheet(Stile_Normale);
    ui->lineEdit_Ang2->setStyleSheet(Stile_Normale);
    ui->lineEdit_Ang3->setStyleSheet(Stile_Normale);
    ui->lineEdit_Ang4->setStyleSheet(Stile_Normale);
    ui->lineEdit_Ang5->setStyleSheet(Stile_Normale);
    ui->lineEdit_Ang6->setStyleSheet(Stile_Normale);
    ui->lineEdit_Ang7->setStyleSheet(Stile_Normale);
    ui->lineEdit_Ang8->setStyleSheet(Stile_Normale);
    ui->lineEdit_T12hh->setStyleSheet(Stile_Normale);
    ui->lineEdit_T12mm->setStyleSheet(Stile_Normale);
    ui->lineEdit_T34hh->setStyleSheet(Stile_Normale);
    ui->lineEdit_T34mm->setStyleSheet(Stile_Normale);
    ui->lineEdit_T56hh->setStyleSheet(Stile_Normale);
    ui->lineEdit_T56mm->setStyleSheet(Stile_Normale);
    ui->pushButton_Rot1234->setStyleSheet(StileB_Normale);
    ui->pushButton_Rot3456->setStyleSheet(StileB_Normale);
    ui->pushButton_Rot5678->setStyleSheet(StileB_Normale);
    ui->pushButton_Update->setStyleSheet(StileB_Normale);
}

bool WidgetProg::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if(ProgSel==1){
            Ang1_sel=false;
            if(Ang1_P1_changed) ui->lineEdit_Ang1->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang1->setStyleSheet(Stile_Normale);
            Ang2_sel=false;
            if(Ang2_P1_changed) ui->lineEdit_Ang2->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang2->setStyleSheet(Stile_Normale);
            Ang3_sel=false;
            if(Ang3_P1_changed) ui->lineEdit_Ang3->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang3->setStyleSheet(Stile_Normale);
            Ang4_sel=false;
            if(Ang4_P1_changed) ui->lineEdit_Ang4->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang4->setStyleSheet(Stile_Normale);
            Ang5_sel=false;
            if(Ang5_P1_changed) ui->lineEdit_Ang5->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang5->setStyleSheet(Stile_Normale);
            Ang6_sel=false;
            if(Ang6_P1_changed) ui->lineEdit_Ang6->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang6->setStyleSheet(Stile_Normale);
            Ang7_sel=false;
            if(Ang7_P1_changed) ui->lineEdit_Ang7->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang7->setStyleSheet(Stile_Normale);
            Ang8_sel=false;
            if(Ang8_P1_changed) ui->lineEdit_Ang8->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang8->setStyleSheet(Stile_Normale);
            Tempo12hh_sel=false;
            if((Tempo12hh_P1_changed)||(Metri12_P1_changed)) ui->lineEdit_T12hh->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T12hh->setStyleSheet(Stile_Normale);
            Tempo12mm_sel=false;
            if(Tempo12mm_P1_changed) ui->lineEdit_T12mm->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T12mm->setStyleSheet(Stile_Normale);
            Tempo34hh_sel=false;
            if((Tempo34hh_P1_changed)||(Metri34_P1_changed)) ui->lineEdit_T34hh->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T34hh->setStyleSheet(Stile_Normale);
            Tempo34mm_sel=false;
            if(Tempo34mm_P1_changed) ui->lineEdit_T34mm->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T34mm->setStyleSheet(Stile_Normale);
            Tempo56hh_sel=false;
            if((Tempo56hh_P1_changed)||(Metri56_P1_changed)) ui->lineEdit_T56hh->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T56hh->setStyleSheet(Stile_Normale);
            Tempo56mm_sel=false;
            if(Tempo56mm_P1_changed) ui->lineEdit_T56mm->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T56mm->setStyleSheet(Stile_Normale);
        }
        else if(ProgSel==2){
            Ang1_sel=false;
            if(Ang1_P2_changed) ui->lineEdit_Ang1->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang1->setStyleSheet(Stile_Normale);
            Ang2_sel=false;
            if(Ang2_P2_changed) ui->lineEdit_Ang2->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang2->setStyleSheet(Stile_Normale);
            Ang3_sel=false;
            if(Ang3_P2_changed) ui->lineEdit_Ang3->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang3->setStyleSheet(Stile_Normale);
            Ang4_sel=false;
            if(Ang4_P2_changed) ui->lineEdit_Ang4->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang4->setStyleSheet(Stile_Normale);
            Ang5_sel=false;
            if(Ang5_P2_changed) ui->lineEdit_Ang5->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang5->setStyleSheet(Stile_Normale);
            Ang6_sel=false;
            if(Ang6_P2_changed) ui->lineEdit_Ang6->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang6->setStyleSheet(Stile_Normale);
            Ang7_sel=false;
            if(Ang7_P2_changed) ui->lineEdit_Ang7->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang7->setStyleSheet(Stile_Normale);
            Ang8_sel=false;
            if(Ang8_P2_changed) ui->lineEdit_Ang8->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Ang8->setStyleSheet(Stile_Normale);
            Tempo12hh_sel=false;
            if((Tempo12hh_P2_changed)||(Metri12_P2_changed)) ui->lineEdit_T12hh->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T12hh->setStyleSheet(Stile_Normale);
            Tempo12mm_sel=false;
            if(Tempo12mm_P2_changed) ui->lineEdit_T12mm->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T12mm->setStyleSheet(Stile_Normale);
            Tempo34hh_sel=false;
            if((Tempo34hh_P2_changed)||(Metri34_P2_changed)) ui->lineEdit_T34hh->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T34hh->setStyleSheet(Stile_Normale);
            Tempo34mm_sel=false;
            if(Tempo34mm_P2_changed) ui->lineEdit_T34mm->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T34mm->setStyleSheet(Stile_Normale);
            Tempo56hh_sel=false;
            if((Tempo56hh_P2_changed)||(Metri56_P2_changed)) ui->lineEdit_T56hh->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T56hh->setStyleSheet(Stile_Normale);
            Tempo56mm_sel=false;
            if(Tempo56mm_P2_changed) ui->lineEdit_T56mm->setStyleSheet(Stile_Mod);
            else ui->lineEdit_T56mm->setStyleSheet(Stile_Normale);
        }

        if (object == ui->lineEdit_Ang1)
        {
            Ang1_sel=true;
            ui->lineEdit_Ang1->setStyleSheet(Stile_Selez);

        }
        else if (object == ui->lineEdit_Ang2)
        {
            Ang2_sel=true;
            ui->lineEdit_Ang2->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_Ang3)
        {
            Ang3_sel=true;
            ui->lineEdit_Ang3->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_Ang4)
        {
            Ang4_sel=true;
            ui->lineEdit_Ang4->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_Ang5)
        {
            Ang5_sel=true;
            ui->lineEdit_Ang5->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_Ang6)
        {
            Ang6_sel=true;
            ui->lineEdit_Ang6->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_Ang7)
        {
            Ang7_sel=true;
            ui->lineEdit_Ang7->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_Ang8)
        {
            Ang8_sel=true;
            ui->lineEdit_Ang8->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_T12hh)
        {
            Tempo12hh_sel=true;
            ui->lineEdit_T12hh->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_T12mm)
        {
            Tempo12mm_sel=true;
            ui->lineEdit_T12mm->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_T34hh)
        {
            Tempo34hh_sel=true;
            ui->lineEdit_T34hh->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_T34mm)
        {
            Tempo34mm_sel=true;
            ui->lineEdit_T34mm->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_T56hh)
        {
            Tempo56hh_sel=true;
            ui->lineEdit_T56hh->setStyleSheet(Stile_Selez);
        }
        else if (object == ui->lineEdit_T56mm)
        {
            Tempo56mm_sel=true;
            ui->lineEdit_T56mm->setStyleSheet(Stile_Selez);
        }
    }
    else  if (event->type() == QEvent::FocusOut)
    {

    }

    return false;
}


void WidgetProg::on_pushButton_Meno_clicked()
{
    if(ProgSel==1){
        if(Ang1_sel){
            if(((Ang2_P1-Ang1_P1)>=0)&&((Ang2_P1-Ang1_P1)<(360-15))) Ang1_P1--;
            if(((Ang1_P1-Ang2_P1)>=0)&&((Ang1_P1-Ang2_P1)>15)) Ang1_P1--;
            if(Ang1_P1<0) Ang1_P1 = Ang1_P1+360;
            Ang1_P1_changed=true;
        }
        else if(Ang2_sel){
            if(((Ang2_P1-Ang1_P1)>=0)&&((Ang2_P1-Ang1_P1)>15)) Ang2_P1--;
            if(((Ang1_P1-Ang2_P1)>=0)&&((Ang1_P1-Ang2_P1)<(360-15))) Ang2_P1--;
            if(Ang2_P1<0) Ang2_P1 = Ang2_P1+360;
            Ang2_P1_changed=true;
        }
        else if(Ang3_sel){
            if(((Ang4_P1-Ang3_P1)>=0)&&((Ang4_P1-Ang3_P1)<(360-15))) Ang3_P1--;
            if(((Ang3_P1-Ang4_P1)>=0)&&((Ang3_P1-Ang4_P1)>15)) Ang3_P1--;
            if(Ang3_P1<0) Ang3_P1 = Ang3_P1+360;
            Ang3_P1_changed=true;
        }
        else if(Ang4_sel){
            if(((Ang4_P1-Ang3_P1)>=0)&&((Ang4_P1-Ang3_P1)>15)) Ang4_P1--;
            if(((Ang3_P1-Ang4_P1)>=0)&&((Ang3_P1-Ang4_P1)<(360-15))) Ang4_P1--;
            if(Ang4_P1<0) Ang4_P1 = Ang4_P1+360;
            Ang4_P1_changed=true;
        }
        else if(Ang5_sel){
            if(((Ang6_P1-Ang5_P1)>=0)&&((Ang6_P1-Ang5_P1)<(360-15))) Ang5_P1--;
            if(((Ang5_P1-Ang6_P1)>=0)&&((Ang5_P1-Ang6_P1)>15)) Ang5_P1--;
            if(Ang5_P1<0) Ang5_P1 = Ang5_P1+360;
            Ang5_P1_changed=true;
        }
        else if(Ang6_sel){
            if(((Ang6_P1-Ang5_P1)>=0)&&((Ang6_P1-Ang5_P1)>15)) Ang6_P1--;
            if(((Ang5_P1-Ang6_P1)>=0)&&((Ang5_P1-Ang6_P1)<(360-15))) Ang6_P1--;
            if(Ang6_P1<0) Ang6_P1 = Ang6_P1+360;
            Ang6_P1_changed=true;
        }
        else if(Ang7_sel){
            if(((Ang8_P1-Ang7_P1)>=0)&&((Ang8_P1-Ang7_P1)<(360-15))) Ang7_P1--;
            if(((Ang7_P1-Ang8_P1)>=0)&&((Ang7_P1-Ang8_P1)>15)) Ang7_P1--;
            if(Ang7_P1<0) Ang7_P1 = Ang7_P1+360;
            Ang7_P1_changed=true;
        }
        else if(Ang8_sel){
            if(((Ang8_P1-Ang7_P1)>=0)&&((Ang8_P1-Ang7_P1)>15)) Ang8_P1--;
            if(((Ang7_P1-Ang8_P1)>=0)&&((Ang7_P1-Ang8_P1)<(360-15))) Ang8_P1--;
            if(Ang8_P1<0) Ang8_P1 = Ang8_P1+360;
            Ang8_P1_changed=true;
        }
        else if(Tempo12hh_sel){
            if(SelTM_P1==0){
                if((Tempo12_P1/60)>0) Tempo12_P1-=60;
                Tempo12hh_P1_changed=true;
            }
            else {
                if(Metri12_P1>0) Metri12_P1-=5;
                Metri12_P1_changed=true;
            }
        }
        else if(Tempo12mm_sel){
            if((Tempo12_P1%60)>0) Tempo12_P1-=1;
            Tempo12mm_P1_changed=true;
        }
        else if(Tempo34hh_sel){
            if(SelTM_P1==0){
                if((Tempo34_P1/60)>0) Tempo34_P1-=60;
                Tempo34hh_P1_changed=true;
            }
            else {
                if(Metri34_P1>0) Metri34_P1-=5;
                Metri34_P1_changed=true;
            }
        }
        else if(Tempo34mm_sel){
            if((Tempo34_P1%60)>0) Tempo34_P1-=1;
            Tempo34mm_P1_changed=true;
        }
        else if(Tempo56hh_sel){
            if(SelTM_P1==0){
                if((Tempo56_P1/60)>0) Tempo56_P1-=60;
                Tempo56hh_P1_changed=true;
            }
            else {
                if(Metri56_P1>0) Metri56_P1-=5;
                Metri56_P1_changed=true;
            }
        }
        else if(Tempo56mm_sel){
            if((Tempo56_P1%60)>0) Tempo56_P1-=1;
            Tempo56mm_P1_changed=true;
        }
    }
    else if(ProgSel==2){
        if(Ang1_sel){
            if(((Ang2_P2-Ang1_P2)>=0)&&((Ang2_P2-Ang1_P2)<(360-15))) Ang1_P2--;
            if(((Ang1_P2-Ang2_P2)>=0)&&((Ang1_P2-Ang2_P2)>15)) Ang1_P2--;
            if(Ang1_P2<0) Ang1_P2 = Ang1_P2+360;
            Ang1_P2_changed=true;
        }
        else if(Ang2_sel){
            if(((Ang2_P2-Ang1_P2)>=0)&&((Ang2_P2-Ang1_P2)>15)) Ang2_P2--;
            if(((Ang1_P2-Ang2_P2)>=0)&&((Ang1_P2-Ang2_P2)<(360-15))) Ang2_P2--;
            if(Ang2_P2<0) Ang2_P2 = Ang2_P2+360;
            Ang2_P2_changed=true;
        }
        else if(Ang3_sel){
            if(((Ang4_P2-Ang3_P2)>=0)&&((Ang4_P2-Ang3_P2)<(360-15))) Ang3_P2--;
            if(((Ang3_P2-Ang4_P2)>=0)&&((Ang3_P2-Ang4_P2)>15)) Ang3_P2--;
            if(Ang3_P2<0) Ang3_P2 = Ang3_P2+360;
            Ang3_P2_changed=true;
        }
        else if(Ang4_sel){
            if(((Ang4_P2-Ang3_P2)>=0)&&((Ang4_P2-Ang3_P2)>15)) Ang4_P2--;
            if(((Ang3_P2-Ang4_P2)>=0)&&((Ang3_P2-Ang4_P2)<(360-15))) Ang4_P2--;
            if(Ang4_P2<0) Ang4_P2 = Ang4_P2+360;
            Ang4_P2_changed=true;
        }
        else if(Ang5_sel){
            if(((Ang6_P2-Ang5_P2)>=0)&&((Ang6_P2-Ang5_P2)<(360-15))) Ang5_P2--;
            if(((Ang5_P2-Ang6_P2)>=0)&&((Ang5_P2-Ang6_P2)>15)) Ang5_P2--;
            if(Ang5_P2<0) Ang5_P2 = Ang5_P2+360;
            Ang5_P2_changed=true;
        }
        else if(Ang6_sel){
            if(((Ang6_P2-Ang5_P2)>=0)&&((Ang6_P2-Ang5_P2)>15)) Ang6_P2--;
            if(((Ang5_P2-Ang6_P2)>=0)&&((Ang5_P2-Ang6_P2)<(360-15))) Ang6_P2--;
            if(Ang6_P2<0) Ang6_P2 = Ang6_P2+360;
            Ang6_P2_changed=true;
        }
        else if(Ang7_sel){
            if(((Ang8_P2-Ang7_P2)>=0)&&((Ang8_P2-Ang7_P2)<(360-15))) Ang7_P2--;
            if(((Ang7_P2-Ang8_P2)>=0)&&((Ang7_P2-Ang8_P2)>15)) Ang7_P2--;
            if(Ang7_P2<0) Ang7_P2 = Ang7_P2+360;
            Ang7_P2_changed=true;
        }
        else if(Ang8_sel){
            if(((Ang8_P2-Ang7_P2)>=0)&&((Ang8_P2-Ang7_P2)>15)) Ang8_P2--;
            if(((Ang7_P2-Ang8_P2)>=0)&&((Ang7_P2-Ang8_P2)<(360-15))) Ang8_P2--;
            if(Ang8_P2<0) Ang8_P2 = Ang8_P2+360;
            Ang8_P2_changed=true;
        }
        else if(Tempo12hh_sel){
            if(SelTM_P2==0){
                if((Tempo12_P2/60)>0) Tempo12_P2-=60;
                Tempo12hh_P2_changed=true;
            }
            else {
                if(Metri12_P2>0) Metri12_P2-=5;
                Metri12_P2_changed=true;
            }
        }
        else if(Tempo12mm_sel){
            if((Tempo12_P2%60)>0) Tempo12_P2-=1;
            Tempo12mm_P2_changed=true;
        }
        else if(Tempo34hh_sel){
            if(SelTM_P2==0){
                if((Tempo34_P2/60)>0) Tempo34_P2-=60;
                Tempo34hh_P2_changed=true;
            }
            else {
                if(Metri34_P2>0) Metri34_P2-=5;
                Metri34_P2_changed=true;
            }
        }
        else if(Tempo34mm_sel){
            if((Tempo34_P2%60)>0) Tempo34_P2-=1;
            Tempo34mm_P2_changed=true;
        }
        else if(Tempo56hh_sel){
            if(SelTM_P2==0){
                if((Tempo56_P2/60)>0) Tempo56_P2-=60;
                Tempo56hh_P2_changed=true;
            }
            else {
                if(Metri56_P2>0) Metri56_P2-=5;
                Metri56_P2_changed=true;
            }
        }
        else if(Tempo56mm_sel){
            if((Tempo56_P2%60)>0) Tempo56_P2-=1;
            Tempo56mm_P2_changed=true;
        }
    }

    RefreshProg();
}

void WidgetProg::on_pushButton_Piu_clicked()
{
    if(ProgSel==1){
        if(Ang1_sel){
            if(((Ang2_P1-Ang1_P1)>=0)&&((Ang2_P1-Ang1_P1)>15)) Ang1_P1++;
            if(((Ang1_P1-Ang2_P1)>=0)&&((Ang1_P1-Ang2_P1)<(360-15))) Ang1_P1++;
            if(Ang1_P1>=360) Ang1_P1 = Ang1_P1-360;
            Ang1_P1_changed=true;
        }
        else if(Ang2_sel){
            if(((Ang2_P1-Ang1_P1)>=0)&&((Ang2_P1-Ang1_P1)<(360-15))) Ang2_P1++;
            if(((Ang1_P1-Ang2_P1)>=0)&&((Ang1_P1-Ang2_P1)>15)) Ang2_P1++;
            if(Ang2_P1>=360) Ang2_P1 = Ang2_P1-360;
            Ang2_P1_changed=true;
        }
        else if(Ang3_sel){
            if(((Ang4_P1-Ang3_P1)>=0)&&((Ang4_P1-Ang3_P1)>15)) Ang3_P1++;
            if(((Ang3_P1-Ang4_P1)>=0)&&((Ang3_P1-Ang4_P1)<(360-15))) Ang3_P1++;
            if(Ang3_P1>=360) Ang3_P1 = Ang3_P1-360;
            Ang3_P1_changed=true;
        }
        else if(Ang4_sel){
            if(((Ang4_P1-Ang3_P1)>=0)&&((Ang4_P1-Ang3_P1)<(360-15))) Ang4_P1++;
            if(((Ang3_P1-Ang4_P1)>=0)&&((Ang3_P1-Ang4_P1)>15)) Ang4_P1++;
            if(Ang4_P1>=360) Ang4_P1 = Ang4_P1-360;
            Ang4_P1_changed=true;
        }
        else if(Ang5_sel){
            if(((Ang6_P1-Ang5_P1)>=0)&&((Ang6_P1-Ang5_P1)>15)) Ang5_P1++;
            if(((Ang5_P1-Ang6_P1)>=0)&&((Ang5_P1-Ang6_P1)<(360-15))) Ang5_P1++;
            if(Ang5_P1>=360) Ang5_P1 = Ang5_P1-360;
            Ang5_P1_changed=true;
        }
        else if(Ang6_sel){
            if(((Ang6_P1-Ang5_P1)>=0)&&((Ang6_P1-Ang5_P1)<(360-15))) Ang6_P1++;
            if(((Ang5_P1-Ang6_P1)>=0)&&((Ang5_P1-Ang6_P1)>15)) Ang6_P1++;
            if(Ang6_P1>=360) Ang6_P1 = Ang6_P1-360;
            Ang6_P1_changed=true;
        }
        else if(Ang7_sel){
            if(((Ang8_P1-Ang7_P1)>=0)&&((Ang8_P1-Ang7_P1)>15)) Ang7_P1++;
            if(((Ang7_P1-Ang8_P1)>=0)&&((Ang7_P1-Ang8_P1)<(360-15))) Ang7_P1++;
            if(Ang7_P1>=360) Ang7_P1 = Ang7_P1-360;
            Ang7_P1_changed=true;
        }
        else if(Ang8_sel){
            if(((Ang8_P1-Ang7_P1)>=0)&&((Ang8_P1-Ang7_P1)<(360-15))) Ang8_P1++;
            if(((Ang7_P1-Ang8_P1)>=0)&&((Ang7_P1-Ang8_P1)>15)) Ang8_P1++;
            if(Ang8_P1>=360) Ang8_P1 = Ang8_P1-360;
            Ang8_P1_changed=true;
        }
        else if(Tempo12hh_sel){
            if(SelTM_P1==0){
                if((Tempo12_P1/60)<99) Tempo12_P1+=60;
                Tempo12hh_P1_changed=true;
            }
            else {
                if(Metri12_P1<900) Metri12_P1+=5;
                Metri12_P1_changed=true;
            }
        }
        else if(Tempo12mm_sel){
            if((Tempo12_P1%60)<59) Tempo12_P1+=1;
            Tempo12mm_P1_changed=true;
        }
        else if(Tempo34hh_sel){
            if(SelTM_P1==0){
                if((Tempo34_P1/60)<99) Tempo34_P1+=60;
                Tempo34hh_P1_changed=true;
            }
            else {
                if(Metri34_P1<900) Metri34_P1+=5;
                Metri34_P1_changed=true;
            }
        }
        else if(Tempo34mm_sel){
            if((Tempo34_P1%60)<59) Tempo34_P1+=1;
            Tempo34mm_P1_changed=true;
        }
        else if(Tempo56hh_sel){
            if(SelTM_P1==0){
                if((Tempo56_P1/60)<99) Tempo56_P1+=60;
                Tempo56hh_P1_changed=true;
            }
            else {
                if(Metri56_P1<900) Metri56_P1+=5;
                Metri56_P1_changed=true;
            }
        }
        else if(Tempo56mm_sel){
            if((Tempo56_P1%60)<59) Tempo56_P1+=1;
            Tempo56mm_P1_changed=true;
        }
    }
    else if(ProgSel==2){
        if(Ang1_sel){
            if(((Ang2_P2-Ang1_P2)>=0)&&((Ang2_P2-Ang1_P2)>15)) Ang1_P2++;
            if(((Ang1_P2-Ang2_P2)>=0)&&((Ang1_P2-Ang2_P2)<(360-15))) Ang1_P2++;
            if(Ang1_P2>=360) Ang1_P2 = Ang1_P2-360;
            Ang1_P2_changed=true;
        }
        else if(Ang2_sel){
            if(((Ang2_P2-Ang1_P2)>=0)&&((Ang2_P2-Ang1_P2)<(360-15))) Ang2_P2++;
            if(((Ang1_P2-Ang2_P2)>=0)&&((Ang1_P2-Ang2_P2)>15)) Ang2_P2++;
            if(Ang2_P2>=360) Ang2_P2 = Ang2_P2-360;
            Ang2_P2_changed=true;
        }
        else if(Ang3_sel){
            if(((Ang4_P2-Ang3_P2)>=0)&&((Ang4_P2-Ang3_P2)>15)) Ang3_P2++;
            if(((Ang3_P2-Ang4_P2)>=0)&&((Ang3_P2-Ang4_P2)<(360-15))) Ang3_P2++;
            if(Ang3_P2>=360) Ang3_P2 = Ang3_P2-360;
            Ang3_P2_changed=true;
        }
        else if(Ang4_sel){
            if(((Ang4_P2-Ang3_P2)>=0)&&((Ang4_P2-Ang3_P2)<(360-15))) Ang4_P2++;
            if(((Ang3_P2-Ang4_P2)>=0)&&((Ang3_P2-Ang4_P2)>15)) Ang4_P2++;
            if(Ang4_P2>=360) Ang4_P2 = Ang4_P2-360;
            Ang4_P2_changed=true;
        }
        else if(Ang5_sel){
            if(((Ang6_P2-Ang5_P2)>=0)&&((Ang6_P2-Ang5_P2)>15)) Ang5_P2++;
            if(((Ang5_P2-Ang6_P2)>=0)&&((Ang5_P2-Ang6_P2)<(360-15))) Ang5_P2++;
            if(Ang5_P2>=360) Ang5_P2 = Ang5_P2-360;
            Ang5_P2_changed=true;
        }
        else if(Ang6_sel){
            if(((Ang6_P2-Ang5_P2)>=0)&&((Ang6_P2-Ang5_P2)<(360-15))) Ang6_P2++;
            if(((Ang5_P2-Ang6_P2)>=0)&&((Ang5_P2-Ang6_P2)>15)) Ang6_P2++;
            if(Ang6_P2>=360) Ang6_P2 = Ang6_P2-360;
            Ang6_P2_changed=true;
        }
        else if(Ang7_sel){
            if(((Ang8_P2-Ang7_P2)>=0)&&((Ang8_P2-Ang7_P2)>15)) Ang7_P2++;
            if(((Ang7_P2-Ang8_P2)>=0)&&((Ang7_P2-Ang8_P2)<(360-15))) Ang7_P2++;
            if(Ang7_P2>=360) Ang7_P2 = Ang7_P2-360;
            Ang7_P2_changed=true;
        }
        else if(Ang8_sel){
            if(((Ang8_P2-Ang7_P2)>=0)&&((Ang8_P2-Ang7_P2)<(360-15))) Ang8_P2++;
            if(((Ang7_P2-Ang8_P2)>=0)&&((Ang7_P2-Ang8_P2)>15)) Ang8_P2++;
            if(Ang8_P2>=360) Ang8_P2 = Ang8_P2-360;
            Ang8_P2_changed=true;
        }
        else if(Tempo12hh_sel){
            if(SelTM_P2==0){
                if((Tempo12_P2/60)<99) Tempo12_P2+=60;
                Tempo12hh_P2_changed=true;
            }
            else {
                if(Metri12_P2<900) Metri12_P2+=5;
                Metri12_P2_changed=true;
            }
        }
        else if(Tempo12mm_sel){
            if((Tempo12_P2%60)<59) Tempo12_P2+=1;
            Tempo12mm_P2_changed=true;
        }
        else if(Tempo34hh_sel){
            if(SelTM_P2==0){
                if((Tempo34_P2/60)<99) Tempo34_P2+=60;
                Tempo34hh_P2_changed=true;
            }
            else {
                if(Metri34_P2<900) Metri34_P2+=5;
                Metri34_P2_changed=true;
            }
        }
        else if(Tempo34mm_sel){
            if((Tempo34_P2%60)<59) Tempo34_P2+=1;
            Tempo34mm_P2_changed=true;
        }
        else if(Tempo56hh_sel){
            if(SelTM_P2==0){
                if((Tempo56_P2/60)<99) Tempo56_P2+=60;
                Tempo56hh_P2_changed=true;
            }
            else {
                if(Metri56_P2<900) Metri56_P2+=5;
                Metri56_P2_changed=true;
            }
        }
        else if(Tempo56mm_sel){
            if((Tempo56_P2%60)<59) Tempo56_P2+=1;
            Tempo56mm_P2_changed=true;
        }
    }

    RefreshProg();
}

void WidgetProg::on_pushButton_Update_clicked()
{
    ui->pushButton_Update->setStyleSheet(StileB_Mod);
    emit update_prog();
}


void WidgetProg::on_pushButton_P1_clicked()
{
    ProgSel=1;

    Ang1_sel=false;
    if(Ang1_P1_changed) ui->lineEdit_Ang1->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang1->setStyleSheet(Stile_Normale);
    Ang2_sel=false;
    if(Ang2_P1_changed) ui->lineEdit_Ang2->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang2->setStyleSheet(Stile_Normale);
    Ang3_sel=false;
    if(Ang3_P1_changed) ui->lineEdit_Ang3->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang3->setStyleSheet(Stile_Normale);
    Ang4_sel=false;
    if(Ang4_P1_changed) ui->lineEdit_Ang4->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang4->setStyleSheet(Stile_Normale);
    Ang5_sel=false;
    if(Ang5_P1_changed) ui->lineEdit_Ang5->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang5->setStyleSheet(Stile_Normale);
    Ang6_sel=false;
    if(Ang6_P1_changed) ui->lineEdit_Ang6->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang6->setStyleSheet(Stile_Normale);
    Ang7_sel=false;
    if(Ang7_P1_changed) ui->lineEdit_Ang7->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang7->setStyleSheet(Stile_Normale);
    Ang8_sel=false;
    if(Ang8_P1_changed) ui->lineEdit_Ang8->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang8->setStyleSheet(Stile_Normale);
    Tempo12hh_sel=false;
    if((Tempo12hh_P1_changed)||(Metri12_P1_changed)) ui->lineEdit_T12hh->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T12hh->setStyleSheet(Stile_Normale);
    Tempo12mm_sel=false;
    if(Tempo12mm_P1_changed) ui->lineEdit_T12mm->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T12mm->setStyleSheet(Stile_Normale);
    Tempo34hh_sel=false;
    if((Tempo34hh_P1_changed)||(Metri34_P1_changed)) ui->lineEdit_T34hh->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T34hh->setStyleSheet(Stile_Normale);
    Tempo34mm_sel=false;
    if(Tempo34mm_P1_changed) ui->lineEdit_T34mm->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T34mm->setStyleSheet(Stile_Normale);
    Tempo56hh_sel=false;
    if((Tempo56hh_P1_changed)||(Metri56_P1_changed)) ui->lineEdit_T56hh->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T56hh->setStyleSheet(Stile_Normale);
    Tempo56mm_sel=false;
    if(Tempo56mm_P1_changed) ui->lineEdit_T56mm->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T56mm->setStyleSheet(Stile_Normale);
    if(Rot1234_P1_changed) ui->pushButton_Rot1234->setStyleSheet(StileB_Mod);
    else ui->pushButton_Rot1234->setStyleSheet(StileB_Normale);
    if(Rot3456_P1_changed) ui->pushButton_Rot3456->setStyleSheet(StileB_Mod);
    else ui->pushButton_Rot3456->setStyleSheet(StileB_Normale);
    if(Rot5678_P1_changed) ui->pushButton_Rot5678->setStyleSheet(StileB_Mod);
    else ui->pushButton_Rot5678->setStyleSheet(StileB_Normale);

    RefreshProg();
}

void WidgetProg::on_pushButton_P2_clicked()
{
    ProgSel=2;
    Ang1_sel=false;
    if(Ang1_P2_changed) ui->lineEdit_Ang1->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang1->setStyleSheet(Stile_Normale);
    Ang2_sel=false;
    if(Ang2_P2_changed) ui->lineEdit_Ang2->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang2->setStyleSheet(Stile_Normale);
    Ang3_sel=false;
    if(Ang3_P2_changed) ui->lineEdit_Ang3->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang3->setStyleSheet(Stile_Normale);
    Ang4_sel=false;
    if(Ang4_P2_changed) ui->lineEdit_Ang4->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang4->setStyleSheet(Stile_Normale);
    Ang5_sel=false;
    if(Ang5_P2_changed) ui->lineEdit_Ang5->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang5->setStyleSheet(Stile_Normale);
    Ang6_sel=false;
    if(Ang6_P2_changed) ui->lineEdit_Ang6->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang6->setStyleSheet(Stile_Normale);
    Ang7_sel=false;
    if(Ang7_P2_changed) ui->lineEdit_Ang7->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang7->setStyleSheet(Stile_Normale);
    Ang8_sel=false;
    if(Ang8_P2_changed) ui->lineEdit_Ang8->setStyleSheet(Stile_Mod);
    else ui->lineEdit_Ang8->setStyleSheet(Stile_Normale);
    Tempo12hh_sel=false;
    if((Tempo12hh_P2_changed)||(Metri12_P2_changed)) ui->lineEdit_T12hh->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T12hh->setStyleSheet(Stile_Normale);
    Tempo12mm_sel=false;
    if(Tempo12mm_P2_changed) ui->lineEdit_T12mm->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T12mm->setStyleSheet(Stile_Normale);
    Tempo34hh_sel=false;
    if((Tempo34hh_P2_changed)||(Metri34_P2_changed)) ui->lineEdit_T34hh->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T34hh->setStyleSheet(Stile_Normale);
    Tempo34mm_sel=false;
    if(Tempo34mm_P2_changed) ui->lineEdit_T34mm->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T34mm->setStyleSheet(Stile_Normale);
    Tempo56hh_sel=false;
    if((Tempo56hh_P2_changed)||(Metri56_P2_changed)) ui->lineEdit_T56hh->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T56hh->setStyleSheet(Stile_Normale);
    Tempo56mm_sel=false;
    if(Tempo56mm_P2_changed) ui->lineEdit_T56mm->setStyleSheet(Stile_Mod);
    else ui->lineEdit_T56mm->setStyleSheet(Stile_Normale);
    if(Rot1234_P2_changed) ui->pushButton_Rot1234->setStyleSheet(StileB_Mod);
    else ui->pushButton_Rot1234->setStyleSheet(StileB_Normale);
    if(Rot3456_P2_changed) ui->pushButton_Rot3456->setStyleSheet(StileB_Mod);
    else ui->pushButton_Rot3456->setStyleSheet(StileB_Normale);
    if(Rot5678_P2_changed) ui->pushButton_Rot5678->setStyleSheet(StileB_Mod);
    else ui->pushButton_Rot5678->setStyleSheet(StileB_Normale);

    RefreshProg();
}

void WidgetProg::on_pushButton_Rot1234_clicked()
{
    if(ProgSel==1){
        if(Rot1234_P1==1) Rot1234_P1=2; else Rot1234_P1=1;
        Rot1234_P1_changed=true;
    }
    else if(ProgSel==2){
        if(Rot1234_P2==1) Rot1234_P2=2; else Rot1234_P2=1;
        Rot1234_P2_changed=true;
    }
    ui->pushButton_Rot1234->setStyleSheet(StileB_Mod);
    RefreshProg();
}

void WidgetProg::on_pushButton_Rot3456_clicked()
{
    if(ProgSel==1){
        if(Rot3456_P1==1) Rot3456_P1=2; else Rot3456_P1=1;
        Rot3456_P1_changed=true;
    }
    else if(ProgSel==2){
        if(Rot3456_P2==1) Rot3456_P2=2; else Rot3456_P2=1;
        Rot3456_P2_changed=true;
    }
    ui->pushButton_Rot3456->setStyleSheet(StileB_Mod);
    RefreshProg();
}

void WidgetProg::on_pushButton_Rot5678_clicked()
{
    if(ProgSel==1){
        if(Rot5678_P1==1) Rot5678_P1=2; else Rot5678_P1=1;
        Rot5678_P1_changed=true;
    }
    else if(ProgSel==2){
        if(Rot5678_P2==1) Rot5678_P2=2; else Rot5678_P2=1;
        Rot5678_P2_changed=true;
    }
    ui->pushButton_Rot5678->setStyleSheet(StileB_Mod);
    RefreshProg();
}

void WidgetProg::on_pushButton_TM_clicked()
{
    if(ProgSel==1){
        if(SelTM_P1==0) SelTM_P1=1; else SelTM_P1=0;
    }
    else if(ProgSel==2){
        if(SelTM_P2==0) SelTM_P2=1; else SelTM_P2=0;
    }
    RefreshProg();
}

void WidgetProg::on_pushButton_LoadSave_clicked()
{
    PageLoadSave=true;
    ui->stackedWidget_Prog->setCurrentIndex(ui->stackedWidget_Prog->indexOf(ui->page_LoadSave));

    ui->label_Prog->setFont(FontLabel);
    ui->label_Name->setFont(FontLabel);
    ui->lineEdit_Prog->setFont(FontLineEdit);
    ui->lineEdit_Save->setFont(FontLineEdit);

    ui->label_Name->setFocus();

    if(!OkResize){
        OkResize=true;
        QSize IconSizes;
        IconSizes.setHeight(ui->pushButton_Previous->height());
        IconSizes.setWidth(ui->pushButton_Previous->width());
        ui->pushButton_Previous->setIconSize(IconSizes);
        IconSizes.setHeight(ui->pushButton_Next->height());
        IconSizes.setWidth(ui->pushButton_Next->width());
        ui->pushButton_Next->setIconSize(IconSizes);
        IconSizes.setHeight(ui->pushButton_Load->height());
        IconSizes.setWidth(ui->pushButton_Load->width());
        ui->pushButton_Load->setIconSize(IconSizes);
        IconSizes.setHeight(ui->pushButton_Erase->height());
        IconSizes.setWidth(ui->pushButton_Erase->width());
        ui->pushButton_Erase->setIconSize(IconSizes);
        IconSizes.setHeight(ui->pushButton_Save->height()*0.9);
        IconSizes.setWidth(ui->pushButton_Save->width()*0.9);
        ui->pushButton_Save->setIconSize(IconSizes);
        IconSizes.setHeight(ui->pushButton_Import->height());
        IconSizes.setWidth(ui->pushButton_Import->width());
        ui->pushButton_Import->setIconSize(IconSizes);
        IconSizes.setHeight(ui->pushButton_Export->height());
        IconSizes.setWidth(ui->pushButton_Export->width());
        ui->pushButton_Export->setIconSize(IconSizes);
    }

    // Numero programmi (max 50)
    SelectedProg=0;
    if(ListaProg.count()!=0){
        ui->label_Prog->setText(tr("Program  %1/%2").arg(SelectedProg+1).arg(ListaProg.count()));
        ui->lineEdit_Prog->setText(ListaProg.at(SelectedProg)->Nome);
        ui->pushButton_Previous->setEnabled(true);
        ui->pushButton_Next->setEnabled(true);
        ui->pushButton_Load->setEnabled(true);
        ui->pushButton_Erase->setEnabled(true);
    }
    else {
        ui->label_Prog->setText("Program  0/0");
        ui->lineEdit_Prog->setText("");
        ui->pushButton_Previous->setEnabled(false);
        ui->pushButton_Next->setEnabled(false);
        ui->pushButton_Load->setEnabled(false);
        ui->pushButton_Erase->setEnabled(false);
    }

    if(ListaProg.count()>=50) ui->groupBox_Save->setEnabled(false);

}

void WidgetProg::ResizeButtons()
{

    qDebug() << "Height:" << ui->pushButton_Piu->height();

    QSize IconSizes;
    IconSizes.setHeight(ui->pushButton_P1->height());
    IconSizes.setWidth(ui->pushButton_P1->width());
    ui->pushButton_P1->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_P2->height());
    IconSizes.setWidth(ui->pushButton_P2->width());
    ui->pushButton_P2->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_TM->height());
    IconSizes.setWidth(ui->pushButton_TM->width());
    ui->pushButton_TM->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_LoadSave->height());
    IconSizes.setWidth(ui->pushButton_LoadSave->width());
    ui->pushButton_LoadSave->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_Rot1234->height());
    IconSizes.setWidth(ui->pushButton_Rot1234->width());
    ui->pushButton_Rot1234->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_Rot3456->height());
    IconSizes.setWidth(ui->pushButton_Rot3456->width());
    ui->pushButton_Rot3456->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_Rot5678->height());
    IconSizes.setWidth(ui->pushButton_Rot5678->width());
    ui->pushButton_Rot5678->setIconSize(IconSizes);
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

void WidgetProg::Default_Page()
{
    PageLoadSave=false;
    ui->stackedWidget_Prog->setCurrentIndex(ui->stackedWidget_Prog->indexOf(ui->page_Prog));
}

void WidgetProg::CaricaProg()
{
    ListaProg.clear();

    // Carico la lista programmi
    ProgDirectory=QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/DATI"));
    ProjectPath.setPath(ProgDirectory);
    FileLista.setFileName(ProjectPath.absolutePath()+QString("/Lista.txt"));
    FileLista.open(QFile::ReadOnly);
    QString row;
    while( !FileLista.atEnd()){
        row = FileLista.readLine();
        if(row.length()>10){
            Prog_t* NewProg=new Prog_t;
            NewProg->Nome = row.section(';', 0, 0, QString::SectionSkipEmpty);
            NewProg->Ang1 = row.section(';', 1, 1, QString::SectionSkipEmpty).toInt();
            NewProg->Ang2 = row.section(';', 2, 2, QString::SectionSkipEmpty).toInt();
            NewProg->Ang3 = row.section(';', 3, 3, QString::SectionSkipEmpty).toInt();
            NewProg->Ang4 = row.section(';', 4, 4, QString::SectionSkipEmpty).toInt();
            NewProg->Ang5 = row.section(';', 5, 5, QString::SectionSkipEmpty).toInt();
            NewProg->Ang6 = row.section(';', 6, 6, QString::SectionSkipEmpty).toInt();
            NewProg->Ang7 = row.section(';', 7, 7, QString::SectionSkipEmpty).toInt();
            NewProg->Ang8 = row.section(';', 8, 8, QString::SectionSkipEmpty).toInt();
            NewProg->Tempo12 = row.section(';', 9, 9, QString::SectionSkipEmpty).toInt();
            NewProg->Tempo34 = row.section(';', 10, 10, QString::SectionSkipEmpty).toInt();
            NewProg->Tempo56 = row.section(';', 11, 11, QString::SectionSkipEmpty).toInt();
            NewProg->Rot1234 = row.section(';', 12, 12, QString::SectionSkipEmpty).toInt();
            NewProg->Rot3456 = row.section(';', 13, 13, QString::SectionSkipEmpty).toInt();
            NewProg->Rot5678 = row.section(';', 14, 14, QString::SectionSkipEmpty).toInt();
            NewProg->Metri12 = row.section(';', 15, 15, QString::SectionSkipEmpty).toInt();
            NewProg->Metri34 = row.section(';', 16, 16, QString::SectionSkipEmpty).toInt();
            NewProg->Metri56 = row.section(';', 17, 17, QString::SectionSkipEmpty).toInt();
            NewProg->SelTM = row.section(';', 18, 18, QString::SectionSkipEmpty).toInt();
            ListaProg.append(NewProg);
        }
    }
    FileLista.close();
}

void WidgetProg::FileSave()
{
    FileLista.open(QFile::WriteOnly|QFile::Truncate);
    QString row;
    for(int i=0; i<ListaProg.count(); i++){
        row = ListaProg.at(i)->Nome + ";" + tr("%1").arg(ListaProg.at(i)->Ang1) + ";" + tr("%1").arg(ListaProg.at(i)->Ang2) + ";" + tr("%1").arg(ListaProg.at(i)->Ang3) + ";" +
                tr("%1").arg(ListaProg.at(i)->Ang4) + ";" + tr("%1").arg(ListaProg.at(i)->Ang5) + ";" + tr("%1").arg(ListaProg.at(i)->Ang6) + ";" +
                tr("%1").arg(ListaProg.at(i)->Ang7) + ";" + tr("%1").arg(ListaProg.at(i)->Ang8) + ";" + tr("%1").arg(ListaProg.at(i)->Tempo12) + ";" +
                tr("%1").arg(ListaProg.at(i)->Tempo34) + ";" + tr("%1").arg(ListaProg.at(i)->Tempo56) + ";" + tr("%1").arg(ListaProg.at(i)->Rot1234) + ";" +
                tr("%1").arg(ListaProg.at(i)->Rot3456) + ";" + tr("%1").arg(ListaProg.at(i)->Rot5678) + ";" + tr("%1").arg(ListaProg.at(i)->Metri12) + ";" +
                tr("%1").arg(ListaProg.at(i)->Metri34) + ";" + tr("%1").arg(ListaProg.at(i)->Metri56) + ";" + tr("%1").arg(ListaProg.at(i)->SelTM) + "\n";
        FileLista.write(row.toUtf8());
    }
    FileLista.close();
}

void WidgetProg::on_pushButton_Save_clicked()
{
    ui->lineEdit_Save->clearFocus();

    if(ui->lineEdit_Save->text() == "") return;

    Prog_t* NewProg=new Prog_t;
    NewProg->Nome = ui->lineEdit_Save->text();
    if(ProgSel==1){
        NewProg->Ang1 = Ang1_P1;
        NewProg->Ang2 = Ang2_P1;
        NewProg->Ang3 = Ang3_P1;
        NewProg->Ang4 = Ang4_P1;
        NewProg->Ang5 = Ang5_P1;
        NewProg->Ang6 = Ang6_P1;
        NewProg->Ang7 = Ang7_P1;
        NewProg->Ang8 = Ang8_P1;
        NewProg->Tempo12 = Tempo12_P1;
        NewProg->Tempo34 = Tempo34_P1;
        NewProg->Tempo56 = Tempo56_P1;
        NewProg->Rot1234 = Rot1234_P1;
        NewProg->Rot3456 = Rot3456_P1;
        NewProg->Rot5678 = Rot5678_P1;
        NewProg->Metri12 = Metri12_P1;
        NewProg->Metri34 = Metri34_P1;
        NewProg->Metri56 = Metri56_P1;
        NewProg->SelTM = SelTM_P1;
    }
    else if(ProgSel==2){
        NewProg->Ang1 = Ang1_P2;
        NewProg->Ang2 = Ang2_P2;
        NewProg->Ang3 = Ang3_P2;
        NewProg->Ang4 = Ang4_P2;
        NewProg->Ang5 = Ang5_P2;
        NewProg->Ang6 = Ang6_P2;
        NewProg->Ang7 = Ang7_P2;
        NewProg->Ang8 = Ang8_P2;
        NewProg->Tempo12 = Tempo12_P2;
        NewProg->Tempo34 = Tempo34_P2;
        NewProg->Tempo56 = Tempo56_P2;
        NewProg->Rot1234 = Rot1234_P2;
        NewProg->Rot3456 = Rot3456_P2;
        NewProg->Rot5678 = Rot5678_P2;
        NewProg->Metri12 = Metri12_P2;
        NewProg->Metri34 = Metri34_P2;
        NewProg->Metri56 = Metri56_P2;
        NewProg->SelTM = SelTM_P2;
    }
    ListaProg.append(NewProg);

    // Salvataggio su file
    FileSave();

    ui->lineEdit_Save->setText("");
    ui->pushButton_LoadSave->setFocus();
    Update_OK();
    Default_Page();
}

void WidgetProg::on_pushButton_Previous_clicked()
{
    if(SelectedProg>0){
        SelectedProg--;
        ui->label_Prog->setText(tr("Program  %1/%2").arg(SelectedProg+1).arg(ListaProg.count()));
        ui->lineEdit_Prog->setText(ListaProg.at(SelectedProg)->Nome);
    }
}

void WidgetProg::on_pushButton_Next_clicked()
{
    if(SelectedProg<ListaProg.count()-1){
        SelectedProg++;
        ui->label_Prog->setText(tr("Program  %1/%2").arg(SelectedProg+1).arg(ListaProg.count()));
        ui->lineEdit_Prog->setText(ListaProg.at(SelectedProg)->Nome);
    }
}

void WidgetProg::on_pushButton_Load_clicked()
{
    if(ProgSel==1){
        Ang1_P1 = ListaProg.at(SelectedProg)->Ang1;
        Ang2_P1 = ListaProg.at(SelectedProg)->Ang2;
        Ang3_P1 = ListaProg.at(SelectedProg)->Ang3;
        Ang4_P1 = ListaProg.at(SelectedProg)->Ang4;
        Ang5_P1 = ListaProg.at(SelectedProg)->Ang5;
        Ang6_P1 = ListaProg.at(SelectedProg)->Ang6;
        Ang7_P1 = ListaProg.at(SelectedProg)->Ang7;
        Ang8_P1 = ListaProg.at(SelectedProg)->Ang8;
        Tempo12_P1 = ListaProg.at(SelectedProg)->Tempo12;
        Tempo34_P1 = ListaProg.at(SelectedProg)->Tempo34;
        Tempo56_P1 = ListaProg.at(SelectedProg)->Tempo56;
        Rot1234_P1 = ListaProg.at(SelectedProg)->Rot1234;
        Rot3456_P1 = ListaProg.at(SelectedProg)->Rot3456;
        Rot5678_P1 = ListaProg.at(SelectedProg)->Rot5678;
        Metri12_P1 = ListaProg.at(SelectedProg)->Metri12;
        Metri34_P1 = ListaProg.at(SelectedProg)->Metri34;
        Metri56_P1 = ListaProg.at(SelectedProg)->Metri56;
        SelTM_P1 = ListaProg.at(SelectedProg)->SelTM;
    }
    else if(ProgSel==2){
        Ang1_P2 = ListaProg.at(SelectedProg)->Ang1;
        Ang2_P2 = ListaProg.at(SelectedProg)->Ang2;
        Ang3_P2 = ListaProg.at(SelectedProg)->Ang3;
        Ang4_P2 = ListaProg.at(SelectedProg)->Ang4;
        Ang5_P2 = ListaProg.at(SelectedProg)->Ang5;
        Ang6_P2 = ListaProg.at(SelectedProg)->Ang6;
        Ang7_P2 = ListaProg.at(SelectedProg)->Ang7;
        Ang8_P2 = ListaProg.at(SelectedProg)->Ang8;
        Tempo12_P2 = ListaProg.at(SelectedProg)->Tempo12;
        Tempo34_P2 = ListaProg.at(SelectedProg)->Tempo34;
        Tempo56_P2 = ListaProg.at(SelectedProg)->Tempo56;
        Rot1234_P2 = ListaProg.at(SelectedProg)->Rot1234;
        Rot3456_P2 = ListaProg.at(SelectedProg)->Rot3456;
        Rot5678_P2 = ListaProg.at(SelectedProg)->Rot5678;
        Metri12_P2 = ListaProg.at(SelectedProg)->Metri12;
        Metri34_P2 = ListaProg.at(SelectedProg)->Metri34;
        Metri56_P2 = ListaProg.at(SelectedProg)->Metri56;
        SelTM_P2 = ListaProg.at(SelectedProg)->SelTM;
    }

    RefreshProg();

    Default_Page();
}

void WidgetProg::on_pushButton_Erase_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirm"), QString("Delete \"" + ListaProg.at(SelectedProg)->Nome + "\" ?"), QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes){
        ListaProg.removeAt(SelectedProg);
        if(SelectedProg>ListaProg.count()) SelectedProg=ListaProg.count();
        // Salvataggio su file
        FileSave();
        Default_Page();
    }
    else if (reply == QMessageBox::No){
    }
}

void WidgetProg::on_pushButton_Import_clicked()
{
#ifdef Q_OS_ANDROID

    // Fino ad Android 10 (SDK 29)
    if(QtAndroid::androidSdkVersion()<30){
        ImpExpDirectory = QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    }
    // Da Android 11 (SDK 30)
    else {
        ImpExpDirectory = QFileDialog::getExistingDirectory(this);
    }

#elif defined Q_OS_IOS

    // Cerco il file dei programmi nella cartella Documenti
    ImpExpDirectory = QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

#endif

    if (QFile::exists(ImpExpDirectory + "/ekrain_prog_list.txt"))
    {
        if (QFile::exists(FileLista.fileName()))
        {
            QFile::remove(FileLista.fileName());
        }
        QFile::copy(ImpExpDirectory + "/ekrain_prog_list.txt", FileLista.fileName());
        CaricaProg();
        Default_Page();
    }

}

void WidgetProg::on_pushButton_Export_clicked()
{
#ifdef Q_OS_ANDROID

    // Creo una copia del file dei programmi nella cartella Download
    ImpExpDirectory = QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));

#elif defined Q_OS_IOS

    // Creo una copia del file dei programmi nella cartella Documenti
    ImpExpDirectory = QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

#endif

    if (QFile::exists(ImpExpDirectory + "/ekrain_prog_list.txt"))
    {
        QFile::remove(ImpExpDirectory + "/ekrain_prog_list.txt");
    }

    QFile::copy(FileLista.fileName(), ImpExpDirectory + "/ekrain_prog_list.txt");

    Default_Page();

}
