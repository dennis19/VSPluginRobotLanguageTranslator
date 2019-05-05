TEMPLATE    = lib
CONFIG      += dll
TARGET      = VSPluginRobotLanguageTranslator

include(../../../Main/Global/Defines.pro)
QT += opengl
!unix{
include($$VEROSIMSourceFolder/3rdParty/GLEW/glew.pri)
}
unix{
CONFIG += glew
}

# Input

#DEFINES     += INTEGRATE_IN_EXPLORER

CONFIG  += exceptions

SOURCES =  \
   VSPluginRobotLanguageTranslatorExtension.cpp \
   VSPluginRobotLanguageTranslatorExtensionRotate.cpp \
   VSPluginRobotLanguageTranslatorExtensionTranslate.cpp\
   VSPluginRobotLanguageTranslatorInterface.cpp \
   VSPluginRobotLanguageTranslatorMainWindow.cpp \
   VSPluginRobotLanguageTranslatorOptions.cpp \
   VSPluginRobotLanguageTranslatorProject.cpp \
   VSPluginRobotLanguageTranslatorVisu.cpp \
   VSPluginRobotLanguageTranslatorPropertyWidget.cpp \
   VSPluginRobotLanguageTranslatorExplorerPage.cpp \
   VSPluginRobotLanguageTranslatorMovement.cpp \
   #VSPluginRobotLanguageTranslatorSelectionComponentFolder.cpp 
 
HEADERS =  \
   ../VSPluginRobotLanguageTranslatorExtension.h \
   ../VSPluginRobotLanguageTranslatorExtensionRotate.h \
   ../VSPluginRobotLanguageTranslatorExtensionTranslate.h \
   ../VSPluginRobotLanguageTranslatorInterface.h \
   ../VSPluginRobotLanguageTranslatorMainWindow.h \
   ../VSPluginRobotLanguageTranslatorOptions.h \
   ../VSPluginRobotLanguageTranslatorProject.h \
   ../VSPluginRobotLanguageTranslatorVisu.h \
   ../VSPluginRobotLanguageTranslatorPropertyWidget.h \
   ../VSPluginRobotLanguageTranslatorExplorerPage.h \
   ../VSPluginRobotLanguageTranslatorMovement.h \
   #../VSPluginRobotLanguageTranslatorSelectionComponentFolder.h

FORMS = \
      VSPluginRobotLanguageTranslatorOptionsBase.ui \
     
TRANSLATIONS = \
      VSPluginRobotLanguageTranslator_de.ts
      
RESOURCES = \
      VSPluginRobotLanguageTranslator.qrc


#VS_DATA_START

VS_DEPENDENCIES = \
   VSLibMemory \
   VEROSIM \
   VSD \
   VSD3D \
   VSDTools \
   VSG \
   VSL \
   VSLibGUI \
   VSLibRenderGL \
   VSLibSelection \
   VSLibCommandsVSD \
   VSS \
   VSM \
   VSP \
   VSPluginExplorer \
   VSPluginExplorerVSD \
   VSPluginProperties \
   VSLibUnitConversion \
   VSDIO \
   VSLibPoseList \
   VSLibPython \
   VSPluginPython \
   VSLibActionBlocks \
   VSLibIOTools \
   VSLibKinematics
   
VS_DESCRIPTION = "VS Plugin to demonstrate the basic construction of a VS Plugin and it's access of the different frameworks."
VS_MAINTAINER = bjoern.grothe@rt.rif-ev.de
VS_ACTIVE = Win32VC9,Win64VC9,Win64VC11,Win64VC14,LinuxUbuntu14,LinuxQtCreator,LinuxUbuntu16,LinuxCentos7,FreeBSDQtCreator
VS_STATUS = Undefined
#VS_DATA_END

