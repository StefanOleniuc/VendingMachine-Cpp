#include<iostream>
#include<string>
#include<list>
#include<fstream>
#include<windows.h>
using namespace std;

class client
{
private:
	string nume, adresa;
	string parola;
public:
	client(string nume, string adresa, string parola)
	{
		this->nume = nume;
		this->adresa = adresa;
		this->parola = parola;
	}
	string get_nume()
	{
		return this->nume;
	}

	string get_adresa()
	{
		return this->adresa;
	}

	string get_parola()
	{
		return this->parola;
	}
	void set_nume(string newnume)
	{
		this->nume = newnume;
	}
	void set_adresa(string new_adresa)
	{
		this->adresa = new_adresa;
	}
	void set_parola(string new_parola)
	{
		this->parola = new_parola;
	}
	void afisare(ofstream& f)
	{
		f << this->nume << " " << this->adresa << " " << this->parola << endl;
	}
};

client* curent;

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
	virtual void afisare()
	{
		cout << "Numele sucului este: " << this->nume << " ";
		cout << "Fructul de provenienta este: " << this->fruct << " ";
	}

	string get_Suc()
	{
		return this->nume;
	}
	virtual void afisare_fisier(ofstream& f)
	{
		f << this->nume << " " << this->fruct << " " << tip() << " ";
	}
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
	void afisare() override
	{
		suc::afisare();
		cout << "Cantitatea de zahar este " << this->cantitate << endl;
	}
	void afisare_fisier(ofstream& f) override
	{
		suc::afisare_fisier(f);
		f << this->cantitate << " ";
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
	void afisare() override
	{
		suc::afisare();
		cout << "Indulcitorul este: " << this->indulcitor << endl;
	}
	void afisare_fisier(ofstream& f) override
	{
		suc::afisare_fisier(f);
		f << this->indulcitor << " ";
	}
};

class depozit
{
private:
	int capacitate_max;
public:
	static int nr_total;
	list<suc*>sucuri;
	list<int>nr_bucati;
	depozit(int capacitate_max)
	{
		this->capacitate_max = capacitate_max;
	}
	void adaugare_suc(suc* s, int nr)
	{
		bool suc_gasit = false;
		auto k = nr_bucati.begin();
		for (suc* i : sucuri)
		{
			if (i->get_Suc() == s->get_Suc())
			{
				*k += nr;
				suc_gasit = true;
				break;
			}
		}
		if (!suc_gasit && nr_bucati.size() < capacitate_max)
		{
			this->sucuri.push_back(s);
			this->nr_bucati.push_back(nr);
		}
		else
			cout << "Depozitul este plin" << endl;
	}
	void afisare_sucuri()
	{
		auto k = nr_bucati.begin();
		for (suc* s : sucuri)
		{
			s->afisare();
			cout << *k << " bucati";
			++k;
			cout << endl;
		}
	}
	//afisarea in fisier noului depozit in urma comenzii
	void afisare_depozit_fisier(ofstream& f)
	{
		auto k = nr_bucati.begin();
		for (suc* s : sucuri)
		{
			//afisare nume,fruct,tip(cant zahar sau indulcitor)
			s->afisare_fisier(f);
			//afisare numar bucati din lista de nr bucati
			f << *k << endl;
			//incrementam indexul in lista de nr bucati odata cu for-ul
			++k;
		}
	}

	void reduceStocSuc(string nume, int cantitate)
	{
		auto S = sucuri.begin();
		auto Nr = nr_bucati.begin();

		//la sucul respectiv din lista de sucuri ii scad cantitatea
		while (S != sucuri.end() && Nr != nr_bucati.end())
		{
			if ((*S)->get_Suc() == nume)
				*Nr -= cantitate;
			if ((*Nr) <= 0 && this->nr_total > 1)//nr total=nr total de tipuri de sucuri in depozit iar *Nr e nr de bucati pt fiecare suc
			{
				//stergem din liste sucul respectiv
				S = sucuri.erase(S);
				Nr = nr_bucati.erase(Nr);
				this->nr_total--;
				break;
			}
			else
				//reumplerea stocului cu 10 sucuri in cazul in care a mai ramas un tip si ala are 0 bucati dupa livrare
				if (this->nr_total == 1 && (*Nr) <= 0)
				{
					*Nr = capacitate_max;
					//simulam umplerea depozitului
					Sleep(10000);
				}
			++S;
			++Nr;
		}
	}
};

int depozit::nr_total = 0;

ifstream& operator>>(ifstream& fin, suc*& s)
{
	string nume, fruct, indulcitor;
	int cantitate;
	int nr;
	fin >> nume >> fruct >> nr;
	if (nr == 0)
	{
		fin >> indulcitor;
		s = (suc*)new zero_zahar(nume, fruct, indulcitor);
	}
	else
		if (nr == 1)
		{
			fin >> cantitate;
			s = (suc*)new zahar(nume, fruct, cantitate);
		}
	return fin;
}

ifstream& operator>>(ifstream& fin, client*& c)
{
	string nume, adresa, parola;
	fin >> nume >> adresa >> parola;
	c = new client(nume, adresa, parola);
	return fin;
}

void adaugare_client(list<client*>& clienti)
{
	string nume, adresa, parola;
	client* c;
	cout << "Introduceti numele: "; cin >> nume;
	cout << "Introduceti adresa de mail: "; cin >> adresa;
	cout << "Introduceti parola: "; cin >> parola;
	c = new client(nume, adresa, parola);
	clienti.push_back(c);
	curent = c;
}

client* autentificare(list<client*>& clienti, string nume, string adresa)
{
	for (client* c : clienti)
	{
		if (c->get_nume() == nume && c->get_adresa() == adresa)
		{
			return c;
		}
	}
	return 0;
}

client* reautentificare(list<client*>& clienti, string& nume, string& adresa)
{
	client* c = nullptr;
	while (c == nullptr)
	{
		cout << "Nume sau adresa incorecte!\n";
		cout << "Introduceti numele: "; cin >> nume;
		cout << "Introduceti adresa de mail: "; cin >> adresa;
		c = autentificare(clienti, nume, adresa);
	}
	return c;
}

void resetare(client* c)
{
	bool resetare_parola;
	string parola, new_parola;
	cout << "Parola este incorecta!\n";
	cout << "Doriti sa reincercati sau resetati parola?(1-Incerc,0-Resetez)"; cin >> resetare_parola;
	if (resetare_parola)
	{
		cout << "Introduceti din nou parola: "; cin >> parola;
		if (c->get_parola() == parola)
		{
			cout << "Parola a fost introdusa corect! Bine ai revenit la noi, " << c->get_nume() << endl;
		}
		else {
			cout << "Parola a fost introdusa gresit! Trebuie sa resetati parola pentru a intra in cont!\n";
			cout << "Introduceti noua parola: "; cin >> new_parola;
			c->set_parola(new_parola);
			cout << "Parola resetata cu succes!\n";
		}
	}
	else {
		cout << "Introduceti noua parola: "; cin >> new_parola;
		c->set_parola(new_parola);
		cout << "Parola resetata cu succes!\n";
	}
}

void autentificare_client(list<client*>& clienti)
{
	string nume, adresa, parola, new_parola;
	bool modif_parola, resetare_parola;
	client* c = nullptr;
	cout << "Introduceti numele: "; cin >> nume;
	cout << "Introduceti adresa de mail: "; cin >> adresa;
	cout << "Introduceti parola: "; cin >> parola;
	c = autentificare(clienti, nume, adresa);
	if (c != nullptr)
	{
		if (c->get_parola() == parola)
		{
			cout << "Bine ai revenit la noi, " << c->get_nume() << "!\n";
			cout << "Doriti sa modificati parola?(1-DA,0-NU)"; cin >> modif_parola;
			if (modif_parola)
			{
				cout << "Introduceti noua parola: "; cin >> new_parola;
				c->set_parola(new_parola);
				cout << "Parola modificata cu succes!\n";
			}
		}
		else
		{
			resetare(c);

		}
	}
	else {
		c = reautentificare(clienti, nume, adresa);
		if (c != nullptr)
		{
			resetare(c);
		}
	}
	curent = c;
}

void comanda_suc(depozit* d, list<suc*>sucuri_disponibile, list<string>& comandate)//lista de comandate e initializata dupa functie de aceea apare &
{
	auto i = d->sucuri.begin();
	auto j = d->nr_bucati.begin();
	int cantitate_disponibila = 0;
	int timp = 0, cantitate;
	string nume;
	bool gasit = false, suc_corect = false;

	ofstream fout;
	//fisier pentru istoric comenzi deschis cu app pentru a scrie la sfarsit
	fout.open("istoric.txt", ios::app);

	ofstream fout1;
	//fisier pentru erori deschis cu app pentru a scrie la sfarsit
	fout1.open("log.txt", ios::app);

	if (!fout1.is_open())
	{
		cout << "Eroare!";
		exit(0);
	}
	if (!fout.is_open())
	{
		cout << "Eroare!";
		exit(0);
	}
	cout << "Introduceti numele sucului dorit: "; cin >> nume;

	//verificarea numelui sucului in lista din meniu(sucuri disponibile)
	for (suc* s : sucuri_disponibile)
	{
		if (s->get_Suc() == nume)
		{
			suc_corect = true;
			break;
		}
	}

	//aflarea cantitatii sucului introdus corect
	if (suc_corect)
	{
		cout << "Introduceti cantitatea dorita: "; cin >> cantitate;
		while (i != d->sucuri.end() && j != d->nr_bucati.end())
		{
			if ((*i)->get_Suc() == nume)
			{
				cantitate_disponibila = (*j);
				gasit = true;
				break;
			}
			++i;
			++j;
		}

		//dupa aflarea cantitatii sucului in depozit facem ipotezele simularii sucurilor comandate(gasit = gasit in depozit si aflata cantitatea)
		if (gasit)
		{

			//daca nu am destule sucuri in depozit)
			if (cantitate_disponibila < cantitate)
			{
				//simulez crearea celor necesare inmultind nr lor cu 10 secunde
				timp = (cantitate - cantitate_disponibila) * 10;
				cout << "Sucul " << nume << " nu este disponibil in cantitatea dorita. Asteptati " << timp << " secunde pentru simularea crearii sucului!" << endl;
				Sleep(timp * 1000);
				cout << "Va rugam ridicati sucul!" << endl;
				d->reduceStocSuc(nume, cantitate);
			}
			else
				if (cantitate_disponibila >= cantitate)
				{
					cout << "Sucul " << nume << " este disponibil!" << endl;
					cout << "Va rugam ridicati sucul!" << endl;
					//reducem stocul chiar daca acesta este 0
					d->reduceStocSuc(nume, cantitate);
				}
		}
		else
		{
			//sucul nu a fost gasit in lista de tipuri de sucuri din depozit
			timp = cantitate * 10;
			cout << "Sucul " << nume << " nu este disponibil. Asteptati " << timp << " secunde pentru simularea crearii sucului!" << endl;
			Sleep(timp * 1000);
			cout << "Va rugam ridicati sucul!" << endl;
		}
		//memoram intr-o lista de sucuri numele tipului de suc comandat
		comandate.push_back(nume);
	}
	else
	{
		//daca nu a introdus numele corect al sucului aruncam o exceptie
		try
		{
			throw new exception("Optiune gresita!Introduceti un suc din meniu!");
		}
		catch (exception* e)
		{
			cout << e->what() << endl;
			//si afisam in fisierul log.txt aceasta exceptie
			fout1 << e->what() << endl;
		}
	}
	fout.close();
	fout1.close();
}

void istoric(client* curent, list<string>& comandate)
{
	string nume, adresa, numesuc, cantitate;
	ofstream fout;
	fout.open("istoric.txt", ios::app);
	if (!fout)
	{
		cout << "Eroare!";
		exit(0);
	}
	fout << curent->get_adresa() << " ";
	if (!comandate.empty())
	{
		for (auto i = comandate.begin(); i != comandate.end(); i++)
		{
			fout << *i << " ";
		}
	}
	fout << endl;
	fout.close();
}

void cautare_fisier()
{
	ifstream citire_fisier("istoric.txt");
	string adresa, sucuri_citite;
	while (citire_fisier >> adresa)
	{
		getline(citire_fisier, sucuri_citite);
		if (adresa == curent->get_adresa())
		{
			cout << sucuri_citite;
		}
	}
	citire_fisier.close();
}

void comanda_gresita_op(int op1, int op)
{
	ofstream fout;
	fout.open("log.txt", ios::app);
	if (!fout.is_open())
	{
		cout << "Eroare!";
		exit(0);
	}
	try
	{
		if (op1 < 0 || op1>2)
		{
			throw new exception("Optiune gresita! Optiunea trebuie sa fie 0,1 sau 2!");
		}
		if (op < 0 || op>4)
		{
			throw new exception("Optiune gresita! Optiunea trebuie sa fie 0,1,2,3 sau 4!");
		}

	}
	catch (exception* e)
	{
		cout << e->what() << endl;
		fout << e->what() << endl;
		exit(0);
	}
	fout.close();
}

int main()
{
	int op = 0, op1, nr;
	depozit* d;
	ifstream meniu, sucuri_depozit, lista_clienti;
	ofstream fout;
	suc* s;
	list<client*> clienti;
	list<suc*>sucuri_disponibile;
	list<string>comandate;
	client* c;
	d = new depozit(10);
	fout.open("log.txt", ios::app);
	sucuri_depozit.open("depozit.txt", ios::in);
	if (!sucuri_depozit.is_open())
	{
		cout << "Eroare!";
		exit(0);
	}
	while (sucuri_depozit >> s >> nr)
	{
		d->adaugare_suc(s, nr);
		depozit::nr_total++;
	}
	sucuri_depozit.close();
	lista_clienti.open("clienti.txt", ios::in);
	if (!lista_clienti.is_open())
	{
		cout << "Eroare!";
		exit(0);
	}
	while (lista_clienti >> c)
	{
		clienti.push_back(c);
	}
	lista_clienti.close();

	meniu.open("sucuri_disponibile.txt", ios::in);
	if (!meniu.is_open())
	{
		cout << "Eroare!";
		exit(0);
	}
	while (meniu >> s)
	{
		sucuri_disponibile.push_back(s);
	}
	meniu.close();
	if (!fout.is_open())
	{
		cout << "Eroare!";
		exit(0);
	}
	cout << "0.Iesire" << endl;
	cout << "1.Inregistrare client nou" << endl;
	cout << "2.Autentificare client existent" << endl;
	cout << "Dati optiunea: " << endl;
	try
	{
		cin >> op1;
		if (cin.fail())
		{
			throw new exception("Optiune gresita!Optiunea trebuie sa fie de tipul int!");
		}
	}
	catch (exception* e)
	{
		cout << e->what() << endl;
		fout << e->what() << endl;
		exit(0);
	}

	switch (op1)
	{
	case 0:
		exit(0);
		break;
	case 1:
		adaugare_client(clienti);
		cout << "Contul a fost creat cu succes!" << endl;
		break;
	case 2:
		autentificare_client(clienti);
		break;
	default:
		comanda_gresita_op(op1, op);
		break;
	}
	Sleep(1000);
	system("cls");
	ofstream fout_clienti, fout_depozit;
	fout_clienti.open("clienti.txt", ios::out);
	if (!fout_clienti.is_open())
	{
		cout << "Eroare!";
		exit(0);
	}
	for (client* c : clienti)
	{
		c->afisare(fout_clienti);
	}
	fout_clienti.close();
	do {
		cout << endl;
		cout << "0.Iesire" << endl;
		cout << "1.Afisare sucuri disponibile" << endl;
		cout << "2.Afisare sucuri existente in depozit" << endl;
		cout << "3.Comanda suc" << endl;
		cout << "4.Afisare istoric comenzi" << endl << endl;
		cout << "Dati optiunea dumneavoastra:" << endl;
		try
		{
			cin >> op;
			if (cin.fail())
			{
				throw new exception("Optiune gresita!Optiunea trebuie sa fie de tipul int!");
			}
		}
		catch (exception* e)
		{
			cout << e->what() << endl;
			fout << e->what() << endl;
			exit(0);
		}
		switch (op)
		{
		case 0:
			istoric(curent, comandate);
			exit(0);
			break;
		case 1:
			for (suc* s : sucuri_disponibile)
			{
				s->afisare();
			}
			break;
		case 2:
			d->afisare_sucuri();
			break;
		case 3:
			comanda_suc(d, sucuri_disponibile, comandate);
			//cu ajutorul celor doua liste de sucuri si nr bucati din clasa depozit voi rescrie de fiecare data fisierul depozit.txt
			fout_depozit.open("depozit.txt", ios::out);
			if (!fout_depozit.is_open())
			{
				cout << "Eroare!";
				exit(0);
			}
			d->afisare_depozit_fisier(fout_depozit);
			fout_depozit.close();
			break;

		case 4:
			cout << " " << curent->get_nume() << " " << curent->get_adresa() << endl;
			cout << " "<<"Sucurile comandate sunt: " << endl;
			cautare_fisier();
			if (!comandate.empty())
			{
				for (auto i = comandate.begin(); i != comandate.end(); i++)
				{
					cout <<" " << *i << " ";
				}
			}
			cout << endl;
			break;
		default:
			istoric(curent, comandate);
			comanda_gresita_op(op1, op);
			break;
		}
	} while (1);
	fout.close();
	return 0;
}