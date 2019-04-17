#pragma once
#ifndef VSPluginRobotLanguageTranslatorProject
#define VSPluginRobotLanguageTranslatorProject


// base
#include "../../Lib/VSP/VSPProject.h"
// specific
#include "../../Lib/VSD/VSDRefList.h"
//#include "../../Lib/VSD3D/VSD3DMapNode.h"
#include "../../Lib/VSS/VSSTaskStep.h"
#include "../../Lib/VSM/VSMVector3.h"

#include "VSPluginRobotLanguageTranslatorInterface.h"

namespace VSD
{
	class Node;
}

namespace VSPluginExplorer
{
	class BasePage;
}

namespace VSPluginRobotLanguageTranslator
{
	static QString settingsGroupName("/Extensions/VSPluginRobotLanguageTranslator");

	class Visu;
	class Project : public VSP::Project
	{
		Q_OBJECT

			// construction	   
	public:
		Project(VSP::Interface* interface, VEROSIM::Project* workcell);
		virtual ~Project();

	public:
		bool getVisuRotAxis() const;
		void triggerRepaint();

	private:
		enum MovementType
		{
			Translate,
			Rotate
		};
		enum GeometryType
		{
			Box,
			Sphere
		};

	private:
		void createAndAppendRotationExtension(VSD::SimState& simstate,
			VSD::Node& objectNode,
			const double velocity);
		void createAndAppendTranslationExtension(VSD::SimState& simstate,
			VSD::Node& objectNode,
			const double velocity);
		VSM::Vector3 createRandomisedVector(const double lowerLimit,
			const double upperLimit);
		void restrictVectorToPositivCoordinateAxes(VSM::Vector3& vec);

		// services      
	//signals:
	//	void signalVisuRotAxesModified(bool on);
	//	void signalMinVelBoarderModified(double val);
	//	void signalMaxVelBoarderModified(double val);
	//	void signalTriggerPaintAllWindows();

	//	public slots:
	//	void slotCreateNode(const GeometryType geometryType,
	//		const MovementType movementType);
	//	void slotCreateTransNode();
	//	void slotCreateRotNode();
	//	void slotCreateTransSphereNode();
	//	//void slotVelocitySpinValueChanged();
	//	void slotCreateExplorerPage(const QString& internalName, VSPluginExplorer::BasePage*& newPage);

	//	// management      
	//	private slots:
	//	void slotInitProject(VEROSIM::Project*, VSD::SimState* simState);
	//	void slotInitRenderGL(VSD::SimState* renderSimState);
	//	void slotReadSettings();
		void slotCreateImport();
	//	void slotToggleVisuRotAxes(bool on);
	//	void slotSetMinVelocitySpinBox(double val);
	//	void slotSetMaxVelocitySpinBox(double val);
	//	void slotProjectWillBeClosed();
	//	//void slotExtensionCreated(VSD::Element *);

		// data      
	private:
		bool visuRotAxes;
		double minVelBoarder;
		double maxVelBoarder;
		Visu* visu;
	}; // class Project
}; // namespace VSPluginRotateNodeWithPose


#endif // VSPluginRobotLanguageTranslatorProject 