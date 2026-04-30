#pragma once

#include <vtkOpenVRInteractorStyle.h>

class VRManager;

class VRInteractorStyle : public vtkOpenVRInteractorStyle {
public:
	static VRInteractorStyle* New();
	vtkTypeMacro(VRInteractorStyle, vtkOpenVRInteractorStyle);

	void setManager(VRManager* manager) { m_manager = manager; }

	void OnMenu3D(vtkEventData* edata) override;

protected:
	VRInteractorStyle() = default;
	~VRInteractorStyle() = default;

private:
	VRManager* m_manager = nullptr;
};