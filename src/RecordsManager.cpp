#include "RecordsManager.h"

template<> RecordsManager* Ogre::Singleton<RecordsManager>::msSingleton = 0;

RecordsManager::RecordsManager() {
	_maxElementos = 5;
	_recordsFile = "records.txt";
	loadFromFile();
}

RecordsManager::~RecordsManager() {
	saveToFile();
}

RecordsManager* RecordsManager::getSingletonPtr (){
	return msSingleton;
}

RecordsManager& RecordsManager::getSingleton (){
  assert(msSingleton);
  return *msSingleton;
}

void RecordsManager::insertarRecord(Record& record){

	ModoJuego modo = record.getModoJuego();

	switch (modo) {
	case ModoJuego::facil:

		_facil.push_back(record);

		std::sort(_facil.begin(),_facil.end());

		if (_facil.size() > _maxElementos) {
			// Se elimina el ultimo
			_facil.pop_back();
		}
		break;
	case ModoJuego::medio:
		_medio.push_back(record);
		std::sort(_medio.begin(),_medio.end());
		if (_medio.size() > _maxElementos) {
			// Se elimina el ultimo
			_medio.pop_back();
		}
		break;
	case ModoJuego::alto:
		_alto.push_back(record);
		std::sort(_alto.begin(),_alto.end());
		if (_alto.size() > _maxElementos) {
			// Se elimina el ultimo
			_alto.pop_back();
		}
		break;
	}
}

vector<Record> RecordsManager::getRecords(ModoJuego modo) const{
	vector<Record> retorno;
	switch (modo) {
	case ModoJuego::facil:
		retorno = _facil;
		break;
	case ModoJuego::medio:
		retorno = _medio;
		break;
	case ModoJuego::alto:
		retorno = _alto;
		break;
	}
	return retorno;
}

void RecordsManager::loadFromFile(){
	string line;
	ifstream myfile (_recordsFile);
	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			string modo;
			string nombre;
			string tiempo;
			size_t pos = line.find(";",0);
			if (pos == string::npos)
				continue;
			modo = line.substr(0,pos);

			size_t pos2 = line.find(";",pos+1);
			if (pos2 == string::npos)
				continue;
			nombre = line.substr(pos+1,pos2-pos-1);

			tiempo = line.substr(pos2+1);

			Record record;
			if (modo == "facil") {
				record.setModoJuego(ModoJuego::facil);
			} else if (modo == "medio") {
				record.setModoJuego(ModoJuego::medio);
			} else {
				record.setModoJuego(ModoJuego::alto);
			}
			record.setNombre(nombre);
			record.setSegundosTranscurridos(Ogre::StringConverter::parseReal(tiempo));
			insertarRecord(record);
		}
		myfile.close();
	}
}

void RecordsManager::saveToFile(){
	ofstream myfile (_recordsFile,std::ofstream::trunc);

	if (myfile.is_open()){

		for (unsigned int i=0;i<_facil.size();i++){
			myfile << "facil;" << _facil[i].getNombre() << ";" << _facil[i].getSegundosTranscurridos() << endl;
		}

		for (unsigned int i=0;i<_medio.size();i++){
			myfile << "medio;" << _medio[i].getNombre() << ";" << _medio[i].getSegundosTranscurridos() << endl;
		}

		for (unsigned int i=0;i<_alto.size();i++){
			myfile << "alto;" << _alto[i].getNombre() << ";" << _alto[i].getSegundosTranscurridos() << endl;
		}

		myfile.close();
	}
}

