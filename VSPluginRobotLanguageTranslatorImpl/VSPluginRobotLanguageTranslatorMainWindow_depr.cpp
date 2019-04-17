// specific
#ifdef QT_WIDGETS_LIB
#include <QStackedWidget>

#include <Lib/VSLibGUI/VSLibGUIAction.h>
//#include <Lib/VSLibGUI/VSDQtQPushButton.h>
//#include "../VSPluginRotateNodeWithPoseOptions.h"
//#include "../VSPluginRotateNodeWithPosePropertyWidget.h"
#endif

#include <Main/VEROSIM/VEROSIMApplication.h>
#include <Main/VEROSIM/VEROSIMProject.h>

#include "../VSPluginRobotLanguageTranslatorInterface.h"
#include "../VSPluginRobotLanguageTranslatorProject.h"

// header
#include "../VSPluginRobotLanguageTranslatorMainWindow.h"


VSPluginRobotLanguageTranslator::MainWindow::MainWindow(VSP::Interface* interface, VEROSIM::MainWindow* mainwindow)
	: VSP::MainWindow(interface, mainwindow)
	, toggleVisuRotAxes(0)
	, createTransNode(0)
	, createRotNode(0)
	, createTransSphereNode(0)
	//, mainLayout(new QVBoxLayout(this))
{


	// connect to workcell lifecycle
	connect(mainwindow, &VEROSIM::MainWindow::signalInitMainWindow,
		this, &MainWindow::slotInitMainWindow);
	connect(mainwindow, &VEROSIM::MainWindow::signalProjectAttached,
		this, &MainWindow::slotProjectAttached);
	connect(mainwindow, &VEROSIM::MainWindow::signalProjectDetached,
		this, &MainWindow::slotProjectDetached);
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

	// new toggle button
	//
	//importButton->setCheckable(true);
	//importButton->setEnabled(false);
	//QWidget* mainwidget = new QWidget(getMainWindow());


	//QWidget *widget = new QWidget();
	//QVBoxLayout *lay = new QVBoxLayout(widget);
	//importButton = new QPushButton();
	//lay->addWidget(importButton);
	//widget->setLayout(lay);
	


	//getMainWindow()->getGUIMainWindow()->
	//setCentralWidget(widget);

	//import try 
	importButton = new VSLibGUI::Action(getMainWindow(),
		"VSPluginRotateNodeWithPose.ImportButton",
		QPixmap(":/VSPluginRotateNodeWithPose/ToggleVisuRotAxes.png"),false);

	importButton->setCheckable(true);
	importButton->setEnabled(false);


	//toggleVisuRotAxes = new VSLibGUI::Action(getMainWindow(),
	//	"VSPluginRotateNodeWithPose.ToggleVisuRotAxes",
	//	QPixmap(":/VSPluginRotateNodeWithPose/ToggleVisuRotAxes.png"));
	//toggleVisuRotAxes->setCheckable(true);
	//toggleVisuRotAxes->setEnabled(false);

	//createTransNode = new VSLibGUI::Action(getMainWindow(),
	//	"VSPluginRotateNodeWithPose.CreateTransNode",
	//	QPixmap(":/VSPluginRotateNodeWithPose/CreateTransNode.png"));
	//createTransNode->setCheckable(false);
	//createTransNode->setEnabled(false);

	//createRotNode = new VSLibGUI::Action(getMainWindow(),
	//	"VSPluginRotateNodeWithPose.CreateRotNode",
	//	QPixmap(":/VSPluginRotateNodeWithPose/CreateRotNode.png"));
	//createRotNode->setCheckable(false);
	//createRotNode->setEnabled(false);

	//createTransSphereNode = new VSLibGUI::Action(getMainWindow(),
	//	"VSPluginRotateNodeWithPose.CreateTransSphereNode",
	//	QPixmap(":/VSPluginRotateNodeWithPose/CreateTransSphereNode.png"));
	//createTransSphereNode->setCheckable(false);
	//createTransSphereNode->setEnabled(false);

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
	if (name == "VSPluginRotateNodeWithPose.MovingNodePropertyWidget")
	{
		VSPluginRobotLanguageTranslator::Project* extProjectObject = static_cast<VSPluginRobotLanguageTranslator::Project*>(findExtProject("VSPluginRotateNodeWithPose::Project"));
		Q_ASSERT(extProjectObject);
		//widget = new QWidget((*extProjectObject));
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
	Project* extProjectObject = static_cast<Project*>(findExtProject("VSPluginRotateNodeWithPose::Project"));
	Q_ASSERT(extProjectObject);

	// connect toggle button to workcell slots
	// import helper
	connect(importButton, SIGNAL(triggered(bool)),
		extProjectObject, SLOT(slotCreateImport()));

	//connect(toggleVisuRotAxes, SIGNAL(toggled(bool)),
	//	extProjectObject, SLOT(slotToggleVisuRotAxes(bool)));

	//connect(extProjectObject, SIGNAL(signalVisuRotAxesModified(bool)),
	//	toggleVisuRotAxes, SLOT(setChecked(bool)));

	//connect(createRotNode, SIGNAL(triggered(bool)),
	//	extProjectObject, SLOT(slotCreateRotNode()));

	//connect(createTransNode, SIGNAL(triggered(bool)),
	//	extProjectObject, SLOT(slotCreateTransNode()));

	//connect(createTransSphereNode, SIGNAL(triggered(bool)),
	//	extProjectObject, SLOT(slotCreateTransSphereNode()));
	importButton->setEnabled(true);
	//toggleVisuRotAxes->setEnabled(true);
	//toggleVisuRotAxes->setChecked(extProjectObject->getVisuRotAxis());
	//createRotNode->setEnabled(true);
	//createTransNode->setEnabled(true);
	//createTransSphereNode->setEnabled(true);
#endif
}

void VSPluginRobotLanguageTranslator::MainWindow::slotProjectDetached()
{
#ifdef QT_WIDGETS_LIB
	importButton->setEnabled(false);
	toggleVisuRotAxes->setEnabled(false);
	createRotNode->setEnabled(false);
	createTransNode->setEnabled(false);
	createTransSphereNode->setEnabled(false);
#endif
}

void VSPluginRobotLanguageTranslator::MainWindow::slotAddWidgetToProjectOptions(QStackedWidget* widgetStack)
{
#ifdef QT_WIDGETS_LIB
	//new options widget
	//VSPluginRobotLanguageTranslator::Options* options =
	//	new VSPluginRobotLanguageTranslator::Options(widgetStack
	//		, getProject()
	//		, getProject()->GetSettings()->Group(settingsGroupName)
	//		, getInterface()->getApplication()->GetUserSettings()->Group(settingsGroupName));
	// add options widget to stack                                          
	/*widgetStack->addWidget(options);*/

	// enable options widget to read settings
	//connect(options, SIGNAL(signalUpdateSettings()),
	//	findExtProject("VSPluginRotateNodeWithPose::Project"), SLOT(slotReadSettings()));
#else
	Q_UNUSED(widgetStack);
#endif
}
