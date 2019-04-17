// header
#include "../VSPluginRobotLanguageTranslatorExplorerPage.h"
using VSPluginRobotLanguageTranslator::ExplorerPage;
#include "../VSPluginRobotLanguageTranslatorSelectionComponentFolder.h"
#include "../VSPluginRobotLanguageTranslatorExtension.h"



// specific
#include "../../VSPluginExplorer/VSPluginExplorerEntry.h"
#include "../../VSPluginExplorer/VSPluginExplorerProject.h"
using VSPluginExplorer::Entry;
#include "../../../Lib/VSD/VSDDatabase.h"
#include <Lib/VSD/VSDConnection.h>
#include "../VSPluginRobotLanguageTranslatorProject.h"
using VSPluginRobotLanguageTranslator::Project;
#include "../../../Main/VEROSIM/VEROSIMProject.h"
#include "../../../Lib/VSD/VSDElementIndex.h"
#include "../../VSLibSelection/VSLibSelectionComponentSimStateInstance.h"
#include "../../VSLibSelection/VSLibSelectionComponentSimStateInstancePartProperty.h"



ExplorerPage::ExplorerPage( Project* thisProject
                          , const QString& displayName
                          , VSD::ElementContainer* container
                          , const VSD::MetaInstance* baseMetaInstance
                          , bool includeBaseMetaInstance)
: VSPluginExplorerVSD::ElementContainerPage(thisProject->getProject(),
                                            displayName,
                                            container)
, thisProject(thisProject)
, baseMetaInstance(baseMetaInstance)
, includeBaseMetaInstance(includeBaseMetaInstance)
, folderEntry(0)
{
   //Gruppe für Datencontainer erstellen
   folderGroup = createPayloadGroup();
   entryGroup = createPayloadGroup();

   VSD::Connection::connect(container, SIGNAL(signalElementHasBeenAdded(VSD::Element*)),
                            this, SLOT(slotElementHasBeenAdded(VSD::Element*)));
   VSD::Connection::connect(container, SIGNAL(signalElementWillBeRemoved(VSD::Element*)),
                            this, SLOT(slotElementWillBeRemoved(VSD::Element*)));

   //Filtern: standardmäßig ist eine Skiplist implementiert (alternative: WhiteList) 
   //überspringe die MetaProperty
   addMetaPropertyToFilter(VSPluginRobotLanguageTranslator::Extension::getMetaPropertyAdditionalNamedVariants()); 

   setStreamingEnabled(false);
}

ExplorerPage::~ExplorerPage()
{
   VSD::Connection::disconnect(container, SIGNAL(signalElementHasBeenAdded(VSD::Element*)),
                               this, SLOT(slotElementHasBeenAdded(VSD::Element*)));
   VSD::Connection::disconnect(container, SIGNAL(signalElementWillBeRemoved(VSD::Element*)),
                               this, SLOT(slotElementWillBeRemoved(VSD::Element*)));
}

void ExplorerPage::clear()
{
   VSPluginExplorerVSD::ElementContainerPage::clear();
   folderEntry = 0;
}

// entry management
QList<Entry*> ExplorerPage::createRootEntries()
{
   QList<Entry*> rootEntries;
   folderEntry = createEntry(Payload(folderGroup), 0);

   rootEntries << folderEntry;

   return rootEntries;
}

Entry* ExplorerPage::createEntry(const Payload& payload, Entry* parent)
{

   // check for special case: entry for childNodes of rootNode
   Payload::Group group = payload.getGroup();

   Entry* newEntry = 0;

   // create Entry for known types
   if (group == folderGroup)
   {
      newEntry = new Entry();
      newEntry->setPayload(payload);
      newEntry->setName(tr("Moving Node Extensions"));
   }
   else //erzeuge immer über Base, damit auch die Properties der Extensions angezeigt werden.. Ausnahmen: Skiplist
   {
      newEntry = VSPluginExplorerVSD::ElementContainerPage::createEntry(payload, parent);
   }
   return newEntry;
}

void ExplorerPage::entryHasBeenInserted(VSPluginExplorer::Entry* entry)
{
   //create all children:
   VSD::Database* db =
      thisProject->getProject()->getEnvironment()->getGuiSimState()->getDatabase();
   VSL_ASSERT_ELSE_RETURN(db);

   VSPluginExplorerVSD::ElementContainerPage::entryHasBeenInserted(entry);
   if(entry == folderEntry)
   {
      VSDForAllElementsInherits(VSPluginRobotLanguageTranslator::Extension*, ext, db)
      {
         createAndInsertEntry(Payload(entryGroup, ext), folderEntry, true);
      }
   }
}

void VSPluginRobotLanguageTranslator::ExplorerPage::slotElementHasBeenAdded(VSD::Element*element)
{
   //ignoriere alle Elemente, die nicht vom gewünschten Typ sind (Kinder werden über die internen Mechanismen erzeugt)
   if (!baseMetaInstance)
   {
      return;
   }
   if (element->getMetaInstance()->inherits(baseMetaInstance->getMetaInstanceIndex()))
   {
      if (folderEntry != 0 && folderEntry->checkedForChildren() && !folderEntry->isZombie())
      {
         createAndInsertEntry(Payload(entryGroup, element), folderEntry, true);
      }
   }
}

//VSLibSelection::Component* VSPluginRobotLanguageTranslator::ExplorerPage::compileEntryComponent(const VSPluginExplorer::Entry*entry)
//{
//   Payload::Group group = entry->getPayload().getGroup();
//   if(group == folderGroup)
//   {
//      return new SelectionComponentFolder(thisProject->getProject()->getEnvironment()->getGuiSimState());
//   }
//   return VSPluginExplorerVSD::ElementContainerPage::compileEntryComponent(entry);
//}


void VSPluginRobotLanguageTranslator::ExplorerPage::slotElementWillBeRemoved(VSD::Element* element)
{
   QList<Entry*> entries = this->getEntriesForInstanceID(element->getMyEnvironmentInstanceId());
   if(entries.isEmpty())
   {
      return;
   }
   removeAndDeleteEntries(entries);
}
