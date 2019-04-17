#include "../VSPluginRobotLanguageTranslatorExtensionTranslate.h"
#include "../../../Lib/VSDIO/VSDIOExtensionIOBoard.h"
#include "../../../Lib/VSDIO/VSDIOInputBool.h"
#include "../../../Lib/VSD3D/VSD3DNode.h"

VSPluginRobotLanguageTranslator::ExtensionTranslate::ExtensionTranslate(VSD::SimState* mySimState, VSD::SimStateInstance* otherSimStateInstance)
: VSPluginRobotLanguageTranslator::Extension(mySimState, otherSimStateInstance)
, dataTransAxis(this)
, dataTransVelocity(this)
{ 
}

VSPluginRobotLanguageTranslator::ExtensionTranslate::~ExtensionTranslate()
{
}

void VSPluginRobotLanguageTranslator::ExtensionTranslate::setTransAxis(const VSM::Vector3& value)
{ 
   // set translation axis
   dataTransAxis.set(value);

   // diff angle to default rotation axis
   VSM::Vector3 start(0.0, 0.0, 1.0);
   double cosAngle = VSM::dot(start, value.getNorm()); 
   double angle = acos(cosAngle);
   
   // check difference to default rotation axis
   if (fabs(angle) < VSM::epsilon)
   {
      return;
   }
   
   // current visu frame
   const VSM::Vector3 direction = VSM::cross(start, value).getNorm();
   VSM::Frame visuFrame(VSM::Matrix3x3(direction, -angle, true), VSM::Vector3());

   dataVisuFrame.set(visuFrame);
}

void VSPluginRobotLanguageTranslator::ExtensionTranslate::execute()
{
   // check enabled
   if (!getEnabled())
   {
      return;
   }

   // absolute change of location
   double diffVal = getDiffTime() * getTransVelocity();
   
   // directed change is add to current position
   relFrame += diffVal * getTransAxis().getNorm();
}

