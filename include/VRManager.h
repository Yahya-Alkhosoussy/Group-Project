#pragma once

#include <memory>

class vtkActor; // forward declaration to not include QT

class VRManager {
public:
	VRManager(); // constructor
	~VRManager(); // destructor

	bool start();			// create VR session
	void stop();			// turn off VR session
	bool isActive() const;	// See if the VR session is currently active or not

	void addActor(vtkActor* actor);
	void removeActor(vtkActor* actor);

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl; // PImpl: hides VTK/OpenVR types from this header
};