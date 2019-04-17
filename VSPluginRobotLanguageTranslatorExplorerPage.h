#ifndef VSPluginRobotLanguageTranslatorExplorerPageH
#define VSPluginRobotLanguageTranslatorExplorerPageH

// base
#include "../VSPluginExplorerVSD/VSPluginExplorerVSDElementContainerPage.h"

// specific
#include "../VSPluginExplorer/VSPluginExplorerPayload.h"
using VSPluginExplorer::Payload;


namespace VSD
{
   class Node;
   class MetaInstance;
   class Element;
   class SimStateInstance;
}

namespace VSPluginExplorer
{
   class Entry;
}

namespace VSPluginRobotLanguageTranslator
{
   class Project;
   
   class ExplorerPage : public VSPluginExplorerVSD::ElementContainerPage
   {
      Q_OBJECT
   // construction
   public:
      ExplorerPage( Project* thisProject
                  , const QString& displayName
                  , VSD::ElementContainer* container
                  , const VSD::MetaInstance* baseMetaInstance
                  , bool includeBaseMetaInstance );
      virtual ~ExplorerPage();

      virtual void clear();
      
      // entry management
      virtual QList<VSPluginExplorer::Entry*> createRootEntries();
      virtual VSPluginExplorer::Entry* createEntry(const Payload& payload, VSPluginExplorer::Entry* parent);
      virtual void entryHasBeenInserted(VSPluginExplorer::Entry* entry);

	  //virtual VSLibSelection::Component* compileEntryComponent(const VSPluginExplorer::Entry* entry);
      
      
      // VSD-updates
      
   public slots:
      void slotElementHasBeenAdded(VSD::Element*);
      void slotElementWillBeRemoved(VSD::Element*);


   private:
      Project* thisProject;
      const VSD::MetaInstance* baseMetaInstance;
      bool includeBaseMetaInstance;
      VSPluginExplorer::Payload::Group folderGroup;
      VSPluginExplorer::Payload::Group entryGroup;
      VSPluginExplorer::Entry* folderEntry;


   }; // class ExplorerPage
   
}; // namespace VSPluginRobotLanguageTranslatorTrees

#endif // VSPluginRobotLanguageTranslatorExplorerPageH