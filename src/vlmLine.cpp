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

#include <QDebug>
#include "Orthodromie.h"
#include "vlmLine.h"
#include "settings.h"
#include "Projection.h"

vlmLine::vlmLine(Projection * proj, QGraphicsScene * myScene,int z_level) : QGraphicsWidget()
{
    this->proj=proj;
    this->myScene=myScene;
    connect(proj,SIGNAL(projectionUpdated()),this,SLOT(slot_showMe()));
    myScene->addItem(this);
    this->setZValue(z_level);
    this->linePen=QPen(Qt::red);
    linePen.setBrush(Qt::red);
    linePen.setWidth(2);
    setFocusPolicy(Qt::NoFocus);
    pt_color=QColor(Qt::red);
    mode=VLMLINE_LINE_MODE;
    nbVacPerHour=12;
    this->onePoint=false;
    this->hidden=false;
    this->boundingR.setRect(0,0,0,0);
    this->labelHidden=false;
    show();
}

vlmLine::~vlmLine()
{
//    myScene->removeItem(this);
}
QRectF vlmLine::boundingRect() const
{
    return boundingR;
}

void vlmLine::addPoint(float lat,float lon)
{
    vlmPoint point;
    point.lat=lat;
    point.lon=lon;
    line.append(point);
}

void vlmLine::setPoly(QList<vlmPoint> & points)
{
    line=points;
    calculatePoly();
    update();
}

void vlmLine::slot_showMe()
{
    int n=0;
    if(this->zValue()==4)
    {
        n=1;
    }
    calculatePoly();
    update();
}

void vlmLine::setLineMode()
{
    mode = VLMLINE_LINE_MODE;
    calculatePoly();
    update();
}

void vlmLine::setPointMode(QColor pt_color)
{
    mode = VLMLINE_POINT_MODE;
    this->pt_color = pt_color;
    calculatePoly();
    update();
}
void vlmLine::setGateMode(QString desc)
{
    mode = VLMLINE_GATE_MODE;
    this->desc=desc;
    calculatePoly();
    update();
}

void vlmLine::calculatePoly(void)
{
    int n=0;
    int X,Y,previousX=0,previousY,Xbis,Ybis;
//    int debug;
//    if(this->desc=="WP2: Arrivee Bayonne") //for debug point
//        debug=0;
    QRectF tempBound;
    tempBound.setRect(0,0,0,0);
    Orthodromie orth(0,0,0,0);
    QPolygon * poly;
    QListIterator<QPolygon*> nPoly (polyList);
    if(polyList.count()!=0)
    {
        while(nPoly.hasNext())
        {
            poly=nPoly.next();
            polyList.removeFirst();
            delete poly;
        }
    }
    poly=new QPolygon();
    polyList.append(poly);
    poly->resize(0);
    vlmPoint worldPoint,previousWorldPoint;
    previousWorldPoint.lat=0;
    previousWorldPoint.lon=0;
    if(line.count()>1)
    {
        QListIterator<vlmPoint> i (line);
        while(i.hasNext())
        {
            worldPoint=i.next();
            Util::computePos(proj,worldPoint.lat, worldPoint.lon, &X, &Y);
            X=X-(int)x();
            Y=Y-(int)y();
            if(n>0)
            {
                orth.setPoints(previousWorldPoint.lon,previousWorldPoint.lat,worldPoint.lon,worldPoint.lat);
                int azimut=qRound(orth.getAzimutDeg());
                int wrongEW=0;
                if(azimut>180 && X>previousX+10)
                    wrongEW=1; // on devrait etre a l'ouest et on est a droite
                else if(azimut<180 && X<previousX-10)
                    wrongEW=-1; // on devrait etre a l'est et on est a gauche
// case not in place
//               int wrongNS=0;
//               if((azimut<90 || azimut>270)  && previousY<Y)
//                    wrongNS=1; // on devrait etre au nord et on est au dessus
//                else if((azimut>90 && azimut<270)  && previousY>Y)
//                    wrongNS=-1; // on devrait etre au sud et on est en dessous
                if(wrongEW!=0 && mode==VLMLINE_GATE_MODE)
                {
                    proj->map2screen(worldPoint.lon-wrongEW*360, worldPoint.lat, &Xbis, &Ybis);
                    poly->putPoints(n,1,Xbis,Ybis);
                    tempBound=tempBound.united(poly->boundingRect());
                    proj->map2screen(previousWorldPoint.lon+wrongEW*360, previousWorldPoint.lat, &Xbis, &Ybis);
                    poly=new QPolygon();
                    poly->resize(0);
                    polyList.append(poly);
                    n=0;
                    poly->putPoints(n,1,Xbis,Ybis);
                    n++;
                }
            }
            poly->putPoints(n,1,X,Y);
            previousWorldPoint=worldPoint;
            previousX=X;
            previousY=Y;
            n++;
        }
        tempBound=tempBound.united(poly->boundingRect());
    }
    if(!polyList.isEmpty())
    {
        poly=polyList.first();
        if(poly->size()!=0)
        {
            int width=20;
            int height=10;
            setFont(QFont("Helvetica",9));
            QFontMetrics fm(font());
            width = fm.width(desc) + 10 +2;
            height = qMax(fm.height()+2,10);
            int x=poly->point(0).x()+5;
            int y=poly->point(0).y()-5;
            QRectF r;
            r.setRect(x,y, width-10,height-1);
            double x1,y1,x2,y2;
            tempBound=tempBound.united(r);
            tempBound.normalized();
            tempBound.getCoords(&x1,&y1,&x2,&y2);
            tempBound.setCoords(x1-linePen.widthF()*2,y1-linePen.widthF()*2,x2+linePen.widthF()*2,y2+linePen.widthF()*2);
        }
    }
    prepareGeometryChange();
    boundingR=tempBound;
}

void vlmLine::deleteAll()
{
    while(line.count()!=0)
        line.removeFirst();
    calculatePoly();
    update();
}

void vlmLine::paint(QPainter * pnt, const QStyleOptionGraphicsItem * , QWidget * )
{
//    int debug;
//    if(this->desc=="WP1: Latitude Cap Vert") //for debug point
//        debug=0;
    pnt->setPen(linePen);
    float penW=linePen.widthF();
    if(polyList.isEmpty()) return;
    QPolygon * poly;
    QListIterator<QPolygon*> nPoly (polyList);
    bool labelAlreadyMade=false;
    while(nPoly.hasNext())
    {
        poly=nPoly.next();
        if(poly->size()==0) continue;
        switch(mode)
        {
        case VLMLINE_LINE_MODE:
            if(!hidden)
                pnt->drawPolyline(*poly);
            break;
        case VLMLINE_POINT_MODE:
            if(!hidden)
            {
                int nbVac=nbVacPerHour*Settings::getSetting("trace_length",12).toInt();
                int step=Settings::getSetting("trace_step",60/5-1).toInt()+1;
                int x0=poly->point(0).x();
                int y0=poly->point(0).y();
                for(int i=1;i<poly->count() && i<nbVac;i++)
                {
                    int x,y;
                    if(i%step) /* not taking all vac*/
                        continue;
                    x=poly->point(i).x();
                    y=poly->point(i).y();
                    if(i%10)
                        linePen.setWidthF(penW+1);
                    else
                        linePen.setWidthF(penW+2);
                    pnt->setPen(linePen);
                    pnt->drawPoint(x,y);
                    linePen.setWidthF(penW);
                    pnt->setPen(linePen);
                    pnt->drawLine(x0,y0,x,y);
                    x0=x;
                    y0=y;
                }
            }
            break;
        case VLMLINE_GATE_MODE:
            if(onePoint)
            {
                linePen.setStyle(Qt::DashDotDotLine);
                pnt->setPen(linePen);
            }
            if(!hidden)
                pnt->drawPolyline(*poly);
            linePen.setWidthF(penW*2);
            if(onePoint)
            {
                linePen.setStyle(Qt::SolidLine);
            }
            pnt->setPen(linePen);
            if(onePoint)
                pnt->drawPoint(poly->point(0));
            else
                pnt->drawPoints(*poly);
            linePen.setWidthF(penW);
            pnt->setPen(linePen);
            if(labelAlreadyMade || labelHidden) break;
            QColor bgcolor = QColor(255,255,255,150);
            int width=20;
            int height=10;
            setFont(QFont("Helvetica",9));
            QFontMetrics fm(font());
            width = fm.width(desc) + 10 +2;
            height = qMax(fm.height()+2,10);
            int x=poly->point(0).x()+5;
            int y=poly->point(0).y()-5;
            pnt->setFont(font());
            QPen linePenBis(Qt::black,1);
            linePenBis.setWidthF(1);
            pnt->setPen(linePenBis);
            QRectF r;
            r.setRect(x,y, width-10,height-1);
            pnt->fillRect(r, QBrush(bgcolor));
            pnt->drawRect(r);
            pnt->drawText(x,y+fm.height()-2,desc);
            pnt->setPen(linePen);
            labelAlreadyMade=true;
            break;
        }
    }
//    if(this->zValue()==4) //uncomment to see the boundingRect for routes
//    {
//        QPen linePenBis(Qt::black,1);
//        linePenBis.setWidthF(1);
//        pnt->setPen(linePenBis);
//        pnt->drawRect(boundingR);
//        pnt->setPen(linePen);
//    }
}