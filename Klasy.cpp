#include <iostream>
#include "Klasy.h"

void paletka::ruch(paletka& P, poziom& LVL) {
    P.prawo();
    P.lewo();
}
void pilka::ruch(pilka& O, poziom& LVL, paletka& P) {
    O.x += O.dx;
    O.y += O.dy;
    if (O.x <= 1 || O.x >= LVL.szerokosc - 2) {
        O.dx = -O.dx;
    }
    if (O.y <= 0) {
        O.dy = -O.dy;
    }
    if (O.y == P.y - 1 && O.x >= P.x && O.x <= P.x + P.paletka_dlugosc) {
        if (GetAsyncKeyState(VK_LEFT) & 0x8000){
            O.dx = -1;
            O.dy = -O.dy;
        }
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            O.dx = 1;
            O.dy = -O.dy;
        }
        else {
            O.dx = 0;
            O.dy = -O.dy;
        }     

    }

 }
bool pilka::wypadniecie(pilka& O, poziom& LVL) {
    if (O.y >= LVL.wysokosc + 1)
    {
        return true;
    }
    else {
        return false;
    }
}
bool paletka::koniec_poziomu(paletka& P, poziom LVL) {
    if (P.x + P.paletka_dlugosc == LVL.szerokosc || P.x == 0) {
        return true;
    }
    return false;
}
void poziom::inicjalizacja(poziom& LVL, int& nr_poziomu, std::vector<blok>& bloki, blok& B) {
    switch (nr_poziomu) {
    case 1: {
        for (int j = 1; j < 4; j++) {
            for (int i = 2; i < LVL.szerokosc - 2; i++) {
                if (i % 2 != 0 && j == 1 || i % 2 == 0 && j == 2) {
                    B.x = i;
                    B.y = j;
                    bloki.push_back(B);
                }
            }
        }
    }
    case 2: {
        for (int j = 1; j < 4; j++) {
            for (int i = 2; i < LVL.szerokosc - 2; i++) {
                B.x = i;
                B.y = j;
                bloki.push_back(B);
            }
        }
    }
    case 3: {
        for (int j = 1; j < 4; j++) {
            for (int i = 2; i < LVL.szerokosc - 2; i++) {
                B.x = i;
                B.y = j;
                bloki.push_back(B);
            }
        }
    }
    }
}
void gra::wyswietlanie(gra G, paletka P, blok B, pilka O, poziom LVL) {
    system("cls");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pozycja_poczatkowa;
    pozycja_poczatkowa.X = 0;
    pozycja_poczatkowa.Y = 0;
    SetConsoleCursorPosition(hOut, pozycja_poczatkowa);
    G.Wynik_gracza = 0;
    P.x = 5;
    P.y = 9;
    B.x = 0;
    B.y = 3;
    O.x = P.x + P.paletka_dlugosc / 2;
    O.y = P.y - 1;

    // Alokacja pamiêci dla planszy
    char** plansza = new char* [LVL.wysokosc+2];
    for (int y = 0; y < LVL.wysokosc + 2; y++)
    {
        plansza[y] = new char[LVL.szerokosc];
    }

    // Inicjalizacja planszy jako pustej
    for (int y = 0; y < LVL.wysokosc+2; y++)
    {
        for (int x = 0; x < LVL.szerokosc; x++)
        {
            plansza[y][x] = ' ';
        }
    }
    // Inicjalizacja bloków
    std::vector<blok> bloki;
    if (G.poziom_nr == 1) {
        for (int j = 1; j < 4; j++) {
            for (int i = 2; i < LVL.szerokosc - 2; i++) {
                if (i % 2 != 0 && j == 1 || i % 2 == 0 && j == 2) {
                    B.x = i;
                    B.y = j;
                    bloki.push_back(B);
                }
            }
        }
    }
    else if (G.poziom_nr == 2) {
        for (int j = 1; j < 6; j++) {
            for (int i = 2; i < LVL.szerokosc - 2; i++) {
                if (i % 2 != 0 && j% 2 != 0 || i % 2 == 0 && j % 2 == 0) {
                    B.x = i;
                    B.y = j;
                    bloki.push_back(B);
                }
            }
        }
    }
    else if (G.poziom_nr == 3) {
        for (int j = 1; j < 4; j++) {
            for (int i = 2; i < LVL.szerokosc - 2; i++) {
                if (i % 2 != 0 && j == 1 || i % 2 == 0 && j == 2) {
                    B.x = i;
                    B.y = j;
                    bloki.push_back(B);
                }
            }
        }
    }
    
    // G³ówna pêtla gry
    while (true)
    {
            if (O.y >= LVL.wysokosc+1)
            {
                G.Zycia--;
                P.x = 5;
                P.y = 9;
                O.x = P.x + P.paletka_dlugosc / 2;
                O.y = P.y - 1;
                O.dx = 1;
                O.dy = -1;
                Sleep(1000);
            }
            // Tworzenie tymczasowej kopii planszy
            char** plansza_tmp = new char* [LVL.wysokosc + 2];
            for (int y = 0; y < LVL.wysokosc + 2; y++)
            {
                plansza_tmp[y] = new char[LVL.szerokosc];
                memcpy(plansza_tmp[y], plansza[y], LVL.szerokosc);
            }

            for (int y = 0; y < LVL.wysokosc+2; y++)
            {
                for (int x = 0; x < LVL.szerokosc; x++)
                {
                    
                    if (x == 0 || x == LVL.szerokosc - 1 && y<=LVL.wysokosc)
                    {
                        plansza[y][x] = '|'; 
                    }
                    else if (y == P.y && x >= P.x && x < P.x + P.paletka_dlugosc)
                    {
                        plansza[y][x] = P.paletka_linia[x - P.x]; 
                    }
                    else if (y == O.y && x == O.x)
                    {
                        plansza[y][x] = 'O'; 
                    }
                    else if (y == LVL.wysokosc + 1)
                    {
                       if (x > 0 && x <= G.Zycia)
                        {
                            plansza[y][x] = 'X'; 
                        }
                        else if (x > LVL.szerokosc - 5 && x < LVL.szerokosc - 1)
                        {
                           for (int x = LVL.szerokosc - 5; x < LVL.szerokosc - 1; x++)
                           {
                               int* wynik_temp = new int;
                               *wynik_temp = G.Wynik_gracza;
                               if (G.Wynik_gracza == 0) {
                                   plansza[LVL.wysokosc + 1][x] = '-'; 
                               }
                               else {
                                   for (int x = LVL.szerokosc - 5; x < LVL.szerokosc - 1; x++) {
                                       plansza[LVL.wysokosc + 1][x] = '-' + (*wynik_temp % 10);
                                       *wynik_temp = *wynik_temp - *wynik_temp % 10;
                                   }
                                   
                               }
                           }
                        }
                        else
                        {
                            plansza[y][x] = ' '; 
                        }
                    }

                    else
                    {
                        plansza[y][x] = ' '; 
                    }
                    
                }
            }
            // Dodanie paletki do tymczasowej kopii planszy
            for (int x = P.x; x < P.x + P.paletka_dlugosc; x++)
            {
                if (x >= 0 && x < LVL.szerokosc && P.y >= 0 && P.y < LVL.wysokosc)
                {
                    plansza_tmp[P.y][x] = P.paletka_linia[x - P.x];
                }
            }

            // Wyœwietlanie planszy i innych elementów gry
            for (int y = 0; y < LVL.wysokosc+2; y++)
            {
                for (int x = 0; x < LVL.szerokosc; x++)
                {
                    std::cout << plansza_tmp[y][x];
                }
                std::cout << std::endl;
            }
            // Wyœwietlanie bloków
            for (const auto& blok : bloki) {
                plansza[blok.y][blok.x] = blok.blok_linia;
            }

            // Sprawdzanie kolizji pi³ki z blokami
            for (int i = 0; i < bloki.size(); i++) {
                if (O.x >= bloki[i].x && O.x <= bloki[i].x && O.y == bloki[i].y) {
                    int dx_diff = abs(O.x - bloki[i].x);
                    int dy_diff = abs(O.y - bloki[i].y);
                    if (dx_diff > 1 && dy_diff > 1) {
                        O.dx = -O.dx;
                        O.dy = -O.dy;
                        break;
                    }
                    bloki.erase(bloki.begin() + i); 
                     //SprawdŸ, z którego kierunku nast¹pi³o odbicie
                    //if (O.x == bloki[i].x || O.x == bloki[i].x + 1 || O.x == bloki[i].x - 1) {
                    //    // Odbicie od boku bloku
                    //    O.dx = -O.dx;
                    //}
                    //else if (O.y == bloki[i].y || O.x == bloki[i].y + 1 || O.x == bloki[i].y - 1) {
                    //    // Odbicie od góry lub do³u bloku
                    //    O.dy = -O.dy;
                    //}
                    O.dy = -O.dy;
                    G.Wynik_gracza++;
                    break;
                }
            }
            P.ruch(P, LVL);
            O.ruch(O, LVL, P);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 || G.Zycia == 0 || bloki.size() == 0) {
                for (int y = 0; y < LVL.wysokosc; y++)
                {
                    delete[] plansza[y];
                }
                delete[] plansza;
                std::cin.clear();
                G.koniec_gry(G.Wybor, G);
                break;
            }
            for (int y = 0; y < LVL.wysokosc+2; y++)
            {
                delete[] plansza_tmp[y];
            }
            delete[] plansza_tmp;
            SetConsoleCursorPosition(hOut, pozycja_poczatkowa);
            Sleep(100);
            
        }
    }
void gra::menu(int nr_poziomu, gra Gra) {
    std::cin.clear();
    std::cout << "BREAKOUT" << std::endl << "Wybor poziomu (1-3): ";
    std::cin >> Gra.poziom_nr;
    std::cin.clear();
    while (!std::cin || Gra.poziom_nr > 3 || Gra.poziom_nr < 1)
    {
        
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Blad!" << std::endl << "Wybor: " << std::endl;
        std::cin >> Gra.poziom_nr;
    }
    Gra.wyswietlanie(Gra, Gra.Paletka_gracza, Gra.Bloczek, Gra.Pilka_gracza, Gra.Poziom);
    std::cin.clear();
    }
void gra::koniec_gry(int wybor, gra Gra) {
    system("cls");
    std::cin.ignore();
    Gra.Tabelka_gry.wczytajZPliku("tabela_wynikow.txt");
    std::cout << "Koniec gry!" << std::endl;
    Gra.wynik.wynik = Gra.Wynik_gracza;
    if (Gra.Tabelka_gry.czyNowyNajwyzszyWynik(Gra.wynik)) {
        std::cout << "Gratulacje! Nowy najlepszy wynik." << std::endl;
    }
        std::cout << "Podaj nazwe gracza w celu zapisania wyniku: ";
        std::cin >> Gra.wynik.nazwaGracza; 
        Gra.Tabelka_gry.dodajWynik(Gra.wynik);
        Gra.Tabelka_gry.sortujWyniki();
        Gra.Tabelka_gry.zapiszDoPliku("tabela_wynikow.txt");
        Gra.Tabelka_gry.wyswietlWyniki(Gra.Tabelka_gry);
    std::cout << "1- Uruchom ponownie" << std::endl << "2- Wyjdz" << std::endl;
    std::cout << "Wybor: ";
    std::cin >> wybor;
    if (wybor == 1) {
        Gra.menu(Gra.poziom_nr, Gra);
    }
    else {
        return;
    }
    
}