#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include "IntroState.h"
#include "Contador.h"
#include "Panel.h"
#include "RecordsManager.h"
#include "Record.h"

enum class ModoJuego{facil, medio, alto};

class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:
  PlayState ();

  virtual ~PlayState ();

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();


  void setModoJuego(ModoJuego modo);

  bool _salir(const CEGUI::EventArgs &e);
  void salir();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Ogre::RaySceneQuery *_raySceneQuery;
  Panel* _panel;
  Ogre::RenderWindow* _win;
  CEGUI::Window* _sheet;
  CEGUI::Window* _widgetReloj;
  CEGUI::Window* _widgetMinasRestantes;
  CEGUI::Window* _widgetMensaje;
  CEGUI::Window* _widgetNombreUsuarioStatic;
  CEGUI::Window* _widgetNombreUsuarioEdit;
  CEGUI::Window* _widgetNombreUsuarioSalir;



  Contador _reloj;
  bool _exitGame;
  ModoJuego _modoJuego;
  int _filas;
  int _columnas;
  int _numMinas;
  int _distanciaCamara;
  bool _finDescubirtaMina;
  bool _finCompletado;



  void createSceneGame();
  Ogre::Ray setRayQuery(int posx, int posy, Ogre::uint32 mask);

};

#endif
