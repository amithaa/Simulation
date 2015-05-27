/// \file MainWindow.h
/// \brief contains attributes and methods for the user interface
/// \author Amitha Arun
/// \date 29/01/2015

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QMainWindow>
#include "GLWindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
		Ui::MainWindow *m_ui;
		/// @brief our openGL widget
		GLWindow *m_gl;


private slots:

		//----------------------------------------------------------------------------------------------------------------------
		/// @brief a slot to reset simulation
		//----------------------------------------------------------------------------------------------------------------------
//		inline void startSimulation(){m_gl->initCloth();}
		//----------------------------------------------------------------------------------------------------------------------
		/// @brief a slot to stop simulation
		//----------------------------------------------------------------------------------------------------------------------
	//	inline void stopSimulation(){exit(0);}
};

#endif // MAINWINDOW_H
