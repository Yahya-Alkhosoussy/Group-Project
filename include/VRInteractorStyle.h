#pragma once

#include <vtkOpenVRInteractorStyle.h>

class VRManager;

/** @file
* This file contains the declarations of the VRInteractorStyle class
*/

/**
 * @brief Custom interaction class for custom VR controls.
 *
 * This class handles any overrides to VTK VR interaction functions
 * in order to customize the controls.
 */
class VRInteractorStyle : public vtkOpenVRInteractorStyle {
public:
	/**
	 * @brief VTK factory method.
	 * @return A new VRInteractorStyle instance owned by VTK's reference counter.
	 */
	static VRInteractorStyle* New();
	vtkTypeMacro(VRInteractorStyle, vtkOpenVRInteractorStyle);

	/**
	* @brief Set the VRManager that this interactor forwards events to.
	* @param manager Non-owning pointer to the manager. Must outlive this object.
	*/
	void setManager(VRManager* manager) { m_manager = manager; }

	/**
	* @brief Handles a press of the VR controller's menu button.
	* @param edata VTK event data describing the controller and button state.
	*/
	void OnMenu3D(vtkEventData* edata) override;

	/**
	* @brief Handles a press of the VR controller's "Right Grip" button.
	* @param edata VTK event data describing the controller and button state.
	*/
	void handleRightGrip(vtkEventData* edata);

	/**
	* @brief Handles a press of the VR controller's "Left Grip" button.
	* @param edata VTK event data describing the controller and button state.
	*/
	void handleLeftGrip(vtkEventData* edata);


protected:
	VRInteractorStyle() = default;
	~VRInteractorStyle() = default;

private:
	VRManager* m_manager = nullptr; ///< A pointer to the VR session
};