
// header
#include "../VSPluginRobotLanguageTranslatorSelectionComponentFolder.h"



VSPluginRobotLanguageTranslator::SelectionComponentFolder::SelectionComponentFolder(VSD::SimState* simState)
: VSLibSelection::Component(simState)   
{
} // SelectionComponentFolder()



VSPluginRobotLanguageTranslator::SelectionComponentFolder::~SelectionComponentFolder()
{
} // ~SelectionComponentFolder()



QString VSPluginRobotLanguageTranslator::SelectionComponentFolder::compileType() const
{
   return "Folder";
} // compileType()



QString VSPluginRobotLanguageTranslator::SelectionComponentFolder::compileDescription() const
{
   return (QObject::tr("A folder containing objects."));
} // compileDescription()



QStringList VSPluginRobotLanguageTranslator::SelectionComponentFolder::getKey() const
{
	return QStringList();
} // getKey()



bool VSPluginRobotLanguageTranslator::SelectionComponentFolder::restoreFromKey(VSD::SimState* simState, const QStringList& key)
{
	Q_UNUSED(key);
	Q_UNUSED(simState);

	return false;
} // restoreFromKey()