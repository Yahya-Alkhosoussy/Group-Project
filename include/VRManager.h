#pragma once

#include <memory>
#include <QString>
#include <QObject>


/** @file
* This file contains the declarations of the VRInteractorStyle class
*/

class vtkActor;

/**
* @brief The Main manager for the VR section of the project
* 
* Handles everything related to the VR like loading the STL files into VR,
* rendering objects in VR, rendering a skybox in VR, model management and 
* the VTK VR display.
*/
class VRManager : public QObject {
    Q_OBJECT
public:
	/**
	* VRManager constructor
	* - Sets up the VR Impl struct
	* - Activates a QT timer for rendering frames
	*/
	explicit VRManager(QObject* parent = nullptr); // constructor
	/**
	* VRManager destructor
	* - Releases all of the pointers that were held in the Impl struct
	*/
	~VRManager(); // destructor

	/**
	* Handles the start of the VR rendering
	* - Sets up a renderer
	* - Sets up a render window
	* - Sets up an interactor
	* - Initialises the actors and puts them on the render window
	* - Sets up the skybox
	*/
	bool start(const QString& manifestDir);			// create VR session
	/**
	* Handles the end of the VR session
	* - Stops the QT timer
	* - clears all the Actors
	* - Terminate the interactor
	* - set all smart pointers to a null pointer
	*/
	void stop();			// turn off VR session
	/**
	* Gets whether or not the VR session is active or not
	* @return the boolean value of if the VR session is active
	*/
	bool isActive() const;	// See if the VR session is currently active or not

	/**
	* Adds an actor to the VR Render
	* @param actor is the actor to be added into the VR Render
	*/
	void addActor(vtkActor* actor);
	/**
	* Removes an actor from the VR Render
	* @param actor is the actor to be removed from the VR Render
	*/
	void removeActor(vtkActor* actor);
	/**
	* Removes all the actors from the VR Render
	*/
    void clearActors();
	/**
	* Resets the positions of the actors
	*/
	void resetActors();
	/**
	* Toggles the animation value in the struct
	*/
	void toggleAnimation();
    
signals:
    void vrStarted(QString);
    void vrStopped(QString);
    void vrError(QString);
    
private slots:
	/**
	* This function is responsible to what happens in every frame
	*/
    void onRenderTick();

private:
	/**
	* @brief Hides VTK and OpenVR implementation details from the header.
	*
	* PImpl idiom — keeps heavy VTK/OpenVR includes out of VRManager.h
	* so consumers of the public API don't pull them in transitively.
	*/
	struct Impl;
	std::unique_ptr<Impl> m_impl; // PImpl: hides VTK/OpenVR types from this header
};
