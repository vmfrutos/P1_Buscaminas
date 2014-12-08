#ifndef _RECORD_H
#define _RECORD_H

#include <string>
#include "PlayState.h"
#include "Ogre.h"
using namespace std;

enum class ModoJuego;

class Record {


public:

	Record();
	Record(const Record& r);
	~Record();

	void setNombre(string nombre);
	void setSegundosTranscurridos(Ogre::Real  segundosTranscurridos);
	void setModoJuego(ModoJuego modo);

	string getNombre()  const;
	Ogre::Real  getSegundosTranscurridos()  const;
	ModoJuego getModoJuego()  const;
	bool operator<(const Record& r) const;
	Record& operator=(const Record &r);

private:
	string _nombre;
	Ogre::Real _segundosTranscurridos;
	ModoJuego _modoJuego;
};
#endif
