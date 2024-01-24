using namespace std;
#include <fstream>
#include "Family.h"
#include <iostream>
#include <queue>
void setFile(std::fstream& familysData) {
	Family family;
	familysData.open("fd.bin");
	for (int i = 0; i < 100; i++)
	{
		familysData.write((char*)&family, sizeof(Family));
	}
}
void add(std::fstream& familysData) {
    familysData.open("fd.bin", std::ios::in | std::ios::out | std::ios::binary);

    Family family;
    cout << "Enter key family: ";
    cin >> family.keyFamily;

    if (family.keyFamily < 1 || family.keyFamily > 100) {
        throw("Error: Invalid family number");
        familysData.close();
        return;
    }
    Family f;
    familysData.seekg((family.keyFamily - 1) * sizeof(Family));
    familysData.read(reinterpret_cast<char*>(&f), sizeof(Family));

    if (f.keyFamily != 0) {
        throw "ERROR: Family is already in the file";
        familysData.close();
        return;
    }

    cout << "Enter family name: ";
    cin >> family.familyName;
    cout << "Enter count of people in the family: ";
    cin >> family.countChildren;
    cout << "Enter family phone: ";
    cin >> family.phon;

    for (int i = 0; i < 6; i++)
        family.isChug[i] = false;

    familysData.seekp((family.keyFamily - 1) * sizeof(Family));
    familysData.write(reinterpret_cast<const char*>(&family), sizeof(Family));

    familysData.close();
}
void del(std::fstream& familysData, int key) {
    familysData.open("fd.bin", std::ios::in | std::ios::out | std::ios::binary);

    if (key < 1 || key > 100) {
        throw("Error: Invalid family number");
        familysData.close();
        return;
    }

    Family emptyFamily; 
    familysData.seekp((key - 1) * sizeof(Family));
    familysData.write(reinterpret_cast<const char*>(&emptyFamily), sizeof(Family));

    familysData.close();
}
void update(std::fstream& familysData, int key, std::queue<Family>& waitingQueue) {
    familysData.open("fd.bin", std::ios::in | std::ios::out | std::ios::binary);
   
    if (key < 1 || key > 100) {
        familysData.close();
        throw("Error: Invalid family number");
        return;
    }
    Family family;
    bool isWaiting = false;
    familysData.seekp((key - 1) * sizeof(Family));
    familysData.read((char*)&family, sizeof(Family));
    bool arrChug[6];
    if (family.keyFamily == 0) {
        familysData.close();
        throw "ERROR: Family is not in the file";
        return;
    }
    else {
        for (int i = 0; i < 6; i++)
        {
            cout << "enter true/false for chug: " << i + 1 << endl;
            cin >> arrChug[i];
        }
        for (int i = 0; i < 6; i++)
        {
            int count=0;
            if (arrChug[i]) {
                familysData.seekg(0, std::ios::beg);
                Family f;
                for (int j = 0; j < 100; j++) {
                    familysData.read((char*)&f, sizeof(Family));
                    if (f.keyFamily != 0) 
                        if (f.isChug[i] == true)
                            count++;
                }
                if (count < 10) {
                    family.isChug[i] = true;
                    arrChug[i] = false;
                }
                else {
                    family.isChug[i] = false;
                    isWaiting = true;
                }
            }
        }
        familysData.seekp((key - 1) * sizeof(Family));
        familysData.write((char*)&family, sizeof(Family));
        if (isWaiting)
        {
            for (int i = 0; i < 6; i++)
                family.isChug[i] = arrChug[i];
            waitingQueue.push(family);
        }
    }
    familysData.close();
}
void waiting(std::fstream& familysData, std::queue<Family>& waitingQueue) {
    queue<Family> newWaitingQueue;

    while (!waitingQueue.empty()) {
        Family waitingFamily = waitingQueue.front();
        waitingQueue.pop();

        bool isWaiting = false;

        for (int i = 0; i < 6; i++) {
            int count = 0;
            familysData.seekg(0, std::ios::beg);
            Family f;

            for (int j = 0; j < 100; j++) {
                familysData.read(reinterpret_cast<char*>(&f), sizeof(Family));
                if (f.keyFamily != 0 && f.isChug[i] == true)
                    count++;
            }

            if (count < 10) {
                char c;
                cout << "A place has become available in the class you wanted, do you want to register for the class?" << endl;
                cin >> c;

                if (c == 'y' || c == 'Y') 
                    waitingFamily.isChug[i] = false;
                else 
                    isWaiting = true;
            }
            else 
                isWaiting = true;
        }

        if (isWaiting) 
            newWaitingQueue.push(waitingFamily);
    }

    waitingQueue = newWaitingQueue;
    familysData.close();
}
bool rishum(std::fstream& familysData, int key, int numChug) {
    
    if (key < 1 || key > 100) {
        familysData.close();
        throw("Error: Invalid family number");
        return false;
    }

    if (numChug < 1 || numChug > 6) {
        familysData.close();
        throw "ERROR: Invalid class number";
        return false;
    }

    Family family;
    familysData.seekg((key - 1) * sizeof(Family));
    familysData.read(reinterpret_cast<char*>(&family), sizeof(Family));

    if (family.keyFamily == 0) {
        throw "ERROR: Family is not in the file";
        return false;
    }
    familysData.close();
    return family.isChug[numChug - 1];
}
void print(std::fstream& familysData, int key) {
   
    if (key < 1 || key > 100) {
        familysData.close();
        throw("Error: Invalid family number");
        return;
    }

    Family family;
    familysData.seekg((key - 1) * sizeof(Family));
    familysData.read(reinterpret_cast<char*>(&family), sizeof(Family));

    if (family.keyFamily == 0) {
        throw "ERROR: Family is not in the file";
        return;
    }
    cout << "Family Name: " << family.familyName << "\n";
    cout << "Number of People: " << family.countChildren << "\n";
    cout << "Phone Number: " << family.phon << "\n";
    cout << "Chug Registration: ";
    for (int i = 0; i < 6; i++) {
        cout << (family.isChug[i] ? 'Y' : 'N') << ", ";
    }
    cout << endl;
    familysData.close();
}
void inClass(std::fstream& familysData, int numChug) {
   
    if (numChug < 1 || numChug > 6) {
        familysData.close();
        throw "ERROR: Invalid class number";
        return;
    }
    familysData.seekg(0, std::ios::beg);
    Family f;

    for (int j = 0; j < 100; j++) {
        familysData.read(reinterpret_cast<char*>(&f), sizeof(Family));
        if (f.isChug[numChug - 1])
            cout << f.familyName << endl;
    }
    familysData.close();
}
int main() {
        queue<Family> q;
        fstream file;
        file.open("families.txt", ios::binary | ios::in | ios::out);
        if (!file)
        {
            cout << "ERROR: couldn't open file\n";
            return 0;
        }
        setFile(file);
        int choice;
        int snum;
        int cnum;
        cout << "Choices are:\n0 to exit\n1 to add a family\n2 to delete a family\n3 to update rishum to classes\n4 to update waiting to classes \n5 to check rishum for a classas\n6 to print a family\n7 to print all the families that participate in a specific class\n";
        cout << "enter 0-7:\n";
        cin >> choice;
        while (choice)
        {
            switch (choice)
            {
            case 1://add to the file
                try { add(file); }
                catch (const char* msg) { cout << msg; }
                break;
            case 2://delete from file
                cout << "enter number of family to delete:\n";
                cin >> snum;
                try { del(file, snum); }
                catch (const char* msg) { cout << msg; }
                break;
            case 3://update the list of classes of a family
                cout << "enter number of family to update:\n";
                cin >> snum;
                try { update(file, snum, q); }
                catch (const char* msg) {
                    cout << msg;
                }
                break;
            case 4://update the list of classes of a waiting family
                waiting(file, q);
                break;
            case 5://check rishum to a specific class
                cout << "enter number of family to check rishum:\n";
                cin >> snum;
                cout << "enter number of class to check rishum:\n";
                cin >> cnum;
                try
                {
                    cout << "The family is" << (rishum(file, snum, cnum) ? " " : " not ") << "taking the class\n";
                }
                catch (const char* msg) { cout << msg; }
                break;
            case 6://print the details of a specific family
                cout << "enter number of family to print:\n";
                cin >> snum;
                try { print(file, snum); }
                catch (const char* msg) { cout << msg; }
                break;
            case 7://print the details of all the families that are taking a specific class
                cout << "enter number of class to check rishum:\n";
                cin >> cnum;
                try { inClass(file, cnum); }
                catch (const char* msg) { cout << msg; }
                break;
            default:
                cout << "ERROR: invalid choice\n";

            }
            cout << "\nenter 0-7:\n";
            cin >> choice;
        }
        file.close();
        return 0;

    }