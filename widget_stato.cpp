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

#include "widget_stato.h"
#include "ui_widget_stato.h"
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QDebug>

WidgetStato::WidgetStato(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetStato)
{
    ui->setupUi(this);

    ui->lineEdit_AngI->installEventFilter(this);
    ui->lineEdit_AngF->installEventFilter(this);
    ui->lineEdit_Vel->installEventFilter(this);

    ui->pushButton_Auto->setVisible(false);
}

WidgetStato::~WidgetStato()
{
    delete ui;
}

void WidgetStato::resizeEvent(QResizeEvent *event){

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
    ui->label_AngI->setFont(FontLabel);
    ui->label_AngF->setFont(FontLabel);
    ui->label_Vel->setFont(FontLabel);
    FontLabel.setBold(true);
    ui->label_Pressione->setFont(FontLabel);
    ui->label_Settore->setFont(FontLabel);
    ui->label_Tempo->setFont(FontLabel);

    FontLabel.setPixelSize(Height*0.8);
    FontLabel.setBold(false);
    ui->label_VelUnit->setFont(FontLabel);

    FontLabel.setPixelSize(Height*1.2);
    FontLabel.setBold(true);
    ui->lineEdit_AngI->setFont(FontLabel);
    ui->lineEdit_AngF->setFont(FontLabel);
    ui->lineEdit_Vel->setFont(FontLabel);

    QSize IconSizes;
    IconSizes.setHeight(ui->pushButton_P->height());
    IconSizes.setWidth(ui->pushButton_P->width());
    ui->pushButton_P->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_ReverseS->height());
    IconSizes.setWidth(ui->pushButton_ReverseS->width());
    ui->pushButton_ReverseS->setIconSize(IconSizes);
    IconSizes.setHeight(ui->pushButton_ReverseN->height());
    IconSizes.setWidth(ui->pushButton_ReverseN->width());
    ui->pushButton_ReverseN->setIconSize(IconSizes);
    ui->pushButton_Rompigetto->setIconSize(IconSizes);
    ui->pushButton_Auto->setIconSize(IconSizes);
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

void WidgetStato::RefreshStato(){

    // Pagina senza dati
    if(ProgInCorso==0){
        ui->label_Vel->setFocus();
        ui->lineEdit_AngI->setText("");
        ui->lineEdit_AngF->setText("");
        ui->lineEdit_Vel->setText("");
        ui->label_Settore->setText("");
        ui->label_Tempo->setText("");
        ui->label_Pressione->setText("");
        QPixmap ImgP=QPixmap(tr(":/Resources/P1.png"));
        ui->pushButton_P->setIcon(ImgP);
        ui->pushButton_P->setEnabled(false);
        ui->pushButton_ReverseS->setEnabled(false);
        ui->pushButton_ReverseN->setEnabled(false);
        ui->pushButton_Auto->setVisible(false);
        ui->pushButton_Rompigetto->setEnabled(false);
        ui->pushButton_Meno->setEnabled(false);
        ui->pushButton_Piu->setEnabled(false);
        ui->pushButton_Update->setEnabled(false);

        // Disegno solo tubo
        QPixmap Img=QPixmap(tr(":/Resources/Rotolone.png"));
        scala = ((float)ui->label_Draw->height())/Img.height();
        height = ui->label_Draw->height()*0.7;
        width = Img.width()*scala*0.7;

        if(ui->label_Draw->width()<(ui->label_Draw->height()*0.6)){
            lato_arco = ui->label_Draw->width();
        }
        else {
            lato_arco = ui->label_Draw->height()*0.6;
        }
        ui->label_Draw->setPixmap(Img);

    }
    // Pagina con dati
    else {
        ui->lineEdit_AngI->setText(tr("%1").arg(AngI));
        ui->lineEdit_AngF->setText(tr("%1").arg(AngF));
        ui->lineEdit_Vel->setText(tr("%1").arg(VelRot));
        ui->label_Pressione->setText(tr("%1 bar").arg(Pressione));
        if(SelTM==0){
            if(Settore==1) ui->label_Settore->setText("T1-2");
            else if(Settore==2) ui->label_Settore->setText("T3-4");
            else if(Settore==3) ui->label_Settore->setText("T5-6");
            else if(Settore==4) ui->label_Settore->setText("T7-8");
            ui->label_Tempo->setText(tr("%1:%2").arg(TempoRes/60, 2, 10, QChar('0')).arg(TempoRes%60, 2, 10, QChar('0')));
        }
        else {
            if(Settore==1) ui->label_Settore->setText("M1-2");
            else if(Settore==2) ui->label_Settore->setText("M3-4");
            else if(Settore==3) ui->label_Settore->setText("M5-6");
            else if(Settore==4) ui->label_Settore->setText("M7-8");
            ui->label_Tempo->setText(tr("%1").arg(MetriRes));
        }

        if(Auto<=1){
            if(ProgInCorso==1) {
                QPixmap ImgP=QPixmap(tr(":/Resources/P1.png"));
                ui->pushButton_P->setIcon(ImgP);
            }
            else if(ProgInCorso==2) {
                QPixmap ImgP=QPixmap(tr(":/Resources/P2.png"));
                ui->pushButton_P->setIcon(ImgP);
            }
            ui->pushButton_Auto->setVisible(false);
        }
        else if (Auto==2) {
            QPixmap ImgP=QPixmap(tr(":/Resources/auto_v.png"));
            ui->pushButton_P->setIcon(ImgP);
            ui->pushButton_Auto->setVisible(true);
            ui->label_Settore->setVisible(false);
            ui->label_Tempo->setVisible(false);
        }
        else if (Auto==3) {
            QPixmap ImgP=QPixmap(tr(":/Resources/auto_r.png"));
            ui->pushButton_P->setIcon(ImgP);
            ui->pushButton_Auto->setVisible(true);
            ui->label_Settore->setVisible(false);
            ui->label_Tempo->setVisible(false);
        }

        if(Auto<1){
            if(StatoMotore) ui->pushButton_P->setEnabled(false);
            else ui->pushButton_P->setEnabled(true);
        }
        else {
            ui->pushButton_P->setEnabled(true);
        }
        ui->pushButton_ReverseS->setEnabled(true);
        ui->pushButton_ReverseN->setEnabled(true);
        ui->pushButton_Rompigetto->setEnabled(true);
        ui->pushButton_Meno->setEnabled(true);
        ui->pushButton_Piu->setEnabled(true);
        ui->pushButton_Update->setEnabled(true);

        // Disegno getto
        QPixmap Img=QPixmap(tr(":/Resources/Rotolone.png"));
        scala = ((float)ui->label_Draw->height())/Img.height();
        height = ui->label_Draw->height()*0.7;
        width = Img.width()*scala*0.7;

        if(ui->label_Draw->width()<(ui->label_Draw->height()*0.6)){
            lato_arco = ui->label_Draw->width();
        }
        else {
            lato_arco = ui->label_Draw->height()*0.6;
        }

        static QImage ArcoGetto(lato_arco, lato_arco, QImage::Format_ARGB32);       
        int DimLine;
#ifndef Q_OS_IOS
        DimLine = 4;
#else
        DimLine = 2;
#endif
        ArcoGetto.fill(Qt::transparent);
        QPainter painter (&ArcoGetto);
        QPen pen(Qt::blue, DimLine, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(pen);
        painter.setBrush(Qt::cyan);

        QRectF rectangle(2, 2, ArcoGetto.width()-4, ArcoGetto.height()-4);
        int startAngle=(270-AngF) * 16;
        int spanAngle=0;
        if(AngF>AngI) spanAngle = (AngF-AngI) * 16;
        else spanAngle=(AngF-AngI+360) * 16;
        painter.drawPie(rectangle, startAngle, spanAngle);

        QPixmap ImgComposta(ui->label_Draw->width(),ui->label_Draw->height());
        ImgComposta.fill(Qt::transparent);
        QPainter painterTot (&ImgComposta);
        painterTot.drawPixmap((ui->label_Draw->width()-width)/2, ui->label_Draw->height()*0.3, Img.scaled(width, height));
        painterTot.drawPixmap((ui->label_Draw->width()-ArcoGetto.width())/2, (ui->label_Draw->height()*0.3)-(ArcoGetto.height()/2), QPixmap::fromImage(ArcoGetto));
        ui->label_Draw->setPixmap(ImgComposta);
    }

}

void WidgetStato::Update_OK(){

    AngI_sel=false;
    AngF_sel=false;
    Vel_sel=false;
    AngI_changed=false;
    AngF_changed=false;
    Vel_changed=false;
    ui->lineEdit_AngI->setStyleSheet(Stile_Normale);
    ui->lineEdit_AngF->setStyleSheet(Stile_Normale);
    ui->lineEdit_Vel->setStyleSheet(Stile_Normale);
    ui->pushButton_Update->setStyleSheet(StileB_Normale);
}

bool WidgetStato::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if (object == ui->lineEdit_AngI)
        {
            AngI_sel=true;
            ui->lineEdit_AngI->setStyleSheet(Stile_Selez);
            AngF_sel=false;
            if(AngF_changed) ui->lineEdit_AngF->setStyleSheet(Stile_Mod);
            else ui->lineEdit_AngF->setStyleSheet(Stile_Normale);
            Vel_sel=false;
            if(Vel_changed) ui->lineEdit_Vel->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Vel->setStyleSheet(Stile_Normale);
        }
        else if (object == ui->lineEdit_AngF)
        {
            AngI_sel=false;
            if(AngI_changed) ui->lineEdit_AngI->setStyleSheet(Stile_Mod);
            else ui->lineEdit_AngI->setStyleSheet(Stile_Normale);
            AngF_sel=true;
            ui->lineEdit_AngF->setStyleSheet(Stile_Selez);
            Vel_sel=false;
            if(Vel_changed) ui->lineEdit_Vel->setStyleSheet(Stile_Mod);
            else ui->lineEdit_Vel->setStyleSheet(Stile_Normale);
        }
        else if (object == ui->lineEdit_Vel)
        {
            AngI_sel=false;
            if(AngI_changed) ui->lineEdit_AngI->setStyleSheet(Stile_Mod);
            else ui->lineEdit_AngI->setStyleSheet(Stile_Normale);
            AngF_sel=false;
            if(AngF_changed) ui->lineEdit_AngF->setStyleSheet(Stile_Mod);
            else ui->lineEdit_AngF->setStyleSheet(Stile_Normale);
            Vel_sel=true;
            ui->lineEdit_Vel->setStyleSheet(Stile_Selez);
        }
    }
    else  if (event->type() == QEvent::FocusOut)
    {

    }

    return false;
}


void WidgetStato::on_pushButton_Meno_clicked()
{
    if(AngI_sel){
        if(((AngF-AngI)>=0)&&((AngF-AngI)<(360-15))) AngI--;
        if(((AngI-AngF)>=0)&&((AngI-AngF)>15)) AngI--;
        if(AngI<0) AngI = AngI+360;
        AngI_changed=true;
    }
    else if(AngF_sel){
        if(((AngF-AngI)>=0)&&((AngF-AngI)>15)) AngF--;
        if(((AngI-AngF)>=0)&&((AngI-AngF)<(360-15))) AngF--;
        if(AngF<0) AngF = AngF+360;
        AngF_changed=true;
    }
    else if(Vel_sel){
        if(VelRot>120) VelRot-=10;
        Vel_changed=true;
    }
    RefreshStato();
}

void WidgetStato::on_pushButton_Piu_clicked()
{
    if(AngI_sel){
        if(((AngF-AngI)>=0)&&((AngF-AngI)>15)) AngI++;
        if(((AngI-AngF)>=0)&&((AngI-AngF)<(360-15))) AngI++;
        if(AngI>=360) AngI = AngI-360;
        AngI_changed=true;
    }
    else if(AngF_sel){
        if(((AngF-AngI)>=0)&&((AngF-AngI)<(360-15))) AngF++;
        if(((AngI-AngF)>=0)&&((AngI-AngF)>15)) AngF++;
        if(AngF>=360) AngF = AngF-360;
        AngF_changed=true;
    }
    else if(Vel_sel){
        if(VelRot<300) VelRot+=10;
        Vel_changed=true;
    }
    RefreshStato();
}

void WidgetStato::on_pushButton_Update_clicked()
{
    ui->pushButton_Update->setStyleSheet(StileB_Mod);
    emit update_stato();
}

void WidgetStato::on_pushButton_ReverseS_clicked()
{
    emit reverse_save();
}

void WidgetStato::on_pushButton_ReverseN_clicked()
{
    emit reverse_notsave();
}

void WidgetStato::on_pushButton_P_clicked()
{
    if(Auto<1) emit change_prog();
    else emit change_manaut();
}

void WidgetStato::on_pushButton_Rompigetto_clicked()
{
    emit pagina_rompigetto();
}

void WidgetStato::on_pushButton_Auto_clicked()
{
    emit pagina_auto();
}
