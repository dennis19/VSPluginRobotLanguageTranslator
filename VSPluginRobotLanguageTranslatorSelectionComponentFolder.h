#ifndef VSPluginRobotLanguageTranslatorSelectionComponentFolderH
#define VSPluginRobotLanguageTranslatorSelectionComponentFolderH

#include "../VSLibSelection/VSLibSelectionComponent.h"



namespace VSPluginRobotLanguageTranslator
{
	/// A selection component for the project folder
	class SelectionComponentFolder : public VSLibSelection::Component
   {
		VSD_INSTANCE(
         HAS_DEFAULT_CONSTRUCTOR false
      );

		// construction
		public:
			/// @brief The constructor.		
			/// @param simState The sim state.
			SelectionComponentFolder(VSD::SimState* simState);

			/// @brief The destructor.
			virtual ~SelectionComponentFolder();
		
			/// @brief Returns the type of this selection component.
			/// @returns The type of this selection component.
			QString compileType() const;

			/// @brief Returns a description for this selection component.
			/// @returns A description for this selection component.
			QString compileDescription() const;

			/// @brief TODO
			/// @returns An empty QStringList.
			QStringList getKey() const;

			/// @brief TODO
			/// @param simState TODO
			/// @param key TODO
			/// @returns false. Always.
			bool restoreFromKey(VSD::SimState* simState, const QStringList& key);

     
   }; // class SelectionComponentFolder
   
}; // namespace VSPluginRotateNodeWithPose

VSD_DECLARE_INSTANCE(VSPluginRobotLanguageTranslator::SelectionComponentFolder);

#endif // VSPluginRotateNodeWithPoseSelectionComponentFolderH