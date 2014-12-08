#ifndef BALDOSA_H_
#define BALDOSA_H_

#include <Ogre.h>
#include <string>
#include "Panel.h"

using namespace std;
class Panel;
class Baldosa {
public:
	Baldosa();
	virtual ~Baldosa();

	bool esMina();
	void setMina(bool valor);
	bool estaDescubierta();
	void setNumMinasAlrededor(int num);
	int getNumMinasAlrededor();
	void setNodoBaldosa(Ogre::SceneNode* nodoBaldosa);
	Ogre::SceneNode* getNodoBaldosa();
	void setName(const string& name);
	string& getName();
	void setFila(int fila);
	int getFila();
	void setColumna(int columna);
	int getColumna();
	void setPanel(Panel *panel);

	void selected();
	void initializeMaterial();
	void unselected();
	void descubrir();
	void descubrirEnBloque();
	bool marcar();



private:
	bool _mina;
	bool _descubierta;
	bool _marcada;
	bool _seleccionada;
	int _numMinasAlrededor;
	int _fila;
	int _columna;
	Ogre::SceneNode* _nodoBaldosa;
	string _name;
	Panel *_panel;

	// Metodos Privados
	void establecerMaterial(string nombreMat);


};

#endif /* BALDOSA_H_ */
