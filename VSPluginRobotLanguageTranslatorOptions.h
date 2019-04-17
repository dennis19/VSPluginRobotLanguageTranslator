#ifndef VSPluginRotateNodeWithPoseOptionsH
#define VSPluginRotateNodeWithPoseOptionsH

// standard
#include <qlineedit.h>
#include <qradiobutton.h>
// base
#include "ui_VSPluginRobotLanguageTranslatorOptionsBase.h"
// specific
#include "../../Lib/VSL/VSLSettings.h"
#include "../../Lib/VSL/VSLMacros.h"

namespace VEROSIM
{
   class Project;
};


namespace VSPluginRobotLanguageTranslator
{
   class MainWindow;

   class Options : public QWidget, private VSPluginRobotLanguageTranslator::Ui::OptionsBase
   {
       Q_OBJECT

   // construction       
   public:
      Options(QWidget* parent, VEROSIM::Project* project, const VSL::SettingsGroup& settings, const VSL::SettingsGroup& defaultSettings);
      virtual ~Options();

   // services      
   signals:
      void signalContentsModified(QWidget* widget, bool modified);
      void signalUpdateSettings();
      void signalReloadModel(QWidget* widget, bool modified);

   public slots:
      void slotDeferredInitialize();
      void slotApply();
      void slotModified();
      void slotRestoreDefault();
      void slotSetAsDefault();
      void slotRestart();

   // data
   private:
      VEROSIM::Project* project;
      const VSL::SettingsGroup settings;
      const VSL::SettingsGroup defaultSettings;
      
   }; // class Options
}; // namespace VSPluginRotateNodeWithPose

#endif // VSPluginRotateNodeWithPoseOptionsH
