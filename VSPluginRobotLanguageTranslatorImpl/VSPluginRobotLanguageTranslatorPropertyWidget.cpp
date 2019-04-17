// header
#include "../VSPluginRobotLanguageTranslatorPropertyWidget.h"
// special
#include <QLabel>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>

#include <Lib/VSD3D/VSD3DNode.h>
#include "../VSPluginRobotLanguageTranslatorExtensionRotate.h"
#include "../VSPluginRobotLanguageTranslatorExtensionTranslate.h"
#include "../VSPluginRobotLanguageTranslatorProject.h"
#include <Lib/VSDIO/VSDIOExtensionIOBoard.h>
#include <Lib/VSDIO/VSDIOInputBool.h>
#include <Lib/VSD/VSDConnection.h>

VSPluginRobotLanguageTranslator::MovingNodePropertyWidget::MovingNodePropertyWidget(const VSPluginRobotLanguageTranslator::Project& project)
: QWidget()
, translateBox(0)
, rotateBox(0)
, velocitySpinBox(0)
, mainLayout(new QVBoxLayout(this))
, firstRowLayout(new QHBoxLayout())
, secRowLayout(new QHBoxLayout())
, project(project)
//, converterRotVel(VSD::Unit::RadianPerSecond)
//, converterTransVel(VSD::Unit::MeterPerSecond)
{
   setObjectName("VSPluginRotateNodeWithPose.MovingNodePropertyWidget");
   setWindowTitle(tr("Moving Nodes"));
   setWindowIcon(QPixmap(":/VSPluginRotateNodeWithPose/ToggleVisuRotAxes.png"));

  // QLabel* modeLabel = new QLabel(tr("Controll:"));
   
   translateBox = new QCheckBox(this);
   translateBox->setText(tr("Translation"));

   rotateBox = new QCheckBox(this);
   rotateBox->setText(tr("Rotation"));

   velocityLabel = new QLabel(tr("Velocity:"), this);
   velocitySpinBox = new VSPluginProperties::UnitDoubleSpinBox(VSD::Unit::Undefined, project.getProject(), this);
   velocitySpinBox->setMaximumSI(VSM::maxDouble);
   
   //mainLayout->addWidget(modeLabel);
   firstRowLayout->addWidget(translateBox);
   firstRowLayout->addWidget(rotateBox);
   mainLayout->addLayout(firstRowLayout); 
   
   secRowLayout->addWidget(velocityLabel);
   secRowLayout->addWidget(velocitySpinBox);
   mainLayout->addLayout(secRowLayout);

   connect(velocitySpinBox, SIGNAL(editingFinished()),
           this, SLOT(slotVelocityValueChanged()));
   connect(translateBox, SIGNAL(stateChanged(int)),
           this, SLOT(slotTranslateBoxStateChanged()));
   connect(rotateBox, SIGNAL(stateChanged(int)),
           this, SLOT(slotRotateBoxStateChanged()));

   QObject* propProject = project.findExtProject("VSPluginProperties::Project");
   bool success = connect(this, SIGNAL(signalFindConversionForVSDUnit(const VSD::Unit&, QString&)),
                          propProject, SLOT(slotFindConversionForVSDUnit(const VSD::Unit&, QString&)));
   Q_ASSERT(success);

   success = connect(this, SIGNAL(signalFindConversionForMetaPropertyVal(const VSD::MetaPropertyVal*, QString&)),
                     propProject, SLOT(slotFindConversionForMetaPropertyVal(const VSD::MetaPropertyVal*, QString&)));
   Q_ASSERT(success);
   // Create connection between this widget and the VSPluginCommand.
   // This way, commands will be created for changes on the UI.
   QObject* cmdProject  = project.findExtProject("VSPluginCommandsVSD::Project");
   success = connect(this, SIGNAL(signalVelocityModified(const QVariant&, VSD::SimStateInstance*, const VSD::MetaProperty*, QString)),
                     cmdProject, SLOT(slotModifyPropertyVal(const QVariant&, VSD::SimStateInstance*, const VSD::MetaProperty*, QString)));
   Q_ASSERT(success);
   success = connect(this, SIGNAL(signalExtensionEnabledModified(const QVariant&, VSD::SimStateInstance*, const VSD::MetaProperty*, QString)),
                     cmdProject, SLOT(slotModifyPropertyVal(const QVariant&, VSD::SimStateInstance*, const VSD::MetaProperty*, QString)));
}

VSPluginRobotLanguageTranslator::MovingNodePropertyWidget::~MovingNodePropertyWidget()
{
   // The following objects are deleted by their parent (this)
   // translateBox;
   // rotateBox;
   // velocitySpinBox;
   // velocityLabel;
   // mainLayout;*/

   // These layouts have to be deleted manually.
   delete firstRowLayout;
   delete secRowLayout;
}

void VSPluginRobotLanguageTranslator::MovingNodePropertyWidget::slotSelectionChanged(const VSLibSelection::ComponentRefList& selection)
{
   // check selection is not empty and contains exactly one SimStateInstance
   if ((selection.count() == 1) && (selection.first()->inherits<VSLibSelection::ComponentSimStateInstance*>()))
   {
      // check if selected instance is a NodeWithPose
      movingNode = selection.first()->instanceConstCast<const VSLibSelection::ComponentSimStateInstance*>()->getSimStateInstance()->instanceCast<VSD3D::Node*>();
      if (movingNode != 0)
      {
         // Disconnect old extensions.
         if (extRot)
         {
            VSD::Connection::disconnect(extRot, SIGNAL(signalPropertyModified(VSD::SimStateInstance* , const VSD::MetaProperty*)),
                                        this, SLOT(slotUpdateRotationNodeUI()));
         }
         if (extTrans)
         {
            VSD::Connection::disconnect(extTrans, SIGNAL(signalPropertyModified(VSD::SimStateInstance* , const VSD::MetaProperty*)),
                                        this, SLOT(slotUpdateTranslationNodeUI()));
         }

         extRot = movingNode->findFirstExtension<ExtensionRotate*>();
         extTrans = movingNode->findFirstExtension<ExtensionTranslate*>();

         // selection can be interpreted -> enable tab in properties window
         if (extRot != 0 || extTrans != 0)
         {
            if(isVisible())
            {
               if (extRot != 0)
               {
                  VSD::Connection::connect(extRot, SIGNAL(signalPropertyModified(VSD::SimStateInstance* , const VSD::MetaProperty*)),
                                           this, SLOT(slotUpdateRotationNodeUI(VSD::SimStateInstance*, const VSD::MetaProperty*)));

                  translateBox->setEnabled(false);
                  translateBox->blockSignals(true);
                  translateBox->setChecked(false);
                  translateBox->blockSignals(false);

                  rotateBox->setEnabled(true);
                  rotateBox->blockSignals(true);
                  rotateBox->setChecked(extRot->getEnabled());
                  rotateBox->blockSignals(false);

                  slotUpdateRotationNodeUI(0, 0);
               }
               if (extTrans != 0)
               {
                  VSD::Connection::connect(extTrans, SIGNAL(signalPropertyModified(VSD::SimStateInstance* , const VSD::MetaProperty*)),
                                           this, SLOT(slotUpdateTranslationNodeUI(VSD::SimStateInstance*, const VSD::MetaProperty*)));

                  rotateBox->setEnabled(false);
                  rotateBox->blockSignals(true);
                  rotateBox->setChecked(false);
                  rotateBox->blockSignals(false);

                  translateBox->setEnabled(true);
                  translateBox->blockSignals(true);
                  translateBox->setChecked(extTrans->getEnabled());
                  translateBox->blockSignals(false);

                  extTrans->getTransVelocity();
                  slotUpdateTranslationNodeUI(0, 0);
               }
            } // end if visible
            setEnabled(true);
            return;
         } // end if (!extRot || !extTrans)
      }  // end if (!movingNode)
   } // end if (selection...)
   // selection cannot be interpreted -> disable tab in properties window
   setEnabled(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn  void VSPluginRotateNodeWithPose::MovingNodePropertyWidget::slotVelocityValueChanged()
///
/// @brief  Slot velocity value changed. 
///         The functions creates a command (via signal) if the velocity value changes.
///
/// @author Albrecht
/// @date   31.10.2013
////////////////////////////////////////////////////////////////////////////////////////////////////

void VSPluginRobotLanguageTranslator::MovingNodePropertyWidget::slotVelocityValueChanged()
{
   qDebug() << "slot velocity changed";
   if (extRot != 0 || extTrans != 0)
   {
      if (extRot != 0)
      {
         if (extRot->getRotVelocity() != velocitySpinBox->value() )
         {
            emit signalVelocityModified(velocitySpinBox->siValue(),
                                        extRot,
                                        extRot->getDataRotVelocity()->getMetaProperty());
         }
      }
      if (extTrans != 0)
      {
         if (extTrans->getTransVelocity() != velocitySpinBox->value() )
         {
            emit signalVelocityModified(velocitySpinBox->siValue(),
                                        extTrans,
                                        extTrans->getDataTransVelocity()->getMetaProperty());
         }
      }
      return;
   }
   setEnabled (false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn  void VSPluginRotateNodeWithPose::MovingNodePropertyWidget::slotTranslateBoxStateChanged()
///
/// @brief  Slot translate box state changed. 
///         The functions creates a command (via signal) if the state of the translation box changes.
///
/// @author Albrecht
/// @date   31.10.2013
////////////////////////////////////////////////////////////////////////////////////////////////////

void VSPluginRobotLanguageTranslator::MovingNodePropertyWidget::slotTranslateBoxStateChanged()
{
   if (extTrans != 0)
   {
      if (extTrans->getEnabled() != translateBox->isChecked())
      {
         //extTrans->blockSignals(true);
         emit signalExtensionEnabledModified(translateBox->isChecked(),
                                             extTrans,
                                             extTrans->getDataEnabled()->getMetaProperty());
         //extTrans->blockSignals(false);
      }
      extTrans->setEnabled(translateBox->isChecked());
   }
   //setEnabled (false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn  void VSPluginRotateNodeWithPose::MovingNodePropertyWidget::slotRotateBoxStateChanged()
///
/// @brief  Slot rotate box state changed. 
///         The functions creates a command (via signal) if the state of the rotation box changes.
///
/// @author Albrecht
/// @date   31.10.2013
////////////////////////////////////////////////////////////////////////////////////////////////////

void VSPluginRobotLanguageTranslator::MovingNodePropertyWidget::slotRotateBoxStateChanged()
{
   if (extRot != 0)
   {
      if (extRot->getEnabled() != rotateBox->isChecked())
      {
         emit signalExtensionEnabledModified(rotateBox->isChecked(),
                                             extRot,
                                             extRot->getDataEnabled()->getMetaProperty());
      }
      extRot->setEnabled(rotateBox->isChecked());
   }
   //setEnabled (false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn  void VSPluginRotateNodeWithPose::MovingNodePropertyWidget::slotUpdateTranslationNodeUI()
///
/// @brief  Updates the UI if the property of the translation extension changes.
///
/// @author Albrecht
/// @date   31.10.2013
////////////////////////////////////////////////////////////////////////////////////////////////////

void VSPluginRobotLanguageTranslator::MovingNodePropertyWidget::slotUpdateTranslationNodeUI(VSD::SimStateInstance* instance, const VSD::MetaProperty* metaProp)
{
   Q_UNUSED(instance);
   if (extTrans)
   {
     if (!metaProp || metaProp->getName() == "transVelocity")
     {
        velocitySpinBox->blockSignals(true);
        velocitySpinBox->setSIValue(extTrans->getTransVelocity());
        velocitySpinBox->setUnitViaMetaPropertyVal(extTrans->getDataTransVelocity()->getMetaProperty()->toMetaPropertyVal());
        velocitySpinBox->blockSignals(false);
     }
     if (!metaProp || metaProp->getName() == "enabled")
     {
        translateBox->blockSignals(true);
        translateBox->setChecked(extTrans->getEnabled());
        translateBox->blockSignals(false);
     }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn  void VSPluginRotateNodeWithPose::MovingNodePropertyWidget::slotUpdateRotationNodeUI()
///
/// @brief  Updates the UI if the property of the rotation extension changes.
///
/// @author Albrecht
/// @date   31.10.2013
////////////////////////////////////////////////////////////////////////////////////////////////////

void VSPluginRobotLanguageTranslator::MovingNodePropertyWidget::slotUpdateRotationNodeUI(VSD::SimStateInstance* instance, const VSD::MetaProperty* metaProp)
{
   Q_UNUSED(instance);
   if (extRot)
   {
      if (!metaProp || metaProp->getName() == "rotVelocity")
      {
         velocitySpinBox->blockSignals(true);
         velocitySpinBox->setSIValue(extRot->getRotVelocity());
         velocitySpinBox->setUnitViaMetaPropertyVal(extRot->getDataRotVelocity()->getMetaProperty()->toMetaPropertyVal());
         velocitySpinBox->blockSignals(false);
      }
      if (!metaProp || metaProp->getName() == "enabled")
      {
         rotateBox->blockSignals(true);
         rotateBox->setChecked(extRot->getEnabled());
         rotateBox->blockSignals(false);
      }
   }
}
