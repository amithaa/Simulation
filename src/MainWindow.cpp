/// @file MainWindow.cpp
/// @brief Defines the methods to create the user interface

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  // create an openGL format and pass to the new GLWidget
  QGLFormat format;
  format.setVersion(4,1);
  format.setProfile( QGLFormat::CoreProfile);

  m_gl=new GLWindow(format,this);

  m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,2,1);

  /*connect(m_ui->CohesionSlider,SIGNAL(sliderMoved(int)),m_gl,SLOT(setCohesion(int)));
  connect(m_ui->SeparationSlider,SIGNAL(sliderMoved(int)),m_gl,SLOT(setSeparation(int)));
  connect(m_ui->AlignmentSlider,SIGNAL(sliderMoved(int)),m_gl,SLOT(setAlignment(int)));
  connect(m_ui->m_startsimulation,SIGNAL(clicked()),this,SLOT(startSimulation()));
  connect(m_ui->m_exit,SIGNAL(clicked()),this,SLOT(stopSimulation()));
  connect(m_ui->m_suspension,SIGNAL(toggled(bool)),m_gl,SLOT(changeSuspension(bool)));*/
}

MainWindow::~MainWindow()
{
  delete m_ui;
  delete m_gl;
}
