// specific
#include "../../../Main/VEROSIM/VEROSIMProject.h"

#include "../../../Lib/VSLibGUI/VSLibGUISettingsWrapper.h"
#include "../../../Lib/VSD/VSDElementIndex.h"
#include "../../../Lib/VSD/VSDDatabase.h"
#include "../../../Lib/VSDTools/VSDTools.h"

#include "../VSPluginRobotLanguageTranslatorMainWindow.h"
//#include "../VSPluginRotateNodeWithPoseExtensionRotate.h"
//#include "../VSPluginRotateNodeWithPoseExtensionTranslate.h"
// header
#include "../VSPluginRobotLanguageTranslatorOptions.h"


VSPluginRobotLanguageTranslator::Options::Options(QWidget* parent, VEROSIM::Project* project, const VSL::SettingsGroup& settings, const VSL::SettingsGroup& defaultSettings)
: QWidget(parent)
, project(project)
, settings(settings)
, defaultSettings(defaultSettings)
{
   // setup gui from form file
   setupUi(this);
   setObjectName("VSPluginRotateNodeWithPose.ImportOptions");
}

VSPluginRobotLanguageTranslator::Options::~Options()
{
}

void VSPluginRobotLanguageTranslator::Options::slotDeferredInitialize()
{
   // init check box from settings
   new VSLibGUI::SettingsWrapper::CheckBox(this, "visuRotAxesCheck", settings, defaultSettings, true);
   new VSLibGUI::SettingsWrapper::SpinBoxDouble(this, "minVelSpinBox", settings, defaultSettings);
   new VSLibGUI::SettingsWrapper::SpinBoxDouble(this, "maxVelSpinBox", settings, defaultSettings);

   // init extension list from database
   extensionList->setEnabled(false);

   VSDForAllElements(VSPluginRotateNodeWithPose::ExtensionRotate*, extension, project->getEnvironment()->getGuiSimState()->getDatabase())
   {
      extensionList->addItem(VSDTools::compileNameOfNode(extension->getMyModelInstance()->instanceCast<VSD::Node*>()));
   }
   VSDForAllElements(VSPluginRotateNodeWithPose::ExtensionTranslate*, extensionTranslate, project->getEnvironment()->getGuiSimState()->getDatabase())
   {
      extensionList->addItem(VSDTools::compileNameOfNode(extensionTranslate->getMyModelInstance()->instanceCast<VSD::Node*>()));
   }
}

void VSPluginRobotLanguageTranslator::Options::slotModified()
{
   // feedback to OK/APPLY-management
   emit signalContentsModified(this, true);
}

void VSPluginRobotLanguageTranslator::Options::slotApply()
{
   // feedback to OK/APPLY-management
   VSLibGUI::SettingsWrapper::applyChildren(this);
   emit signalContentsModified(this, false);
   
   // write settings
   emit signalUpdateSettings();
}

void VSPluginRobotLanguageTranslator::Options::slotRestoreDefault()
{
   VSLibGUI::SettingsWrapper::restoreDefault(this);
}

void VSPluginRobotLanguageTranslator::Options::slotSetAsDefault()
{
   VSLibGUI::SettingsWrapper::setAsDefault(this);
}

void VSPluginRobotLanguageTranslator::Options::slotRestart()
{
	emit signalReloadModel(this, true);
}
