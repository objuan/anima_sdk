#include "BulkGUI.h"

void BulkGUIManager::UpdateProgressBar(Anima::ProgressBar * progress)
{
	if(progress)
		StatusSetBar(progress->PercentStep());
}
void BulkGUIManager::CloseProgressBar(Anima::ProgressBar * progress)
{
	StatusClear();
}
