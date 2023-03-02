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

#ifndef GETTO_H
#define GETTO_H
#include <QString>

typedef enum{
ON_LINE=0,
OFF_LINE=1,
CONNECTING=2,
CONNECTED=3,
ERROR_TCP=4
}Getto_Status;

typedef struct{
QString                 Nome;
QString                 NumeroID;
QString                 NumeroSIM;
QString                 DataScadenza;
QString                 DataBonus;
Getto_Status            Status;
bool                    AbbScaduto = false;
}Getto_t;

typedef struct{
QString                 Nome;
qint16                  Ang1=0, Ang2=0, Ang3=0, Ang4=0, Ang5=0, Ang6=0, Ang7=0, Ang8=0;
quint16                 Tempo12=0, Tempo34=0, Tempo56=0;
quint16                 Rot1234=0, Rot3456=0, Rot5678=0;
quint16                 Metri12=0, Metri34=0, Metri56=0;
quint8                  SelTM=0;
}Prog_t;

#endif // GETTO_H
