// standard
#include "../../../Main/VEROSIM/VEROSIMApplication.h"
#include "../../../Main/VEROSIM/VEROSIMFeatures.h"
// specific
#include "../VSPluginRobotLanguageTranslatorMainWindow.h"
#include "../VSPluginRobotLanguageTranslatorProject.h"
// header
#include "../VSPluginRobotLanguageTranslatorInterface.h"


const bool AllowMultipleProjects=true;

VSPluginRobotLanguageTranslator::Interface::Interface(VSL::Application* application, VSL::Library* library)
   : VSP::Interface(application, library, AllowMultipleProjects)
{
   // check application context
   if (0 == getVEROSIMApplication())
   {
      ShowError(QString("%1: This is not a VEROSIM::Application!").arg(metaObject()->className()));
      return;
   }

   // Dieses Plugin grundsätzlich zulässig!
   VEROSIMFeatureNoFeatureCheckHere;

   // connect to mainwindow initiators
   // these signals originate from VSDQt::VEROSIM::Application
   connect(application, SIGNAL(signalNewMainWindow(VEROSIM::MainWindow*)),
           this, SLOT(slotCreateNewExtMainWindow(VEROSIM::MainWindow*)));
   // connect to workcell initiators            
   connect(application, SIGNAL(signalNewProject(VEROSIM::Project*,VSD::SimState*)),
           this, SLOT(slotCreateNewExtProject(VEROSIM::Project*)));
}

VSPluginRobotLanguageTranslator::Interface::~Interface()
{
}

void VSPluginRobotLanguageTranslator::Interface::slotCreateNewExtMainWindow(VEROSIM::MainWindow* mainwindow)
{
   (void) new VSPluginRobotLanguageTranslator::MainWindow(this, mainwindow);
}

void VSPluginRobotLanguageTranslator::Interface::slotCreateNewExtProject(VEROSIM::Project* project)
{
   (void) new VSPluginRobotLanguageTranslator::Project(this, project);
}

VEROSIM_DLL_ENTRY(VSPluginRobotLanguageTranslator::Interface)
