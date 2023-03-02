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

#ifndef WIDGETGETTO_H
#define WIDGETGETTO_H

#include <QWidget>
#include "getto.h"

namespace Ui {
class WidgetGetto;
}

class WidgetGetto : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetGetto(QWidget *parent = 0, Getto_t *pElemento = NULL);
    ~WidgetGetto();

    int height, heightImg;
    bool SimNum=false;

    void Refresh();
private:
    Ui::WidgetGetto *ui;

    Getto_t* Elemento;

};

#endif // WidgetGetto_H
