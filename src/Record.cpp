#include "Record.h"


Record::Record(){
	_nombre = "";
	_segundosTranscurridos = 0;
	_modoJuego = ModoJuego::facil;
}

Record::Record(const Record& r){
	_modoJuego = r.getModoJuego();
	_nombre = r.getNombre();
	_segundosTranscurridos = r.getSegundosTranscurridos();

}

Record::~Record(){

}

void Record::setNombre(string nombre){
	_nombre = nombre;
}

void Record::setSegundosTranscurridos(Ogre::Real segundosTranscurridos){
	_segundosTranscurridos = segundosTranscurridos;
}

void Record::setModoJuego(ModoJuego modoJuego){
	_modoJuego = modoJuego;
}

string Record::getNombre() const{
	return _nombre;
}

Ogre::Real Record::getSegundosTranscurridos() const{
	return _segundosTranscurridos;
}

ModoJuego Record::getModoJuego() const{
	return _modoJuego;
}

bool Record::operator<(const Record& r) const{
	return this->getSegundosTranscurridos() <  r.getSegundosTranscurridos();

}

Record &Record::operator = (const Record &r){
	_modoJuego = r.getModoJuego();
	_nombre = r.getNombre();
	_segundosTranscurridos = r.getSegundosTranscurridos();
	return(*this);
}





