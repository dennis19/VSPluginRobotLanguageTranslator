// specific
#ifdef QT_WIDGETS_LIB
   #include <QStackedWidget>
   
   #include <Lib/VSLibGUI/VSLibGUIAction.h>
   #include "../VSPluginRobotLanguageTranslatorOptions.h"
   #include "../VSPluginRobotLanguageTranslatorPropertyWidget.h"
#endif

#include <Main/VEROSIM/VEROSIMApplication.h>
#include <Main/VEROSIM/VEROSIMProject.h>

#include "../VSPluginRobotLanguageTranslatorInterface.h"
#include "../VSPluginRobotLanguageTranslatorProject.h"

// header
#include "../VSPluginRobotLanguageTranslatorMainWindow.h"

#include "../../../Plugin/VSLibPython/api/VSLibPythonAPI.h"


VSPluginRobotLanguageTranslator::MainWindow::MainWindow(VSP::Interface* interface, VEROSIM::MainWindow* mainwindow)
   : VSP::MainWindow(interface, mainwindow)
   , toggleVisuRotAxes(0)
   , createTransNode(0)
   , createRotNode(0)
   , createTransSphereNode(0)
{
   // connect to workcell lifecycle
   connect(mainwindow,  &VEROSIM::MainWindow::signalInitMainWindow,
           this,        &MainWindow::slotInitMainWindow);
   connect(mainwindow,  &VEROSIM::MainWindow::signalProjectAttached,
           this,        &MainWindow::slotProjectAttached);
   connect(mainwindow,  &VEROSIM::MainWindow::signalProjectDetached,
           this,        &MainWindow::slotProjectDetached);


}

VSPluginRobotLanguageTranslator::MainWindow::~MainWindow()
{
   // Werden automatisch gelöscht ...
   //delete toggleVisuRotAxes;
   //delete createTransNode;
   //delete createRotNode;
}




void VSPluginRobotLanguageTranslator::MainWindow::slotInitMainWindow()
{
   #ifdef QT_WIDGETS_LIB
      // connect to VSPluginOptions
      connect(findExtMainWindow("VSPluginOptions::MainWindow"), // TBD: VSPluginOptions::MainWindow
              SIGNAL(signalCreatingProjectOptionsWindow(QStackedWidget*)), 
              this, SLOT(slotAddWidgetToProjectOptions(QStackedWidget*)));

	  //import try 
	  ImportButton = new VSLibGUI::Action(getMainWindow(),
		  "VSPluginRobotLanguageTranslator.ImportButton",
		  QPixmap(":/VSPluginRotateNodeWithPose/ToggleVisuRotAxes.png"));
	  ExportButton = new VSLibGUI::Action(getMainWindow(),
		  "VSPluginRobotLanguageTranslator.ExportButton",
		  QPixmap(":/VSPluginRotateNodeWithPose/ToggleVisuRotAxes.png"));
	  //ImportButton->setCheckable(true);
	  //ImportButton->setEnabled(false);

      // new toggle button
      toggleVisuRotAxes = new VSLibGUI::Action( getMainWindow(), 
                                             "VSPluginRobotLanguageTranslator.ToggleVisuRotAxes", 
                                             QPixmap(":/VSPluginRotateNodeWithPose/ToggleVisuRotAxes.png"));
      toggleVisuRotAxes->setCheckable(true);
      toggleVisuRotAxes->setEnabled(false);

      createTransNode = new VSLibGUI::Action( getMainWindow(),
                                             "VSPluginRobotLanguageTranslator.CreateTransNode",
                                             QPixmap(":/VSPluginRotateNodeWithPose/CreateTransNode.png"));
      createTransNode->setCheckable(false);
      createTransNode->setEnabled(false);

      createRotNode = new VSLibGUI::Action( getMainWindow(),
                                             "VSPluginRobotLanguageTranslator.CreateRotNode",
                                             QPixmap(":/VSPluginRotateNodeWithPose/CreateRotNode.png"));
      createRotNode->setCheckable(false);
      createRotNode->setEnabled(false);
   
      createTransSphereNode = new VSLibGUI::Action( getMainWindow(),
                                                    "VSPluginRobotLanguageTranslator.CreateTransSphereNode",
                                                    QPixmap(":/VSPluginRotateNodeWithPose/CreateTransSphereNode.png"));
      createTransSphereNode->setCheckable(false);
      createTransSphereNode->setEnabled(false);

      //propertyWidget:
      VSP::MainWindow* propertiesMainWindow = findExtMainWindow("VSPluginProperties::MainWindow");
      if (propertiesMainWindow)
      {
          connect(propertiesMainWindow, SIGNAL(signalCreatePropertyWidget(const QString&, QWidget*&)),
                  this, SLOT(slotCreatePropertyWidget(const QString&, QWidget*&)));
      }
   #endif
}

void VSPluginRobotLanguageTranslator::MainWindow::slotCreatePropertyWidget(const QString& name, QWidget*& widget)
{
   #ifdef QT_WIDGETS_LIB
      if (name == "VSPluginRobotLanguageTranslator.MovingNodePropertyWidget")
      {
		  VSPluginRobotLanguageTranslator::Project* extProjectObject = static_cast<VSPluginRobotLanguageTranslator::Project*>(findExtProject("VSPluginRotateNodeWithPose::Project"));
         Q_ASSERT(extProjectObject);
         widget = new MovingNodePropertyWidget((*extProjectObject));
      }
   #else
      Q_UNUSED(name);
      Q_UNUSED(widget);
   #endif
}

void VSPluginRobotLanguageTranslator::MainWindow::slotProjectAttached()
{
   //// new tool widget
   //ToolWidget* toolWidget = new ToolWidget(getMainWindow());
   //// add tool widget to general mainwindow
   //getMainWindow()->getGUIMainWindow()->addToolWidget(toolWidget);

   #ifdef QT_WIDGETS_LIB
      toggleVisuRotAxes->setEnabled(true);

      // get project object
      // QObjects are used to do connections without dealing with header 
      Project* extProjectObject = static_cast<Project*>(findExtProject("VSPluginRobotLanguageTranslator::Project"));
      Q_ASSERT(extProjectObject);
	  // import helper
	  connect(ImportButton, SIGNAL(triggered(bool)),
		  extProjectObject, SLOT(slotImportButton(bool)));
	  connect(ExportButton, SIGNAL(triggered(bool)),
		  extProjectObject, SLOT(slotExportButton(bool)));
   
      // connect toggle button to workcell slots
      connect(toggleVisuRotAxes, SIGNAL(toggled(bool)),
              extProjectObject, SLOT(slotToggleVisuRotAxes(bool)));

      connect(extProjectObject, SIGNAL(signalVisuRotAxesModified(bool)),
              toggleVisuRotAxes, SLOT(setChecked(bool)));

      connect(createRotNode, SIGNAL(triggered(bool)),
              extProjectObject, SLOT(slotCreateRotNode()));

      connect(createTransNode, SIGNAL(triggered(bool)),
              extProjectObject, SLOT(slotCreateTransNode()));

      connect(createTransSphereNode, SIGNAL(triggered(bool)),
              extProjectObject, SLOT(slotCreateTransSphereNode()));
   
      toggleVisuRotAxes->setEnabled(true);
      toggleVisuRotAxes->setChecked(extProjectObject->getVisuRotAxis());
	  ImportButton->setEnabled(true);
	  ExportButton->setEnabled(true);

	  createRotNode->setEnabled(true);
      createTransNode->setEnabled(true);  
      createTransSphereNode->setEnabled(true);
   #endif
}

void VSPluginRobotLanguageTranslator::MainWindow::slotProjectDetached()
{
   #ifdef QT_WIDGETS_LIB
      toggleVisuRotAxes->setEnabled(false);
      createRotNode->setEnabled(false);
	  ImportButton->setEnabled(false);
	  ExportButton->setEnabled(false);

	  createTransNode->setEnabled(false);
      createTransSphereNode->setEnabled(false);
   #endif
}

void VSPluginRobotLanguageTranslator::MainWindow::slotAddWidgetToProjectOptions(QStackedWidget* widgetStack)
{
   #ifdef QT_WIDGETS_LIB
      // new options widget
	VSPluginRobotLanguageTranslator::Options* options =
         new VSPluginRobotLanguageTranslator::Options(widgetStack
                                                 , getProject()
                                                 , getProject()->GetSettings()->Group(settingsGroupName)
                                                 , getInterface()->getApplication()->GetUserSettings()->Group(settingsGroupName) );
      // add options widget to stack                                          
      widgetStack->addWidget(options);

      // enable options widget to read settings
      connect(options, SIGNAL(signalUpdateSettings()),
              findExtProject("VSPluginRobotLanguageTranslator::Project"), SLOT(slotReadSettings()));
   #else
      Q_UNUSED(widgetStack);
   #endif
}
