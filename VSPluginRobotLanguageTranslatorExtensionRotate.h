#ifndef VSPluginRotateNodeWithPoseRotateExtensionH
#define VSPluginRotateNodeWithPoseRotateExtensionH

// base
#include "VSPluginRobotLanguageTranslatorExtension.h"

namespace VSDIO
{
   class Input; 
}

namespace VSPluginRobotLanguageTranslator
{
   class ExtensionRotate : public VSPluginRobotLanguageTranslator::Extension
   {
      VSD_ENVIRONMENT_INSTANCE(0);

      // construction      
   protected:
      ExtensionRotate(VSD::SimState* mySimState, VSD::SimStateInstance* otherSimStateInstance);
      virtual ~ExtensionRotate();

   public:
      virtual void execute();

      //private slots:
      //void slotUpdate(double tSim, double dtSim);

      private slots:
         //void slotEnabledChangedByInput(VSDIO::Input* input);
         
         // data
   private:
      // rotation axis
      VSD_PROPERTY_VAL(VSM::Vector3 rotAxis
         DATA getDataRotAxis 
         READ getRotAxis 
         WRITE setRotAxis 
         SAVE true
         EDIT_VIEW true
         EDIT_MODIFY true
         CLONE true
         SYNC_LOCAL_VIA_PROPERTY true
         SYNC_DISTRIBUTED_VIA_INSTANCE true);
      VSD_PROPERTY_TEMPLATE_DATA_VAL_INTERNAL(VSM::Vector3, RotAxis);
      VSD_PROPERTY_TEMPLATE_GET_DATA_VAL_INTERNAL(VSM::Vector3, RotAxis);
      VSD_PROPERTY_TEMPLATE_GET_VAL_INTERNAL(VSM::Vector3, RotAxis);
      VSD_PROPERTY_TEMPLATE_SET_DECL_VAL(VSM::Vector3, RotAxis);

      // rotation velocity
      VSD_PROPERTY_VAL(double rotVelocity
         UNIT VSD::Unit::RadianPerSecond
         DATA getDataRotVelocity 
         READ getRotVelocity 
         WRITE setRotVelocity
         SAVE true
         EDIT_VIEW true
         EDIT_MODIFY true
         CLONE true
         SYNC_LOCAL_VIA_PROPERTY true
         SYNC_DISTRIBUTED_VIA_INSTANCE true);
      VSD_PROPERTY_TEMPLATE_VAL_INTERNAL(double, RotVelocity);
      // no overwritten GET
      // no overwritten SET

   }; // class Extension
}; // namespace VSPluginRotateNodeWithPose

VSD_DECLARE_ENVIRONMENT_INSTANCE(VSPluginRobotLanguageTranslator::ExtensionRotate);

#endif //VSPluginRotateNodeWithPoseExtensionH
