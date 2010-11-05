#include "twaline.h"
#include "mycentralwidget.h"
#include "Grib.h"
#include "boatVLM.h"
#include "vlmLine.h"
#include "Util.h"
#include "Polar.h"
#include <QDebug>

twaLine::twaLine(QPointF start, myCentralWidget *parent, MainWindow *main) : QDialog(parent)
{
    this->parent=parent;
    this->start=start;
    this->grib=parent->getGrib();
    this->myBoat=parent->getSelectedBoat();
    pen.setColor(Qt::yellow);
    pen.setBrush(Qt::yellow);
    pen.setWidthF(3);
    this->line=new vlmLine(parent->getProj(),parent->getScene(),Z_VALUE_ROUTE);
    line->setLinePen(pen);
    this->setWindowFlags(Qt::Tool);
    setupUi(this);
    this->tabWidget->setCurrentIndex(0);
    this->doubleSpinBox->setFocus();
    this->doubleSpinBox->selectAll();
    this->main=main;
    this->position=this->pos();
    connect(parent,SIGNAL(twaDelPoi(POI *)),this,SLOT(slot_delPOI_list(POI *)));
    traceIt();
}
twaLine::~twaLine()
{
}
void twaLine::slot_delPOI_list(POI * poi)
{
    list.removeAll(poi);
}
void twaLine::setStart(QPointF start)
{
    this->start=start;
    if(line!=NULL)
    {
        delete line;
        line=NULL;
    }
    QListIterator<POI*> i (list);
    while(i.hasNext())
    {
        POI * poi=i.next();
        list.removeOne(poi);
        if(poi->isPartOfTwa())
        {
            parent->slot_delPOI_list(poi);
            delete poi;
        }
    }
    this->line=new vlmLine(parent->getProj(),parent->getScene(),Z_VALUE_ROUTE);
    line->setLinePen(pen);
    this->tabWidget->setCurrentIndex(0);
    this->doubleSpinBox->setFocus();
    this->doubleSpinBox->selectAll();
    this->move(position);
    traceIt();
}

void twaLine::closeEvent(QCloseEvent */*event => unused*/)
{
    if(!this->checkBox_2->isChecked())
    {
        delete line;
        line=NULL;
    }
    if(!this->checkBox->isChecked())
    {
        QListIterator<POI*> i (list);
        while(i.hasNext())
        {
            POI * poi=i.next();
            list.removeOne(poi);
            if(poi->isPartOfTwa())
            {
                parent->slot_delPOI_list(poi);
                delete poi;
            }
        }
    }
    this->position=this->pos();
    QDialog::close();
}

void twaLine::traceIt()
{
    line->deleteAll();
    QListIterator<POI*> i (list);
    while(i.hasNext())
    {
        POI * poi=i.next();
        list.removeOne(poi);
        if(poi->isPartOfTwa())
        {
            parent->slot_delPOI_list(poi);
            delete poi;
        }
    }
    this->myBoat=parent->getSelectedBoat();
    if(myBoat==NULL) return;
    if(!grib->isOk()) return;
    if(!myBoat->getPolarData()) return;
    time_t eta=grib->getCurrentDate();
    nbVac[0]=this->spinBox->value();
    nbVac[1]=this->spinBox_2->value();
    nbVac[2]=this->spinBox_3->value();
    nbVac[3]=this->spinBox_4->value();
    nbVac[4]=this->spinBox_5->value();
    twa[0]=this->doubleSpinBox->value();
    twa[1]=this->doubleSpinBox_2->value();
    twa[2]=this->doubleSpinBox_3->value();
    twa[3]=this->doubleSpinBox_4->value();
    twa[4]=this->doubleSpinBox_5->value();
    int vacLen=myBoat->getVacLen();
    vlmPoint current(start.x(),start.y());
    line->addVlmPoint(current);
    double wind_speed,wind_angle,cap;
    double lon,lat;
    time_t maxDate=grib->getMaxDate();
    for (int page=0;page<5;page++)
    {
        if (nbVac[page]==0) continue;
        for(int i=1;i<=nbVac[page];i++)
        {
            if(!grib->getInterpolatedValue_byDates(current.lon, current.lat,
                eta,&wind_speed,&wind_angle,INTERPOLATION_DEFAULT) || eta>maxDate)
                break;
            wind_angle=radToDeg(wind_angle);
            cap=A360(wind_angle+twa[page]);
            float newSpeed=myBoat->getPolarData()->getSpeed(wind_speed,twa[page]);
            float distanceParcourue=newSpeed*vacLen/3600.00;
            Util::getCoordFromDistanceAngle(current.lat, current.lon, distanceParcourue, cap,&lat,&lon);
            current.lon=lon;
            current.lat=lat;
            line->addVlmPoint(current);
            eta=eta+vacLen;
        }
        QDateTime tm;
        tm.setTimeSpec(Qt::UTC);
        tm.setTime_t(eta);
        QString name;
        name.sprintf("Twa %.1f",twa[page]);
        POI * arrival=parent->slot_addPOI(name+tr(" ETA: ")+tm.toString("dd MMM-hh:mm"),0,lat,lon,-1,0,false,myBoat);
        arrival->setPartOfTwa(true);
        list.append(arrival);
    }
    line->slot_showMe();
    QApplication::processEvents();
}
double twaLine::A360(double hdg)
{
    if(hdg>=360) hdg=hdg-360;
    if(hdg<0) hdg=hdg+360;
    return hdg;
}

void twaLine::on_doubleSpinBox_valueChanged(double /*d => unused*/)
{
    traceIt();
}
void twaLine::on_spinBox_valueChanged(int /* i => unused*/)
{
    traceIt();
}
void twaLine::on_doubleSpinBox_2_valueChanged(double )
{
    traceIt();
}
void twaLine::on_spinBox_2_valueChanged(int )
{
    traceIt();
}

void twaLine::on_doubleSpinBox_3_valueChanged(double )
{
    traceIt();
}

void twaLine::on_spinBox_3_valueChanged(int )
{
    traceIt();
}

void twaLine::on_doubleSpinBox_4_valueChanged(double )
{
    traceIt();
}

void twaLine::on_spinBox_4_valueChanged(int )
{
    traceIt();
}

void twaLine::on_doubleSpinBox_5_valueChanged(double )
{
    traceIt();
}

void twaLine::on_spinBox_5_valueChanged(int )
{
    traceIt();
}
