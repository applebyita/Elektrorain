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

#include "widgetgetto.h"
#include "ui_widgetgetto.h"
#include <QPixmap>
#include <QLabel>
#include <QDate>

WidgetGetto::WidgetGetto(QWidget *parent, Getto_t* pElemento) :
    QWidget(parent),
    ui(new Ui::WidgetGetto)
{
    ui->setupUi(this);

    Elemento=pElemento;

    // Riscala l'altezza in modo da vedere le scritte
    int width=this->width();
    height=this->fontMetrics().height()*4;
    this->setGeometry(0,0,width,height);

    QFont FontLabel=qApp->font();
    FontLabel.setPixelSize(height*0.25);
    ui->label_Campi->setFont(FontLabel);

    // Icona getto
    QPixmap Img=QPixmap(tr(":/Resources/getto.png"));
//    ui->label_Tipo->setPixmap(Img.scaledToHeight(height));
    ui->label_Tipo->setPixmap(Img.scaledToHeight(height*0.7));

    if(Elemento->DataScadenza!=NULL){
        QDate oggi = QDate::currentDate();
        QDate scadenza = QDate::fromString(Elemento->DataScadenza,"dd/MM/yyyy");
        if(oggi>scadenza) {
            ui->label_Campi->setStyleSheet("QLabel {color : red;}");
            Elemento->AbbScaduto = true;
        }
    }

    // Compila il campo descrizione: nome getto e data scadenza oppure nome getto e numero SIM
    if(SimNum==false) ui->label_Campi->setText(Elemento->Nome+tr("\r\n")+Elemento->DataScadenza);
    else ui->label_Campi->setText(Elemento->Nome+tr("\r\n")+Elemento->NumeroSIM);

#ifdef Q_OS_ANDROID
    heightImg = height*0.9;
#elif defined Q_OS_IOS
    heightImg = height*0.8;
#endif

    // Ridimensiona la label in modo che sia quadrata
    ui->label_Status->setGeometry(0,0,height,height);

    //Icona di stato
    if (Elemento->Status==ON_LINE){
        QPixmap Img=QPixmap(tr(":/Resources/LEDVerde.png"));
        ui->label_Status->setPixmap(Img.scaled(heightImg,heightImg));
    }else if (Elemento->Status==OFF_LINE){
        QPixmap Img=QPixmap(tr(":/Resources/LEDGrigio.png"));
        ui->label_Status->setPixmap(Img.scaled(heightImg,heightImg));
//        ui->label_Tipo->setEnabled(false);
//        ui->label_Campi->setEnabled(false);
    }else if (Elemento->Status==CONNECTING){
        QPixmap Img=QPixmap(tr(":/Resources/LEDGiallo.png"));
        ui->label_Status->setPixmap(Img.scaled(heightImg,heightImg));
    }else {
        QPixmap Img=QPixmap(tr(":/Resources/LEDRosso.png"));
        ui->label_Status->setPixmap(Img.scaled(heightImg,heightImg));
    }

}

WidgetGetto::~WidgetGetto()
{
    delete ui;
}

void WidgetGetto::Refresh(){

    // Compila il campo descrizione: nome getto e data scadenza oppure nome getto e numero SIM
    if(SimNum==false) ui->label_Campi->setText(Elemento->Nome+tr("\r\n")+Elemento->DataScadenza);
    else ui->label_Campi->setText(Elemento->Nome+tr("\r\n")+Elemento->NumeroSIM);

}

