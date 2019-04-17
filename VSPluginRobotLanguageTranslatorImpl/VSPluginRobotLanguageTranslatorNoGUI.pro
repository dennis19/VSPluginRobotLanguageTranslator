TEMPLATE    = lib
CONFIG      += dll
TARGET      = VSPluginRotateNodeWithPoseNoGUI

include(../../../Main/Global/Defines.pro)
DoNotUseWidgets()
#???Eigentlich müsste hier DoNotUseGUI() stehen, dann kompilieren aber die VSD3D-Anteile wegen QImage und QColor nicht mehr.

# Input

#DEFINES     += INTEGRATE_IN_EXPLORER

CONFIG  += exceptions

SOURCES =  \
   VSPluginRotateNodeWithPoseExtension.cpp \
   VSPluginRotateNodeWithPoseExtensionRotate.cpp \
   VSPluginRotateNodeWithPoseExtensionTranslate.cpp\
   VSPluginRotateNodeWithPoseInterface.cpp \
   VSPluginRotateNodeWithPoseMainWindow.cpp \
   VSPluginRotateNodeWithPoseProject.cpp
 
HEADERS =  \
   ../VSPluginRotateNodeWithPoseExtension.h \
   ../VSPluginRotateNodeWithPoseExtensionRotate.h \
   ../VSPluginRotateNodeWithPoseExtensionTranslate.h \
   ../VSPluginRotateNodeWithPoseInterface.h \
   ../VSPluginRotateNodeWithPoseMainWindow.h \
   ../VSPluginRotateNodeWithPoseProject.h

TRANSLATIONS = \
      VSPluginRotateNodeWithPose_de.ts
      
#VS_DATA_START

VS_DEPENDENCIES = \
   VSLibMemory \
   VEROSIMNoGUI \
   VSD \
   VSD3D \
   VSDTools \
   VSG \
   VSL \
   VSS \
   VSM \
   VSP \
   VSDIO \
   VSLibCommandsVSD

VS_DESCRIPTION = "VS Plugin to demonstrate the basic construction of a VS Plugin and it's access of the different frameworks."
VS_MAINTAINER = bjoern.grothe@rt.rif-ev.de
VS_ACTIVE = Win32VC9,Win64VC9,Win64VC11,Win64VC14,LinuxUbuntu14,LinuxQtCreator,LinuxUbuntu16,LinuxCentos7,FreeBSDQtCreator,AndroidArm7QtCreator
VS_STATUS = Undefined
#VS_DATA_END

