#pragma once

#include <memory>
#include <QString>
#include <QObject>

class vtkActor;

class VRManager : public QObject {
    Q_OBJECT
public:
	explicit VRManager(QObject* parent = nullptr); // constructor
	~VRManager(); // destructor

	bool start(const QString& manifestDir);			// create VR session
	void stop();			// turn off VR session
	bool isActive() const;	// See if the VR session is currently active or not

	void addActor(vtkActor* actor);
	void removeActor(vtkActor* actor);
    void clearActors();
	void resetActors();

	void toggleAnimation();
    
signals:
    void vrStarted(QString);
    void vrStopped(QString);
    void vrError(QString);
    
private slots:
    void onRenderTick();

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl; // PImpl: hides VTK/OpenVR types from this header
};
