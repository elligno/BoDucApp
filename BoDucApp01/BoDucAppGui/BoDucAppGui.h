#pragma once

// QT include
#include <QtWidgets/QWidget>
// app  include
#include "ui_BoDucAppGui.h"
#include "BoDucApp.h"

// Qt forward declarations
class QGridLayout;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QStringList;
class QFileDialog;
class QTextEdit;
class QDir;

class BoDucAppGui : public QWidget
{
	Q_OBJECT
public:
	BoDucAppGui( QWidget *parent = Q_NULLPTR);
private slots:
	void parseCmd();        // process according to reader
	void OpenCsvFile();     // set file name 
	void saveCmdToFile();   //
	void loadCmdFromFile(); //
protected:
	// create the layout of GUI
	virtual void createGUI();
private:
	Ui::BoDucAppGuiClass ui;
// 	QPushButton* m_startButton;
// 	QPushButton* m_loadButton;
// 	QPushButton* m_saveButton;
// 	QString m_cmdFileName;
// 	QDir* m_appDir; // set t BoDucApp folder as root
	std::vector<std::string> m_vecStr;
	// some of the widget in the layout
	QPushButton* m_open;
	QPushButton* m_exit;
	QPushButton* m_process;
	QPushButton* m_load;
	QPushButton* m_save;
	QGridLayout* m_layout;
	QTextEdit* m_console;
	QHBoxLayout* m_rowLayout;
	QVBoxLayout* m_columnLayout[5];
	QStringList* m_buttonTitle;
	int m_spacinVal;
	//	QFileDialog* m_openFile;
	QString m_fileName;        // name of the csv file 
	QStringList m_filesName;   // list of all csv files name
	int m_numButtons;
	QString m_bonLivraison;
	// Application to parse BoDuc command
	bdGui::BoDucApp m_bdApp;
	// helper method
//	QGridLayout* createGridGroupBox();
	void connectButtons();
	// create horizontal buttons
	void createHBoxButtons();
	// ...
	void createDataReport();
//	QTextStream m_fileDataReport;
	QString m_bonLivraisonFile;
	unsigned short m_fileLoaded;
};
