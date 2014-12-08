#include "PlayState.h"


template<> PlayState*      Ogre::Singleton<PlayState>::msSingleton = 0;

PlayState::PlayState() {
	  _root = NULL;
	  _sceneMgr = NULL;
	  _viewport = NULL;
	  _camera = NULL;
	  _panel = NULL;
	  _exitGame = false;
	  _raySceneQuery = NULL;
	  _win = NULL;
	  _filas = 8;
	  _columnas = 8;
	  _numMinas = 10;
	  _distanciaCamara = 25;
	  _sheet = NULL;
	  _widgetReloj = NULL;
	  _widgetMinasRestantes = NULL;
	  _widgetMensaje = NULL;
	  _widgetNombreUsuarioStatic = NULL;
	  _widgetNombreUsuarioEdit = NULL;
	  _widgetNombreUsuarioSalir = NULL;
	  _finDescubirtaMina = false;
	  _finCompletado = false;
	  _modoJuego = ModoJuego::facil;
}

PlayState::~PlayState (){

}

void
PlayState::enter ()
{
	_root = Ogre::Root::getSingletonPtr();
	_win = _root->getAutoCreatedWindow();

	// Se obtiene el gestor de escena
	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC);

	// Se define una camara
  	_camera = _sceneMgr->createCamera("MainCamera");
	_camera->setPosition(Ogre::Vector3(0,0,_distanciaCamara));
	_camera->lookAt(Ogre::Vector3(0,0,0));
	_camera->setNearClipDistance(15);
	_camera->setFarClipDistance(90);

	_viewport = _win->addViewport(_camera);

	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

	double width = _viewport->getActualWidth();
	double height = _viewport->getActualHeight();
	_camera->setAspectRatio(width / height);
	_exitGame = false;
	createSceneGame();
	_raySceneQuery = _sceneMgr->createRayQuery(Ogre::Ray());


	// Se inicializa el HUD
	_sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");
	CEGUI::Window* hud = CEGUI::WindowManager::getSingleton().loadWindowLayout("hud.layout");

	_widgetReloj = hud->getChild("Hud/Reloj");
	_widgetMinasRestantes = hud->getChild("Hud/MinasRestantes");
	_widgetMensaje = hud->getChild("Hud/MensajeFin");
	_widgetNombreUsuarioStatic = hud->getChild("Hud/NombreUsuarioStatic");
	_widgetNombreUsuarioEdit = hud->getChild("Hud/NombreUsuarioEdit");
	_widgetNombreUsuarioSalir = hud->getChild("Hud/Salir");

	ostringstream minasRestantes;
	minasRestantes << _panel->getNumMinasRestantes();
	_widgetMinasRestantes->setText(minasRestantes.str());
	_sheet->addChildWindow(hud);
	CEGUI::System::getSingleton().setGUISheet(_sheet);

	// se inicilizan las variables y widgets
	_finDescubirtaMina = false;
	_finCompletado = false;
	_reloj.reset();
	_widgetNombreUsuarioStatic->hide();
	_widgetNombreUsuarioEdit->hide();
	_widgetMensaje->hide();

	hud->getChild("Hud/Salir")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&PlayState::_salir,this));

}

void
PlayState::exit ()
{
	if (_panel)
		delete _panel;
	_sheet->destroy();
	_sceneMgr->clearScene();
	_root->destroySceneManager(_sceneMgr);
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void
PlayState::pause()
{
}

void
PlayState::resume()
{
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	// Si la partida no ha finalizado se incrementa el reloj
	if (!_finCompletado && !_finDescubirtaMina) {

		// Se incrememta el reloj
		_reloj.incrementar(evt.timeSinceLastFrame);
		_widgetReloj->setText(_reloj.getContadorStr());
	}


	return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
	  changeState(IntroState::getSingletonPtr());
  
  return true;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &evt)
{
	CEGUI::System::getSingleton().injectKeyDown(evt.key);
	CEGUI::System::getSingleton().injectChar(evt.text);

	// Si ha terminado la partida no se hace nada con las pulsaciones
	if (_finDescubirtaMina || _finCompletado) {
		return;
	}

	if (evt.key == OIS::KC_UP) {
		_panel->selectArriba();
	} else if (evt.key == OIS::KC_DOWN) {
		_panel->selectAbajo();
	} else if (evt.key == OIS::KC_LEFT) {
		_panel->selectIzquierda();
	} else if (evt.key == OIS::KC_RIGHT) {
		_panel->selectDerecha();
	} else if (evt.key == OIS::KC_RETURN) {
		_panel->descubrirBaldosa();
		if (_panel->seHaDescubiertoMina()){
			_finDescubirtaMina = true;
			_widgetMensaje->show();
			_widgetMensaje->setText("Partida Perdida");

		} else if (_panel->seHaCompletado()) {
			_finCompletado = true;
			_widgetNombreUsuarioStatic->show();
			_widgetNombreUsuarioEdit->show();
			_widgetMensaje->show();
			_widgetMensaje->setText("Partida Ganada");
		}
	} else if (evt.key == OIS::KC_SPACE) {
		_panel->marcarBaldosa();
		ostringstream minasRestantes;
		minasRestantes << _panel->getNumMinasRestantes();
		_widgetMinasRestantes->setText(minasRestantes.str());
	}
}

void
PlayState::keyReleased
(const OIS::KeyEvent &evt)
{
	CEGUI::System::getSingleton().injectKeyUp(evt.key);
	if (evt.key == OIS::KC_ESCAPE) {
		salir();
	}


}

void
PlayState::mouseMoved
(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));

	// Si ha terminado la partida no se hace nada con las pulsaciones
	if (_finDescubirtaMina || _finCompletado) {
		return;
	}

	int posx = e.state.X.abs;
	int posy = e.state.Y.abs;
    setRayQuery(posx, posy, BALDOSA);
    Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator it;
    it = result.begin();
    // En este juego solo puede colisionar con un elemento en el eje z
    // por lo tanto solo se itera por el primer elemento
    if (it != result.end()) {
    	Baldosa* baldosa = _panel->getBaldosaByName(it->movable->getParentNode()->getName());
    	if (baldosa == NULL) {
    		return;
    	}
    	int row = baldosa->getFila();
    	int col =  baldosa->getColumna();
    	if (id == OIS::MB_Left){
    		_panel->descubrirBaldosa(row,col);

    		if (_panel->seHaDescubiertoMina()){
    			_finDescubirtaMina = true;
    			_widgetMensaje->show();
    			_widgetMensaje->setText("Partida Perdida");

    		} else if (_panel->seHaCompletado()) {
    			_finCompletado = true;
    			_widgetNombreUsuarioStatic->show();
    			_widgetNombreUsuarioEdit->show();
    			_widgetMensaje->show();
    			_widgetMensaje->setText("Partida Ganada");
    		}
    	}

    	if (id == OIS::MB_Right){
    		_panel->marcarBaldosa(row,col);
    		ostringstream minasRestantes;
    		minasRestantes << _panel->getNumMinasRestantes();
    		_widgetMinasRestantes->setText(minasRestantes.str());
    	}
    }
}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
}

PlayState*
PlayState::getSingletonPtr ()
{
return msSingleton;
}

PlayState&
PlayState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void PlayState::createSceneGame() {

	// Se crea y posiciona una luz direccional para iluminar el panel
	//_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	Ogre::Light* light = _sceneMgr->createLight("Light1");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3(10,10,-50));

	// Se añade la luz al nodo panel
	_sceneMgr->getRootSceneNode()->attachObject(light);
	// Se crea un panel y se inicializa
	_panel = new Panel(_filas,_columnas,_numMinas,_sceneMgr);
	_panel->inicializarPanel();
}

Ogre::Ray PlayState::setRayQuery(int posx, int posy, Ogre::uint32 mask) {



	CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
	Ogre::Ray mouseRay = _camera->getCameraToViewportRay(mousePos.d_x/float(_win->getWidth()), mousePos.d_y/float(_win->getHeight()));
	_raySceneQuery->setRay(mouseRay);
	_raySceneQuery->setSortByDistance(true);
	_raySceneQuery->setQueryMask(mask);
	return mouseRay;
	/*
	Ogre::Ray rayMouse = _camera->getCameraToViewportRay(posx/float(_win->getWidth()), posy/float(_win->getHeight()));
	_raySceneQuery->setRay(rayMouse);
	_raySceneQuery->setSortByDistance(true);
	_raySceneQuery->setQueryMask(mask);
	return rayMouse;
	*/
}

void PlayState::setModoJuego(ModoJuego modo) {

	_modoJuego = modo;
	switch(_modoJuego) {
	case ModoJuego::facil:
		_filas = 8;
		_columnas = 8;
		_numMinas = 10;
		_distanciaCamara = 23;
		break;
	case ModoJuego::medio:
		_filas = 16;
		_columnas = 16;
		_numMinas = 40;
		_distanciaCamara = 46;
		break;
	case ModoJuego::alto:
		_filas = 32;
		_columnas = 32;
		_numMinas = 180;
		_distanciaCamara = 85;
		break;
	}

}

bool PlayState::_salir(const CEGUI::EventArgs &e) {
	salir();
	return true;
}

void PlayState::salir() {

	// Si se ha ganado la partida
	if (_finCompletado) {
		// Se consulta el nombre del usuario
		CEGUI::String cegui_nombre = _widgetNombreUsuarioEdit->getText();
		string nombre = cegui_nombre.c_str();
		if (nombre != "") {
			Record r1;
			r1.setModoJuego(_modoJuego);
			r1.setNombre(nombre);
			r1.setSegundosTranscurridos(_reloj.getSegundosTranscurridos());
			RecordsManager::getSingletonPtr()->insertarRecord(r1);
		}
	}
	_exitGame = true;
}


