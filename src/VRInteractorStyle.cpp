#include "VRInteractorStyle.h"
#include "VRManager.h"

#include <vtkObjectFactory.h>
#include <vtkEventData.h>

vtkStandardNewMacro(VRInteractorStyle);

void VRInteractorStyle::OnMenu3D(vtkEventData* edata) {
	// only react on press not release
	auto bd = edata->GetAsEventDataDevice3D();
	if (!bd) return; // no action
	if (bd->GetAction() != vtkEventDataAction::Press) return;

	if (m_manager) {
		m_manager->resetActors();
	}
}