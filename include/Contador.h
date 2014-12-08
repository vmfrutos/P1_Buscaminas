#ifndef _CONTADOR_H
#define _CONTADOR_H

#include <string>
#include <Ogre.h>

using namespace std;

class Contador {
public:
	Contador();
	~Contador();
	void incrementar(Ogre::Real segundosTranscurridos);
	int getHoras();
	int getMinutos();
	int getSegundos();
	string getContadorStr();
	void reset();
	Ogre::Real getSegundosTranscurridos();

private:
	Ogre::Real _segundosTranscurridos;
	int _horas;
	int _minutos;
	int _segundos;



};

#endif
