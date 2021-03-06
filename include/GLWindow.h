/// @file GLWindow.h
/// @brief a basic Qt GL window class for ngl demos
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/10/10
/// Revision History :
/// Initial Version 10/10/10 (Binary day ;-0 )
/// @class GLWindow
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file

#ifndef _GL_WINDOW_H__
#define _GL_WINDOW_H__

#include <ngl/Camera.h>
#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include <QEvent>
#include <QResizeEvent>
#include <QGLWidget>
#include <QtDebug>
#include <ngl/Text.h>
#include "Flock.h"

class GLWindow : public QGLWidget
{
  Q_OBJECT        // must include this if you use Qt signals/slots
  public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for GLWindow
    /// @param[in] _timer the time value for simulation updates
    /// @param [in] _parent the parent window to create the GL context in
    //----------------------------------------------------------------------------------------------------------------------
    GLWindow(const QGLFormat _format,QWidget *_parent );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor
    //----------------------------------------------------------------------------------------------------------------------
    ~GLWindow();


  public slots :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief slot to set the cohesion force
    //----------------------------------------------------------------------------------------------------------------------
    /*inline void setCohesion(double c) { m_cohesion = 0.1 * c; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief slot to set the separation force
    //----------------------------------------------------------------------------------------------------------------------
    inline void setSeparation(double s) { m_separation = 0.1 * s; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief slot to set the alignment
    //----------------------------------------------------------------------------------------------------------------------
    inline void setAlignment(double a) { m_alignment = 0.1 * a; }*/


	private :
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief Our Camera
		//----------------------------------------------------------------------------------------------------------------------
		ngl::Camera *m_cam;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief the timer value in ms
		//----------------------------------------------------------------------------------------------------------------------
		int m_flockTimer;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief used to store the x rotation mouse value
		//----------------------------------------------------------------------------------------------------------------------
		int m_spinXFace;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief used to store the y rotation mouse value
		//----------------------------------------------------------------------------------------------------------------------
		int m_spinYFace;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief flag to indicate if the mouse button is pressed when dragging
		//----------------------------------------------------------------------------------------------------------------------
		bool m_rotate;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief flag to indicate if the Right mouse button is pressed when dragging
		//----------------------------------------------------------------------------------------------------------------------
		bool m_translate;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief the previous x mouse value for Position changes
		//----------------------------------------------------------------------------------------------------------------------
		int m_origXPos;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief the previous y mouse value for Position changes
		//----------------------------------------------------------------------------------------------------------------------
		int m_origYPos;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief the previous x mouse value
		//----------------------------------------------------------------------------------------------------------------------
		int m_origX;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief the previous y mouse value
		//----------------------------------------------------------------------------------------------------------------------
		int m_origY;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief used to store the global mouse transforms
		//----------------------------------------------------------------------------------------------------------------------
		ngl::Mat4 m_mouseGlobalTX;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief the model position for mouse movement
		//----------------------------------------------------------------------------------------------------------------------
		ngl::Vec3 m_modelPos;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief the pointer to the flock
		//----------------------------------------------------------------------------------------------------------------------
		Flock *m_flock;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief the constant for the cohesion force
		//----------------------------------------------------------------------------------------------------------------------
		double m_cohesion;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief the constant for the separation force
		//----------------------------------------------------------------------------------------------------------------------
		double m_separation;
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief the constant for the alignment force
		//----------------------------------------------------------------------------------------------------------------------
		double m_alignment;

	protected:

		//----------------------------------------------------------------------------------------------------------------------
		/// @brief  The following methods must be implimented in the sub class
		/// this is called when the window is created
		//----------------------------------------------------------------------------------------------------------------------
		void initializeGL();
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief this is called whenever the window is re-sized
		/// @param[in] _w the width of the resized window
		/// @param[in] _h the height of the resized window
		//----------------------------------------------------------------------------------------------------------------------
		void resizeGL(const int _w, const int _h );
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief this is the main gl drawing routine which is called whenever the window needs to
		/// be re-drawn
		//----------------------------------------------------------------------------------------------------------------------
		void paintGL();
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief our transformation stack used for drawing
		//----------------------------------------------------------------------------------------------------------------------
		ngl::Transformation m_transform;

	private :
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief this method is called every time a mouse is moved
		/// @param _event the Qt Event structure
		//----------------------------------------------------------------------------------------------------------------------
		void mouseMoveEvent (QMouseEvent * _event   );
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief this method is called everytime the mouse button is pressed
		/// inherited from QObject and overridden here.
		/// @param _event the Qt Event structure
		//----------------------------------------------------------------------------------------------------------------------
		void mousePressEvent ( QMouseEvent *_event  );
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief this method is called everytime the mouse button is released
		/// inherited from QObject and overridden here.
		/// @param _event the Qt Event structure
		//----------------------------------------------------------------------------------------------------------------------
		void mouseReleaseEvent (QMouseEvent *_event );
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief this method is called everytime the mouse wheel is moved
		/// inherited from QObject and overridden here.
		/// @param _event the Qt Event structure
		//----------------------------------------------------------------------------------------------------------------------
		void wheelEvent( QWheelEvent *_event);
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief Qt Event called when a key is pressed
		/// @param [in] _event the Qt event to query for size etc
		//----------------------------------------------------------------------------------------------------------------------
		void keyPressEvent(QKeyEvent *_event);
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief build our VAO
		//----------------------------------------------------------------------------------------------------------------------
		void buildVAO();
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief timer event
		//----------------------------------------------------------------------------------------------------------------------
		void timerEvent(QTimerEvent *);
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief bounding box
		//----------------------------------------------------------------------------------------------------------------------
		void drawBoundingBox();

		void loadMatricesToShader();


};

#endif
