#include "VRManager.h"

#include <QTimer>

#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRCamera.h>
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
    vtkSmartPointer<vtkOpenVRCamera> camera;
    QTimer* timer = nullptr;
	bool active = false;
};

VRManager::VRManager(QObject* parent) : QObject(parent), m_impl(std::make_unique<Impl>()) {
    m_impl->timer = new QTimer(this);
    m_impl->timer->setInterval(11);
    connect(m_impl->timer, &QTimer::timeout, this, &VRManager::onRenderTick);
}
VRManager::~VRManager() { stop(); }

bool VRManager::start(const QString& manifestDir) {
    if (m_impl->active) { return true; } // The VR side is already online
    vtkNew<vtkRenderer> renderer;
	vtkNew <vtkOpenVRRenderWindow> window;
    return false;
}

void VRManager::stop() {
	// to be filled in
}

bool VRManager::isActive() const { return m_impl->active; }

void VRManager::onRenderTick(){
    // later
}

void VRManager::addActor(vtkActor* actor) {
	// later
}

void VRManager::removeActor(vtkActor* actor) {
	// later
}
