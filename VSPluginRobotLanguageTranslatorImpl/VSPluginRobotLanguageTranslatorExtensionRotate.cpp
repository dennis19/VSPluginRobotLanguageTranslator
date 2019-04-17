#include "../VSPluginRobotLanguageTranslatorExtensionRotate.h"
#include "../../../Lib/VSDIO/VSDIOExtensionIOBoard.h"
#include "../../../Lib/VSDIO/VSDIOInputBool.h"
#include "../../../Lib/VSD3D/VSD3DNode.h"

VSPluginRobotLanguageTranslator::ExtensionRotate::ExtensionRotate(VSD::SimState* mySimState, VSD::SimStateInstance* otherSimStateInstance)
: VSPluginRobotLanguageTranslator::Extension(mySimState, otherSimStateInstance)
, dataRotAxis(this)
, dataRotVelocity(this)
{
}

VSPluginRobotLanguageTranslator::ExtensionRotate::~ExtensionRotate()
{
}

void VSPluginRobotLanguageTranslator::ExtensionRotate::setRotAxis(const VSM::Vector3& value)
{
   // set rotation axis
   dataRotAxis.set(value);

   // diff angle to default rotation axis
   VSM::Vector3 start(0.0, 0.0, 1.0);
   double cosAngle = VSM::dot(start, value.getNorm());
   double angle = acos(cosAngle);
   
   // current visu frame
   const VSM::Vector3 direction = VSM::cross(start, value).getNorm();
   VSM::Frame visuFrame(VSM::Matrix3x3(direction, -angle, true), VSM::Vector3());
   dataVisuFrame.set(visuFrame);
}

//void VSPluginRotateNodeWithPose::ExtensionRotate::slotEnabledChangedByInput(VSDIO::Input* input)
//{
//   if(input->inherits<VSDIO::InputBool*>())
//   {
//      VSDIO::InputBool* inputBool = input->instanceCast<VSDIO::InputBool*>();
//      if(inputBool)
//      {
//         setEnabled(inputBool->getValue());
//      }
//   }
//}

void VSPluginRobotLanguageTranslator::ExtensionRotate::execute()
{
   // check enabled
   if (!getEnabled())
   {
      return;
   }

   // absolute change of direction
   double diffAngle = getDiffTime() * getRotVelocity();

   // currentFrame is multiplied with the rotation matrix
   relFrame *= VSM::Matrix3x3(getRotAxis().getNorm(), -diffAngle);
}





