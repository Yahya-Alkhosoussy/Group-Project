#include "VRInteractorStyle.h"
#include "VRManager.h"

#include <vtkObjectFactory.h>
#include <vtkEventData.h>

/** @file
* This file contains the definitions of the VRInteractorStyle class
*/

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

void VRInteractorStyle::handleRightGrip(vtkEventData* edata) {
	auto* edd = edata->GetAsEventDataDevice3D();
	if (!edd) return;
	// make sure it's a press and from the right controller
	if (edd->GetAction() != vtkEventDataAction::Press) return;
	if (edd->GetDevice() != vtkEventDataDevice::RightController) return;

	if (m_manager) {
		m_manager->toggleAnimation();
	}

}

void VRInteractorStyle::handleLeftGrip(vtkEventData* edata) {
	auto* edd = edata->GetAsEventDataDevice3D();
	if (!edd) return;
	// make sure it's a press and from the right controller
	if (edd->GetAction() != vtkEventDataAction::Press) return;
	if (edd->GetDevice() != vtkEventDataDevice::LeftController) return;

	if (m_manager) {
		m_manager->toggleAnimation();
	}

}