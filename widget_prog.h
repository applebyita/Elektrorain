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

#ifndef WIDGET_PROG_H
#define WIDGET_PROG_H

#include <QWidget>
#include <QDir>
#include "getto.h"

namespace Ui {
class WidgetProg;
}

class WidgetProg : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetProg(QWidget *parent = 0);
    ~WidgetProg();

    bool PageLoadSave=false;

    // Valori Prog Getto
    quint8          ProgSel=0;
    qint16          Ang1_P1=0, Ang2_P1=0, Ang3_P1=0, Ang4_P1=0, Ang5_P1=0, Ang6_P1=0, Ang7_P1=0, Ang8_P1=0;
    qint16          Ang1_P2=0, Ang2_P2=0, Ang3_P2=0, Ang4_P2=0, Ang5_P2=0, Ang6_P2=0, Ang7_P2=0, Ang8_P2=0;
    quint16         Tempo12_P1=0, Tempo34_P1=0, Tempo56_P1=0;
    quint16         Tempo12_P2=0, Tempo34_P2=0, Tempo56_P2=0;
    quint16         Rot1234_P1=0, Rot3456_P1=0, Rot5678_P1=0;
    quint16         Rot1234_P2=0, Rot3456_P2=0, Rot5678_P2=0;
    quint16         Metri12_P1=0, Metri34_P1=0, Metri56_P1=0;
    quint16         Metri12_P2=0, Metri34_P2=0, Metri56_P2=0;
    quint8          SelTM_P1=0, SelTM_P2=0;		// 0: tempo , 1: metri

signals:
    void update_prog();

public slots:
    void RefreshProg();
    void Update_OK();
    void Default_Page();

private:
    Ui::WidgetProg *ui;

    int lato_arco=0;
    bool OkResize=false;

    QFont FontLabel, FontLineEdit;

    QDir ProjectPath;
    QString ProgDirectory, ImpExpDirectory;
    QFile FileLista;
    QList<Prog_t*> ListaProg;
    quint8 SelectedProg=0;

    // Stili
    QString Stile_Normale  = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: white;}";
    QString Stile_Selez = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: orange;}";
    QString Stile_Mod = "QLineEdit{ border-width: 1px; border-style: solid; border-radius: 4px; border-color: rgb(0, 0, 0); background-color: yellow;}";

    QString StileB_Normale = "QPushButton{border-width: 0px; border-style: solid; border-color: rgb(0, 0, 0); outline: none; background-color: white;}";
    QString StileB_Mod = "QPushButton{border-width: 0px; border-style: solid; border-color: rgb(0, 0, 0); outline: none; background-color: yellow;}";

    bool Ang1_sel=false, Ang2_sel=false, Ang3_sel=false, Ang4_sel=false, Ang5_sel=false, Ang6_sel=false, Ang7_sel=false, Ang8_sel=false;
    bool Tempo12hh_sel=false, Tempo12mm_sel=false, Tempo34hh_sel=false, Tempo34mm_sel=false, Tempo56hh_sel=false, Tempo56mm_sel=false;
    bool Ang1_P1_changed=false, Ang2_P1_changed=false, Ang3_P1_changed=false, Ang4_P1_changed=false, Ang5_P1_changed=false, Ang6_P1_changed=false, Ang7_P1_changed=false, Ang8_P1_changed=false;
    bool Tempo12hh_P1_changed=false, Tempo12mm_P1_changed=false, Tempo34hh_P1_changed=false, Tempo34mm_P1_changed=false, Tempo56hh_P1_changed=false, Tempo56mm_P1_changed=false;
    bool Metri12_P1_changed=false, Metri34_P1_changed=false, Metri56_P1_changed=false;
    bool Rot1234_P1_changed=false, Rot3456_P1_changed=false, Rot5678_P1_changed=false;
    bool Ang1_P2_changed=false, Ang2_P2_changed=false, Ang3_P2_changed=false, Ang4_P2_changed=false, Ang5_P2_changed=false, Ang6_P2_changed=false, Ang7_P2_changed=false, Ang8_P2_changed=false;
    bool Tempo12hh_P2_changed=false, Tempo12mm_P2_changed=false, Tempo34hh_P2_changed=false, Tempo34mm_P2_changed=false, Tempo56hh_P2_changed=false, Tempo56mm_P2_changed=false;
    bool Metri12_P2_changed=false, Metri34_P2_changed=false, Metri56_P2_changed=false;
    bool Rot1234_P2_changed=false, Rot3456_P2_changed=false, Rot5678_P2_changed=false;

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_pushButton_Meno_clicked();
    void on_pushButton_Piu_clicked();
    void on_pushButton_Update_clicked();
    void on_pushButton_P1_clicked();
    void on_pushButton_P2_clicked();
    void on_pushButton_Rot1234_clicked();
    void on_pushButton_Rot3456_clicked();
    void on_pushButton_Rot5678_clicked();
    void on_pushButton_TM_clicked();
    void on_pushButton_LoadSave_clicked();

    void ResizeButtons();
    void CaricaProg();
    void FileSave();

    void on_pushButton_Save_clicked();
    void on_pushButton_Load_clicked();
    void on_pushButton_Previous_clicked();
    void on_pushButton_Next_clicked();
    void on_pushButton_Erase_clicked();
    void on_pushButton_Import_clicked();
    void on_pushButton_Export_clicked();
};

#endif // Widget_Prog_H
