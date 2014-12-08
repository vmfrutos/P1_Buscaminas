/*
 * Panel.cpp
 *
 *  Created on: 12/10/2014
 *      Author: vmfrutos
 */

#include "Panel.h"


Panel::Panel(int rows,int cols,int numMinas,Ogre::SceneManager* sceneManager) {

	_cols = cols;
	_rows = rows;
	_numMinas = numMinas;
	_sceneManager = sceneManager;
	_nodoPanel = _sceneManager->createSceneNode("PanelNode");
	_sceneManager->getRootSceneNode()->addChild(_nodoPanel);
	_row_selected = 0;
	_col_selected = 0;
	_NumMinasRestantes = _numMinas;
	_NumBaldosasDescubiertas = 0;


	// Se reserva memoria para tabla de baldosas
	_tabla = new Baldosa*[rows];
	for (int i=0;i<rows;i++) {
		_tabla[i] = new Baldosa[cols];
	}

}

Panel::~Panel() {
	// Se eliminan de memoria todas las baldosas de la tabla
	for (int i=0;i<_rows;i++) {
		delete[] _tabla[i];
	}
	delete[] _tabla;
}

void Panel::inicializarPanel() {

	// Se establece la semilla para generar numeros aleatorios
	srand(time(NULL));

	// Se establecen las minas
	for (int i=0;i<_numMinas;i++) {
		bool minaEstablecida = false;
		while (!minaEstablecida) {
			 int fila =rand()%_rows;
			 int colum =rand()%_cols;
			 // Si la posicion obtenida no es ya una mina
			 if (!_tabla[fila][colum].esMina()) {
				 // Se establece mina
				 _tabla[fila][colum].setMina(true);
				minaEstablecida = true;
			 }
		}
	}

	// Se establecen los numeros de cada baldosa en función de las
	// minas de alrederor
	for (int i=0;i<_rows;i++) {
		for (int j=0;j<_cols;j++) {
			int numMinasAlrededor = 0;
			// Si es mina no se hace nada
			if (!_tabla[i][j].esMina())
			{
				// Se recorren las casillas circundantes sumando el numero de minas
				if (i>0 &&_tabla[i-1][j].esMina()) {
					numMinasAlrededor++;
				}
				if (i>0 && j<(_cols-1) &&_tabla[i-1][j+1].esMina()) {
					numMinasAlrededor++;
				}
				if (j<(_cols-1) &&_tabla[i][j+1].esMina()) {
					numMinasAlrededor++;
				}
				if (i<(_rows-1) && j<(_cols-1) &&_tabla[i+1][j+1].esMina()) {
					numMinasAlrededor++;
				}
				if (i<(_rows-1) && _tabla[i+1][j].esMina()) {
					numMinasAlrededor++;
				}
				if (i<(_rows-1) && j>0 &&_tabla[i+1][j-1].esMina()) {
					numMinasAlrededor++;
				}
				if (j>0 &&_tabla[i][j-1].esMina()) {
					numMinasAlrededor++;
				}
				if (i>0 && j>0 &&_tabla[i-1][j-1].esMina()) {
					numMinasAlrededor++;
				}

				// Se establece el numero de minas alrededor de la casilla actual
				_tabla[i][j].setNumMinasAlrededor(numMinasAlrededor);
			}
		}
	}


	// Se crean todos los nodos con las baldosas y se añaden al panel
	for (int i=0;i<_rows;i++) {

		for (int j=0;j<_cols;j++) {
			std::stringstream nombreNodo;
			nombreNodo << "baldosa_" << i << "_" << j;
			Ogre::Entity* ent1 = _sceneManager->createEntity(nombreNodo.str(),"baldosa.mesh");
			Ogre::SceneNode* nodoBaldosa = _sceneManager->createSceneNode(nombreNodo.str());
			nodoBaldosa->attachObject(ent1);
			nodoBaldosa->translate(Ogre::Vector3(((j*2.1)-((2*_cols)/2))+1,((i*2.1)-((2*_rows)/2))+1,0),Ogre::Node::TS_LOCAL);
			ent1->setQueryFlags(BALDOSA);
			_nodoPanel->addChild(nodoBaldosa);
			_tabla[i][j].setNodoBaldosa(nodoBaldosa);
			_tabla[i][j].setName(nombreNodo.str());
			_tabla[i][j].initializeMaterial();
			_tabla[i][j].setFila(i);
			_tabla[i][j].setColumna(j);
			_tabla[i][j].setPanel(this);
		}
	}



	// Se selecciona la actual
	_tabla[_row_selected][_col_selected].selected();
}

void Panel::selectIzquierda(){
	_tabla[_row_selected][_col_selected].unselected();
	if (_col_selected >0) {
		_col_selected--;
	}
	_tabla[_row_selected][_col_selected].selected();
}
void  Panel::selectDerecha(){
	_tabla[_row_selected][_col_selected].unselected();
	if (_col_selected < _cols-1) {
		_col_selected++;
	}
	_tabla[_row_selected][_col_selected].selected();
}
void  Panel::selectArriba(){
	_tabla[_row_selected][_col_selected].unselected();
	if (_row_selected < _rows-1) {
		_row_selected++;
	}
	_tabla[_row_selected][_col_selected].selected();
}
void  Panel::selectAbajo(){
	_tabla[_row_selected][_col_selected].unselected();
	if (_row_selected > 0) {
		_row_selected--;
	}
	_tabla[_row_selected][_col_selected].selected();
}

void  Panel::descubrirBaldosa(){
	_tabla[_row_selected][_col_selected].descubrir();
}

void  Panel::descubrirBaldosa(int row, int col){

	_tabla[_row_selected][_col_selected].unselected();
	_row_selected = row;
	_col_selected = col;
	_tabla[_row_selected][_col_selected].selected();
	_tabla[_row_selected][_col_selected].descubrir();

}
void  Panel::marcarBaldosa(){
	if (!_tabla[_row_selected][_col_selected].estaDescubierta()) {
		bool marcada =_tabla[_row_selected][_col_selected].marcar();
		if (marcada){
			_NumMinasRestantes--;
		} else {
			_NumMinasRestantes++;
		}
	}

}

void  Panel::marcarBaldosa(int row, int col){
	_tabla[_row_selected][_col_selected].unselected();
	_row_selected = row;
	_col_selected = col;
	_tabla[_row_selected][_col_selected].selected();
	marcarBaldosa();
}

Ogre::SceneNode*  Panel::getNode(){
	return _nodoPanel;
}

void Panel::aperturaBackTracking(Baldosa *baldosa) {
	int fila = baldosa->getFila();
	int columna = baldosa->getColumna();
	// Se recorren las casillas circundantes para descubrilas si el valor numerico es 0
	if (fila>0 && !_tabla[fila-1][columna].esMina() && !_tabla[fila-1][columna].estaDescubierta()){
		_tabla[fila-1][columna].descubrirEnBloque();
	}
	if (fila>0 && columna<(_cols-1) && !_tabla[fila-1][columna+1].esMina() && !_tabla[fila-1][columna+1].estaDescubierta()) {
		_tabla[fila-1][columna+1].descubrirEnBloque();
	}
	if (columna<(_cols-1) && !_tabla[fila][columna+1].esMina() && !_tabla[fila][columna+1].estaDescubierta()) {
		_tabla[fila][columna+1].descubrirEnBloque();
	}
	if (fila<(_rows-1) && columna<(_cols-1) && !_tabla[fila+1][columna+1].esMina() && !_tabla[fila+1][columna+1].estaDescubierta()) {
		_tabla[fila+1][columna+1].descubrirEnBloque();
	}
	if (fila<(_rows-1) && !_tabla[fila+1][columna].esMina() && !_tabla[fila+1][columna].estaDescubierta()) {
		_tabla[fila+1][columna].descubrirEnBloque();
	}
	if (fila<(_rows-1) && columna>0 && !_tabla[fila+1][columna-1].esMina() && !_tabla[fila+1][columna-1].estaDescubierta()) {
		_tabla[fila+1][columna-1].descubrirEnBloque();
	}
	if (columna>0 && !_tabla[fila][columna-1].esMina() && !_tabla[fila][columna-1].estaDescubierta()) {
		_tabla[fila][columna-1].descubrirEnBloque();
	}
	if (fila>0 && columna>0 && !_tabla[fila-1][columna-1].esMina() && !_tabla[fila-1][columna-1].estaDescubierta()) {
		_tabla[fila-1][columna-1].descubrirEnBloque();
	}
}

Baldosa* Panel::getBaldosaByName(const string& name){

	for (int i=0;i<_rows;i++) {
		for (int j=0;j<_cols;j++) {
			if (_tabla[i][j].getName() == name){
				return &_tabla[i][j];
			}
		}
	}
	return NULL;
}

int Panel::getNumMinasRestantes(){
	return _NumMinasRestantes;
}
int Panel::getNumBaldosasDescubiertas(){
	return _NumBaldosasDescubiertas;
}

bool Panel::seHaDescubiertoMina() {
	for (int i=0;i<_rows;i++) {
		for (int j=0;j<_cols;j++) {
			if (_tabla[i][j].estaDescubierta() && _tabla[i][j].esMina())
				return true;
		}
	}

	return false;
}

bool Panel::seHaCompletado(){
	for (int i=0;i<_rows;i++) {
		for (int j=0;j<_cols;j++) {
			if (!_tabla[i][j].estaDescubierta() && !_tabla[i][j].esMina())
				return false;
		}
	}

	return true;
}



