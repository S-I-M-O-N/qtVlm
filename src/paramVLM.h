/**********************************************************************
qtVlm: Virtual Loup de mer GUI
Copyright (C) 2008 - Christophe Thomas aka Oxygen77

http://qtvlm.sf.net

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/



#ifndef PARAM_VLM_H
#define PARAM_VLM_H

#include <QDialog>

#include "ui_paramVLM.h"

class paramVLM : public QDialog, public Ui::VLM_param_ui
{
    Q_OBJECT
    public:
        paramVLM(QWidget * parent = 0);
        void done(int result);

    public slots:
        void forceUserAgent_changed(int newVal);
        void changeColor_POI(void);
        void changeColor_qtBoat(void);
        void changeColor_qtBoat_sel(void);
        void changeColor_POI_WP(void);

    signals:
        void paramVLMChanged(void);
        void inetUpdated(void);

    private:
        void changeColor(int type);
        void setColor(QString color,int type);
        QColor getColor(int type);

        QString POI_color,POI_WP_color,qtBoat_color,qtBoat_sel_color;
};



#endif
