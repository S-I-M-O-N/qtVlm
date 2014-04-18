message($$[QT_VERSION])

contains ( QT_VERSION, "^5.*"){
    warning("qt5 detected")
    DEFINES += QT_V5
    CONFIG += QT_V5
    QT+=core gui widgets multimedia concurrent uitools positioning
#    QT+= qml quick
}
android: QT+=androidextras
CONFIG += qt
TEMPLATE = app
TARGET = qtVlm
DEPENDPATH += .
INCLUDEPATH += \
    Dialogs \
    Dialogs/ctrl \
    Dialogs/view \
    Dialogs/view/pc \
    libs/bzip2 \
    libs/zlib-1.2.7 \
        libs/qextserialport12/src \
    libs/nmealib/src/nmea \
    libs/libbsb \
    libs/miniunz \
    libs/g2clib-1.4.0 \
    libs/jasper/include \
    .

LIBS += -Llibs/build \
    -lminiunz \
    -lbz2 \
    -lz \
    -lnmea \
    -lbsb \
    -lg2clib \
    -ljpc \
    -ljp2 \
    -lbase

QT_V5{
LIBS += -lQt5ExtSerialPort
} else {
LIBS += -lqextserialport \
    -lqjson
INCLUDEPATH += libs/qjson
CONFIG += uitools
}

asan {
    QMAKE_CC=clang
    QMAKE_CXX=clang++
    QMAKE_LINK=clang++
    QMAKE_CFLAGS="-O1 -fsanitize=address -fno-omit-frame-pointer -g"
    QMAKE_CXXFLAGS="-O1 -fsanitize=address -fno-omit-frame-pointer -g"
    QMAKE_LFLAGS="-fsanitize=address -g -rdynamic"
}

MOC_DIR = objs
OBJECTS_DIR = objs
UI_DIR = objs
SOURCES_DIR = src
DESTDIR = ../
QT += network \
      xmlpatterns \
      xml
CODECFORTR = UTF-8
CODECFORSRC = UTF-8
TRANSLATIONS = ../tr/qtVlm_en.ts \
    ../tr/qtVlm_fr.ts \
    ../tr/qtVlm_cz.ts \
    ../tr/qtVlm_es.ts
RC_FILE = qtVlm.rc
HEADERS += Dialogs/DialogGraphicsParams.h \
    Dialogs/DialogProxy.h \
    Dialogs/DialogHorn.h \
    Dialogs/DialogRace.h \
    Dialogs/DialogRoute.h \
    Dialogs/DialogGribValidation.h \
    Dialogs/DialogBoatAccount.h \
    Dialogs/DialogPoiInput.h \
    Dialogs/DialogPoi.h \
    Dialogs/DialogPoiDelete.h \
    Dialogs/DialogFinePosit.h \
    Dialogs/DialogParamVlm.h \
    Dialogs/DialogPilototo.h \
    Dialogs/DialogPilototoParam.h \
    Dialogs/DialogPlayerAccount.h \
    Dialogs/DialogSailDocs.h \
    Dialogs/DialogTwaLine.h \
    Dialogs/DialogWp.h \
    Dialogs/DialogInetProgess.h \
    GshhsReader.h \
    GisReader.h \
    Grib.h \
    GribRecord.h \
    inetConnexion.h \
    LoadGribFile.h \
    MainWindow.h \
    MenuBar.h \
    Orthodromie.h \
    opponentBoat.h \
    POI.h \
    Polar.h \
    Projection.h \
    libs/sha1/sha1.h \
    Terrain.h \
    Util.h \
    Version.h \
    xmlBoatData.h \
    zuFile.h \
    mapcompass.h \
    mycentralwidget.h \
    orthoSegment.h \
    selectionWidget.h \
    vlmLine.h \
    inetClient.h \
    route.h \
    routage.h \
    settings.h \
    class_list.h \
    Triangle.h \
    Segment.h \
    Polygon.h \
    Point.h \
    triangulation.h \
    vlmPoint.h \
    IsoLine.h \
    dataDef.h \
    boatReal.h \
    boat.h \
    boatVLM.h \
    faxMeteo.h \
    loadImg.h \
    Player.h \
    interpolation.h \
    Dialogs/DialogRealBoatConfig.h \
    vlmpointgraphic.h \
    Dialogs/DialogRealBoatPosition.h \
    Dialogs/dialogviewpolar.h \
    GshhsPolyReader.h \
    Dialogs/dialogpoiconnect.h \
    Dialogs/DialogVlmLog.h \
    Dialogs/DialogDownloadTracks.h \
    Dialogs/dialogFaxMeteo.h \
    Dialogs/dialogLoadImg.h \
    Dialogs/routeInfo.h \
    GshhsDwnload.h \
    Dialogs/DialogRemovePoi.h \
    MyView.h \
    ToolBar.h \
    Progress.h \
    StatusBar.h \
    Magnifier.h \
    Board.h \
    BoardReal.h \
    BoardVLM.h \
    BoardVLM_tools.h \
    BoardTools.h \
    BarrierSet.h \
    Barrier.h \
    Dialogs/DialogEditBarrier.h \
    XmlFile.h \
    Dialogs/DialogRouteComparator.h \
    MapDataDrawer.h \
    DataColors.h \
    Dialogs/DialogRemoveRoute.h \
    GribV2.h \
    GribV2Record.h \
    GribV1Record.h \
    GribV1.h \
    DataManager.h \
    Dialogs/DialogGribDrawing.h \
    BoardInterface.h \
    MainWindowInterface.h \
    BoatInterface.h \
    PolarInterface.h \
    settings_ini.h \
    settings_def.h \
    AngleUtil.h \
    Dialogs/DialogLanguageSelection.h \
    Dialogs/ctrl/DialogVlmGrib_ctrl.h \
    Dialogs/view/DialogVlmGrib_view.h \
    Dialogs/view/pc/DialogVlmGrib_view_pc.h \
    Dialogs/ctrl/DialogGribDate_ctrl.h \
    Dialogs/view/DialogGribDate_view.h \
    Dialogs/view/pc/DialogGribDate_view_pc.h \
    Dialogs/view/pc/Dialog_view_pc.h \
    Dialogs/view/pc/DialogChooseBarrierSet_view_pc.h \
    Dialogs/view/DialogChooseBarrierSet_view.h \
    Dialogs/ctrl/DialogChooseBarrierSet_ctrl.h \
    Dialogs/view/DialogChooseMultiple_view.h \
    Dialogs/view/pc/DialogChooseMultiple_view_pc.h \
    Dialogs/ctrl/DialogChooseMultiple_ctrl.h \
    BoardTools.h \
    GpsReceiver.h \
    Dialogs/DialogZygrib.h \
    Dialogs/DialogRoutage.h


FORMS += Ui/boatAccount_dialog.ui \
    Ui/BoardVLM.ui \
    Ui/BoardReal.ui \
    Ui/paramVLM.ui \
    Ui/POI_input.ui \
    Ui/POI_editor.ui \
    Ui/Pilototo_param.ui \
    Ui/instructions.ui \
    Ui/Pilototo.ui \
    Ui/race_dialog.ui \
    Ui/WP_dialog.ui \
    Ui/DialogGribDate_pc.ui \
    Ui/DialogVLMGrib_pc.ui \
    Ui/inetConn_progessDialog.ui \
    Ui/poi_delete.ui \
    Ui/Route_Editor.ui \
    Ui/gribValidation.ui \
    Ui/finePosit.ui \
    Ui/dialoghorn.ui \
    Ui/twaline.ui \
    Ui/paramAccount.ui \
    Ui/playerAccount.ui \
    Ui/sailDocs.ui \
    Ui/realBoatConfig.ui \
    Ui/realBoatPosition.ui \
    Ui/paramProxy.ui \
    Ui/dialogviewpolar.ui \
    Ui/dialogpoiconnect.ui \
    Ui/DialogVlmLog.ui \
    Ui/DialogDownloadTracks.ui \
    Ui/dialogFaxMeteo.ui \
    Ui/dialogLoadImg.ui \
    Ui/routeInfo.ui \
    Ui/DialogRemovePoi.ui \
    Ui/DialogEditBarrier.ui \
    Ui/DialogChooseMultiple_pc.ui \
    Ui/DialogChooseBarrierSet_pc.ui \
    Ui/RouteComparator.ui \
    Ui/DialogGribDrawing.ui \
    Ui/DialogLanguageSelection.ui \
    Ui/DialogZygrib.ui \
    Ui/Routage_Editor.ui

SOURCES += Dialogs/DialogGraphicsParams.cpp \
    Dialogs/DialogProxy.cpp \
    Dialogs/DialogHorn.cpp \
    Dialogs/DialogRace.cpp \
    Dialogs/DialogRoute.cpp \
    Dialogs/DialogGribValidation.cpp \
    Dialogs/DialogBoatAccount.cpp \
    Dialogs/DialogPoiInput.cpp \
    Dialogs/DialogPoi.cpp \
    Dialogs/DialogPoiDelete.cpp \
    Dialogs/DialogFinePosit.cpp \
    Dialogs/DialogParamVlm.cpp \
    Dialogs/DialogPilototo.cpp \
    Dialogs/DialogPilototoParam.cpp \
    Dialogs/DialogPlayerAccount.cpp \
    Dialogs/DialogSailDocs.cpp \
    Dialogs/DialogTwaLine.cpp \
    Dialogs/DialogWp.cpp \
    Dialogs/DialogInetProgess.cpp \
    Board.cpp \
    BoardVLM.cpp \
    BoardReal.cpp \
    GshhsReader.cpp \
    GisReader.cpp \
    Grib.cpp \
    GribRecord.cpp \
    inetConnexion.cpp \
    LoadGribFile.cpp \
    main.cpp \
    MainWindow.cpp \
    MenuBar.cpp \
    Orthodromie.cpp \
    opponentBoat.cpp \
    POI.cpp \
    Polar.cpp \
    Projection.cpp \
    libs/sha1/sha1.cpp \
    Terrain.cpp \
    Util.cpp \
    xmlBoatData.cpp \
    zuFile.cpp \
    mapcompass.cpp \
    mycentralwidget.cpp \
    orthoSegment.cpp \
    selectionWidget.cpp \
    vlmLine.cpp \
    inetClient.cpp \
    route.cpp \
    routage.cpp \
    settings.cpp \
    triangulation.cpp \
    Triangle.cpp \
    Segment.cpp \
    Polygon.cpp \
    Point.cpp \
    vlmPoint.cpp \
    IsoLine.cpp \
    boatReal.cpp \
    boat.cpp \
    boatVLM.cpp \
    faxMeteo.cpp \
    loadImg.cpp \
    Player.cpp \
    interpolation.cpp \
    Dialogs/DialogRealBoatConfig.cpp \
    vlmpointgraphic.cpp \
    Dialogs/DialogRealBoatPosition.cpp \
    Dialogs/dialogviewpolar.cpp \
    GshhsPolyReader.cpp \
    Dialogs/dialogpoiconnect.cpp \
    Dialogs/DialogVlmLog.cpp \
    Dialogs/DialogDownloadTracks.cpp \
    Dialogs/dialogFaxMeteo.cpp \
    Dialogs/dialogLoadImg.cpp \
    Dialogs/routeInfo.cpp \
    GshhsDwnload.cpp \
    Dialogs/DialogRemovePoi.cpp \
    MyView.cpp \
    ToolBar.cpp \
    Progress.cpp \
    StatusBar.cpp \
    Magnifier.cpp \
    BarrierSet.cpp \
    Barrier.cpp \
    Dialogs/DialogEditBarrier.cpp \
    XmlFile.cpp \
    Dialogs/DialogRouteComparator.cpp \
    MapDataDrawer.cpp \
    DataColors.cpp \
    Dialogs/DialogRemoveRoute.cpp \
    GribV2.cpp \
    GribV2Record.cpp \
    GribV1Record.cpp \
    GribV1.cpp \
    DataManager.cpp \
    Dialogs/DialogGribDrawing.cpp \
    AngleUtil.cpp \
    Dialogs/DialogLanguageSelection.cpp \
    Dialogs/ctrl/DialogVlmGrib_ctrl.cpp \
    Dialogs/view/DialogVlmGrib_view.cpp \
    Dialogs/view/pc/DialogVlmGrib_view_pc.cpp \
    Dialogs/ctrl/DialogGribDate_ctrl.cpp \
    Dialogs/view/DialogGribDate_view.cpp \
    Dialogs/view/pc/DialogGribDate_view_pc.cpp \
    Dialogs/view/pc/Dialog_view_pc.cpp \
    Dialogs/view/pc/DialogChooseBarrierSet_view_pc.cpp \
    Dialogs/view/DialogChooseBarrierSet_view.cpp \
    Dialogs/ctrl/DialogChooseBarrierSet_ctrl.cpp \
    Dialogs/view/DialogChooseMultiple_view.cpp \
    Dialogs/view/pc/DialogChooseMultiple_view_pc.cpp \
    Dialogs/ctrl/DialogChooseMultiple_ctrl.cpp \
    GpsReceiver.cpp \
    Dialogs/DialogRoutage.cpp \
    Dialogs/DialogZygrib.cpp




unix:!macx: DEFINES += _TTY_POSIX_ __TERRAIN_QIMAGE __UNIX_QTVLM
unix: LIBS += -lgps
unix: INCLUDEPATH += libs/libgps

win32:DEFINES += _TTY_WIN_ \
    QWT_DLL \
    QT_DLL \
    __TERRAIN_QPIXMAP __WIN_QTVLM \
    _CRT_SECURE_NO_WARNINGS \
    NOMINMAX


macx: DEFINES += _TTY_POSIX_ __TERRAIN_QPIXMAP __MAC_QTVLM

ICON = qtVlm.icns

#DEFINES += __REAL_BOAT_ONLY


RESOURCES +=
