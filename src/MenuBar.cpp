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

Original code zyGrib: meteorological GRIB file viewer
Copyright (C) 2008 - Jacques Zaninetti - http://zygrib.free.fr
***********************************************************************/

#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QIcon>


#include "MenuBar.h"
#include "boatVLM.h"
#include "Util.h"
#include "route.h"
#include "routage.h"
#include "Terrain.h"
#include "settings.h"
#include "ToolBar.h"
//#include "Board.h"
#include "BarrierSet.h"
#include "MapDataDrawer.h"


//===================================================================================
MenuBar::MenuBar(MainWindow *parent)
    : QMenuBar(parent)
{
    mainWindow=parent;
    this->setAccessibleName("mainMenuQtvlm");

     menuAltitude=NULL;
     acAlt_GroupAltitude=NULL;
    //-------------------------------------
    // Menu + Actions
    //-------------------------------------
    menuFile = new QMenu(tr("QtVlm"));

        acFile_Quit = addAction(menuFile,
                    tr("Quitter"), tr("Ctrl+Q"), tr("Bye"), appFolder.value("img")+"exit.png");

        acFile_QuitNoSave = addAction(menuFile,
                                      tr("Quitter sans sauver"), "", "", appFolder.value("img")+"exit2.png");
        //acFile_QuitNoSave->setMenuRole(QAction::ApplicationSpecificRole);
        menuFile->addSeparator();
        acFile_Lock = addAction(menuFile,
                    tr("Verrouiller"), tr("Ctrl+L"), tr("Verrouiller l'envoi d'ordre a VLM"), appFolder.value("img")+"unlock.png");
        separator1=menuFile->addSeparator();
        mn_img=new QMenu(tr("Gestion des fichiers KAP"));
        acImg_Open = addAction(mn_img, tr("Ouvrir un fichier KAP"), "K", tr(""));
        mn_img->addAction(acImg_Open);
        acImg_Close = addAction(mn_img, tr("Fermer le fichier KAP"), "Shift+K", tr(""));
        mn_img->addAction(acImg_Close);
        menuFile->addMenu(mn_img);
        menuFile->addSeparator();



        acHorn=addAction(menuFile,tr("Configurer la corne de brume"),"","",tr(""));
        //acHorn->setMenuRole(QAction::ApplicationSpecificRole);
        acReplay=addAction(menuFile,tr("Rejouer l'historique des traces"),"Y","",tr(""));
        acScreenshot=addAction(menuFile,tr("Photo d'ecran"),"Ctrl+E","",tr(""));

    addMenu(menuFile);

    menuView = new QMenu(tr("View"));
    connect(menuView,SIGNAL(aboutToShow()),this,SLOT(slot_showViewMenu()));
    /*boardMenu = new QMenu(tr("Board"));
    menuView->addMenu(boardMenu);*/
    toolBarMenu = new QMenu(tr("ToolBar"));
    menuView->addMenu(toolBarMenu);
    menuView->addSeparator();
    acOptions_SH_sAll = addAction(menuView, tr("Tout montrer"), "S", tr(""));
    acOptions_SH_hAll = addAction(menuView, tr("Tout cacher sauf les bateaux actifs"), "H", tr(""));
    menuView->addSeparator();
    QMenu * boatPoiSH = new QMenu(tr("Show/Hide boat and POI"));
        acOptions_SH_Boa = addAction(boatPoiSH, tr("Centrer sur le bateau actif"), "B", tr(""));
        acKeep=addAction(boatPoiSH,tr("Conserver la position du bateau dans l'ecran lors de zoom +/-"),"Z","",tr(""));
        acKeep->setCheckable(true);
        separator2=boatPoiSH->addSeparator();
        acOptions_SH_Fla = addAction(boatPoiSH, tr("Montrer les pavillons sur la carte"), "F", tr(""));
        acOptions_SH_Fla->setCheckable(true);
        acOptions_SH_Opp = addAction(boatPoiSH, tr("Montrer les bateaux opposants"), "O", tr(""));
        acOptions_SH_Opp->setCheckable(true);
        boatPoiSH->addSeparator();
        acOptions_SH_Por = addAction(boatPoiSH, tr("Montrer les portes et WPs"), "W", tr(""));
        acOptions_SH_Por->setCheckable(true);
        acOptions_SH_Poi = addAction(boatPoiSH, tr("Montrer les POIs"), "P", tr(""));
        acOptions_SH_Poi->setCheckable(true);
        acOptions_SH_Rou = addAction(boatPoiSH, tr("Montrer les routes"), "R", tr(""));
        acOptions_SH_Rou->setCheckable(true);
        acOptions_SH_Lab = addAction(boatPoiSH, tr("Montrer les etiquettes"), "E", tr(""));
        acOptions_SH_Lab->setCheckable(true);
        acOptions_SH_barSet = addAction(boatPoiSH, tr("Montrer les barrieres"), "D", tr(""));
        acOptions_SH_barSet->setCheckable(true);
    menuView->addMenu(boatPoiSH);
    QMenu * compasSH = new QMenu(tr("Show/Hide compas"));
    acOptions_SH_Com = addAction(compasSH, tr("Cacher/Montrer le compas"), "C", tr(""));
    acOptions_SH_Com->setCheckable(true);
    acOptions_SH_Pol = addAction(compasSH, tr("Cacher/Montrer la polaire"), "L", tr(""));
    acOptions_SH_Pol->setCheckable(true);

    menuView->addMenu(compasSH);

    menuView->addSeparator();

    acOptions_SH_Nig = addAction(menuView, tr("Montrer les zones de jour et nuit"), "N", tr(""));
    acOptions_SH_Nig->setCheckable(true);

    acOptions_SH_Tdb = addAction(menuView, tr("Montrer le tableau de bord"), "T", tr(""));
    acOptions_SH_Tdb->setCheckable(true);

    addMenu(menuView);

    //-------------------------------------
    menuGrib = new QMenu(tr("Fichier GRIB"));
        acFile_Open = addAction(menuGrib, tr("Ouvrir"),
                    tr("Ctrl+O"),
                    tr("Ouvrir un fichier GRIB"), appFolder.value("img")+"fileopen.png");
        acFile_Reopen = addAction(menuGrib, tr("Recharcher"),
                    "",
                    tr("Recharger le fichier GRIB actuel"), appFolder.value("img")+"fileopen.png");
        acFile_Close = addAction(menuGrib, tr("Fermer"),
                    tr("Ctrl+W"),
                    tr("Fermer"), appFolder.value("img")+"fileclose.png");
        acFile_Load_GRIB = addAction(menuGrib, tr("Telechargement"),
                    tr("Ctrl+D"),
                    tr("Telechargement"), appFolder.value("img")+"network.png");
        acFile_Load_VLM_GRIB = addAction(menuGrib, tr("Telechargement VLM"),
                    tr(""),
                    tr("Telechargement VLM"), appFolder.value("img")+"VLM_mto.png");
        acFile_Load_SAILSDOC_GRIB = addAction(menuGrib, tr("Telechargement SailsDoc"),
                                                tr(""),
                                                tr("Telechargement SailsDoc"), appFolder.value("img")+"kmail.png");
        acFile_Info_GRIB_main = addAction(menuGrib, tr("Informations sur le fichier"),
                    tr("Ctrl+I"),
                    tr("Informations sur le fichier GRIB"), appFolder.value("img")+"info.png");
        menuGrib->addSeparator();
        acFile_Open_Current = addAction(menuGrib, tr("Ouvrir un GRIB Courants"),
                    tr(""),
                    tr("Ouvrir un fichier GRIB Courants"), appFolder.value("img")+"fileopen.png");
        acFile_Close_Current = addAction(menuGrib, tr("Fermer le GRIB Courants"),
                    tr(""),
                    tr("Fermer le GRIB Courants"), appFolder.value("img")+"fileclose.png");
        acFile_Info_GRIB_current = addAction(menuGrib, tr("Informations sur le fichier"),
                    tr(""),
                    tr("Informations sur le fichier GRIB"), appFolder.value("img")+"info.png");
        menuGrib->addSeparator();

        menuGroupColorMap = new QMenu(tr("Type de carte"));
        acView_GroupColorMap = new ZeroOneActionGroup (menuGroupColorMap);
                acView_WindColors = addActionCheck(menuGroupColorMap, tr("Carte du vent"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawWind,acView_WindColors);
                acView_GroupColorMap->addAction(acView_WindColors);
                acView_CurrentColors = addActionCheck(menuGroupColorMap, tr("Carte du courant"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawCurrent,acView_CurrentColors);
                acView_GroupColorMap->addAction(acView_CurrentColors);
                acView_RainColors = addActionCheck(menuGroupColorMap, tr("Carte des preecipitations"),"","");
                gribDataActionMap.insert(MapDataDrawer::drawRain,acView_RainColors);
                acView_GroupColorMap->addAction(acView_RainColors);
                acView_CloudColors = addActionCheck(menuGroupColorMap, tr("Couverture nuageuse"), "","");
                gribDataActionMap.insert(MapDataDrawer::drawCloud,acView_CloudColors);
                acView_GroupColorMap->addAction(acView_CloudColors);
                acView_HumidColors = addActionCheck(menuGroupColorMap, tr("Carte de l'humidite relative"),"","");
                gribDataActionMap.insert(MapDataDrawer::drawHumid,acView_HumidColors);
                acView_GroupColorMap->addAction(acView_HumidColors);
                acView_TempColors = addActionCheck(menuGroupColorMap, tr("Carte de la temperature"),"","");
                gribDataActionMap.insert(MapDataDrawer::drawTemp,acView_TempColors);
                acView_GroupColorMap->addAction(acView_TempColors);
                acView_TempPotColors = addActionCheck(menuGroupColorMap, tr("Carte de la temperature potentielle"),"","");
                gribDataActionMap.insert(MapDataDrawer::drawTempPot,acView_TempPotColors);
                acView_GroupColorMap->addAction(acView_TempPotColors);
                acView_DeltaDewpointColors = addActionCheck(menuGroupColorMap, tr("Ecart temperature-point de rosee"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawDeltaDewpoint,acView_DeltaDewpointColors);
                acView_GroupColorMap->addAction(acView_DeltaDewpointColors);
                acView_SnowCateg = addActionCheck(menuGroupColorMap, tr("Neige (chute possible)"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawSnowCateg,acView_SnowCateg);
                acView_GroupColorMap->addAction(acView_SnowCateg);
                //acView_SnowDepth = addActionCheck(menuGroupColorMap, tr("Neige (Epaisseur)"), "", "");
                //acView_GroupColorMap->addAction(acView_SnowDepth);
                acView_FrzRainCateg = addActionCheck(menuGroupColorMap, tr("Pluie verglacante (chute possible)"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawFrzRainCateg,acView_FrzRainCateg);
                acView_GroupColorMap->addAction(acView_FrzRainCateg);
                acView_CAPEsfc = addActionCheck(menuGroupColorMap, tr("CAPE (surface)"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawCAPEsfc,acView_CAPEsfc);
                acView_GroupColorMap->addAction(acView_CAPEsfc);
                acView_CINsfc = addActionCheck(menuGroupColorMap, tr("CIN (surface)"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawCINsfc,acView_CINsfc);
                acView_GroupColorMap->addAction(acView_CINsfc);
                // Waves
                menuGroupWaves = new QMenu(tr("Waves"));
                //acView_GroupWaves = new ZeroOneActionGroup (menuGroupWaves);
                acView_WavesSigHgtComb = addActionCheck(menuGroupWaves, tr("Waves combined"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawWavesSigHgtComb,acView_WavesSigHgtComb);
                acView_GroupColorMap->addAction(acView_WavesSigHgtComb);
                acView_WavesWnd = addActionCheck(menuGroupWaves, tr("Wind waves"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawWavesWnd,acView_WavesWnd);
                acView_GroupColorMap->addAction(acView_WavesWnd);
                acView_WavesSwl = addActionCheck(menuGroupWaves, tr("Swell waves"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawWavesSwl,acView_WavesSwl);
                acView_GroupColorMap->addAction(acView_WavesSwl);
                acView_WavesMax = addActionCheck(menuGroupWaves, tr("Max waves"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawWavesMax,acView_WavesMax);
                acView_GroupColorMap->addAction(acView_WavesMax);
                acView_WavesWhiteCap = addActionCheck(menuGroupWaves, tr("White cap prob"), "", "");
                gribDataActionMap.insert(MapDataDrawer::drawWavesWhiteCap,acView_WavesWhiteCap);
                acView_GroupColorMap->addAction(acView_WavesWhiteCap);
                menuGroupWaves->addSeparator();
                acView_WavesArrow = addActionCheck(menuGroupWaves, tr("Waves arrow"), tr(""),
                            tr("Show arrow for wave direction"));
                acView_WavesArrow->setChecked(Settings::getSetting("showWavesArrows", true).toBool());


                menuGroupColorMap->addMenu(menuGroupWaves);

        menuGrib->addMenu(menuGroupColorMap);
        menuGrib->addSeparator();

        // init string list for levelTypes and units
        /*
        DATA_LV_GND_SURF=0,
        DATA_LV_ISOTHERM0,
        DATA_LV_ISOBARIC,
        DATA_LV_MSL,
        DATA_LV_ABOV_GND,
        DATA_LV_SIGMA,
        DATA_LV_ATMOS_ALL,
        DATA_LV_ORDERED_SEQUENCE_DATA
        */

        levelTypes.append(tr("Surface"));
        levelTypesUnit.append("unit ?");
        levelTypes.append(tr("Isotherm 0C"));
        levelTypesUnit.append("unit ?");
        levelTypes.append(tr("Isobaric"));
        levelTypesUnit.append("hPa");
        levelTypes.append(tr("Mean Sea Level"));
        levelTypesUnit.append("unit ?");
        levelTypes.append(tr("Above ground"));
        levelTypesUnit.append("m");
        levelTypes.append(tr("Sigma"));
        levelTypesUnit.append("?");
        levelTypes.append(tr("Entire atmosphere"));
        levelTypesUnit.append("unit ?");
        levelTypes.append(tr("Ordered sequence"));
        levelTypesUnit.append("?");

        //-------------------------------------
        menuAltitude = new QMenu(tr("Altitude"));
        acAlt_GroupAltitude = new ZeroOneActionGroup (menuAltitude);
        //connect(menuAltitude,SIGNAL(aboutToShow()),this,SLOT(slot_showAltitudeMenu()));

       //menuGrib->addMenu(menuAltitude);
       menuGrib->addSeparator();

       setMenubarColorMapMode(Settings::getSetting("colorMapMode", MapDataDrawer::drawWind).toInt());

        acView_ColorMapSmooth = addActionCheck(menuGrib, tr("Degrades de couleurs"), tr(""),
                    tr(""));
        acView_ColorMapSmooth->setChecked(Settings::getSetting("colorMapSmooth", true).toBool());
        acView_WindArrow = addActionCheck(menuGrib, tr("Fleches du vent"), tr(""),
                    tr("Afficher les fleches de direction du vent"));
        acView_WindArrow->setChecked(Settings::getSetting("showWindArrows", true).toBool());
        acView_Barbules = addActionCheck(menuGrib, tr("Barbules"), tr(""),
                    tr("Afficher les barbules sur les fleches de vent"));
        acView_Barbules->setChecked(Settings::getSetting("showBarbules", true).toBool());
        menuGrib->addSeparator();
        acView_TemperatureLabels = addActionCheck(menuGrib,
                                tr("Temperature"), tr("Ctrl+T"),
                    "");
        acView_TemperatureLabels->setChecked(Settings::getSetting("showTemperatureLabels", false).toBool());
        //--------------------------------
        menuGrib->addSeparator();
                menuIsobars = new QMenu(tr("Isobares"));
                acView_Isobars = addActionCheck(menuIsobars, tr("Afficher les isobares"), "","");
                acView_Isobars->setChecked(Settings::getSetting("showIsobars", true).toBool());
            menuIsobarsStep = new QMenu(tr("Espacement (hPa)"));
            acView_GroupIsobarsStep = new QActionGroup(menuIsobarsStep);
                acView_IsobarsStep1 = addActionCheck(menuIsobarsStep, tr("1"), "", tr("Espacement des isobares"));
                acView_IsobarsStep2 = addActionCheck(menuIsobarsStep, tr("2"), "", tr("Espacement des isobares"));
                acView_IsobarsStep3 = addActionCheck(menuIsobarsStep, tr("3"), "", tr("Espacement des isobares"));
                acView_IsobarsStep4 = addActionCheck(menuIsobarsStep, tr("4"), "", tr("Espacement des isobares"));
                acView_IsobarsStep5 = addActionCheck(menuIsobarsStep, tr("5"), "", tr("Espacement des isobares"));
                acView_IsobarsStep6 = addActionCheck(menuIsobarsStep, tr("6"), "", tr("Espacement des isobares"));
                acView_IsobarsStep8 = addActionCheck(menuIsobarsStep, tr("8"), "", tr("Espacement des isobares"));
                acView_IsobarsStep10 = addActionCheck(menuIsobarsStep, tr("10"), "", tr("Espacement des isobares"));
                acView_GroupIsobarsStep->addAction(acView_IsobarsStep1);
                acView_GroupIsobarsStep->addAction(acView_IsobarsStep2);
                acView_GroupIsobarsStep->addAction(acView_IsobarsStep3);
                acView_GroupIsobarsStep->addAction(acView_IsobarsStep4);
                acView_GroupIsobarsStep->addAction(acView_IsobarsStep5);
                acView_GroupIsobarsStep->addAction(acView_IsobarsStep6);
                acView_GroupIsobarsStep->addAction(acView_IsobarsStep8);
                acView_GroupIsobarsStep->addAction(acView_IsobarsStep10);
            menuIsobars->addMenu(menuIsobarsStep);
        acView_IsobarsLabels = addActionCheck(menuIsobars, tr("Etiquettes des isobares"), "",
                            tr("Afficher les étiquettes des isobares"));
        acView_IsobarsLabels->setChecked(Settings::getSetting("showIsobarsLabels", false).toBool());
        acView_PressureMinMax = addActionCheck(menuIsobars, tr("Pression Mini(L) Maxi(H)"), "",
                            tr("Afficher les points de pression mini et maxi"));
        acView_PressureMinMax->setChecked(Settings::getSetting("showPressureMinMax", false).toBool());
                menuGrib->addMenu(menuIsobars);
        setIsobarsStep(Settings::getSetting("isobarsStep", 2).toInt());
        //--------------------------------
                menuIsotherms0 = new QMenu(tr("Isothermes 0degC"));
        acView_Isotherms0 = addActionCheck(menuIsotherms0, tr("Isothermes 0degC"), "",
                            tr("Afficher les isothermes 0degC"));
        acView_Isotherms0->setChecked(Settings::getSetting("showIsotherms0", false).toBool());
            menuIsotherms0Step = new QMenu(tr("Espacement (m)"));
            acView_GroupIsotherms0Step    = new QActionGroup(menuIsotherms0Step);
                acView_Isotherms0Step10   = addActionCheck(menuIsotherms0Step, tr("10"), "", tr("Espacement des isothermes 0degC"));
                acView_Isotherms0Step20   = addActionCheck(menuIsotherms0Step, tr("20"), "", tr("Espacement des isothermes 0degC"));
                acView_Isotherms0Step50   = addActionCheck(menuIsotherms0Step, tr("50"), "", tr("Espacement des isothermes 0degC"));
                acView_Isotherms0Step100  = addActionCheck(menuIsotherms0Step, tr("100"), "", tr("Espacement des isothermes 0degC"));
                acView_Isotherms0Step200  = addActionCheck(menuIsotherms0Step, tr("200"), "", tr("Espacement des isothermes 0degC"));
                acView_Isotherms0Step500  = addActionCheck(menuIsotherms0Step, tr("500"), "", tr("Espacement des isothermes 0degC"));
                acView_Isotherms0Step1000 = addActionCheck(menuIsotherms0Step, tr("1000"), "", tr("Espacement des isothermes 0degC"));
                acView_GroupIsotherms0Step->addAction(acView_Isotherms0Step10);
                acView_GroupIsotherms0Step->addAction(acView_Isotherms0Step20);
                acView_GroupIsotherms0Step->addAction(acView_Isotherms0Step50);
                acView_GroupIsotherms0Step->addAction(acView_Isotherms0Step100);
                acView_GroupIsotherms0Step->addAction(acView_Isotherms0Step200);
                acView_GroupIsotherms0Step->addAction(acView_Isotherms0Step500);
                acView_GroupIsotherms0Step->addAction(acView_Isotherms0Step1000);
            menuIsotherms0->addMenu(menuIsotherms0Step);
            setIsotherms0Step(Settings::getSetting("isotherms0Step", 50).toInt());
        acView_Isotherms0Labels = addActionCheck(menuIsotherms0,
                                                tr("Etiquettes des isothermes 0degC"), "",
                            tr("Afficher les étiquettes des isothermes 0degC"));
        acView_Isotherms0Labels->setChecked(Settings::getSetting("showIsotherms0Labels", false).toBool());
                menuGrib->addMenu(menuIsotherms0);
        menuGrib->addSeparator();
        mn_fax=new QMenu(tr("Fax meteo"));
        acFax_Open = addAction(mn_fax, tr("Ouvrir un fax meteo"), "", tr(""));
        mn_fax->addAction(acFax_Open);
        acFax_Close = addAction(mn_fax, tr("Fermer le fax meteo"), "", tr(""));
        mn_fax->addAction(acFax_Close);
        menuGrib->addMenu(mn_fax);
        menuGrib->addSeparator();
        acCombineGrib = addAction(menuGrib,tr("Combine grib files"),"","","");

    addMenu(menuGrib);

    //-------------------------------------
    menuBoat = new QMenu(tr("Bateau"));
        acVLMParamPlayer = addAction(menuBoat,tr("Gestion des comptes"),"","","");
 #ifdef __REAL_BOAT_ONLY
        acVLMParamPlayer->setEnabled(false);
#endif
        acVLMParamBoat = addAction(menuBoat,tr("Parametres du/des bateaux"),"","","");
        acRace = addAction(menuBoat,tr("Parametres des courses"),"","","");
        acVLMSync = addAction(menuBoat,tr("VLM Sync"),"F5","","");
        acPilototo = addAction(menuBoat,tr("Pilototo"),"","","");
        acShowLog=addAction(menuBoat,tr("Historique VLM"),"Ctrl+Shift+E","",tr(""));
        acGetTrack=addAction(menuBoat,tr("Telecharger trace"),"Ctrl+Shift+T","",tr(""));
        acShowPolar=addAction(menuBoat,tr("Etudier la polaire"),"","");
    addMenu(menuBoat);

    //-------------------------------------
    //Porte
    menuRoute = new QMenu(tr("Routes"));

        acRoute_add = addAction(menuRoute,
                    tr("Creer une route"),"", "", "");
        mnRoute_delete = new QMenu(tr("Supprimer une route"));
        mnRoute_edit = new QMenu(tr("Editer une route"));
        mnRoute_export = new QMenu(tr("Exporter une route"));
        menuRoute->addMenu(mnRoute_edit);
        menuRoute->addMenu(mnRoute_export);
        mnRoute_import=new QMenu(tr("Importer une route"));
        acRoute_import = addAction(mnRoute_import,
                    tr("En mode VB-VMG"),"", "", "");
        acRoute_import2 = addAction(mnRoute_import,
                    tr("En mode Ortho"),"", "", "");
        acRoute_paste = addAction(menuRoute,
                    tr("Coller une route"),"Ctrl+V", "", "");
        menuRoute->addMenu(mnRoute_import);
        acRoute_comparator= addAction(menuRoute,
                                    tr("Routes comparator"),"", "", "");
        menuRoute->addSeparator();
        menuRoute->addMenu(mnRoute_delete);
        acRouteRemove = addAction(menuRoute,tr("Supprimer des routes"),"","","");
    addMenu(menuRoute);

    menuRoutage = new QMenu(tr("Routages"));
        acRoutage_add = addAction(menuRoutage,
                    tr("Creer un routage"),"", "", "");
        mnRoutage_delete = new QMenu(tr("Supprimer un routage"));
        mnRoutage_edit = new QMenu(tr("Editer un routage"));
        mnRoutage_edit->setEnabled(false);
        mnRoutage_delete->setEnabled(false);
        menuRoutage->addMenu(mnRoutage_edit);
        menuRoutage->addSeparator();
        menuRoutage->addMenu(mnRoutage_delete);
    addMenu(menuRoutage);

    menuPOI = new QMenu(tr("Marques"));
        acPOIinput = addAction(menuPOI,tr("Ajout en masse"),"","","");
        acPOISave = addAction(menuPOI,tr("Sauvegarder POIs et routes"),"Ctrl+S","","");
        acPOIRestore = addAction(menuPOI,tr("Recharger POIs et routes"),"Ctrl+R","","");
        menuPOI->addSeparator();
        QMenu *menuImportPoi = new QMenu(tr("Importer"));
        acPOIimport = addAction(menuImportPoi,tr("Importer de zyGrib"),"","","");
        acPOIgeoData = addAction(menuImportPoi,tr("Importer un fichier GeoData"),"","","");
        menuPOI->addMenu(menuImportPoi);
        acPOIAdd = addAction(menuPOI,tr("Ajouter une marque"),"","","");
        acPOIRemove = addAction(menuPOI,tr("Supprimer des marques"),"","","");
        acPOIRemoveByType = addAction(menuPOI,tr("Remove marks by type..."),"","","");
        menuPOI->addSeparator();

        subMenuBarrier= new QMenu(tr("Barrier set"));
        connect(subMenuBarrier,SIGNAL(aboutToShow()),this,SLOT(slot_showBarrierMenu()));
        menuPOI->addMenu(subMenuBarrier);
        ac_addBarrierSet= addAction(subMenuBarrier,tr("Add barrier set"), tr(""), tr(""), "");
        subMenuBarrier->addSeparator();
        ac_addBarrier= addAction(subMenuBarrier,tr("Add barrier"), tr(""), tr(""), "");        
        subSubMenuEditBarrierSet= new QMenu(tr("Parameters"));
        subMenuBarrier->addMenu(subSubMenuEditBarrierSet);
        subSubMenuDelBarrierSet= new QMenu(tr("Delete"));
        subMenuBarrier->addMenu(subSubMenuDelBarrierSet);

    addMenu(menuPOI);



    //-------------------------------------
    menuOptions = new QMenu(tr("Options"));
        acOptions_Proxy = addAction(menuOptions, tr("Proxy Internet"),tr(""),tr(""),"");
        acOptions_Units = addAction(menuOptions, tr("Unites"),tr("Ctrl+U"),tr(""),"");
        acOptions_GraphicsParams = addAction(menuOptions,
                            tr("Parametres graphiques"),tr("Ctrl+G"),tr(""),"");
        acVLMParam = addAction(menuOptions,tr("Parametres"),"","","");


        QMenu *menuMap = new QMenu(tr("Planisphere"));
        acMap_Orthodromie = addActionCheck(menuMap, tr("Distance orthodromique"), tr(""), tr(""));
        acMap_Orthodromie->setChecked(Settings::getSetting("showOrthodromie", false).toBool());

        menuMap->addSeparator();
        acMap_CountriesBorders = addActionCheck(menuMap, tr("Frontieres"), tr(""), tr("Afficher les frontieres"));
        acMap_CountriesBorders->setChecked(Settings::getSetting("showCountriesBorders", true).toBool());
        acMap_Rivers = addActionCheck(menuMap, tr("Rivieres"), tr(""), tr("Afficher les rivieres"));
        acMap_Rivers->setChecked(Settings::getSetting("showRivers", false).toBool());
        acMap_CountriesNames = addActionCheck(menuMap, tr("Noms des pays"), tr(""), tr("Afficher les noms des pays"));
        acMap_CountriesNames->setChecked(Settings::getSetting("showCountriesNames", false).toBool());


        QMenu *menuCitiesNames = new QMenu(tr("Nom des villes"));
        acMap_GroupCitiesNames = new QActionGroup(menuMap);
            acMap_CitiesNames0 = addActionCheck(menuCitiesNames, tr("Aucun"), tr(""), tr(""));
            acMap_CitiesNames1 = addActionCheck(menuCitiesNames, tr("Niveau 1"), tr(""), tr(""));
            acMap_CitiesNames2 = addActionCheck(menuCitiesNames, tr("Niveau 2"), tr(""), tr(""));
            acMap_CitiesNames3 = addActionCheck(menuCitiesNames, tr("Niveau 3"), tr(""), tr(""));
            acMap_CitiesNames4 = addActionCheck(menuCitiesNames, tr("Niveau 4"), tr(""), tr(""));
            acMap_GroupCitiesNames->addAction(acMap_CitiesNames0);
            acMap_GroupCitiesNames->addAction(acMap_CitiesNames1);
            acMap_GroupCitiesNames->addAction(acMap_CitiesNames2);
            acMap_GroupCitiesNames->addAction(acMap_CitiesNames3);
            acMap_GroupCitiesNames->addAction(acMap_CitiesNames4);
            menuMap->addMenu(menuCitiesNames);
        menuOptions->addMenu(menuMap);
        setCitiesNamesLevel(Settings::getSetting("showCitiesNamesLevel", 0).toInt());

        QMenu *menuLanguage = new QMenu(tr("Language"));
            acOptions_GroupLanguage = new QActionGroup(menuLanguage);
                acOptions_Lang_fr = addActionCheck(menuLanguage, tr("Francais"), tr(""), tr(""));
                acOptions_Lang_en = addActionCheck(menuLanguage, tr("English"), tr(""), tr(""));
                acOptions_Lang_es = addActionCheck(menuLanguage, tr("Spanish"), tr(""), tr(""));
                acOptions_Lang_cz = addActionCheck(menuLanguage, tr("Czech"), tr(""), tr(""));
                acOptions_GroupLanguage->addAction(acOptions_Lang_fr);
                acOptions_GroupLanguage->addAction(acOptions_Lang_en);
                acOptions_GroupLanguage->addAction(acOptions_Lang_es);
                acOptions_GroupLanguage->addAction(acOptions_Lang_cz);
        menuOptions->addMenu(menuLanguage);
        QString lang = Settings::getSetting("appLanguage", "none").toString();
        if (lang == "fr")
            acOptions_Lang_fr->setChecked(true);
        else if (lang == "en")
            acOptions_Lang_en->setChecked(true);



#ifdef __QTVLM_WITH_TEST
        menuOptions->addSeparator();
        acVLMTest = addAction(menuOptions,"Test","","","");
        acGribInterpolation = addAction(menuOptions,"Test interpolation","","","");
#endif

        addMenu(menuOptions);

    //-------------------------------------
    menuHelp = new QMenu(tr("Aide"));
        acHelp_Help = addAction(menuHelp, tr("Aide"),"F1",tr(""),appFolder.value("img")+"help.png");
        acHelp_APropos = addAction(menuHelp, tr("A propos de qtVlm"),tr(""),tr(""),"");
        acHelp_AProposQT = addAction(menuHelp, tr("A propos de QT"),tr(""),tr(""),"");
        acHelp_Forum = addAction(menuHelp, tr("QtVlm forum"),tr(""),tr(""),"");
    addMenu(menuHelp);
    foreach (QAction * act, this->actions())
        setRules(act);
    acFile_Quit->setMenuRole(QAction::QuitRole);

}

void MenuBar::setRules(QAction * act)
{
    Util::setFontDialog(act);
    QMenu *menu=act->menu();
    if(menu)
    {
        foreach(QAction * subAct,menu->actions())
            setRules(subAct); //recursion
    }
    else
    {
        //qWarning()<<act->text();
        act->setMenuRole(QAction::NoRole);
    }
}

//---------------------------------------------------------
// Menu popup : bouton droit de la souris
//---------------------------------------------------------
QMenu * MenuBar::createPopupBtRight(QWidget *parent)
{
    QMenu *popup = new QMenu(parent);

    ac_CreatePOI = addAction(popup, tr("Positionner une nouvelle Marque"),tr(""),tr(""),"");
    ac_pastePOI = addAction(popup, tr("Coller une marque"),tr(""),tr(""),"");
    popup->addSeparator();
    ac_popupBarrier = new QAction(tr("Barrier not in edit mode"),popup);
    popup->addAction(ac_popupBarrier);

    popup->addSeparator();
    ac_twaLine=addAction(popup,tr("Tracer une estime TWA"),tr(""),tr(""),"");
    ac_compassLine = addAction(popup, tr("Tirer un cap"),tr(""),tr(""),"");
    popup->addSeparator();
    ac_compassCenterBoat = addAction(popup, tr("Centrer le compas sur le bateau actif"),tr(""),tr(""),"");
    ac_compassCenterBoat->setCheckable(true);
    ac_compassCenterBoat->setChecked(Settings::getSetting("compassCenterBoat", "0").toString()=="1"?Qt::Checked:Qt::Unchecked);
    ac_compassCenterWp = addAction(popup, tr("Centrer le compas sur le WP VLM"),tr(""),tr(""),"");
    mnCompassCenterRoute=new QMenu(tr("Centrer le compass sur l'interpolation de la route"));
    popup->addMenu(mnCompassCenterRoute);
    popup->addSeparator();
    ac_centerMap = addAction(popup, tr("Centrer la carte ici"),tr(""),tr(""),"");
    ac_positScale = addAction(popup, tr("Positionner l'echelle ici"),tr(""),tr(""),"");

    ac_moveBoatSep = popup->addSeparator();
    ac_moveBoat = addAction(popup, tr("Deplacer le bateau ici"),tr(""),tr(""),"");

    popup->addSeparator();
    ac_editRoute=addAction(popup,tr("Editer la route"),"","","");
    ac_poiRoute=addActionCheck(popup,tr("Montrer les POIs intermediaires de la route"),"","","");
    mn_simplifyRoute=new QMenu(tr("Simplifier la route"),popup);
    ac_simplifyRouteMax=addAction(mn_simplifyRoute,tr("Maximum"),"","","");
    ac_simplifyRouteMin=addAction(mn_simplifyRoute,tr("Minimum"),"","","");
    popup->addMenu(mn_simplifyRoute);
    ac_optimizeRoute=addAction(popup,tr("Optimiser la route"),"","","");
    ac_copyRoute=addAction(popup,tr("Copier la route au format kml"),"","","");
    ac_zoomRoute=addAction(popup,tr("Zoom sur la route "),"","","");
    ac_deleteRoute=addAction(popup,tr("Supprimer la route"),"","","");
    ac_pasteRoute=addAction(popup,tr("Coller une route"),"","","");
    return popup;
}

//===================================================================================
QAction* MenuBar::addAction(QWidget *menu,
                    QString title, QString shortcut, QString statustip,
                    QString iconFileName)
{
    QAction *action;
    action = new QAction(title, menu);
    action->setShortcut  (shortcut);
    action->setShortcutContext (Qt::ApplicationShortcut);
    action->setStatusTip (statustip);
    if (iconFileName != "")
        action->setIcon(QIcon(iconFileName));
    if (menu != NULL)
        menu->addAction(action);
    return action;
}
//-------------------------------------------------
QAction* MenuBar::addActionCheck(QWidget *menu,
                    QString title, QString shortcut, QString statustip,
                    QString iconFileName)
{
    QAction *action;
    action = addAction(menu, title, shortcut, statustip, iconFileName);
    action->setCheckable  (true);
    return action;
}

void MenuBar::addMenuRoute(ROUTE* route)
{
    QAction *action1;
    QAction *action2;
    QAction *action3;
    QAction *action4;
    QPixmap iconI(20,10);
    iconI.fill(route->getColor());
    QIcon icon(iconI);
    action1=addAction(mnRoute_edit,route->getName(),"","","");
    connect(action1, SIGNAL(triggered()), route, SLOT(slot_edit()));
    action2=addAction(mnRoute_delete,route->getName(),"","","");
    action2->setData(QVariant(QMetaType::VoidStar, &route));
    connect(action2, SIGNAL(triggered()), my_CentralWidget, SLOT(slot_deleteRoute()));
    action3=addAction(mnRoute_export,route->getName(),"","","");
    connect(action3, SIGNAL(triggered()), route, SLOT(slot_export()));
    action4=addAction(mnCompassCenterRoute,route->getName(),"","","");
    connect(action4, SIGNAL(triggered()), route, SLOT(slot_compassFollow()));
    action1->setIcon(icon);
    action2->setIcon(icon);
    action3->setIcon(icon);
    action4->setIcon(icon);
}
QAction * MenuBar::addReleaseCompass()
{
    QAction *action;
    action=addAction(mnCompassCenterRoute,tr("Aucune"),"","","");
    return action;
}
void MenuBar::addMenuRoutage(ROUTAGE* routage)
{
    QAction *action1;
    QAction *action2;
    action1=addAction(mnRoutage_edit,routage->getName(),"","","");
    connect(action1, SIGNAL(triggered()), routage, SLOT(slot_edit()));
    action2=addAction(mnRoutage_delete,routage->getName(),"","","");
    action2->setData(QVariant(QMetaType::VoidStar, &routage));
    connect(action2, SIGNAL(triggered()), my_CentralWidget, SLOT(slot_deleteRoutage()));
}
//-------------------------------------------------
void MenuBar::setCitiesNamesLevel(int level) {
    switch (level) {
        case 0: acMap_CitiesNames0->setChecked(true); break;
        case 1: acMap_CitiesNames1->setChecked(true); break;
        case 2: acMap_CitiesNames2->setChecked(true); break;
        case 3: acMap_CitiesNames3->setChecked(true); break;
        case 4: acMap_CitiesNames4->setChecked(true); break;
    }
}

//-------------------------------------------------
void MenuBar::setIsobarsStep(int step) {
    switch (step) {
        case 1: acView_IsobarsStep1->setChecked(true); break;
        case 2: acView_IsobarsStep2->setChecked(true); break;
        case 3: acView_IsobarsStep3->setChecked(true); break;
        case 4: acView_IsobarsStep4->setChecked(true); break;
        case 5: acView_IsobarsStep5->setChecked(true); break;
        case 6: acView_IsobarsStep6->setChecked(true); break;
        case 8: acView_IsobarsStep8->setChecked(true); break;
        case 10: acView_IsobarsStep10->setChecked(true); break;
    }
}
//-------------------------------------------------
void MenuBar::setIsotherms0Step(int step) {
    switch (step) {
        case 10: acView_Isotherms0Step10->setChecked(true); break;
        case 20: acView_Isotherms0Step20->setChecked(true); break;
        case 50: acView_Isotherms0Step50->setChecked(true); break;
        case 100: acView_Isotherms0Step100->setChecked(true); break;
        case 200: acView_Isotherms0Step200->setChecked(true); break;
        case 500: acView_Isotherms0Step500->setChecked(true); break;
        case 1000: acView_Isotherms0Step1000->setChecked(true); break;
    }
}

void MenuBar::slot_updateLockIcon(QIcon ic) {
    acFile_Lock->setIcon(ic);
}

void MenuBar::slot_setChangeStatus(bool ,bool pilototo,bool syncBtn) {
    acPilototo->setEnabled(pilototo);
    acVLMSync->setEnabled(syncBtn);
}

void MenuBar::slot_showViewMenu(void) {
    //boardMenu->clear();
    //mainWindow->get_board()->build_showHideMenu(boardMenu);

    toolBarMenu->clear();
    mainWindow->get_toolBar()->build_showHideMenu(toolBarMenu);

    acKeep->setChecked(Settings::getSetting("keepBoatPosOnScreen",1).toInt()==1);    
    acOptions_SH_Nig->setChecked(Settings::getSetting("showNight",1).toInt()==1);

    acOptions_SH_Fla->setChecked(Settings::getSetting("showFlag",0,"showHideItem").toInt()==1);
    acOptions_SH_Pol->setChecked(Settings::getSetting("showPolar",0,"showHideItem").toInt()==1);
    acOptions_SH_Com->setChecked(Settings::getSetting("showCompass",0,"showHideItem").toInt()==1);

    acOptions_SH_Opp->setChecked(Settings::getSetting("hideOpponent",0,"showHideItem").toInt()==0);
    acOptions_SH_Por->setChecked(Settings::getSetting("hidePorte",0,"showHideItem").toInt()==0);
    acOptions_SH_Poi->setChecked(Settings::getSetting("hidePoi",0,"showHideItem").toInt()==0);
    acOptions_SH_Rou->setChecked(Settings::getSetting("hideRoute",0,"showHideItem").toInt()==0);
    acOptions_SH_Lab->setChecked(Settings::getSetting("hideLabel",0,"showHideItem").toInt()==0);
    acOptions_SH_barSet->setChecked(Settings::getSetting("hideBarrierSet",0,"showHideItem").toInt()==0);
}

void MenuBar::slot_showBarrierMenu(void) {
    subSubMenuDelBarrierSet->clear();
    subSubMenuEditBarrierSet->clear();
    if(::barrierSetList.isEmpty()) {
        subSubMenuDelBarrierSet->setEnabled(false);
        subSubMenuEditBarrierSet->setEnabled(false);
        ac_addBarrier->setEnabled(false);
    }
    else {
        qSort(::barrierSetList.begin(),::barrierSetList.end(),BarrierSet::myLessThan);

        subSubMenuDelBarrierSet->setEnabled(true);
        subSubMenuEditBarrierSet->setEnabled(true);
        ac_addBarrier->setEnabled(true);

        QListIterator<BarrierSet*> i (::barrierSetList);
        while(i.hasNext()) {
            BarrierSet * barrierSet=i.next();
            QAction * action;
            action = addAction(subSubMenuEditBarrierSet,barrierSet->get_name(),"","","");
            connect(action,SIGNAL(triggered()),barrierSet,SLOT(slot_editBarrierSet()));
            action = addAction(subSubMenuDelBarrierSet,barrierSet->get_name(),"","","");
            connect(action,SIGNAL(triggered()),barrierSet,SLOT(slot_delBarrierSet()));
        }
    }

}

void MenuBar::slot_showAltitudeMenu(void) {
    int colorMapMode=Settings::getSetting("colorMapMode", MapDataDrawer::drawWind).toInt();

    if(acAlt_GroupAltitude)
        acAlt_GroupAltitude->clear();
    if(menuAltitude)
        menuAltitude->clear();

    MapDataDrawer * mapDataDrawer = my_CentralWidget->get_mapDataDrawer();
    DataManager * dataManager = my_CentralWidget->get_dataManager();
    if(!dataManager || !mapDataDrawer) return;

    QMap<int,DataCode> *dataMap=mapDataDrawer->get_dataCodeMap();
    if(!dataMap) return;

    int curType = dataMap->value(colorMapMode).dataType;
    QMap<int,QList<int>*> * levelList = dataManager->get_levelList(curType);

    if(levelList) {
        QMapIterator<int,QList<int>*> j(*levelList);
        while (j.hasNext()) {
            j.next();
            QList<int>* lst = j.value();
            if(!lst) continue;

            for(int i=0;i<lst->count();++i) {
                QAction * act = addActionCheck(menuAltitude,
                                               levelTypes[j.key()] + " - " +
                        QString().setNum(lst->at(i)) + " " + levelTypesUnit[j.key()], "", "");
                acAlt_GroupAltitude->addAction(act);
            }
        }
    }

}



//------------------------------------------------------------
void MenuBar::setMenubarColorMapMode(int colorMapMode,bool withoutEvent)
{    
    QMapIterator<int,QAction *> i(gribDataActionMap);
    while(i.hasNext()) {
        i.next();
        QAction * ptr=i.value();
        if(withoutEvent)
            ptr->blockSignals(true);
        ptr->setChecked(colorMapMode == i.key());
        if(withoutEvent)
            ptr->blockSignals(false);
    }
}
void MenuBar::setPlayerType(const int &type)
{
    bool real=type!=BOAT_VLM;
    ac_moveBoat->setVisible(real);
    ac_moveBoatSep->setVisible(real);
    acRace->setVisible(!real);
    acVLMSync->setVisible(!real);
    acPilototo->setVisible(!real);
    acShowPolar->setVisible(!real);
    acFile_Lock->setEnabled(!real);
    acFile_Lock->setVisible(!real);
    separator1->setVisible(!real);
    separator2->setVisible(!real);
    acOptions_SH_Opp->setVisible(!real);
    acOptions_SH_Fla->setVisible(!real);
    acOptions_SH_Por->setVisible(!real);
    acShowLog->setVisible(!real);
    acGetTrack->setVisible(!real);
}

void ZeroOneActionGroup::clear(void) {
    while(!lsactions.isEmpty()) {
        QAction * act = lsactions.takeFirst();
        disconnect(act,SIGNAL(triggered(bool)),
                   this,  SLOT(slot_actionTrigerred(bool)));
    }
}

//===================================================================================
void ZeroOneActionGroup::addAction(QAction *act)
{
        lsactions.append(act);
        connect(act, SIGNAL(triggered(bool)),
                this,  SLOT(slot_actionTrigerred(bool)));
}
//------------------------------------------------------------------
void ZeroOneActionGroup::slot_actionTrigerred(bool b)
{
        QAction *act = (QAction *) sender();
        setCheckedAction(act, b);
}
//------------------------------------------------------------------
void ZeroOneActionGroup::setCheckedAction(QAction *act, bool b)
{
        for (int i=0; i<lsactions.size(); i++) {
                if (lsactions.at(i)== act) {
                        lsactions.at(i)->setChecked(b);
                }
                else {
                        lsactions.at(i)->setChecked(false);
                }
        }
        if (b)
                emit triggered( act );
        else
                emit triggered( NULL );
}


