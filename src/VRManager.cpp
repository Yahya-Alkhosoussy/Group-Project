#include "VRManager.h"

#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkOpenVRInteractorStyle.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>

struct VRManager::Impl {
	vtkSmartPointer<vtkOpenVRRenderWindow> renderWindow;
	vtkSmartPointer<vtkOpenVRRenderWindowInteractor> interactor;
	vtkSmartPointer<vtkOpenVRInteractorStyle> style;
	vtkSmartPointer<vtkRenderer> renderer;
	bool active = false;
};

VRManager::VRManager() : m_impl(std::make_unique<Impl>()) {}
VRManager::~VRManager() { stop(); }

bool VRManager::start() {
	// not filled yet
	vtkNew <vtkOpenVRRenderWindow> w;
	return false;
}

void VRManager::stop() {
	// to be filled in
}

bool VRManager::isActive() const { return m_impl->active; }

void VRManager::addActor(vtkActor*) {
	// later
}

void VRManager::removeActor(vtkActor*) {
	// later
}