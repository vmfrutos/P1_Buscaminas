#include "Baldosa.h"


Baldosa::Baldosa() {
	_nodoBaldosa = NULL;
	_panel = NULL;
	_mina = false;
	_numMinasAlrededor = 0;
	_name = "";
	_descubierta = false;
	_marcada = false;
	_seleccionada = false;
	_fila = 0;
	_columna = 0;
}

Baldosa::~Baldosa() {

}

bool Baldosa::esMina() {
	return _mina;
}

void Baldosa::setMina(bool valor) {
	this->_mina = valor;
}

bool  Baldosa::estaDescubierta() {
	return _descubierta;
}

int Baldosa::getNumMinasAlrededor() {
	return _numMinasAlrededor;
}

void Baldosa::setNumMinasAlrededor(int num) {
	_numMinasAlrededor = num;
}

void Baldosa::setNodoBaldosa(Ogre::SceneNode* nodoBaldosa) {
	_nodoBaldosa = nodoBaldosa;
}
Ogre::SceneNode*  Baldosa::getNodoBaldosa() {
	return _nodoBaldosa;
}

void Baldosa::setName(const string& name) {
	 _name = name;
}

string& Baldosa::getName() {
	return _name;
}


void Baldosa::setFila(int fila){
	_fila = fila;
}
int Baldosa::getFila(){
	return _fila;
}
void Baldosa::setColumna(int columna){
	_columna = columna;
}
int Baldosa::getColumna(){
	return _columna;
}

void Baldosa::setPanel(Panel *panel){
	_panel = panel;
}

void Baldosa::initializeMaterial() {
	establecerMaterial("Inicial");
}

void Baldosa::selected() {
	if (_marcada) {
		establecerMaterial("SeleccionadoMarcado");
	} else if (_descubierta && esMina()) {
		establecerMaterial("SeleccionadoBomba");
	} else if(_descubierta && !esMina()) {
		int num = getNumMinasAlrededor();
		switch(num) {
		case 0:
			establecerMaterial("Seleccionado");
			break;
		case 1:
			establecerMaterial("Seleccionado1");
			break;
		case 2:
			establecerMaterial("Seleccionado2");
			break;
		case 3:
			establecerMaterial("Seleccionado3");
			break;
		case 4:
			establecerMaterial("Seleccionado4");
			break;
		case 5:
			establecerMaterial("Seleccionado5");
			break;
		case 6:
			establecerMaterial("Seleccionado6");
			break;
		case 7:
			establecerMaterial("Seleccionado7");
			break;
		case 8:
			establecerMaterial("Seleccionado8");
			break;
		}
	} else {
		establecerMaterial("Seleccionado");
	}
	_seleccionada = true;

}

void Baldosa::unselected() {
	if (_marcada) {
		establecerMaterial("SinSeleccionMarcado");
	}else if (_descubierta && esMina()) {
		establecerMaterial("SinSeleccionBomba");
	} else if(_descubierta && !esMina()) {
		int num = getNumMinasAlrededor();
		switch(num) {
		case 0:
			establecerMaterial("SinSeleccion");
			break;
		case 1:
			establecerMaterial("SinSeleccion1");
			break;
		case 2:
			establecerMaterial("SinSeleccion2");
			break;
		case 3:
			establecerMaterial("SinSeleccion3");
			break;
		case 4:
			establecerMaterial("SinSeleccion4");
			break;
		case 5:
			establecerMaterial("SinSeleccion5");
			break;
		case 6:
			establecerMaterial("SinSeleccion6");
			break;
		case 7:
			establecerMaterial("SinSeleccion7");
			break;
		case 8:
			establecerMaterial("SinSeleccion8");
			break;
		}
	} else {
		establecerMaterial("Inicial");
	}
	_seleccionada = false;


}

void Baldosa::descubrir() {
	if (!_descubierta && !_marcada) {
		if (esMina()) {
			establecerMaterial("SeleccionadoBomba");
		} else {
			int num = getNumMinasAlrededor();
			switch(num) {
			case 0:
				//// Caso especial. apertura
				_descubierta = true;
				establecerMaterial("SinSeleccion");
				_panel->aperturaBackTracking(this);
				break;
			case 1:
				establecerMaterial("Seleccionado1");
				break;
			case 2:
				establecerMaterial("Seleccionado2");
				break;
			case 3:
				establecerMaterial("Seleccionado3");
				break;
			case 4:
				establecerMaterial("Seleccionado4");
				break;
			case 5:
				establecerMaterial("Seleccionado5");
				break;
			case 6:
				establecerMaterial("Seleccionado6");
				break;
			case 7:
				establecerMaterial("Seleccionado7");
				break;
			case 8:
				establecerMaterial("Seleccionado8");
				break;
			}
		}
		_descubierta = true;
	}
}

void Baldosa::descubrirEnBloque() {
	if (!_descubierta && !_marcada) {
		if (esMina()) {
			establecerMaterial("SeleccionadoBomba");
		} else {
			int num = getNumMinasAlrededor();
			switch(num) {
			case 0:
				//// Caso especial. apertura
				_descubierta = true;
				establecerMaterial("SinSeleccion");
				_panel->aperturaBackTracking(this);
				break;
			case 1:
				establecerMaterial("SinSeleccion1");
				break;
			case 2:
				establecerMaterial("SinSeleccion2");
				break;
			case 3:
				establecerMaterial("SinSeleccion3");
				break;
			case 4:
				establecerMaterial("SinSeleccion4");
				break;
			case 5:
				establecerMaterial("SinSeleccion5");
				break;
			case 6:
				establecerMaterial("SinSeleccion6");
				break;
			case 7:
				establecerMaterial("SinSeleccion7");
				break;
			case 8:
				establecerMaterial("SinSeleccion8");
				break;
			}
		}
		_descubierta = true;
	}
}

bool Baldosa::marcar() {
	if (!_descubierta) {
		if (_marcada) {
			establecerMaterial("Seleccionado");
			_marcada = false;
		} else {
			establecerMaterial("SeleccionadoMarcado");
			_marcada = true;
		}
	}
	return _marcada;
}


void Baldosa::establecerMaterial(string nombreMat) {
	Ogre::Entity *ent = static_cast <Ogre::Entity *> (_nodoBaldosa->getAttachedObject(_name));
	ent->setMaterialName(nombreMat);

}



