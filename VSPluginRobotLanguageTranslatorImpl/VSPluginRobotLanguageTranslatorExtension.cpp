#include "../VSPluginRobotLanguageTranslatorExtension.h"
#include "../../../Lib/VSDIO/VSDIOExtensionIOBoard.h"
#include "../../../Lib/VSDIO/VSDIOInputBool.h"
#include "../../../Lib/VSD3D/VSD3DNode.h"
#include "../../../Lib/VSD/VSDConnection.h"

/**
  * \brief
  * Creates the object.
  */
VSPluginRobotLanguageTranslator::Extension::Extension(VSD::SimState *mySimState, VSD::SimStateInstance *otherSimStateInstance)
: VSD::ModelInstanceExtension(mySimState, otherSimStateInstance)
, VSS::TaskStep("VSPluginRotateNodeWithPose::Extension")
, dataVisuFrame(this)
, dataEnabled(this)
, mIoBoard(0)
, mInputBool(0)
{
   setStepTime(0.002);
   connect(this, SIGNAL(signalPropertyModified(VSD::SimStateInstance* , const VSD::MetaProperty*)),
           this, SLOT(slotEnabledModified()));
}

/**
  * \brief
  * Destroys the object.
  */
VSPluginRobotLanguageTranslator::Extension::~Extension()
{
   // nothing to do
}

void VSPluginRobotLanguageTranslator::Extension::load()
{
   VSD3D::Node* node = getMyModelInstance()->instanceCast<VSD3D::Node*>();
   VSL_ASSERT_ELSE_RETURN(node != 0);

   VSDIO::ExtensionIOBoard *ioboard = node->findFirstExtensionInherits<VSDIO::ExtensionIOBoard*>();
   if (ioboard != 0)
   {
      VSL_ASSERT_ELSE_RETURN(ioboard != 0);
      VSDIO::Input *extInput = ioboard->getInput(0);
      VSL_ASSERT_ELSE_RETURN(extInput != 0);

      VSDIO::InputBool* value = extInput->instanceCast<VSDIO::InputBool*>();
      VSL_ASSERT_ELSE_RETURN(value != 0);
      bool val = value->getValue();
      if (getEnabled() != val)
      {
         setEnabled(val);
      }
   }

   relFrame = node->getRelFrame();
}

void VSPluginRobotLanguageTranslator::Extension::save()
{
   VSD3D::Node* node = getMyModelInstance()->instanceCast<VSD3D::Node*>();
   VSL_ASSERT_ELSE_RETURN(node != 0);
   node->setRelFrame(relFrame);
}

void VSPluginRobotLanguageTranslator::Extension::setInputEnabled(const bool enable)
{
   if (mInputBool)
      mInputBool->setLocalValue(enable);
}

void VSPluginRobotLanguageTranslator::Extension::postHasBeenAddedToElementContainer()
{
   VSS::Scheduler::addSimStateTask(this);
   VSD3D::Node* extensionParent = getMyModelInstance()->instanceCast<VSD3D::Node*>();
   if (extensionParent)
   {
      connect(extensionParent, SIGNAL(signalPropertyModified(VSD::SimStateInstance* , const VSD::MetaProperty*)),
              this, SLOT(slotIOBoardAdded(VSD::SimStateInstance* , const VSD::MetaProperty*)));
   }
}

void VSPluginRobotLanguageTranslator::Extension::preWillBeRemovedFromElementContainer()
{
   VSS::Scheduler::removeSimStateTask(this);
}

/**
  * \brief
  *  Returns the first ExtensionIOBoard of the parentNode if there is one.
  *  Otherwiese 0 will be returned.
  */
VSDIO::ExtensionIOBoard* VSPluginRobotLanguageTranslator::Extension::getIOBoard()
{
   if (!getMyModelInstance())
      return 0;
   VSD3D::Node* extensionParent = getMyModelInstance()->instanceCast<VSD3D::Node*>();
   if (extensionParent)
   {
      VSDIO::ExtensionIOBoard* ioboard = extensionParent->findFirstExtensionInherits<VSDIO::ExtensionIOBoard*>();
      return ioboard;
   }
   return 0;
}

/**
  * \brief 
  *  Returns an VSDUI::InputBool object if there existis one at the 0th index of the ExtensionIOBoard.
  *  Otherwise the function will retunr 0.
  */
VSDIO::InputBool* VSPluginRobotLanguageTranslator::Extension::getInputBool(const VSDIO::ExtensionIOBoard& ioBoard)
{
   if (ioBoard.getInput(0))
   {
      return ioBoard.getInput(0)->instanceCast<VSDIO::InputBool*>();
   }
   return 0;
}

/**
  *
  *
  */
void VSPluginRobotLanguageTranslator::Extension::connectInputBool(VSDIO::InputBool *input)
{
  if (mInputBool)
  {
     disconnect(mInputBool, SIGNAL(signalPropertyModified(VSDIO::Input*)),
                this, SLOT(slotEnabledChangedByInput(VSDIO::Input*)));
  }
  if (!input)
  {
     mInputBool = 0;
     return;
  }
  mInputBool = input;
  // Connect new input and update values.
  input->setLocalValue(getEnabled());
  connect(mInputBool, SIGNAL(signalInputValueModified(VSDIO::Input*)),
          this, SLOT(slotEnabledChangedByInput(VSDIO::Input*)));
}

/**
  *
  *
  */
void VSPluginRobotLanguageTranslator::Extension::slotIOBoardAdded(VSD::SimStateInstance* instance, const VSD::MetaProperty* prop)
{
   Q_UNUSED(instance);
   Q_UNUSED(prop);
   VSDIO::ExtensionIOBoard* ioBoard = getIOBoard();
   if (ioBoard)
   {
      if (mIoBoard)
      {
         // Disconnect the old ioBoard.
         disconnect(mIoBoard, SIGNAL(signalPropertyModified(VSD::SimStateInstance*, const VSD::MetaProperty*)),
                    this, SLOT(slotInputBoolAdded(VSD::SimStateInstance*, const VSD::MetaProperty*)));
      }
      mIoBoard = ioBoard;
      // Connect the new ioBoard.
      connect(mIoBoard, SIGNAL(signalPropertyModified(VSD::SimStateInstance*, const VSD::MetaProperty*)),
              this, SLOT(slotInputBoolAdded(VSD::SimStateInstance*, const VSD::MetaProperty*)));

      // The values will be synchronised, if the ioboard has an InputBool at the 0th 
      // position.
      VSDIO::InputBool* input = getInputBool(*ioBoard);
      // Disconnect old input.
      connectInputBool(input);
   }
}

void VSPluginRobotLanguageTranslator::Extension::slotInputBoolAdded(VSD::SimStateInstance *instance, const VSD::MetaProperty *prop)
{
   Q_UNUSED(instance);
   Q_UNUSED(prop);

   if (!mIoBoard)
      return;
   VSDIO::InputBool* input = getInputBool(*mIoBoard);
   connectInputBool(input);
}

void VSPluginRobotLanguageTranslator::Extension::slotEnabledModified()
{
   if (mInputBool)
   {
      mInputBool->setLocalValue(getEnabled());
   }
}

void VSPluginRobotLanguageTranslator::Extension::slotEnabledChangedByInput(VSDIO::Input* input)
{
   Q_UNUSED(input);
   if (mInputBool && mInputBool == input)
      setEnabled(mInputBool->getLocalValue());
}