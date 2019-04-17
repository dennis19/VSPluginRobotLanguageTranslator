#ifndef VSPluginRobotLanguageTranslatorExtensionH
#define VSPluginRobotLanguageTranslatorExtensionH

// base
#include "../../Lib/VSS/VSSTaskStep.h"
#include "../../Lib/VSD/VSDModelInstanceExtension.h"

// specific
#include "../../Lib/VSM/VSMVector3.h"
#include "../../Lib/VSM/VSMFrame.h"


namespace VSDIO
{
   class ExtensionIOBoard;
   class InputBool;
   class Input;
}

namespace VSPluginRobotLanguageTranslator
{
   class Extension : public VSD::ModelInstanceExtension, 
                     // VSD_MOC_IGNORE_START 
                     public VSS::TaskStep 
                     // VSD_MOC_IGNORE_END   
   {
        
      VSD_ENVIRONMENT_INSTANCE(ABSTRACT true);

   protected:
      Extension(VSD::SimState* mySimState, VSD::SimStateInstance* otherSimStateInstance);
      virtual ~Extension();

   public:
      virtual void load();
      virtual void execute() = 0;
      virtual void save();
      void setInputEnabled(const bool enable);

   private:
      void postHasBeenAddedToElementContainer();
      void preWillBeRemovedFromElementContainer();
      VSDIO::ExtensionIOBoard* getIOBoard();
      VSDIO::InputBool* getInputBool(const VSDIO::ExtensionIOBoard& ioBoard);
      void connectInputBool(VSDIO::InputBool* input);

   private slots:
      void slotIOBoardAdded(VSD::SimStateInstance* instance, const VSD::MetaProperty* prop);
      void slotInputBoolAdded(VSD::SimStateInstance* instance, const VSD::MetaProperty* prop);
      void slotEnabledModified();

   public slots:
      void slotEnabledChangedByInput(VSDIO::Input* input);

   private:
      VSDIO::ExtensionIOBoard* mIoBoard;
      VSDIO::InputBool*        mInputBool;

   protected:
      VSM::Frame relFrame;

       // frame resulting from rotation axis for fast visualisation
      VSD_PROPERTY_VAL(VSM::Frame visuFrame
         DATA getDataVisuFrame
         READ getVisuFrame
         SAVE true
         EDIT_VIEW true
         EDIT_MODIFY true
         CLONE true
         SYNC_LOCAL_VIA_PROPERTY true
         SYNC_DISTRIBUTED_VIA_INSTANCE true);
      // READ_ONLY property
      VSD_PROPERTY_TEMPLATE_DATA_VAL_INTERNAL(VSM::Frame, VisuFrame);
      VSD_PROPERTY_TEMPLATE_GET_DATA_VAL_INTERNAL(VSM::Frame, VisuFrame);
      VSD_PROPERTY_TEMPLATE_GET_VAL_INTERNAL(VSM::Frame, VisuFrame);

        // enable flag
      VSD_PROPERTY_VAL(bool enabled
         DATA getDataEnabled 
         READ getEnabled 
         WRITE setEnabled
         SAVE true
         EDIT_VIEW true
         EDIT_MODIFY true
         CLONE true
         SYNC_LOCAL_VIA_PROPERTY true
         SYNC_DISTRIBUTED_VIA_INSTANCE true);
      VSD_PROPERTY_TEMPLATE_VAL_INTERNAL(bool, Enabled);
      // no overwritten GET
      // no overwritten SET

   }; // class Extension
}; // namespace VSPluginRobotLanguageTranslator

VSD_DECLARE_ENVIRONMENT_INSTANCE(VSPluginRobotLanguageTranslator::Extension);

#endif // VSPluginRobotLanguageTranslatorExtensionH