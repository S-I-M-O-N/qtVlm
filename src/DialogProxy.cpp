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

#include <QMessageBox>
#include <cmath>
#include <cassert>

#include "DialogProxy.h"
#include "Util.h"


//-------------------------------------------------------------------------------
DialogProxy::DialogProxy() : QDialog()
{
    QLabel *label;
    QFrame *ftmp;
    frameGui = createFrameGui(this);

    layout = new QGridLayout(this);
    int lig=0;
    //-------------------------
    lig ++;
    QFont font;
    font.setBold(true);
    label = new QLabel(tr("Mode de connexion à internet"), this);
    label->setFont(font);
    layout->addWidget( label,    lig,0, 1,-1, Qt::AlignCenter);
    lig ++;
    ftmp = new QFrame(this); ftmp->setFrameShape(QFrame::HLine); layout->addWidget( ftmp, lig,0, 1, -1);
    //-------------------------
    lig ++;
    layout->addWidget( frameGui,  lig,0,   1, 2);
    //-------------------------
    lig ++;
    ftmp = new QFrame(this); ftmp->setFrameShape(QFrame::HLine); layout->addWidget( ftmp, lig,0, 1, -1);
    //-------------------------
    lig ++;
    btOK     = new QPushButton(tr("Valider"), this);
    btCancel = new QPushButton(tr("Annuler"), this);
    layout->addWidget( btOK,    lig,0);
    layout->addWidget( btCancel, lig,1);

    //===============================================================
    lineProxyHostname->setText(Util::getSetting("httpProxyHostname", "").toString());
    lineProxyPort->setText(Util::getSetting("httpProxyPort", "").toString());
    lineProxyUsername->setText(Util::getSetting("httpProxyUsername", "").toString());
    lineProxyUserPassword->setText(Util::getSetting("httpProxyUserPassword", "").toString());

    int usep = Util::getSetting("httpUseProxy", 0).toInt();

#ifndef QT_4_5
    if(usep==2) usep=1;
#endif

    switch(usep)
    {
       case 0:
            btUseProxy->setChecked(false);
            btDontUseProxy->setChecked(true);
#ifdef QT_4_5
            btUseIECfg->setChecked(false);
#endif
            break;
       case 1:
            btUseProxy->setChecked(true);
            btDontUseProxy->setChecked(false);
#ifdef QT_4_5
            btUseIECfg->setChecked(false);
#endif
            break;
#ifdef QT_4_5
       case 2:
            btUseProxy->setChecked(false);
            btDontUseProxy->setChecked(false);
            btUseIECfg->setChecked(true);
            break;
#endif
    }
    slotUseProxyChanged();

    //===============================================================
    connect(btUseProxy, SIGNAL(clicked()), this, SLOT(slotUseProxyChanged()));
    connect(btDontUseProxy, SIGNAL(clicked()), this, SLOT(slotUseProxyChanged()));
#ifdef QT_4_5
    connect(btUseIECfg, SIGNAL(clicked()), this, SLOT(slotUseProxyChanged()));
#endif
    connect(btCancel, SIGNAL(clicked()), this, SLOT(slotBtCancel()));
    connect(btOK, SIGNAL(clicked()), this, SLOT(slotBtOK()));
}

//-------------------------------------------------------------------------------
void DialogProxy::slotUseProxyChanged()
{
    bool usep = btUseProxy->isChecked();
#ifdef QT_4_5
    bool useIe = btUseIECfg->isChecked();
#else
    bool useIe = false;
#endif
    lineProxyHostname->setEnabled(usep);
    lineProxyPort->setEnabled(usep);
    lineProxyUsername->setEnabled(usep|useIe);
    lineProxyUserPassword->setEnabled(usep|useIe);
}

//-------------------------------------------------------------------------------
void DialogProxy::slotBtOK()
{
#ifdef QT_4_5
    int proxyType=btUseProxy->isChecked()?1:btUseIECfg->isChecked()?2:0;
#else
    int proxyType=btUseProxy->isChecked()?1:0;
#endif
    Util::setSetting("httpUseProxy",proxyType);
    Util::setSetting("httpProxyHostname", lineProxyHostname->text());
    Util::setSetting("httpProxyPort", lineProxyPort->text());
    Util::setSetting("httpProxyUsername", lineProxyUsername->text());
    Util::setSetting("httpProxyUserPassword", lineProxyUserPassword->text());
    emit proxyUpdated();
    accept();
}
//-------------------------------------------------------------------------------
void DialogProxy::slotBtCancel()
{
    reject();
}

//=============================================================================
// GUI
//=============================================================================
QFrame *DialogProxy::createFrameGui(QWidget *parent)
{
    QFrame * frm = new QFrame(parent);
    QFrame * ftmp;
    QLabel * label;
    QGridLayout  *lay = new QGridLayout(frm);
    int lig=0;
    //-------------------------
    QButtonGroup *grp = new QButtonGroup(frm);
    lig ++;
    btDontUseProxy     = new QRadioButton(tr("Connexion directe à internet"), frm);
    grp->addButton(btDontUseProxy);
    lay->addWidget( btDontUseProxy,    lig,0,   1, 2);
    lig ++;
#ifdef QT_4_5
    btUseIECfg = new QRadioButton(tr("Utilise les parametres de IE"), frm);
    grp->addButton(btUseIECfg);
    lay->addWidget(btUseIECfg,    lig,0,   1, 2);
    lig++;
#endif
    btUseProxy     = new QRadioButton(tr("Connexion à travers un proxy"), frm);
    grp->addButton(btUseProxy);
    lay->addWidget( btUseProxy,    lig,0,   1, 2);
    //-------------------------
    lig ++;
    ftmp = new QFrame(frm); ftmp->setFrameShape(QFrame::HLine); lay->addWidget( ftmp, lig,0, 1, -1);
    //-------------------------
    lig ++;
    label = new QLabel(tr("Serveur de proxy :"), frm);
    lay->addWidget( label,    lig,0, Qt::AlignRight);
    lineProxyHostname = new QLineEdit(frm);
    lineProxyHostname->setFixedWidth(400);
    lay->addWidget( lineProxyHostname, lig,1, Qt::AlignLeft);
    lig ++;
    label = new QLabel(tr("Numéro de port :"), frm);
    lay->addWidget( label,    lig,0, Qt::AlignRight);
    lineProxyPort = new QLineEdit(frm);
    lineProxyPort->setFixedWidth(60);

    lineProxyPort->setValidator(new QIntValidator(0,65536, this));
    lay->addWidget( lineProxyPort, lig,1, Qt::AlignLeft);
    //-------------------------
    lig ++;
    label = new QLabel(tr("Utilisateur * :"), frm);
    lay->addWidget( label,    lig,0, Qt::AlignRight);
    lineProxyUsername = new QLineEdit(frm);
    lineProxyUsername->setFixedWidth(400);
    lay->addWidget( lineProxyUsername, lig,1, Qt::AlignLeft);
    lig ++;
    label = new QLabel(tr("Mot de passe * :"), frm);
    lay->addWidget( label,    lig,0, Qt::AlignRight);
    lineProxyUserPassword = new QLineEdit(frm);
    lineProxyUserPassword->setFixedWidth(400);
    lineProxyUserPassword->setEchoMode(QLineEdit::Password);
    lay->addWidget( lineProxyUserPassword, lig,1, Qt::AlignLeft);
    lig ++;
    label = new QLabel(tr("(* si nécessaire)"), frm);
    lay->addWidget( label,    lig,0, 2,1, Qt::AlignLeft);

    return frm;
}











