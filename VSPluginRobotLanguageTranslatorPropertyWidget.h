#ifndef VSPluginRotateNodeWithPosePropertyWidgetH
#define VSPluginRotateNodeWithPosePropertyWidgetH

// base
#include <QWidget>
// specific
#include "../VSLibSelection/VSLibSelectionComponentRefList.h"
#include "../VSLibSelection/VSLibSelectionComponentSimStateInstance.h"
#include "Main/VEROSIM/VEROSIMProject.h"

#include "Lib/VSD/VSDMetaPropertyVal.h"

//#include "Lib/VSLibUnitConversion/VSLibUnitConversionConverter.h"

#include "Plugin/VSPluginProperties/VSPluginPropertiesUnitDoubleSpinBox.h"

class QCheckBox;
class QDoubleSpinBox;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;


namespace VSD3D
{
   class Node;
}
namespace VSDIO 
{
   class InputBool;
   class Input;
}


namespace VSPluginRobotLanguageTranslator
{
   class Project;
   class ExtensionRotate;
   class ExtensionTranslate;

   class MovingNodePropertyWidget : public QWidget
   {
	   Q_OBJECT

   // construction	   
   public:
	   MovingNodePropertyWidget(const VSPluginRobotLanguageTranslator::Project& project);
      virtual ~MovingNodePropertyWidget();

   // management
   private slots:
      void slotSelectionChanged(const VSLibSelection::ComponentRefList& selection);
      void slotVelocityValueChanged();
      void slotTranslateBoxStateChanged();
      void slotRotateBoxStateChanged();
      void slotUpdateTranslationNodeUI(VSD::SimStateInstance* instance, const VSD::MetaProperty* metaProp);
      void slotUpdateRotationNodeUI(VSD::SimStateInstance* instance, const VSD::MetaProperty* metaProp);
      //void slotUpdateEnableUI();

   signals:
      void signalFindConversionForVSDUnit(const VSD::Unit& unit, QString& targetString);
      void signalFindConversionForMetaPropertyVal(const VSD::MetaPropertyVal* mpVal, QString& targetString);
      void signalVelocityModified(const QVariant& value, 
                                  VSD::SimStateInstance* parent,
                                  const VSD::MetaProperty* parentPropertyVal,
                                  QString commandText = tr("Velocity changed"));
      void signalExtensionEnabledModified(const QVariant& value,
                                          VSD::SimStateInstance* parent,
                                          const VSD::MetaProperty* parentPropertyVal,
                                          QString commandText = tr("RotateNodeWithPoseExtension enabled/disabled"));
   // data
   private:
      VSD::Ref<VSD3D::Node> movingNode;
      VSD::Ref<ExtensionRotate> extRot;
      VSD::Ref<ExtensionTranslate> extTrans;
      QCheckBox* translateBox;
      QCheckBox* rotateBox;
      VSPluginProperties::UnitDoubleSpinBox* velocitySpinBox;
      const VSPluginRobotLanguageTranslator::Project& project;

      QVBoxLayout* mainLayout;
      QHBoxLayout* firstRowLayout;
      QHBoxLayout* secRowLayout;
      QLabel* velocityLabel;

      //VSLibUnitConversion::Converter converterRotVel;
      //VSLibUnitConversion::Converter converterTransVel;

   }; // class MovingNodePropertyWidget
} // namespace VSPluginRotateNodeWithPose

#endif // VSPluginRotateNodeWithPosePropertyWidgetH
