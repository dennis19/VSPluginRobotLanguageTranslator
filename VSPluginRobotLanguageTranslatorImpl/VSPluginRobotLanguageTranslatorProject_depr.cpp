// specific
#include "../../../Main/VEROSIM/VEROSIMProject.h"
#include "../../../Lib/VSD/VSDDatabase.h"
#include "../../../Lib/VSD/VSDElementIndex.h"
#ifdef QT_WIDGETS_LIB
#include "../../VSLibRenderGL/VSLibRenderGLRenderSimStateExtension.h"
//#include "../VSPluginRotateNodeWithPoseVisu.h"
#endif
//#include "../VSPluginRotateNodeWithPoseExtensionRotate.h"
//#include "../VSPluginRotateNodeWithPoseExtensionTranslate.h"

#undef max
#include <algorithm>

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
//#include "../VSPluginRotateNodeWithPoseExtensionRotate.h"
//#include "../VSPluginRotateNodeWithPoseExtensionTranslate.h"
#ifdef QT_WIDGETS_LIB
//#include "../VSPluginRotateNodeWithPoseExplorerPage.h"
#endif


#include "../../../Lib/VSDIO/VSDIOExtensionIOBoard.h"
#include "../../../Lib/VSDIO/VSDIOInputBool.h"

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
	connect(workcell, SIGNAL(signalInitProject(VEROSIM::Project*, VSD::SimState*)),
		this, SLOT(slotInitProject(VEROSIM::Project*, VSD::SimState*)));
	connect(workcell, SIGNAL(signalProjectWillBeClosed(VEROSIM::Project*)),
		this, SLOT(slotProjectWillBeClosed()));
}

VSPluginRobotLanguageTranslator::Project::~Project()
{}

bool VSPluginRobotLanguageTranslator::Project::getVisuRotAxis() const
{
	return visuRotAxes;
}

void VSPluginRobotLanguageTranslator::Project::slotCreateImport()
{
	//slotCreateNode(Box, Translate);
}