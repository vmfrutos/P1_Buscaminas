#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

IntroState::IntroState() {
	_root = NULL;
	_sceneMgr = NULL;
	_viewport = NULL;
	_camera = NULL;
	_exitMenu = false;
	_sheet = NULL;
	_records = NULL;
}

IntroState::~IntroState(){

}

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManagerIntro");
  _camera = _sceneMgr->createCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
  _exitMenu = false;


  //Sheet
  _sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Sheet");

  //Config Window
  _menu = CEGUI::WindowManager::getSingleton().loadWindowLayout("menu.layout");
  _records = CEGUI::WindowManager::getSingleton().loadWindowLayout("records.layout");


  // Se asocia un manejador a cada boton
  _menu->getChild("Menu/Facil")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::modoFacil,this));
  _menu->getChild("Menu/Medio")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::modoMedio,this));
  _menu->getChild("Menu/Alto")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::modoAlto,this));
  _menu->getChild("Menu/Records")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::mostrarRecords,this));
  _menu->getChild("Menu/Salir")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::salir,this));

  _records->getChild("Records/Volver")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::recordsVolver,this));

  _sheet->addChildWindow(_menu);
  _sheet->addChildWindow(_records);
  CEGUI::System::getSingleton().setGUISheet(_sheet);
  _records->hide();
}

void
IntroState::exit()
{
	_sheet->destroy();
	_sceneMgr->clearScene();
	_root->destroySceneManager(_sceneMgr);
	_root->getAutoCreatedWindow()->removeAllViewports();


}

void
IntroState::pause ()
{
}

void
IntroState::resume ()
{
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitMenu)
    return false;
  
  return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &evt)
{
	CEGUI::System::getSingleton().injectKeyDown(evt.key);
	CEGUI::System::getSingleton().injectChar(evt.text);

}

void
IntroState::keyReleased
(const OIS::KeyEvent &evt )
{
	CEGUI::System::getSingleton().injectKeyUp(evt.key);
	if (evt.key == OIS::KC_ESCAPE) {
		_exitMenu = true;
	}
}

void
IntroState::mouseMoved
(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
}

void
IntroState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
}

IntroState*
IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

bool IntroState::salir(const CEGUI::EventArgs &e) {
	_exitMenu = true;
	return true;
}

bool IntroState::modoFacil(const CEGUI::EventArgs &e) {
	PlayState::getSingletonPtr()->setModoJuego(ModoJuego::facil);
	changeState(PlayState::getSingletonPtr());
	return true;
}

bool IntroState::modoMedio(const CEGUI::EventArgs &e) {
	PlayState::getSingletonPtr()->setModoJuego(ModoJuego::medio);
	changeState(PlayState::getSingletonPtr());
	return true;
}

bool IntroState::modoAlto(const CEGUI::EventArgs &e) {
	PlayState::getSingletonPtr()->setModoJuego(ModoJuego::alto);
	changeState(PlayState::getSingletonPtr());
	return true;
}

bool IntroState::mostrarRecords(const CEGUI::EventArgs &e) {
	_menu->hide();
	_records->show();
	std::ostringstream cad;
	Contador reloj;

	// Se recuperan los records
	vector<Record> recordsFacil = RecordsManager::getSingletonPtr()->getRecords(ModoJuego::facil);
	for (unsigned int i=0; i< recordsFacil.size();i++){
		Record record = recordsFacil[i];
		reloj.reset();
		reloj.incrementar(record.getSegundosTranscurridos());
		cad << record.getNombre() << "  " << reloj.getContadorStr() << endl;
	}
	_records->getChild("Records/Facil")->setText(cad.str());

	cad.str("");
	vector<Record> recordsMedio = RecordsManager::getSingletonPtr()->getRecords(ModoJuego::medio);
	for (unsigned int i=0; i< recordsMedio.size();i++){
			Record record = recordsMedio[i];
			reloj.reset();
			reloj.incrementar(record.getSegundosTranscurridos());
			cad << record.getNombre() << "  " << reloj.getContadorStr() << endl;
	}
	_records->getChild("Records/Medio")->setText(cad.str());

	cad.str("");
	vector<Record> recordsAlto = RecordsManager::getSingletonPtr()->getRecords(ModoJuego::alto);
	for (unsigned int i=0; i< recordsAlto.size();i++){
			Record record = recordsAlto[i];
			reloj.reset();
			reloj.incrementar(record.getSegundosTranscurridos());
			cad << record.getNombre() << "  " << reloj.getContadorStr() << endl;
	}
	_records->getChild("Records/Alto")->setText(cad.str());


	return true;
}

bool IntroState::recordsVolver(const CEGUI::EventArgs &e) {
	_records->hide();
	_menu->show();
	return true;
}


