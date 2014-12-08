#ifndef PANEL_H_
#define PANEL_H_

#include <Ogre.h>
#include <string>
#include "Baldosa.h"
#define BALDOSA 1 << 0  // Mascara para la deteccion de colision con baldosa

class Baldosa;
class Panel {
public:
	Panel(int rows,int cols,int numMinas,Ogre::SceneManager* sceneManager);
	virtual ~Panel();
	void inicializarPanel();
	void selectIzquierda();
	void selectDerecha();
	void selectArriba();
	void selectAbajo();
	void descubrirBaldosa();
	void descubrirBaldosa(int row, int col);
	void marcarBaldosa();
	void marcarBaldosa(int x, int y);
	void aperturaBackTracking(Baldosa *baldosaInicial);
	Baldosa* getBaldosaByName(const std::string& name);
	Ogre::SceneNode* getNode();
	int getNumMinasRestantes();
	int getNumBaldosasDescubiertas();
	bool seHaDescubiertoMina();
	bool seHaCompletado();



private:
	int _rows;
	int _cols;
	int _numMinas;
	int _row_selected;
	int _col_selected;
	int _NumMinasRestantes;
	int _NumBaldosasDescubiertas;
	Ogre::SceneManager* _sceneManager;
	Ogre::SceneNode* _nodoPanel;


public:
	Baldosa** _tabla;


};

#endif /* PANEL_H_ */
