#ifndef RECORD_MANAGER_H
#define RECORD_MANAGER_H

#include <Ogre.h>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <fstream>
#include "Record.h"
#include "PlayState.h"

using namespace std;

class Record;
enum class ModoJuego;
class RecordsManager : public Ogre::Singleton<RecordsManager>
{
public:
	RecordsManager();
	~RecordsManager();
	static RecordsManager& getSingleton ();
	static RecordsManager* getSingletonPtr();

	void insertarRecord(Record& record);
	vector<Record> getRecords(ModoJuego modo) const;

private:

	vector<Record> _facil;
	vector<Record> _medio;
	vector<Record> _alto;
	unsigned int _maxElementos;
	string _recordsFile;


	void loadFromFile();
	void saveToFile();



};
#endif
