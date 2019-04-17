#ifndef VSPluginRotateNodeWithPoseExtensionTranslateH
#define VSPluginRotateNodeWithPoseExtensionTranslateH

// base
#include "VSPluginRobotLanguageTranslatorExtension.h"


namespace VSPluginRobotLanguageTranslator
{
   class ExtensionTranslate : public VSPluginRobotLanguageTranslator::Extension
   {
   VSD_ENVIRONMENT_INSTANCE(0);

   // construction      
   protected:
      ExtensionTranslate(VSD::SimState* mySimState, VSD::SimStateInstance* otherSimStateInstance);
      virtual ~ExtensionTranslate();

   public:
      virtual void execute();

   private:
      //VSDIO::ExtensionIOBoard* getIOBoard();

   private:
      // translation axis
      VSD_PROPERTY_VAL(VSM::Vector3 transAxis
         DATA getDataTransAxis 
         READ getTransAxis 
         WRITE setTransAxis 
         SAVE true
         EDIT_VIEW true
         EDIT_MODIFY true
         CLONE true
         SYNC_LOCAL_VIA_PROPERTY true
         SYNC_DISTRIBUTED_VIA_INSTANCE true);
      VSD_PROPERTY_TEMPLATE_DATA_VAL_INTERNAL(VSM::Vector3, TransAxis);
      VSD_PROPERTY_TEMPLATE_GET_DATA_VAL_INTERNAL(VSM::Vector3, TransAxis);
      VSD_PROPERTY_TEMPLATE_GET_VAL_INTERNAL(VSM::Vector3, TransAxis);
      VSD_PROPERTY_TEMPLATE_SET_DECL_VAL(VSM::Vector3, TransAxis);

      // translation velocity
      VSD_PROPERTY_VAL(double transVelocity
         UNIT VSD::Unit::MeterPerSecond
         DATA getDataTransVelocity 
         READ getTransVelocity 
         WRITE setTransVelocity
         SAVE true
         EDIT_VIEW true
         EDIT_MODIFY true
         CLONE true
         SYNC_LOCAL_VIA_PROPERTY true
         SYNC_DISTRIBUTED_VIA_INSTANCE true);
      VSD_PROPERTY_TEMPLATE_VAL_INTERNAL(double, TransVelocity);
      // no overwritten GET
      // no overwritten SET

   };//class ExtensionTranslate
}; //namespace VSPluginRotateNodeWithPose

VSD_DECLARE_ENVIRONMENT_INSTANCE(VSPluginRobotLanguageTranslator::ExtensionTranslate);

#endif
