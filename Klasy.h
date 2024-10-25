#pragma once
#ifndef KLASY_H
#define KLASY_H
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <time.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

class element_gry{
public:
    int x;
    int y;
    int dx;
    int dy;
    void virtual lewo() {
        dx = -1;
        return;
    }
    void virtual prawo() {
        dx = 1;
        return;
    }
    void virtual gora() {
        dy = -1;
        return;
    }
    void virtual dol() {
        dy = 1;
        return;
    }
    element_gry() {
        x = 0;
        y = 0;
        dx = 1;
        dy = -1;
    }

};
class blok :public element_gry {
public:
    int dlugosc_bloczka;
    char blok_linia;
    bool stan_bloczka;
    blok() {
        dlugosc_bloczka = 1;
        blok_linia = '#';
        stan_bloczka = true;
    }
};
class poziom {
public:
     int wysokosc;
     int szerokosc;
     void inicjalizacja(poziom& LVL, int& nr_poziomu, std::vector<blok>& bloki, blok& B);
    poziom() {
        wysokosc = 10;
        szerokosc = 15;
    }
};
class paletka :public element_gry {
public:
    std::string paletka_linia;
    int paletka_dlugosc{};
    int predkosc_paletki;

    paletka& operator++(int) {
        paletka_dlugosc++;
        paletka_linia.push_back('=');
        return *this;
    };
    paletka& operator--(int) {
        paletka_dlugosc--;
        paletka_linia.pop_back();
        return *this;
    }
    void lewo() {
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
        {
            if (x - 1 >= 1)
                x--;
        }
    }
    void prawo() {
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
        {
            if (x + 6 < 14) {
                x++;
            }
        }
    }
    void ruch(paletka&, poziom&);
    bool koniec_poziomu(paletka&, poziom);
    paletka()
        : paletka_linia("======"), paletka_dlugosc(6), predkosc_paletki(0)
    {
    }
    ~paletka() {
    }
};
class pilka :public element_gry{
public:
    std::string cialo_pileczki;
    int dx;
    int dy;
    void ruch(pilka&, poziom&, paletka&);
    bool wypadniecie(pilka&, poziom&);
    pilka() {
        cialo_pileczki = "O";
        dx = 1;
        dy = -1;
    }
};

class Wynik {
public:
    std::string nazwaGracza;
    int wynik;
    Wynik* next;
    Wynik(const std::string& nazwa, int wynik) : nazwaGracza(nazwa), wynik(wynik), next(nullptr) {}
    Wynik() {
        nazwaGracza = " ";
        wynik = 0;
    }
};
class tabela_wynikow {
private:
    Wynik* head;

public:
    tabela_wynikow() : head(nullptr) {}

    ~tabela_wynikow() {
        while (head) {
            Wynik* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void dodajWynik(const Wynik& wynik) {
        Wynik* nowyWynik = new Wynik(wynik.nazwaGracza, wynik.wynik);
        if (!head) {
            head = nowyWynik;
        }
        else {
            Wynik* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = nowyWynik;
        }
    }
   friend std::ostream& operator<<(std::ostream& wyj, tabela_wynikow& t) {
       wyj << "Tabela wynikow:" << std::endl;
       while (t.head) {
           std::cout << "Gracz: " << t.head->nazwaGracza << ", Wynik: " << t.head->wynik << "\n";
           t.head = t.head->next;
       }
       return wyj;
    }
    void wyswietlWyniki(tabela_wynikow& tab) const {
        std::cout << tab;
    }
    void sortujWyniki() {
        if (!head || !head->next) {
            return;
        }

        bool zamieniono = true;
        while (zamieniono) {
            zamieniono = false;
            Wynik* obecny = head;
            Wynik* poprzedni = nullptr;

            while (obecny->next) {
                if (obecny->wynik < obecny->next->wynik) {
                    Wynik* nastepny = obecny->next;
                    obecny->next = nastepny->next;
                    nastepny->next = obecny;

                    if (poprzedni) {
                        poprzedni->next = nastepny;
                    }
                    else {
                        head = nastepny;
                    }

                    poprzedni = nastepny;
                    zamieniono = true;
                }
                else {
                    poprzedni = obecny;
                    obecny = obecny->next;
                }
            }
        }
    }
    void zapiszDoPliku(const std::string& nazwaPliku) const {
        std::ofstream plik(nazwaPliku);
        if (!plik) {
            std::cerr << "Nie mozna otworzyc pliku do zapisu.\n";
            return;
        }

        Wynik* temp = head;
        while (temp) {
            plik << temp->nazwaGracza << "," << temp->wynik << "\n";
            temp = temp->next;
        }

        plik.close();
        std::cout << "Zapisano wyniki do pliku: " << nazwaPliku << "\n";
    }

    void wczytajZPliku(const std::string& nazwaPliku) {
        std::ifstream plik(nazwaPliku);
        if (!plik) {
            std::cerr << "Nie mozna otworzyc pliku do odczytu.\n";
            return;
        }

        while (head) {
            Wynik* temp = head;
            head = head->next;
            delete temp;
        }

        std::string linia;
        while (std::getline(plik, linia)) {
            std::string nazwa;
            int wynik;
            std::size_t pozycja = linia.find(',');
            if (pozycja != std::string::npos) {
                nazwa = linia.substr(0, pozycja);
                wynik = std::stoi(linia.substr(pozycja + 1));
                dodajWynik(Wynik(nazwa, wynik));
            }
        }

        plik.close();
        std::cout << "Wczytano wyniki z pliku: " << nazwaPliku << "\n";
    }
    bool czyNowyNajwyzszyWynik(const Wynik& wynik) const {
        if (!head || wynik.wynik > head->wynik) {
            return true;
        }
        return false;
    }
};

class gra {
public:
    Wynik wynik;
	paletka Paletka_gracza;
	pilka Pilka_gracza;
	int Wynik_gracza;
	tabela_wynikow Tabelka_gry;
    poziom Poziom;
    int Wybor;
    int poziom_nr;
    int Zycia;
    blok Bloczek;
	void wyswietlanie(gra, paletka, blok, pilka, poziom);
    void menu(int, gra);
    void koniec_gry(int, gra);



    gra() {
        Wybor = 0;
        Wynik_gracza = 0;
        poziom_nr = 0;
        Zycia = 3;
    }
};


#endif 