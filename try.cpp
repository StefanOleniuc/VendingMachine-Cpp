#include<iostream>
#include<string>
#include<list>
using namespace std;

class client
{
private:
	string nume, adresa, parola;
public:
	client(string nume, string adresa, string parola)
	{
		this->nume = nume;
		this->adresa = adresa;
		this->parola = parola;
	}
};
//fisier in care le salvez si le scriu
//resetare: 1-2 intrebari cheie la care sa raspunda
//daca nu vtea sa si le puna el,il pun eu by default
//primul ecran: inregistr,autent si resetare 
//window clr...ca sa stearga ecranul->windows.h ->switch( case 1: system("cls"), case 2:...);
//

class suc
{
private:
	string nume, fruct;
protected:
	suc(string nume, string fruct)
	{
		this->nume = nume;
		this->fruct = fruct;
	}
public:
	virtual int tip() = 0;
};

class zahar :private suc
{
private:
	int cantitate;
public:
	zahar(string nume, string fruct, int cantitate) :suc(nume, fruct)
	{
		this->cantitate = cantitate;
	}
	int tip() override
	{
		return 1;
	}
};

class zero_zahar :private suc
{
private:
	string indulcitor;
public:
	zero_zahar(string nume, string fruct, string indulcitor) :suc(nume, fruct)
	{
		this->indulcitor = indulcitor;
	}
	int tip() override
	{
		return 0;
	}
};

class depozit
{
private:
	int capacitate_max;
	list<suc*>sucuri;
public:
	depozit(int capacitate_max)
	{
		this->capacitate_max = capacitate_max;
	}
	void adaugare_suc(suc* s)
	{
		this->sucuri.push_back(s);
	}
};

//nu mai bagam in depozit ce preparam

int main()
{
	int op;
	do
	{
		cout << "0.Iesire" << endl;
		cout << "1.Inregistrare client nou" << endl;
		cout << "2.Autentificare client existent" << endl;
		cout << "3.Afisare sucuri disponibile" << endl;
		cout << "4.Afisare sucuri existente in depozit" << endl;
		cout << "5.Comanda suc" << endl;
		cout << "6.Afisare istoric comenzi" << endl << endl;
		cout << "Dati optiunea dumneavoastra:" << endl; cin >> op;
		switch (op)
		{
		case 0:
			exit(0);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			cout << "Optiune gresita!";
			break;
		}
	} while (1);



	return 0;
}