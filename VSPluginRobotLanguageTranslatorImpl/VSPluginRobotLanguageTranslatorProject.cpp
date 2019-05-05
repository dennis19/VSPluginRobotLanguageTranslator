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

#include <math.h>

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

#include <QFile>
#include <QVariant>

#include "../../../Lib/VSDIO/VSDIOExtensionIOBoard.h"
#include "../../../Lib/VSDIO/VSDIOOutputVSDRef.h"
#include "../../../Lib/VSDIO/VSDIOInputBool.h"
#include "../../../Lib/VSDIO/VSDIOManager.h"
#include "../../../Plugin/VSLibIOTools/VSLibIOToolsExtensionParentToOutput.h"
//soml
#include "../../../Lib/VSD/VSDModelNode.h"

#include "../../../Lib/VSD/VSDNode.h"

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

#include "../../../Lib/VSLibKinematics/VSLibKinematicsExtensionKinematicMappingRoot.h"


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

QVariantList VSPluginRobotLanguageTranslator::Project::SetUpPythonUploadProgram(QString file_name_)
{
	ShowInfo("Calling Python Uploadscript\n");
	QString file;
	////QString script = "return 'hii'";

	//file = QString("C:/Users/d.peuss/Documents/Visual Components/4.1/My Commands/ImportRepo/Exporter/FANUCLS/Test.py");
	file = QString("F:/Dennis/VEROSIM/Projekte/Import/VerosimUpload.py");
	//file= QString("F:/Dennis/Masterarbeit/Test.py");
	VSLibPython::API::PythonWorkspace *workspace = VSLibPython::API::PythonWorkspace::createNewWorkspaceFromFile(file);

	QList<QVariant> args;
	args.append(file_name_);
	QVariantList* arg_list = new QVariantList(args);
	QVariantList list= workspace->call("upload",*arg_list).toList();


	return list;
}

QVariantList VSPluginRobotLanguageTranslator::Project::SetUpPythonUploadGlobalData(QString file_name_)
{
	ShowInfo("Calling Python Uploadscript\n");
	QString file;
	////QString script = "return 'hii'";

	//file = QString("C:/Users/d.peuss/Documents/Visual Components/4.1/My Commands/ImportRepo/Exporter/FANUCLS/Test.py");
	file = QString("F:/Dennis/VEROSIM/Projekte/Import/VerosimUpload.py");
	//file= QString("F:/Dennis/Masterarbeit/Test.py");
	VSLibPython::API::PythonWorkspace *workspace = VSLibPython::API::PythonWorkspace::createNewWorkspaceFromFile(file);

	QList<QVariant> args;
	args.append(file_name_);
	QVariantList* arg_list = new QVariantList(args);
	QVariantList list = workspace->call("uploadGlobalData", *arg_list).toList();


	return list;
}

QVariantList VSPluginRobotLanguageTranslator::Project::SetUpPythonDownloadProgram(QVariantList statement_data_)
{
	ShowInfo("Calling Python Downloadscript\n");
	QString file;
	////QString script = "return 'hii'";

	//file = QString("C:/Users/d.peuss/Documents/Visual Components/4.1/My Commands/ImportRepo/Exporter/FANUCLS/Test.py");
	file = QString("F:/Dennis/VEROSIM/Projekte/Import/VerosimDownload.py");
	//file= QString("F:/Dennis/Masterarbeit/Test.py");
	VSLibPython::API::PythonWorkspace *workspace = VSLibPython::API::PythonWorkspace::createNewWorkspaceFromFile(file);
	QVariantList list;
	list.append(statement_data_);
	QList<QVariant> args;
	
	args.append(list);
	ShowInfo(QString("filename: %1").arg(args[0].toString()));

	QVariantList* arg_list = new QVariantList(args);
	//ShowInfo(QString("filename: %1").arg(arg_list[0][0].toString()));
	
	QVariantList data_ = workspace->call("download", QVariantList(*arg_list)).toList();
	//ShowInfo(QString("filename: %1").arg(arg_list[0][0].toString()));


	return data_;
}

QString VSPluginRobotLanguageTranslator::Project::SetUpPythonDownloadPositionDefinition(QVariantList statement_data_)
{
	ShowInfo("Calling Python Downloadscript\n");
	QString file;
	////QString script = "return 'hii'";

	//file = QString("C:/Users/d.peuss/Documents/Visual Components/4.1/My Commands/ImportRepo/Exporter/FANUCLS/Test.py");
	file = QString("F:/Dennis/VEROSIM/Projekte/Import/VerosimDownload.py");
	//file= QString("F:/Dennis/Masterarbeit/Test.py");
	VSLibPython::API::PythonWorkspace *workspace = VSLibPython::API::PythonWorkspace::createNewWorkspaceFromFile(file);
	QVariantList list;
	list.append(statement_data_);
	QList<QVariant> args;

	args.append(list);
	//ShowInfo(QString("filename: %1").arg(args[0].toString()));

	QVariantList* arg_list = new QVariantList(args);
	//ShowInfo(QString("filename: %1").arg(arg_list[0][0].toString()));

	QString data_ = workspace->call("downloadPositionDefinition", QVariantList(*arg_list)).toString();
	//ShowInfo(QString("filename: %1").arg(data_));

	return data_;
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

	bool createIOs = true;

	VSLibPoseList::PathPose* pathPose = simstate->newSimStateInstance<VSLibPoseList::PathPose>();
	pathPose->setReferencedPose(pose);
	pathPose->init(path, createIOs);

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

void VSPluginRobotLanguageTranslator::Project::createPose(VSLibPoseList::PoseList *poseList, QVariantList statement_data_)//(VSLibPoseList::PoseList *poseList,QVector<float> statement_data_,QString line,QString file_name_)
{
	double values[6];
	QVector<QString> coord;
	VSD::SimState* simstate = this->getProject()->getEnvironment()->getGuiSimState();
	VSLibPoseList::Manager* manager = VSLibPoseList::Manager::the(simstate);

	VSLibPoseList::Pose* pose = simstate->newSimStateInstance<VSLibPoseList::Pose>();
	QString name= statement_data_[1].toList()[0].toString();
	// Create robotPoseExtension
	VSLibPoseList::RobotPoseExtension* robotPoseExtension = simstate->
		newSimStateInstance<VSLibPoseList::RobotPoseExtension>();
	robotPoseExtension->setIgnoreConfiguration(true);
	robotPoseExtension->setIgnoreComment(true);

	VSM::Frame frame;
	if (!name.contains("PR"))
	{
		
		for (int i = 0; i < 6; i++)
		{
			coord.append(statement_data_[1].toList()[2].toList()[i].toString());
		}
		QString config = "0 0 0";


		if (coord.length() != 0)
		{
			for (int i = 0; i < 3; i++)
				values[i] = coord[i].toDouble()*0.001;
			for (int i = 3; i < 6; i++)
				values[i] = coord[i].toDouble();//0.1*i;
		}
		else
		{
			for (int i = 0; i < 6; i++)
				values[i] = 0;//0.1*i;

		}
		frame.setPosition(VSM::Vector3(values[0], values[1], values[2]));
		frame.setOrientation(VSM::Matrix3x3(values[5], values[4], values[3]));
		
	}
	else
	{
		VSD::SimState* simstate = this->getProject()->getEnvironment()->getGuiSimState();
		VSD::Node* nodeRoot = simstate->getDatabase()->getRootNode();
		VSD::Node* child_node_ = nodeRoot->findChildNodeByName(QString("GlobalPoseList"));
		VSLibPoseList::PoseList* global_pose_list = dynamic_cast<VSLibPoseList::PoseList*> (child_node_);
		name = QString(statement_data_[1].toList()[0].toString()).section(']', 0, 0).append("]");
		for (int i=0; i<global_pose_list->getPosesAsQList().length(); i++)
		{
			if (global_pose_list->getPosesAsQList().at(i)->getName() ==name )
			{
				//ShowInfo(QString("hii"));
				//global_pose_list->getPoses().begin();
				VSLibPoseList::Pose* global_position_ = global_pose_list->getPosesAsQList().at(i);
				frame = global_position_->getRelFrame();
				//ShowInfo(QString("filename: %1").arg(global_position_->getName()));
			}
		}
	}
	// Create position         
	pose = poseList->slotAppendNewCartesianPoseNoUndo(frame);
	pose->setName(name);
	//pose->setAssociatedKinematic();
}

void VSPluginRobotLanguageTranslator::Project::createPoseGlobal(VSLibPoseList::PoseList *poseList, QVariantList statement_data_)//(VSLibPoseList::PoseList *poseList,QVector<float> statement_data_,QString line,QString file_name_)
{
	QVector<QString> coord;
	VSD::SimState* simstate = this->getProject()->getEnvironment()->getGuiSimState();
	VSLibPoseList::Manager* manager = VSLibPoseList::Manager::the(simstate);

	VSLibPoseList::Pose* pose = simstate->newSimStateInstance<VSLibPoseList::Pose>();

	// Create robotPoseExtension
	VSLibPoseList::RobotPoseExtension* robotPoseExtension = simstate->
		newSimStateInstance<VSLibPoseList::RobotPoseExtension>();
	robotPoseExtension->setIgnoreConfiguration(true);
	robotPoseExtension->setIgnoreComment(true);

	VSM::Frame frame;
	
	if (statement_data_[0] == "Cartesian")
	{
		for (int i = 0; i < 6; i++)
		{
			coord.append(statement_data_[2].toList()[i].toString());
		}
		QString config = "0 0 0";
		double values[6];

		if (coord.length() != 0)
		{
			for (int i = 0; i < 3; i++)
				values[i] = coord[i].toDouble()*0.001;
			for (int i = 3; i < 6; i++)
				values[i] = coord[i].toDouble();//0.1*i;
		}
		else
		{
			for (int i = 0; i < 6; i++)
				values[i] = 0;//0.1*i;

		}
		frame.setPosition(VSM::Vector3(values[0], values[1], values[2]));
		frame.setOrientation(VSM::Matrix3x3(values[5], values[4], values[3]));
		// Create position         
		pose = poseList->slotAppendNewCartesianPoseNoUndo(frame);
		pose->setName(statement_data_[1].toString());
		//pose->setAssociatedKinematic();
	}
}

bool VSPluginRobotLanguageTranslator::Project::generateActionBlocksForPathMovement(VSLibPoseList::Path* path)
{
	const VSD::MetaInstance* abMetaInstAutoMove = VSD::Manager::the()->findMetaInstance("ActionBlocks::Parameter::AutoMovementTargetFromPosePath");
	abMetaInstMoveToPose = VSD::Manager::the()->findMetaInstance("ActionBlocks::MoveToPose");
	//VSD::SimStateInstance* abInstMoveToPose = abMetaInstMoveToPose->newSimStateInstance<VSD::SimStateInstance*>(this->getProject()->getEnvironment()->getGuiSimState(), nullptr);
	VSD::SimStateInstance* generatedPathParameter = 0;
	//createActionBlocks(path);
	//abInstMoveToPose->setName(QString("Movement Sequence for Path %1").arg(path->getName()));
	findAgentFromPath(path);

	//get agent from output form agent input
	VSD::Ref<VSLibIOTools::ExtensionParentToOutput> agentOutpExt = 0;// agent->findFirstExtension<VSLibIOTools::ExtensionParentToOutput*>();
	VSD::Ref<VSDIO::ExtensionIOBoard> agentIOBoard = this->getProject()->getEnvironment()->getGuiSimState()->newSimStateInstance<VSDIO::ExtensionIOBoard>();//agentOutpExt->findFirstExtension<VSDIO::ExtensionIOBoard>();
	VSD::Ref<VSDIO::OutputVSDRef> agentOutput = 0;//agentIOBoard->getOutput(0)->instanceCast<VSDIO::OutputVSDRef*>();
	
	if (generatedPathParameter == 0)
	{
		generatedPathParameter = abMetaInstAutoMove->newSimStateInstance<VSD::SimStateInstance*>(this->getProject()->getEnvironment()->getGuiSimState(), nullptr);
		generatedPathParameter->setName(QString("Auto Generated Parameter from Path %1").arg(path->getName()));
	}
	VSLibActionBlocks::Parameter* generatedpathparam =generatedPathParameter->instanceCast<VSLibActionBlocks::Parameter*>();
	//generatedpathparam->myPosePath = path;
	//VSLibActionBlocks::Parameter* param = this->getProject()->getEnvironment()->getGuiSimState()->newSimStateInstance<VSLibActionBlocks::Parameter>();
	VSLibIOTools::ExtensionParentToOutput* targetOutpExt = generatedpathparam->findFirstExtension<VSLibIOTools::ExtensionParentToOutput*>();

	if (targetOutpExt == 0)
	{
		targetOutpExt = this->getProject()->getEnvironment()->getGuiSimState()->newSimStateInstance<VSLibIOTools::ExtensionParentToOutput>();//new VSLibIOTools::ExtensionParentToOutput(this->getProject()->getEnvironment()->getGuiSimState(), nullptr);
		generatedpathparam->getExtensions().append(targetOutpExt);
	}
	//get actual output from ParentToOutputExtension
	VSD::Ref<VSDIO::ExtensionIOBoard> targetIOBoard = targetOutpExt->findFirstExtension<VSDIO::ExtensionIOBoard*>();
	//SOMLAssert(targetIOBoard != 0 && targetIOBoard->getOutput(0) != 0, "ExtensionParentToOutput on Target broken - needs an IOBoard!");
	//VSD::Ref<VSDIO::OutputVSDRef> targetOutput = targetIOBoard->getOutput(0)->instanceCast<VSDIO::OutputVSDRef*>();
	//SOMLAssert(targetOutput != 0, "ExtensionParentToOutput has no OutputVSDRef.");



	const bool createSuccess = createActionBlocks(path);
	if (createSuccess == false)
		return false;

	VSDIO::Manager* ioMgr = VSDIO::Manager::the(this->getProject()->getEnvironment()->getGuiSimState());
	//configure first AB
	//VSD::Ref<ActionBlocks::MoveToPose> firstGeneratedAB = generatedActionBlocks.getFirstInstance();
	//if (firstGeneratedAB != 0)
	//{
	//	VSDIO::Manager* ioMgr = VSDIO::Manager::the(getMySimState());
	//	SOMLAssert(ioMgr != 0);
	//	ioMgr->createConnection(agentOutput, firstGeneratedAB->agentIn);
	//	ioMgr->createConnection(targetOutput, firstGeneratedAB->myTargetIn);

	//	if (autostartActionBlocks)
	//	{
	//		firstGeneratedAB->inputActive->localValue = true;
	//	}
	//}


	//this->name = ;
	return false;
}

bool VSPluginRobotLanguageTranslator::Project::createActionBlocks(VSLibPoseList::Path* path)
{
	VSD::SimStateInstance* previousAB = 0;
	path->getOtherSimStateInstance(this->getProject()->getEnvironment()->getGuiSimState());
	
	//for (int i = 0; i < path->getNumberOfPoses(); i++)
	//{
	//	VSD::SimStateInstance* newAB = abMetaInstMoveToPose->newSimStateInstance<VSD::SimStateInstance*>(this->getProject()->getEnvironment()->getGuiSimState(), path->getOtherSimStateInstance(this->getProject()->getEnvironment()->getGuiSimState()));

	//	VSLibPoseList::PathPose* pathpose=path->getPathPose(i);
	//	newAB->setName(QString("Move to pose %1").arg(pathpose->getName()));
	//	path->getChildNodes().append(newAB->instanceCast<VSD::Node*>());

	//	ShowInfo(pathpose->getName());
	//	//if (previousAB != 0)
	//	//{
	//	//	const bool connectSucc = connectABs(previousAB, newAB);
	//	//	if (connectSucc == false)
	//	//		return false;
	//	//}
	//	previousAB = newAB;
	
	//}

	

	VSDForAllRefListInstances(VSLibPoseList::PathPose*, pathpose, path->getElements())
	{
		VSD::SimStateInstance* newAB = abMetaInstMoveToPose->newSimStateInstance<VSD::SimStateInstance*>(this->getProject()->getEnvironment()->getGuiSimState(), path->getOtherSimStateInstance(this->getProject()->getEnvironment()->getGuiSimState()));
		//VSD::Ref<ActionBlocks::MoveToPose> newAB = new ActionBlocks::MoveToPose();
		//childNodes.append(newAB);
		//generatedActionBlocks.append(newAB);
		//newAB->name = "Move to pose %1".arg(pathpose->name);
		
		QList<const VSD::MetaProperty*> abMetaInstMoveToPoseProperties = abMetaInstMoveToPose->getAllProperties();
		const VSD::MetaProperty* abMetaInstMoveToPoseProperty;// = abMetaInstMoveToPoseProperties.at(20);
		for (int i = 0; i < abMetaInstMoveToPoseProperties.length(); i++)
		{
			//abMetaInstMoveToPoseProperties.at(i);
			if (abMetaInstMoveToPoseProperties.at(i)->getName()=="inputActive")
			{
				//abMetaInstMoveToPoseProperties.at(i)= VSD::Ref<VSDIO::ExtensionIOBoard> agentIOBoard
			}
			ShowInfo(abMetaInstMoveToPoseProperties.at(i)->getName());
		}
//		ShowInfo(abMetaInstMoveToPoseProperty->getName());
		newAB->setName(QString("Move to pose %1").arg(pathpose->getName()));
		path->getChildNodes().append(newAB->instanceCast<VSD::Node*>());
		
		//if (previousAB != 0)
		//{
		//	const bool connectSucc = connectABs(previousAB, newAB);
		//	if (connectSucc == false)
		//		return false;
		//}
		previousAB = newAB;
	}
	
/*
	path->getChildNodes().append(newAB->instanceCast<VSD::Node*>());
	newAB->setName(QString("Move to pose %1").arg(path->getPose(0)->getName()));*/

	//ShowInfo(newAB->getName());

	return 1;
}

//agent has to be set to used robot controller
VSLibActionBlocks::Agent* VSPluginRobotLanguageTranslator::Project::findAgentFromPath(VSLibPoseList::Path* path)
{
	const VSD::MetaInstance* abMetaInstBehavMove = VSD::Manager::the()->findMetaInstance("ActionBlocks::Behavior::Simulated::Movement");
	VSD::SimState* simstate = this->getProject()->getEnvironment()->getGuiSimState();
	VSD::SimStateInstance* behavMove=abMetaInstBehavMove->newSimStateInstance<VSD::SimStateInstance*>(this->getProject()->getEnvironment()->getGuiSimState(), nullptr);
	
	

	if (path == 0 || path->getNumberOfPoses(false) == 0)
		return 0;
	VSLibPoseList::Pose* firstPose = path->getPose(0, false);
	if (firstPose == 0)
		return 0;
	
	VSLibKinematics::ExtensionKinematicMappingRoot* kinematic = firstPose->getAssociatedKinematic();
	if (kinematic == 0)
		return 0;
	

	//Robotics::SimpleRobotController* robotController = 0;
	//VSDForAllElements(Robotics::SimpleRobotController*, robotctrl, database)
	//{
	//	if (robotctrl->kinematic == kinematic)
	//	{
	//		robotController = robotctrl;
	//		break;
	//	}
	//}
	//if (robotController == 0)
	//	return 0;
	//VSLibActionBlocks::Behavior* behaviour_ = simstate->newSimStateInstance<VSLibActionBlocks::Behavior>();

	//VSLibActionBlocks::Behavior* behaviorMovement = 0;//::Simulated::Movement* behaviorMovement = 0;
	//
	////VSDForAllElements(ActionBlocks::Behavior::Simulated::Movement*, behav, database)
	////{
	////	if (behav->mySimpleRobotController == robotController)
	////	{
	////		behaviorMovement = behav;
	////		break;
	////	}
	////}
	//if (behaviorMovement == 0)
	//	return 0;
	//else
	//return behaviorMovement->getParent<VSLibActionBlocks::Agent*>();
	return 0;
}

void VSPluginRobotLanguageTranslator::Project::slotImportButton(bool triggered)
{

	
	
	//if (abMetaInst)
		

	QString file_name_glob_ = QFileDialog::getOpenFileName(NULL, "Open FIle", "", "*.va");
	ShowInfo(file_name_glob_);
	QVariantList global_data = SetUpPythonUploadGlobalData(file_name_glob_);
	QFile file_glob(file_name_glob_);
	
	VSLibPoseList::PoseList *poseListGlobal = CreateNewPoseList(true);
	poseListGlobal->setName("GlobalPoseList");
	ShowInfo(QString("global data: %1").arg(global_data[2].toList()[0].toList()[0].toString()));

	for (int i = 0; i < global_data[2].toList().length(); i++)
	{
		createPoseGlobal(poseListGlobal, global_data[2].toList()[i].toList());
	}

	//QFileInfo fileInfo(file);
	//QString baseName = fileInfo.baseName();


	QString file_name_ = QFileDialog::getOpenFileName(NULL, "Open FIle", "", "*.ls");
	ShowInfo(file_name_);

	QFile file(file_name_);

	
	QVector<float> statement_data_;
	//ShowInfo(QString("filename: %1").arg(file_name_));

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

	//QString line;
	//QLocale localeC(QLocale::C);
	//QTextStream in(&file);
	QVariantList data_= SetUpPythonUploadProgram(file_name_);
	QFileInfo fileInfo(file);
	QString baseName = fileInfo.baseName();
	poseList->setName(baseName);
	statement_data_.append( 0);

	for (int i = 0; i < data_.length(); i++)
	{
		QVariantList statement_list = data_[i].toList();
		//QString ret  = statement_list[0].toString();
		//ShowInfo(QString("type %1").arg(ret));
		if (statement_list[0].toString() == "LinearMovement")
		{
			/*ShowInfo(QString("data %1").arg(statement_list[1].toList()[0].toString()));*/
			createPose(poseList, statement_list);
		}
		else if(statement_list[0].toString() == "JointMovement")
		{
			ShowInfo(QString("data %1").arg(statement_list[1].toString()));
			createPose(poseList, statement_list);
		}

	}


	//VSD::ModelNode *model_node = VSD::ModelNode(
	//	"SOML-Net",
	//	"./main.net"
	//);

	VSD::SimState* simstate = this->getProject()->getEnvironment()->getGuiSimState();
	VSLibKinematics::ExtensionKinematicMappingRoot* kinematics;
	for (VSD::Node* child_node = simstate->getDatabase()->getRootNode()->getFirstChildNode(); child_node != simstate->getDatabase()->getRootNode()->getLastChildNode(); child_node = child_node->getNextSiblingNode())
	{
		if (child_node->findFirstExtensionInherits<VSLibKinematics::ExtensionKinematicMappingRoot*>() != nullptr)
			kinematics = child_node->findFirstExtensionInherits<VSLibKinematics::ExtensionKinematicMappingRoot*>();

	}
	for (int i = 0; i < poseList->getNumberOfPoses(); i++)
	{
		poseList->getPose(i)->setAssociatedKinematic(kinematics);
	}
	VSLibPoseList::Path* path = CreateNewPathsFromPoseLists(QList<const VSLibPoseList::PoseList*>() << poseList).first();



	//VSD::SimState* simstate = this->getProject()->getEnvironment()->getGuiSimState();
	//VSLibKinematics::ExtensionKinematicMappingRoot* kinematics;
	//for (VSD::Node* child_node = simstate->getDatabase()->getRootNode()->getFirstChildNode(); child_node != simstate->getDatabase()->getRootNode()->getLastChildNode(); child_node = child_node->getNextSiblingNode())
	//{
	//	if (child_node->findFirstExtensionInherits<VSLibKinematics::ExtensionKinematicMappingRoot*>()!=nullptr)
	//		kinematics=child_node->findFirstExtensionInherits<VSLibKinematics::ExtensionKinematicMappingRoot*>();

	//}
	//poseList->getPose(0)->setAssociatedKinematic(kinematics);
	generateActionBlocksForPathMovement(path);

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



	QString file_name_ = QFileDialog::getSaveFileName(NULL, "Open FIle", "", "*.ls");
	QFile* file_ = new QFile(file_name_);
	QString header;
	QString pos;
	QString end;
	QTextStream out(file_);
	file_name_ = file_name_.section('/', -1, -1).section('.',0,0);
	ShowInfo(file_name_);
	QVariantList coord_;
	QString line;

	if (!file_->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return ;
	}

	QString producer = "FANUC";
	if (producer == "FANUC")
	{	
		header = QString("/PROG  %1\n/ATTR\nOWNER = MNEDITOR;\nCOMMENT = \"%2\";\nPROG_SIZE = 0;\nCREATE = %3;\nMODIFIED = %4;\nFILE_NAME = ;\nVERSION = 0;\nLINE_COUNT = 0;\nMEMORY_SIZE = 0;\nPROTECT = READ_WRITE;\nTCD:  STACK_SIZE = 0,\n	TASK_PRIORITY = 50,\n	TIME_SLICE = 0,\n	BUSY_LAMP_OFF = 0,\n	ABORT_REQUEST = 0,\n	PAUSE_REQUEST = 0;\nDEFAULT_GROUP = 1, *, *, *, *;\nCONTROL_CODE = 00000000 00000000;\n/APPL\n/MN\n" );
		pos = "/POS\n";
		//header.arg(file_name_).arg( file_name_).arg("DATE 01-01-00  TIME 00:00:00").arg( "DATE 01-01-00  TIME 00:00:00");
		end = "/END ";
	}
	//ShowInfo(QString("header %1").arg(header));

	VSD::SimState* simstate = this->getProject()->getEnvironment()->getGuiSimState();
	VSD::Node* nodeRoot = simstate->getDatabase()->getRootNode();
	QVariantList config;
	config.append("N U T");
	config.append("0");
	config.append("0");
	config.append("0");
	//config.append('0');
	//config.append('0');
	QString ut = "1";
	QString uf = "0";
	QString name;
	VSD::Node* child_node_ = nodeRoot->findChildNodeByName(QString("Path (from ").append(file_name_).append(")"));
	//ShowInfo(QString("line %1").arg(line));
	VSLibPoseList::Path* path = dynamic_cast<VSLibPoseList::Path*> (child_node_);
	QString line_pos_def_;
	QVariantList global_data_;

	global_data_.append(1);
	global_data_.append('0');
	global_data_.append('1');
	
	for (int i = 0; i < path->getNumberOfPoses(); i++)
	{
		QVariantList statement_data_;
		QVariantList position_data_;
		name = path->getPathPose(i)->getName();
		QString point_index = name.section('[', 1, 1).section(']',0,0);
		if (point_index.contains(':'))
		{
			point_index = point_index.section(':', 0, 0);

		}
		QString pos_type_ = name.section(':',1,1).section('[', 0, 0);
		coord_ = getCoordinates(path->getPathPose(i));
		QString pos_name = "";
		if (name.section('[', 1, 1).section(']', 0, 0).contains(':'))
		{
			pos_name = QString(':').append(name.section('[', 1, 1).section(']', 0, 0).section(':', 1, 1));

		}

		statement_data_.append(global_data_);
		statement_data_.append("Movement");
		statement_data_.append(uf);
		statement_data_.append(ut);
		statement_data_.append("lin");
		statement_data_.append(pos_type_);
		statement_data_.append(point_index);
		//name = path->getPathPose(i)->getName();
		statement_data_.append(name);
		statement_data_.append(100);
		statement_data_.append("FINE");
		statement_data_.append("");
		statement_data_.append("");
		statement_data_.append("");
		
		line += SetUpPythonDownloadProgram(statement_data_)[0].toString();
		if (!pos_type_.contains("PR"))
		{
			position_data_.append(uf);
			position_data_.append(ut);
			position_data_.append(config);
			position_data_.append(pos_name);
			position_data_.append(point_index);
			position_data_.append(coord_);
			line_pos_def_ += SetUpPythonDownloadPositionDefinition(position_data_);
		}

		global_data_ = SetUpPythonDownloadProgram(statement_data_)[1].toList();
		//QString i_string_;
		//i_string_.setNum(i+1);
		//for (int j = 0; j < (4 - i_string_.length()); j++)
		//{
		//	line.append(" ");
		//}
		//line.append(i_string_).append(":").append("L ").append(path->getPathPose(i)->getName().section(" ", 1, 1)).append(" 1000 mm/sec FINE").append(";\n");
	}
	//line=SetUpPythonDownload(statement_data_);
	out << header.arg(file_name_).arg(file_name_).arg("DATE 01-01-00  TIME 00:00:00").arg("DATE 01-01-00  TIME 00:00:00") +line+pos+line_pos_def_+end;
	file_->close();
	//if (child_node_->getFirstChildNode())
	//{
	//	ShowInfo(QString("line %1").arg(child_node_->getFirstChildNode()->getName()));
	//}

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

QVariantList VSPluginRobotLanguageTranslator::Project::getCoordinates(VSLibPoseList::PathPose* pathPose)
{
	QVariantList coord_;

	VSM::Frame frame=pathPose->getReferencedPose()->getRelFrame();
	coord_.append(frame.getPosition().getX()*1000);
	coord_.append(frame.getPosition().getY() * 1000);
	coord_.append(frame.getPosition().getZ() * 1000);
	coord_.append(frame.getOrientation().getYaw() * 180 / 3.1415926535);
	coord_.append(frame.getOrientation().getPitch() * 180 / 3.1415926535);
	coord_.append(frame.getOrientation().getRoll() * 180 / 3.1415926535);

	return coord_;
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
