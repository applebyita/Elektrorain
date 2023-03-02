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

#ifndef WIDGET_STATO_H
#define WIDGET_STATO_H

#include <QWidget>

namespace Ui {
class WidgetStato;
}

class WidgetStato : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetStato(QWidget *parent = 0);
    ~WidgetStato();

    //Valori Stato Getto
    qint16     AngI=999;
    qint16     AngF=280;
    quint16    VelRot=200;
    quint8     Settore=1;
    quint8     SelTM=0;
    quint16    TempoRes=70;
    quint16    MetriRes=0;
    quint8     ProgInCorso=0;
    bool       StatoMotore=true;
    float      Pressione=0;
    qint16     AngAttuale=0;
    quint8     Auto=0;          // 0: No Abil, 1: Auto OFF, 2: AUTO ON punto dentro campo, 3: AUTO ON punto fuori campo

signals:
    void update_stato();
    void change_prog();
    void change_manaut();
    void reverse_save();
    void reverse_notsave();
    void pagina_rompigetto();
    void pagina_auto();

public slots:
    void RefreshStato();
    void Update_OK();

private:
    Ui::WidgetStato *ui;

    int height=0, width=0, lato_arco=0;
    float scala=0;

    // Stili
    QString Stile_Normale  = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: white;}";
    QString Stile_Selez = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: orange;}";
    QString Stile_Mod = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: yellow;}";

    QString StileB_Normale = "QPushButton{border-width: 0px; border-style: solid; border-color: rgb(0, 0, 0); outline: none; background-color: white;}";
    QString StileB_Mod = "QPushButton{border-width: 0px; border-style: solid; border-color: rgb(0, 0, 0); outline: none; background-color: yellow;}";

    bool AngI_sel=false, AngF_sel=false, Vel_sel=false;
    bool AngI_changed=false, AngF_changed=false, Vel_changed=false;


protected:
    bool eventFilter(QObject *object, QEvent *event);
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_pushButton_Meno_clicked();
    void on_pushButton_Piu_clicked();
    void on_pushButton_Update_clicked();
    void on_pushButton_ReverseS_clicked();
    void on_pushButton_ReverseN_clicked();
    void on_pushButton_P_clicked();
    void on_pushButton_Rompigetto_clicked();
    void on_pushButton_Auto_clicked();
};

#endif // Widget_Stato_H
