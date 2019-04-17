// specific
#include "../../../Main/VEROSIM/VEROSIMProject.h"
#include "../../../Lib/VSD/VSDDatabase.h"
#include "../../../Lib/VSD/VSDElementIndex.h"
#ifdef QT_WIDGETS_LIB
   #include "../../VSLibRenderGL/VSLibRenderGLRenderSimStateExtension.h"
   #include "../VSPluginRobotLanguageTranslatorVisu.h"
#endif
#include "../VSPluginRobotLanguageTranslatorExtensionRotate.h"
#include "../VSPluginRobotLanguageTranslatorExtensionTranslate.h"

#undef max


// specific: for node constructing
#include "../../../Lib/VSD3D/VSD3DNode.h"
#include "../../../Lib/VSD3D/VSD3DHullNode.h"
#include "../../../Lib/VSD3D/VSD3DGeometryPolyhedron.h"
#include "../../../Lib/VSD3D/VSD3DGeometryBox.h"
#include "../../../Lib/VSD3D/VSD3DGeometrySphere.h"
#include "../../../Lib/VSD3D/VSD3DSpatialView.h"

#include "../../../Lib/VSM/VSMPseudoRandom.h"
#include "../../../Lib/VSL/VSLSettings.h"
#include "../../VSLibCommandsVSD/VSLibCommandsVSDCommandInterface.h"
#include "../VSPluginRobotLanguageTranslatorExtensionRotate.h"
#include "../VSPluginRobotLanguageTranslatorExtensionTranslate.h"
#include "../VSPluginRobotLanguageTranslatorMainWindow.h"
#ifdef QT_WIDGETS_LIB
   #include "../VSPluginRobotLanguageTranslatorExplorerPage.h"
#endif


#include "../../../Lib/VSDIO/VSDIOExtensionIOBoard.h"
#include "../../../Lib/VSDIO/VSDIOInputBool.h"
//file dialog
#include "../../../Lib/VSLibGUI/VSLibGUIFileDialog.h"
#include "../../../Lib/VSLibGUI/VSLibGUIAction.h"
//#include "C:\Users\d.peuss\Documents\Visual Components\4.1\My Commands\ImportRepo\Exporter\FANUCLS\IntermediateUpload.py"

// VEROSIM python
//#include "../../../Plugin/VSPluginPython/VSPluginPythonProject.h"
#include "../../../Plugin/VSLibPython/api/VSLibPythonAPI.h"
//#include "../../../Plugin/VSPluginPython/VSPluginPythonPythonMain.h"
//#include "../../../Plugin/VSPluginPython/VSPluginPythonInteraction.h"
//python
//#include "PythonQt.h"
//#include <algorithm>

//#include <C:\Program Files\JetBrains\PyCharm Community Edition 2018.3.1\helpers\pydev\pydevd_attach_to_process\dll\stdafx.h>

//#include <iostream>

//#include <C:\Users\d.peuss\AppData\Local\Programs\Python\Python37\include\Python.h>

//pose
#include "Plugin/VSPluginPoseListCommands/VSPluginPoseListCommandsProject.h"

#include "../../../Plugin/VSLibPoseList/VSLibPoseListPose.h"
#include "../../../Plugin/VSLibPoseList/VSLibPoseListPathPose.h"
#include "../../../Plugin/VSLibPoseList/VSLibPoseListPath.h"
#include "../../../Plugin/VSLibPoseList/VSLibPoseListPoseList.h"
#include "../../../Plugin/VSLibPoseList/VSLibPoseListCartesianPoseExtension.h"
#include "../../../Plugin/VSLibPoseList/VSLibPoseListRobotPoseExtension.h"


#include "../../../Plugin/VSLibActionBlocks/VSLibActionBlocksBaseInstance.h"
#include "../../../Plugin/VSLibActionBlocks/VSLibActionBlocksAgent.h"
// header
#include "../VSPluginRobotLanguageTranslatorProject.h"


VSPluginRobotLanguageTranslator::Project::Project(VSP::Interface* interface, VEROSIM::Project* workcell)
: VSP::Project(interface, workcell)
, visuRotAxes(false)
, minVelBoarder(0.0)
, maxVelBoarder(0.0)
, visu(0)
{
   // connect to workcell initialisation
   connect(workcell, SIGNAL(signalInitProject(VEROSIM::Project*,VSD::SimState*)), 
           this, SLOT(slotInitProject(VEROSIM::Project*,VSD::SimState*)));
   connect(workcell, SIGNAL(signalProjectWillBeClosed(VEROSIM::Project*)), 
           this, SLOT(slotProjectWillBeClosed()));

}

VSPluginRobotLanguageTranslator::Project::~Project()
{}

/**
  * \brief 
  *  Creates an ExtensionRotaion object and appends it to the objectNode.
  */
void VSPluginRobotLanguageTranslator::Project::createAndAppendRotationExtension(VSD::SimState& simstate,
                                                                           VSD::Node& objectNode,
                                                                           const double velocity)
{
   ExtensionRotate *extRot = simstate.newSimStateInstance<ExtensionRotate>();
   if (extRot != 0)
   {
      VSM::Vector3 vec = createRandomisedVector(-1.0, 1.0);
      restrictVectorToPositivCoordinateAxes(vec);
      extRot->setRotAxis(vec);
      extRot->setRotVelocity(velocity);
      extRot->setEnabled(true);
      objectNode.getExtensions().append(extRot);
   }
}

/**
  * \brief
  * Creates an ExtensionTranslation object and appends it to the objectNode.
  */
void VSPluginRobotLanguageTranslator::Project::createAndAppendTranslationExtension(VSD::SimState &simstate,
                                                                              VSD::Node &objectNode, 
                                                                              const double velocity)
{
   ExtensionTranslate* extTrans = simstate.newSimStateInstance<ExtensionTranslate>();
   if (extTrans!= 0)
   {
      VSM::Vector3 vec = createRandomisedVector(0, 10);
      restrictVectorToPositivCoordinateAxes(vec);
      extTrans->setTransAxis(vec);
      extTrans->setTransVelocity(velocity/1000);
      extTrans->setEnabled(true);
      objectNode.getExtensions().append(extTrans);
   }
}


VSM::Vector3 VSPluginRobotLanguageTranslator::Project::createRandomisedVector(const double lowerLimit,
                                                                         const double upperLimit)
{
   VSM::PseudoRandom random;
   random.firstRandomIntegerInMinMax(VSM::PseudoRandom::seedFromTime());
   double valX = random.nextRandomDoubleInRange(lowerLimit, upperLimit);
   double valY = random.nextRandomDoubleInRange(lowerLimit, upperLimit);
   double valZ = random.nextRandomDoubleInRange(lowerLimit, upperLimit);
   return VSM::Vector3(valX, valY, valZ);
}

void VSPluginRobotLanguageTranslator::Project::restrictVectorToPositivCoordinateAxes(VSM::Vector3& vec)
{
   double maxVal = std::max(vec.getX(), std::max(vec.getY(), vec.getZ()) );
   if (maxVal == vec.getX())
   {
      vec = VSM::Vector3(1.0, 0.0, 0.0);
   }
   else if (maxVal == vec.getY())
   {
       vec = VSM::Vector3(0.0, 1.0, 0.0);
   }
   else if (maxVal == vec.getZ())
   {
       vec = VSM::Vector3(0.0, 0.0, 1.0);
   }
}

bool VSPluginRobotLanguageTranslator::Project::getVisuRotAxis() const
{
   return visuRotAxes;
}



void VSPluginRobotLanguageTranslator::Project::triggerRepaint()
{
   emit signalTriggerPaintAllWindows();
}

void VSPluginRobotLanguageTranslator::Project::slotCreateNode(const GeometryType geometryType,
                                                         const MovementType movementType)
{
   VSD::SimState* simstate = this->getProject()->getEnvironment()->getGuiSimState();

   // creation of VSD::Instances is managed by the VSD
   // and append will carry out a proper creation.
   // 
   // never create VSD::Instances yourself, unless they
   // are free-floating and not linked to the system tree.

   // create object NODE
   VSD3D::Node* objectNode = simstate->newSimStateInstance<VSD3D::Node>();
   // ExtensionIOBoard
   VSDIO::ExtensionIOBoard *ioboard = simstate->newSimStateInstance<VSDIO::ExtensionIOBoard>();
   ioboard->createInput<VSDIO::InputBool>(tr("enable"));
   VSDIO::Input *input = ioboard->getInput(0);
   VSDIO::InputBool* inputValue;
   if (input != 0)
   {
      inputValue = input->instanceCast<VSDIO::InputBool*>();
      // initial value:
      inputValue->setLocalValue(true);
   }
   objectNode->getExtensions().append(ioboard);
   
   // create hull NODE
   VSD3D::HullNode* hullNode = simstate->newSimStateInstance<VSD3D::HullNode>();
   // init at fixed position
   VSM::Frame hullframe; 
   // The box has to be moved, because its center doesnt lie in the center of the coordinate system
   if (geometryType == Box)
      hullframe.setPosition(VSM::Vector3(-0.05, -0.05, -0.05));
   hullNode->setRelFrame(hullframe);
   hullNode->setShow(true);
  
   VSM::PseudoRandom pseudoRandom;
   pseudoRandom.firstRandomIntegerInMinMax(VSM::PseudoRandom::seedFromTime());
   // create hull DATA
   VSD3D::Geometry* hullData = 0;
   QString objectNodeName;
   double velocity = pseudoRandom.nextRandomDoubleInRange(minVelBoarder, maxVelBoarder);
   if (movementType == Rotate)
   {
      objectNodeName = tr("Rotating");
      createAndAppendRotationExtension(*simstate, *objectNode, velocity);
   }
   else if (movementType == Translate)
   {
      objectNodeName = tr("Translating");
      createAndAppendTranslationExtension(*simstate,
                                          *objectNode,
                                          velocity);
   }
   objectNodeName.append(" ");
   if (geometryType == Box)
   {
      objectNodeName.append(tr("Box"));
      hullNode->setName(tr("Box"));
      hullData = simstate->newSimStateInstance<VSD3D::GeometryBox>();
      VSD3D::GeometryBox* box = dynamic_cast<VSD3D::GeometryBox*>(hullData);
      if(box)
         box->setSize(VSM::Vector3(0.1, 0.1, 0.1));
   }
   else if(geometryType == Sphere)
   {
      objectNodeName.append(tr("Sphere"));
      hullNode->setName(tr("Sphere"));
      hullData = simstate->newSimStateInstance<VSD3D::GeometrySphere>();
      VSD3D::GeometrySphere* sphere = dynamic_cast<VSD3D::GeometrySphere*>(hullData);
      if (sphere)
      {
         sphere->setRadius(0.1);
         sphere->setApproxParam(30);
      }
   }
   objectNode->setName(objectNodeName);
   
   // init at random position
   VSM::Frame frame;
   VSM::Vector3 posVec = createRandomisedVector(-1.0, 1.0);
   frame.setPosition(VSM::Vector3(posVec.getX(), posVec.getY(), posVec.getZ()));
   
   objectNode->setRelFrame(frame);
   objectNode->setShow(true);
   objectNode->setAttachedToParent(false);
    // init with random 
   VSM::Vector3 colorVec = createRandomisedVector(0.0, 1.0);
   QColor color = QColor::fromRgbF(colorVec.getX(), colorVec.getY(), colorVec.getZ());
   hullData->getOrCreateChildPolyhedron()->setUniformColor(color);
   hullNode->setGeometry(hullData);
   objectNode->getChildNodes().append(hullNode);
   // add the node via commads
   VSLibCommandsVSD::CommandInterface* cmds = VSLibCommandsVSD::CommandInterface::the(*this);
   cmds->appendSimStateInstance(objectNode, 
                                VSLibCommandsVSD::PropertyWrapper(*simstate->getDatabase()->getRootNode()->getDataChildNodes()), 
                                tr("Create rotating node"), 
                                true);
}

void VSPluginRobotLanguageTranslator::Project::slotCreateTransNode()
{
   slotCreateNode(Box, Translate);
}

void VSPluginRobotLanguageTranslator::Project::initializePython()
{
	ShowInfo("Calling Python\n");
	QString file;
	file= QString("F:\Dennis\Masterarbeit\Test.py");
	//VSLibPython::API::PythonWorkspace *workspace = VSLibPython::API::PythonWorkspace::createNewWorkspace();//FromFile(file);
	//
	//QVariant ret= workspace->call("test");

	//workspace->~PythonWorkspace();


}

VSLibPoseList::PoseList* VSPluginRobotLanguageTranslator::Project::CreateNewPoseList(bool select)
{
	VSD::SimState* simstate = getProject()->getEnvironment()->getGuiSimState();
	VSD::Node* nodeRoot = simstate->getDatabase()->getRootNode();

	//CommandCreatePath* command = new CommandCreatePath(nodeRoot, name, commandsVSDProject, tr("Create new path"));
	//command->push();	
	VSLibPoseList::PoseList* psl = simstate->newSimStateInstance<VSLibPoseList::PoseList>();

	//Setup name - count exising poselists
	int existingPoselistCount = 0;
	VSDForAllElements(VSLibPoseList::PoseList*, psl, simstate->getDatabase())
	{
		existingPoselistCount++;
	}

	psl->setName(tr("PoseList #%1").arg(existingPoselistCount + 1));

	VSLibCommandsVSD::CommandInterface::the(*this)->appendSimStateInstance(psl, VSLibCommandsVSD::PropertyWrapper(*nodeRoot->getDataChildNodes()), "create Poselist", select);

	return psl;
}  // slotCreateNewPoseList()

QList<VSLibPoseList::Path*> VSPluginRobotLanguageTranslator::Project::CreateNewPathsFromPoseLists(const QList<const VSLibPoseList::PoseList*>& poseLists) const
{
	QList<VSLibPoseList::Path*> paths;

	if (poseLists.isEmpty())
		return paths;

	QString text = QObject::tr("Create new paths from pose lists");

	if (poseLists.size() == 1)
		text = tr("Create new path from %1").arg(poseLists.first()->getName());

	//VSD::Node* rootNode = getProject()->getEnvironment()->getGuiSimState()->getDatabase()->getRootNode();

	//QList<CommandCreatePath*> commands;
	//CommandCreatePath* currentCommand = 0;
	VSLibCommandsVSD::CommandInterface::the(*this)->beginCommandsMacro(text);

	foreach(const VSLibPoseList::PoseList* poseList, poseLists)
	{
		auto path = CreateNewPath(tr("Path (from %1)").arg(poseList->getName()));

		int poseCount = poseList->getNumberOfPoses();

		for (int j = 0; j < poseCount; j++)
		{
			insertPoseToPath(poseList->getPose(j), path, j);
		}
		paths.append(path);
		//commands.append(currentCommand);				
	}


	VSLibCommandsVSD::CommandInterface::the(*this)->endCommandsMacro();

	return paths;

} // slotCreateNewPathsFromPoseLists()

bool VSPluginRobotLanguageTranslator::Project::insertPoseToPath(VSLibPoseList::Pose* pose, VSLibPoseList::Path* path, int index) const
{
	VSD::SimState* simstate = getProject()->getEnvironment()->getGuiSimState();
	int numberOfPosesInPath = path->getNumberOfElements();

	if ((index < 0) || (index > numberOfPosesInPath))
		index = numberOfPosesInPath;

	//if ((path == 0) /* && (commandCreatePath != 0)*/)
	//	path = commandCreatePath->getNewPath();
	bool createIOs = true;

	VSLibPoseList::PathPose* pathPose = simstate->newSimStateInstance<VSLibPoseList::PathPose>();
	pathPose->setReferencedPose(pose);
	pathPose->init(path, createIOs);

	//return pathPose;
	
	//VSLibPoseList::PathPose* pathPose = path->createNewPathPose(pose, createIOs);
	//pathPose->setName(tr("PathPose #%1 (from %2)").arg(index).arg(pose->getName()));
	pathPose->setName(tr("PathPose: %1").arg(pose->getName()));

	VSLibCommandsVSD::CommandInterface::the(*this)->appendSimStateInstance(pathPose, VSLibCommandsVSD::PropertyWrapper(*path->getDataElements()), index, "insert pathpose", true);

	path->connectElement(pathPose);

	return true;
}

VSLibPoseList::Path* VSPluginRobotLanguageTranslator::Project::CreateNewPath(const QString& name) const
{
	VSD::SimState* simstate = getProject()->getEnvironment()->getGuiSimState();
	VSD::Node* nodeRoot = simstate->getDatabase()->getRootNode();

	//CommandCreatePath* command = new CommandCreatePath(nodeRoot, name, commandsVSDProject, tr("Create new path"));
	//command->push();	
	VSLibPoseList::Path* path = simstate->newSimStateInstance<VSLibPoseList::Path>();
	path->setName(name);
	VSLibCommandsVSD::CommandInterface::the(*this)->appendSimStateInstance(path, VSLibCommandsVSD::PropertyWrapper(*nodeRoot->getDataChildNodes()), "create path", true);

	return path;

}  // slotCreateNewPath()

void VSPluginRobotLanguageTranslator::Project::createPose(VSLibPoseList::PoseList *poseList,QVector<float> statement_data_)
{
	
	VSD::SimState* simstate = this->getProject()->getEnvironment()->getGuiSimState();
	
	VSLibPoseList::Manager* manager = VSLibPoseList::Manager::the(simstate);

	


	VSLibPoseList::Pose* pose = simstate->newSimStateInstance<VSLibPoseList::Pose>();

	// Create robotPoseExtension
	VSLibPoseList::RobotPoseExtension* robotPoseExtension = simstate->
		newSimStateInstance<VSLibPoseList::RobotPoseExtension>();
	robotPoseExtension->setIgnoreConfiguration(true);
	robotPoseExtension->setIgnoreComment(true);

	VSM::Frame frame;

	QString config = "0 0 0";

	double values[6];
	for (int i = 0; i < 3; i++)
		values[i] = 0.1*i;
	for (int i = 3; i < 6; i++)
		values[i] = 0;

	frame.setPosition(VSM::Vector3(values[0], values[1], values[2]));
	frame.setOrientation(VSM::Matrix3x3(values[5], values[4], values[3]));
	// Create position         
	pose = poseList->slotAppendNewCartesianPoseNoUndo(frame);

	//pose->setAssociatedKinematic();

}


void VSPluginRobotLanguageTranslator::Project::slotImportButton(bool triggered)
{
	ShowInfo("hii");
	QString file_name_ = QFileDialog::getOpenFileName(NULL, "Open FIle", "", "*.ls");
	ShowInfo(file_name_);

	QFile file(file_name_);


	QVector<float> statement_data_;
	//ShowInfo(QString("statement type: %1").arg(statement_type_[0]));

	VSLibPoseList::PoseList *poseList = CreateNewPoseList(true);

	if (file.exists())
	{
		ShowInfo(file_name_);
	}

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		ShowInfo(QObject::tr("Unable to access file '%1'.").arg(file_name_));
		return;
	}
	file.open(QIODevice::ReadOnly);
	initializePython();
	QString line;
	QLocale localeC(QLocale::C);
	QTextStream in(&file);

	QFileInfo fileInfo(file);
	QString baseName = fileInfo.baseName();

	statement_data_.append( 0);

	// Retrieve name of position list node
	/*poseList->setName(baseName);*/
	ShowInfo(QString("line %1").arg(baseName));
	while (!in.atEnd())
	{
		QString statement_type_;
		//statement_type_.append("LinearMotion");

		line = in.readLine();

		if (line.contains("L P"))
		{
			statement_type_="LinearMovement";
			ShowInfo(QString("line %1").arg(line));
			ShowInfo("Linear Movement");

		}
		else if (line.contains("J P"))
		{
			statement_type_ = "JointMovement";
			ShowInfo(QString("line %1").arg(line));
			ShowInfo("Joint Movement");

		}
		if (statement_type_ == "LinearMovement")
			createPose(poseList,statement_data_);
		else if (statement_type_ == "JointMovement")
			createPose(poseList,statement_data_);

		//if (statement_type_[0] == "LinearMotion")
		//	createPose(statement_data_);
		//else if (statement_type_[0] == "JointMotion")
		//	createPose(statement_data_);
		//	ShowInfo("statement type %s \n", statement_type_[ 0 ]);


			//QUrl file_=VSLibGUI::FileDialog::GetOpenFileName(action,"Open FIle","",".ls")
			//slotCreateNode(Box, Translate);
	}

	VSLibPoseList::Path* path = CreateNewPathsFromPoseLists(QList<const VSLibPoseList::PoseList*>() << poseList).first();



	//typedef VSLibActionBlocks::BaseInstance ActionBlock;

	//ActionBlock* action_block_ = simstate->newSimStateInstance<ActionBlock>();

	//VSLibActionBlocks::Behavior* behaviour_ = simstate->newSimStateInstance<VSLibActionBlocks::Behavior>();
	//VSLibActionBlocks::Parameter* param = simstate->newSimStateInstance<VSLibActionBlocks::Parameter>();

	////param->

	//path->getChildNodes().append(param);
	file.close();
}

void VSPluginRobotLanguageTranslator::Project::slotExportButton(bool triggered)
{
	//VSLibGUI::FileDialog::selectFile();
	//QString file_=VSLibGUI::FileDialog::getOpenFileName(NULL,"Open FIle","",".ls")
	ShowInfo("hii");
	QString file_ = QFileDialog::getSaveFileName(NULL, "Open FIle", "", "*.ls");
	ShowInfo(file_);
	//initializePython();


	//QVector<QString> statement_type_;
	//statement_type_.append("LinearMotion");

	//QVector<float> statement_data_;
	//ShowInfo(QString("statement type: %1").arg(statement_type_[0]));

	//if (statement_type_[0] == "LinearMotion")
	//	createPose(statement_data_);
	//else if (statement_type_[0] == "JointMotion")
	//	createPose(statement_data_);
	//ShowInfo("statement type %s \n", statement_type_[ 0 ]);


	//QUrl file_=VSLibGUI::FileDialog::GetOpenFileName(action,"Open FIle","",".ls")
	//slotCreateNode(Box, Translate);
}

void VSPluginRobotLanguageTranslator::Project::slotCreateRotNode()
{
   slotCreateNode(Box, Rotate);
}

void VSPluginRobotLanguageTranslator::Project::slotCreateTransSphereNode()
{
   slotCreateNode(Sphere, Translate);
}

void VSPluginRobotLanguageTranslator::Project::slotInitProject(VEROSIM::Project*, VSD::SimState* simState)
{
   Q_UNUSED(simState);
   // get GL workcell Object
   // QObjects are used to do connections without dealing with header files
   QObject* glProjectObject = findExtProject("VSPluginRenderGL::Project"); // TBD: VSPluginRenderGL::Project
   if (glProjectObject != 0)
   {
      connect(glProjectObject, SIGNAL(signalInitRenderGL(VSD::SimState*)), 
              this, SLOT(slotInitRenderGL(VSD::SimState*)));
      connect(this, SIGNAL(signalTriggerPaintAllWindows()), 
              glProjectObject, SLOT(slotTriggerPaintAllWindows()));
   }
   
   //init settings Values
   VSL::SettingsGroup settings = getProject()->GetSettings()->Group(settingsGroupName);
   settings.InitEntry("visuRotAxesCheck", true);
   settings.InitEntry("minVelSpinBox", 0.01);
   settings.InitEntry("maxVelSpinBox", 50.0);

   // init from settings
   slotReadSettings();

   // explorer page:
   QObject* expProject = findExtProject("VSPluginExplorer::Project");
   connect(expProject, SIGNAL(signalCreateExplorerPage(const QString&, VSPluginExplorer::BasePage*&)),
           this, SLOT(slotCreateExplorerPage(const QString&, VSPluginExplorer::BasePage*&)));
}

void VSPluginRobotLanguageTranslator::Project::slotReadSettings(void)
{
   // init toggle status from settings
   slotToggleVisuRotAxes(getProject()->GetSettings()->Group(settingsGroupName).ReadBool("visuRotAxesCheck"));
   slotSetMinVelocitySpinBox(getProject()->GetSettings()->Group(settingsGroupName).ReadDouble("minVelSpinBox"));
   slotSetMaxVelocitySpinBox(getProject()->GetSettings()->Group(settingsGroupName).ReadDouble("maxVelSpinBox"));
}

void VSPluginRobotLanguageTranslator::Project::slotToggleVisuRotAxes(bool on)
{
   // current toggle status
   visuRotAxes = on;
      
   // write toggle status to settings
   getProject()->GetSettings()->Group(settingsGroupName).WriteEntry("visuRotAxesCheck", visuRotAxes);
   emit signalVisuRotAxesModified(on);
   // set toggle status
   emit signalTriggerPaintAllWindows();
}
void VSPluginRobotLanguageTranslator::Project::slotSetMinVelocitySpinBox(double val)
{
   minVelBoarder = val;
   getProject()->GetSettings()->Group(settingsGroupName).WriteEntry("minVelSpinBox", minVelBoarder);
   emit signalMinVelBoarderModified(val);
}

void VSPluginRobotLanguageTranslator::Project::slotSetMaxVelocitySpinBox(double val)
{
   maxVelBoarder = val;
   getProject()->GetSettings()->Group(settingsGroupName).WriteEntry("maxVelSpinBox", maxVelBoarder);
   emit signalMaxVelBoarderModified(val);
}

void VSPluginRobotLanguageTranslator::Project::slotInitRenderGL(VSD::SimState* renderSimState)
{
   Q_UNUSED(renderSimState);
   #ifdef QT_WIDGETS_LIB
      visu = new Visu(this);
   #endif
}

void VSPluginRobotLanguageTranslator::Project::slotProjectWillBeClosed()
{
    // get GL workcell Object
   QObject* glProjectObject = findExtProject("VSPluginRenderGL::Project");
   if (glProjectObject != 0)
   {
      disconnect(glProjectObject, SIGNAL(signalInitRenderGL(VSD::SimState*)), 
                 this, SLOT(slotInitRenderGL(VSD::SimState*)));
      disconnect(this, SIGNAL(signalTriggerPaintAllWindows()), 
                 glProjectObject, SLOT(slotTriggerPaintAllWindows()));
   }
}

void VSPluginRobotLanguageTranslator::Project::slotCreateExplorerPage(const QString&internalName, VSPluginExplorer::BasePage*&newPage)
{
   #ifdef QT_WIDGETS_LIB
      if (internalName == "VSPluginRotateNodeWithPose::ExplorerPage")
      {
         VSD::Database* database = getProject()->getEnvironment()->getGuiSimState()->getDatabase();
         const VSD::MetaInstance* baseMetaInstance = VSD::Manager::the()->findMetaInstance("VSPluginRotateNodeWithPose::Extension");
		 VSPluginRobotLanguageTranslator::ExplorerPage* page = new VSPluginRobotLanguageTranslator::ExplorerPage(this,
                                                                                                       tr("RotateNodeWithPose"),
                                                                                                       database,
                                                                                                       baseMetaInstance,
                                                                                                       true);
         newPage = page;
      }
   #else
      Q_UNUSED(newPage);
      Q_UNUSED(internalName);
   #endif
}
