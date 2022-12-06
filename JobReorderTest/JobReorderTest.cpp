// JobReorderTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

void reorder(string fileName);

int main() {
    int scelta = 0;
    string fileName = "";
    string stemp = "";
    do{
        cout << "File e cartelle presenti nella cartella del programma:" << endl;
        string path = "./";
        for (const auto& entry : filesystem::recursive_directory_iterator(path))
            cout << entry.path().string().substr(2) << endl;

        cout << "\n\nVuoi riordinare i job in: \n[1]Tutta la cartella corrente \n[2]Solo n file\n[3]Istruzioni\n\nScelta: "<<endl;
        stemp.clear();
        getline(cin, stemp);
        scelta = atoi(stemp.c_str());
        
        switch(scelta)
        {
            case 1:
            {   
                for (const auto& entry : filesystem::recursive_directory_iterator(path))
                {
                    fileName = entry.path().string();
                    fileName = fileName.substr(2);
                    reorder(fileName);
                }
            }break;

            case 2:
            {
                cout << "Quanti file vuoi riordinare?\nNumero: ";
                int numeroFile;
                stemp.clear();
                getline(cin, stemp);
                numeroFile = atoi(stemp.c_str());
                string prefix = "Job Config - ";

                for (int i = 0; i < numeroFile; i++)
                {
                    cout << "Inserire nome del file da riordinare" << endl;
                    getline(cin, fileName);
                    fileName = prefix.append(fileName);
                    fileName.append(".ini");

                    reorder(fileName);
                }
            }break;

            case 3:
            {
                cout << ("[Istruzioni]\nPer il corretto funzionamento piazzare questo programma dentro alla " 
                        "cartella nella quale si trovano i file JobConfig da dover riordinare.\n"

                        "Opzione 1: Questa opzione prende tutti i file che si trovano nella stessa cartella del programma "
                        "e gli riordina.\nI file che non iniziano in Job Config o che non terminano in .ini vengono ignorati. "
                        "\nI file .ini dove alla loro apertura c'e stato un errore vengono resi noti, questo include file non testo o cartelle.\n"

                        "Opzione 2: Viene chiesto il numero di file da riordinare, senza prefisso Job Config - o suffisso .ini, "
                        "in quanto vengono messi in automatico. Dopo l'input del numero di file da riordinare"
                        "viene chiesto ogni volta il nome del file da riordinare. "
                        "NOTA: Non importa se il nome del file rispetta le maiuscole/minuscole, ma notare che a fine processo il "
                        "file ordinato segue la le stesse maiuscole/minuscole scritte dall'utente al momento della ricerca\n\n");
            }break;

            default:
            {
                system("CLS");
                cout << "Scelta inesistente" << endl;
            } 
        }
    } while (scelta != 1 && scelta != 2);

    cout << "Programma terminato"<<endl << endl;
    system("pause");

    return 0;
}


void reorder(string fileName)
{
    ifstream inFile;
    ofstream outFile;

    string currLine = "";

    int jobCounter = 0;
    int totalJobNumber = 0;
    bool firstEofReached = false;
    string varFileName = fileName;

    inFile.open(fileName);
    if (!inFile ) 
    {
        varFileName.substr(2);
        cout << "Impossibile aprire file: " << varFileName << endl;
        varFileName = fileName;
        return;
    }
    else if ((varFileName.substr(varFileName.length() - 4) != ".ini")/* || (varFileName.substr(0, 10) != "Job Config")*/)
    {
        if (varFileName.find("Job Config") == string::npos || varFileName.find("xJob Config") != string::npos)
        {
            cout << "File ignorato: " << varFileName << endl;
            return;
        }
    }

    outFile.open(fileName.append("OUT"));

    while (getline(inFile, currLine) || !firstEofReached)
    {
        if (firstEofReached)
        {
            if (currLine.substr(0, 4) == "[JOB" || currLine.substr(0, 4) == "[job" || currLine.substr(0, 4) == "[Job")
            {
                jobCounter++;
                outFile << "[JOB " << jobCounter << "]" << endl;
            }
            else if (currLine.substr(0, 10) == "Numero Job" || 
            currLine.substr(0, 10) == "Numero job" || 
            currLine.substr(0, 10) == "numero Job" || 
            currLine.substr(0, 10) == "numero job")
            {
                outFile << "Numero Job = " << totalJobNumber << endl;
            }
            else
            {
                outFile << currLine << endl;
            }
        }
        else
        {
            if (currLine.substr(0, 4) == "[JOB" || currLine.substr(0, 4) == "[job" || currLine.substr(0, 4) == "[Job")
            {
                totalJobNumber++;
            }
            else if (inFile.eof())
            {
                inFile.clear();
                inFile.seekg(0);
                firstEofReached = true;
            }
        }
    }

    outFile.close();
    inFile.close();

    string finalName = fileName.substr(0, fileName.length() - 3);

    std::remove(finalName.c_str());
    std::rename(fileName.c_str(), finalName.c_str());

    cout << "File riordinato: " << finalName << endl;
}
