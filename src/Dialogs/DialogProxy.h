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

Original code: zyGrib: meteorological GRIB file viewer
Copyright (C) 2008 - Jacques Zaninetti - http://zygrib.free.fr

***********************************************************************/

#ifndef DIALOGPROXY_H
#define DIALOGPROXY_H
#ifdef QT_V5
#include <QtWidgets/QDialog>
#else
#include <QDialog>
#endif
#include "class_list.h"
#include "ui_paramProxy.h"


class DialogProxy : public QDialog, public Ui::paramProxy_ui
{ Q_OBJECT
    public:
        DialogProxy();

        void done(int result);
    
    public slots:
        void slot_proxyType_changed(int);
        void slot_useProxy_changed();

    signals:
        void proxyUpdated(void);
};


#endif
