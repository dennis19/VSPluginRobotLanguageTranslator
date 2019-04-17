#ifndef VSPluginRotateNodeWithPoseMainWindowH
#define VSPluginRotateNodeWithPoseMainWindowH

// base
#include "../../Lib/VSP/VSPMainWindow.h"
// specific
class QStackedWidget;
class QWidget;

namespace VSLibGUI 
{ 
   class Action; 
};


namespace VSPluginRobotLanguageTranslator
{
   class MainWindow : public VSP::MainWindow
   {
	   Q_OBJECT
   
   // construction
   public:
      MainWindow(VSP::Interface* interface, VEROSIM::MainWindow* mainwindow);   	
      virtual ~MainWindow();

	  

   // management
   private slots:
      void slotInitMainWindow();
      void slotCreatePropertyWidget(const QString& name, QWidget*& widget);
      void slotProjectAttached();
      void slotProjectDetached();
      void slotAddWidgetToProjectOptions(QStackedWidget* widgetStack);
   
   // data
   private:
	  VSLibGUI::Action* ImportButton;
	  VSLibGUI::Action* ExportButton;
      VSLibGUI::Action* toggleVisuRotAxes;
      VSLibGUI::Action* createTransNode;
      VSLibGUI::Action* createRotNode;
      VSLibGUI::Action* createTransSphereNode;
      
   }; // class MainWindow
}; // namespace VSPluginRotateNodeWithPose

#endif // VSPluginRotateNodeWithPoseMainWindowH 
