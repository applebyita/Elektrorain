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

#ifndef WIDGET_BORDO_H
#define WIDGET_BORDO_H

#include <QWidget>
#include <QDir>
#include "getto.h"

namespace Ui {
class WidgetBordo;
}

class WidgetBordo : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetBordo(QWidget *parent = 0);
    ~WidgetBordo();

    //Valori Bordo
    bool        DatiAgg=false;
    quint8      Boccaglio=0;
    qint16      OffsetDir=0;
    qint16      OffsetM=0;
    quint8      GradiAper=0;

    quint8      NPunti=0;
    float       IncNord=0, IncEst=0;
    double      GPS_Np[30], GPS_Ep[30];

    QString     GPS_Np0, GPS_Ep0;
    QStringList ListaKML, ListaGPS_Np0, ListaGPS_Ep0;

signals:
    void update_bordo();
    void update_dati();


public slots:
    void RefreshBordo();
    void Update_OK();

private:
    Ui::WidgetBordo *ui;

    int lato_arco=0;

//    int height=0, width=0, lato_arco=0;
//    float scala=0;

    QDir ProjectPath;
    QString FieldDirectory;
    QStringList ListaField;
    quint8 SelectedField=0;
    QFile FileKML;

    // Stili
    QString Stile_Normale  = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: white;}";
    QString Stile_Selez = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: orange;}";
    QString Stile_Mod = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: yellow;}";

    QString StileL_Normale  = "QLabel{ background-color: white;}";
    QString StileL_Selez = "QLabel{ background-color: rgb(255, 190, 100);}";
    QString StileL_Mod = "QLabel{ background-color: yellow;}";

    QString StileB_Normale = "QPushButton{border-width: 0px; border-style: solid; border-color: rgb(0, 0, 0); outline: none; background-color: white;}";
    QString StileB_Mod = "QPushButton{border-width: 0px; border-style: solid; border-color: rgb(0, 0, 0); outline: none; background-color: yellow;}";

    bool Field_sel=false, Boccaglio_sel=false, OffsetDir_sel=false, OffsetM_sel=false, GradiAper_sel=false;
    bool Field_changed=false, Boccaglio_changed=false, OffsetDir_changed=false, OffsetM_changed=false, GradiAper_changed=false;

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_pushButton_Meno_clicked();
    void on_pushButton_Piu_clicked();
    void on_pushButton_Update_clicked();

    void ResizeButtons();
    void CaricaField();
    double distance(double lat1, double lon1, double lat2, double lon2);

    void on_pushButton_Import_clicked();
    void on_pushButton_Erase_clicked();
};

#endif // Widget_Bordo_H
