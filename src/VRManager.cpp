#include "VRManager.h"

#include <QTimer>
#include <QDir>

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
    
	if (!QDir(manifestDir).exists() || !QDir(manifestDir).exists("vtk_openvr_actions.json")) { 
		emit vrError("Required Files do not exist.");
		return false; 
	} // required files aren't available
	
	m_impl->renderer = vtkSmartPointer<vtkRenderer>::New();

	m_impl->renderWindow = vtkSmartPointer<vtkOpenVRRenderWindow>::New();
	m_impl->renderWindow->AddRenderer(m_impl->renderer);

	m_impl->camera = vtkSmartPointer<vtkOpenVRCamera>::New();
	m_impl->renderer->SetActiveCamera(m_impl->camera);

	m_impl->interactor = vtkSmartPointer<vtkOpenVRRenderWindowInteractor>::New();
	m_impl->interactor->SetRenderWindow(m_impl->renderWindow);
	m_impl->interactor->SetActionManifestDirectory(manifestDir.toStdString());
	m_impl->interactor->SetActionManifestFileName("vtk_openvr_actions.json");

	m_impl->style = vtkSmartPointer<vtkOpenVRInteractorStyle>::New();
	m_impl->interactor->SetInteractorStyle(m_impl->style);

	m_impl->interactor->Initialize();
	if (m_impl->renderWindow->GetHMD() == nullptr) {
		m_impl->interactor->TerminateApp();
		m_impl->style			= nullptr;
		m_impl->interactor		= nullptr;
		m_impl->camera			= nullptr;
		m_impl->renderWindow	= nullptr;
		m_impl->renderer		= nullptr;
		emit vrError("Could not start VR session");
		return false;
	}

	m_impl->active = true;
	m_impl->timer->start();
	emit vrStarted("Started the VR session");
	return true;
}

void VRManager::stop() {
	// make sure the VR app is open
	if (!m_impl->active) {
		return;
	}
	m_impl->timer->stop(); // stopping QT's timer

	clearActors(); // remove all the actors before nullifying the pointers

	// removing all the pointers in reverse order.
	m_impl->interactor->TerminateApp();
	m_impl->style = nullptr;
	m_impl->interactor = nullptr;
	m_impl->camera = nullptr;
	m_impl->renderWindow = nullptr;
	m_impl->renderer = nullptr;

	m_impl->active = false;
	emit vrStopped("Stopped the VR Application");
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

void VRManager::clearActors() {

}