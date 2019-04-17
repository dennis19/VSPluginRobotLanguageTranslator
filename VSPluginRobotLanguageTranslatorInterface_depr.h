#ifndef VSPluginRobotLanguageTranslatorH
#define VSPluginRobotLanguageTranslatorH

// base
#include <Lib/VSP/VSPInterface.h>
#include <Lib/VSL/VSLDefines.h>

// specific
class VSPluginInterface;

namespace VEROSIM
{
	class MainWindow;
	class Project;
};

// version
#define VSPluginRobotLanguageTranslatorInterfaceVersion 0.1


namespace VSPluginRobotLanguageTranslator
{
	class Interface : public VSP::Interface
	{
		Q_OBJECT

			// construction	   
	public:
		//Interface();
		Interface(VSL::Application* application, VSL::Library* library);
		virtual ~Interface();

		// management      
	private slots:
		void slotCreateNewExtMainWindow(VEROSIM::MainWindow* mainwindow);
		void slotCreateNewExtProject(VEROSIM::Project* project);

	}; // class Interface
}; // namespace VSPluginRotateNodeWithPose

VEROSIM_DECLARE_DLL_ENTRY

#endif // VSPluginRobotLanguageTranslatorH 