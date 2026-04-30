#include "VRManager.h"
#include "VRInteractorStyle.h"

#include <set>

#include <QTimer>
#include <QDir>
#include <QCoreApplication>

#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRCamera.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkOpenVRInteractorStyle.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkSkybox.h>
#include <vtkTexture.h>
#include <vtkHDRReader.h>
#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <vtkImageShiftScale.h>

struct VRManager::Impl {
	vtkSmartPointer<vtkOpenVRRenderWindow> renderWindow;
	vtkSmartPointer<vtkOpenVRRenderWindowInteractor> interactor;
	vtkSmartPointer<vtkOpenVRInteractorStyle> style;
	vtkSmartPointer<vtkOpenVRRenderer> renderer;
    vtkSmartPointer<vtkOpenVRCamera> camera;
	vtkSmartPointer<vtkSkybox> skybox;
	std::set<vtkActor*> initialisedActors;
    QTimer* timer = nullptr;
	bool active = false;
	bool animating = false;
};

VRManager::VRManager(QObject* parent) : QObject(parent), m_impl(std::make_unique<Impl>()) {
    m_impl->timer = new QTimer(this);
    m_impl->timer->setInterval(11);
    connect(m_impl->timer, &QTimer::timeout, this, &VRManager::onRenderTick);
}
VRManager::~VRManager() { stop(); }

bool VRManager::start(const QString& manifestDir) {

	qDebug() << "Manifest dir:" << manifestDir;
	qDebug() << "Actions JSON exists:"
		<< QDir(manifestDir).exists("vtk_openvr_actions.json");
	qDebug() << "HP binding exists:"
		<< QDir(manifestDir).exists("vtk_openvr_binding_hpmotioncontroller.json");
	qDebug() << "Knuckles binding exists:"
		<< QDir(manifestDir).exists("vtk_openvr_binding_knuckles.json");
	qDebug() << "Oculus binding exists:"
		<< QDir(manifestDir).exists("vtk_openvr_binding_oculus_touch.json");
	qDebug() << "Vive binding exists:"
		<< QDir(manifestDir).exists("vtk_openvr_binding_vive_controller.json");

    if (m_impl->active) { return true; } // The VR side is already online
    
	if (!QDir(manifestDir).exists() || !QDir(manifestDir).exists("vtk_openvr_actions.json")) { 
		emit vrError("Required Files do not exist.");
		return false; 
	} // required files aren't available
	
	m_impl->renderer = vtkSmartPointer<vtkOpenVRRenderer>::New();;
	m_impl->renderer->SetBackground(0.1, 0.1, 0.15);

	m_impl->renderWindow = vtkSmartPointer<vtkOpenVRRenderWindow>::New();
	m_impl->renderWindow->AddRenderer(m_impl->renderer);

	m_impl->camera = vtkSmartPointer<vtkOpenVRCamera>::New();
	m_impl->renderer->SetActiveCamera(m_impl->camera);

	m_impl->interactor = vtkSmartPointer<vtkOpenVRRenderWindowInteractor>::New();
	m_impl->interactor->SetRenderWindow(m_impl->renderWindow);
	
	m_impl->renderWindow->Initialize();

	auto myStyle = vtkSmartPointer<VRInteractorStyle>::New();
	myStyle->setManager(this);
	m_impl->style = myStyle;
	m_impl->interactor->SetInteractorStyle(m_impl->style);

	if (m_impl->renderWindow->GetHMD() == nullptr) {
		m_impl->interactor->TerminateApp();
		m_impl->style			= nullptr;
		m_impl->interactor		= nullptr;
		m_impl->camera			= nullptr;
		m_impl->renderWindow	= nullptr;
		m_impl->renderer		= nullptr;
		m_impl->skybox			= nullptr;
		m_impl->initialisedActors.clear();
		emit vrError("Could not start VR session");
		return false;
	}
	m_impl->interactor->SetActionManifestDirectory(manifestDir.toStdString());
	m_impl->interactor->SetActionManifestFileName("vtk_openvr_actions.json");

	m_impl->skybox = vtkSmartPointer<vtkSkybox>::New();
	auto reader = vtkSmartPointer<vtkHDRReader>::New();

	QString skyboxPath = QCoreApplication::applicationDirPath() + "/../assets/background.hdr";
	qDebug() << "Skybox path:" << skyboxPath;
	qDebug() << "Skybox exists:" << QFile::exists(skyboxPath);
	reader->SetFileName(skyboxPath.toStdString().c_str());
	reader->Update();

	auto img = reader->GetOutput();
	qDebug() << "Image dimensions:"
		<< img->GetDimensions()[0] << "x"
		<< img->GetDimensions()[1];
	qDebug() << "Number of components:"
		<< img->GetNumberOfScalarComponents();
	qDebug() << "Scalar type:"
		<< img->GetScalarTypeAsString();

	// shift is for the lighting and brightness
	auto shift = vtkSmartPointer<vtkImageShiftScale>::New();
	shift->SetInputConnection(reader->GetOutputPort());
	shift->SetScale(25.0);
	shift->ClampOverflowOn();
	shift->Update();

	// cast forces the texture to work with HDR's float values properly
	auto cast = vtkSmartPointer<vtkImageCast>::New();
	cast->SetInputConnection(shift->GetOutputPort());
	cast->SetOutputScalarTypeToUnsignedChar();
	cast->Update();

	auto texture = vtkSmartPointer<vtkTexture>::New();
	texture->SetInputConnection(cast->GetOutputPort());
	texture->MipmapOn();
	texture->InterpolateOn();
	texture->UseSRGBColorSpaceOff();
	texture->Update();

	m_impl->skybox->SetProjection(vtkSkybox::Sphere);
	m_impl->skybox->SetTexture(texture);
	m_impl->renderer->AddActor(m_impl->skybox);


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
	m_impl->skybox = nullptr;
	m_impl->initialisedActors.clear();

	m_impl->active = false;
	emit vrStopped("Stopped the VR Application");
}

bool VRManager::isActive() const { return m_impl->active; }

void VRManager::onRenderTick(){
	if (!m_impl->active) {
		return;
	}
	// check if any of the pointers are null
	if (m_impl->style == nullptr || m_impl->interactor == nullptr ||
		m_impl->camera == nullptr || m_impl->renderWindow == nullptr ||
		m_impl->renderer == nullptr) {
		return;
	}
	if (m_impl->animating) {
		double x_angle = 0.0;
		double y_angle = 0.0;
		double z_angle = 0.0;

		auto actors = m_impl->renderer->GetActors();
		actors->InitTraversal();
		vtkActor* actor = actors->GetNextActor();
		
		while (actor != nullptr) {
			if (actor == m_impl->skybox.GetPointer()) {
				actor = actors->GetNextActor();
				continue; // leave the skybox untouched
			}

			actor->RotateZ(0.1);
			actor = actors->GetNextActor();
		}
	}

	m_impl->interactor->DoOneEvent(m_impl->renderWindow, m_impl->renderer);
	
}

void VRManager::addActor(vtkActor* actor) {
	if (!m_impl->active || m_impl->renderer == nullptr || actor == nullptr) {
		emit vrError("Could not add actor.");
		return;
	}
	if (m_impl->initialisedActors.find(actor) == m_impl->initialisedActors.end()) {
		actor->SetPosition(0, 0.5, -3.0);
		actor->SetScale(0.01);
		actor->SetOrientation(270.0, 0.0, 0.0);
		m_impl->initialisedActors.insert(actor);
	}
	m_impl->renderer->AddActor(actor);
}

void VRManager::removeActor(vtkActor* actor) {
	if (m_impl->active && m_impl->renderer != nullptr && actor != nullptr) {
		m_impl->renderer->RemoveActor(actor);
	}
}

void VRManager::clearActors() {
	if (!m_impl->active || m_impl->renderer == nullptr) return;

	m_impl->renderer->RemoveAllViewProps();
	m_impl->renderer->AddActor(m_impl->skybox);
}

void VRManager::resetActors() {
	auto actors = m_impl->renderer->GetActors();
	actors->InitTraversal();
	vtkActor* actor = actors->GetNextActor();
	while (actor != nullptr) {
		if (actor == m_impl->skybox.GetPointer()) {
			actor = actors->GetNextActor();
			continue; // leave the skybox untouched
		}
		actor->SetPosition(0, 0.5, -3.0);
		actor->SetScale(0.01);
		actor->SetOrientation(270.0, 0.0, 0.0);
		actor = actors->GetNextActor();
	}
}

void VRManager::toggleAnimation() {
	m_impl->animating = !m_impl->animating; // the not inverts it no matter the state.
}