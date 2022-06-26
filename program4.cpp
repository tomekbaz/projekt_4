#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
using namespace std;



typedef  unsigned char  bajt;
    //definicja typu mającego reprezentować (pojedyńczy) bajt


/* deklaracje funkcji (poglądowo) */
string wielkimiLiterami(string S);
string zapisLiczbyZOdstepami(long long a);
string zapisAktCzasu();
int ileRoznicBitow(bajt B1, bajt B2);


/* funkcja zwraca podany łańcuch pisany wielkimi literami;
   zamienia TYLKO (małe) LITERY ANGIELSKIE
*/
string wielkimiLiterami(string S)
{
    for (int i=S.length(); i>=0; i--)
        if (S[i]>='a' && S[i]<='z')   //małą litera
            S[i] = S[i]-32;   //zamień na dużą
    return S;
}


/* funkcja zwraca zapis podanej liczby całkowitej, z odstępami
   "tysięcznymi" - co 3 cyfry od prawej strony
*/
string zapisLiczbyZOdstepami(long long a)
{
    ostringstream ss;
    int i;
    string Wynik;

    ss.str("");   //wyczyść strumień - dla formalności
    if (a>=0)   ss << a;   else   ss << -a;
    Wynik = ss.str();

    i = Wynik.length()-3;
    while (i>0)
    {
        Wynik.insert(i," ");
        i -= 3;
    }

    if (a<0)   Wynik = "-" + Wynik;

    return Wynik;
}


/* funkcja zwraca zapis aktualnego czasu, w postaci (przykładowo):
   Wtorek 7.06.2022 9:05:43
   (dzień miesiąca i godzina bez zbędnego zera, system 24-godzinny)
*/
string zapisAktCzasu()
{
    const int D = 20;
    char t[D];
    time_t czas;
    tm *wCzasInfo;
    string strPom;
    string Wynik;

    czas = time(NULL);
    wCzasInfo = localtime(&czas);

    switch (wCzasInfo->tm_wday)
    {
        case 0:  Wynik = "Niedziela";  break;
        case 1:  Wynik = "Poniedzialek";  break;
        case 2:  Wynik = "Wtorek";  break;
        case 3:  Wynik = "Sroda";  break;
        case 4:  Wynik = "Czwartek";  break;
        case 5:  Wynik = "Piatek";  break;
        case 6:  Wynik = "Sobota";  break;
    }

    strftime(t, D, "%d.%m.%Y", wCzasInfo);     //będzie np.: 01.07.2022
    strPom = t;
    if (strPom[0]=='0')   //dzień miesiąca zaczyna się od cyfry 0 (bo jest <10)
        strPom.erase(0,1);   //usuń pierwszy znak (cyfrę 0)
    Wynik = Wynik + " " + strPom;

    strftime(t, D, "%H:%M:%S", wCzasInfo);     //będzie np.: 09:05:43
    strPom = t;
    if (strPom[0]=='0')   //godzina zaczyna się od cyfry 0 (bo jest <10)
        strPom.erase(0,1);   //usuń pierwszy znak (cyfrę 0)
    Wynik = Wynik + " " + strPom;

    return Wynik;
}


/* funkcja zwraca liczbę różniących się par
   odpowiadających sobie bitów dla dwóch podanych bajtów
*/
int ileRoznicBitow(bajt B1, bajt B2)
{
    bajt X;   //bitowa różnica symetryczna (alternatywa
              //wykluczająca, XOR) dla podanych bajtów
    int wynik;   //wynik funkcji

    X = B1^B2;   //bitowa różnica symetryczna
    wynik = 0;
    while (X>0)
    {
        if (X%2==1)
            wynik++;
        X = X>>1;   //przesunięcie bitowe o 1 pozycję w prawo
    }

    return wynik;
}





int main(int argc, char *argv[])
{
    const string nazwaPlikuLogow = "log.txt";   
    const int D = 1000;   
                          
                         
    ofstream plikLogow;   
    ifstream plik1,plik2;   
    bool OK;   
               
              
    long N1,N2;   
    bajt bufor1[D], bufor2[D];   
                                 
    long long IleBitow;  
    long long IleRoznic;  
    double BER;  
                  
    int ileWczytano;  
                      
    clock_t ileTykniec;  
                         
    double czasPorown;  
                         
    ostringstream ssWiad,ssWiad2;   
                                   
                                    
    string czasAkt;   
                      
    string wciecie;  
    
    int prec;
    int i;



    OK = true;
    cout << endl;


    /* otwarcie pliku logów */

    plikLogow.open(nazwaPlikuLogow.c_str(), ios::out|ios::app);


    /* wpisanie (do pliku logów) wiadomości o uruchomieniu programu */

    plikLogow << zapisAktCzasu() << " :  * Uruchomienie programu *" << endl;


    /* weryfikacja ilości argumentów programu */

    if (argc!=3)   //nieprawidłowa ilość argumentów programu
    {
        if (argc==1)   //czyli brak argumentów
        {
            ssWiad.str("BLAD! Brak argumentow programu (wymagane 2)");
            cout << ssWiad.str() << "." << endl;
            plikLogow << zapisAktCzasu() << " :  " <<
                         "--- " << ssWiad.str() << endl;
        }
        else   //czyli są argumenty, ale nieprawidłowa ilość
        {
            czasAkt = zapisAktCzasu();
            wciecie = string(czasAkt.length()+4, ' ');

            ssWiad.str("BLAD! Nieprawidlowa ilosc argumentow programu");
            ssWiad2.str("");   //wyczyść strumień
            ssWiad2 << "(" << argc-1 << "; wymagane 2)";
            cout << ssWiad.str() << " " << ssWiad2.str() << "." << endl;
            plikLogow << czasAkt << " :  --- " << ssWiad.str() << endl;
            plikLogow << wciecie << "    " << ssWiad2.str() << endl;
        }

        OK = false;
    }


    /* weryfikacja, czy nazwy plików do porównania nie są takie same */

    if (OK)
        if (wielkimiLiterami(argv[1]) == wielkimiLiterami(argv[2]))
                //nazwy plików SĄ takie same
        {
            ssWiad.str("BLAD! Nazwy plikow do porownania sa takie same");
            cout << ssWiad.str() << "." << endl;
            plikLogow << zapisAktCzasu() << " :  " <<
                         "--- " << ssWiad.str() << endl;
            OK = false;
        }


    /* próba otwarcia pierwszego pliku do porównania */

    if (OK==true)
    {
        plik1.open(argv[1], ios::in|ios::binary);

        if (plik1.is_open())   
            plikLogow << zapisAktCzasu() << " :  " <<
                         "Otwarcie pierwszego pliku - " <<
                         "\"" << argv[1] << "\"" << endl;
        else  
        {
            czasAkt = zapisAktCzasu();
            wciecie = string(czasAkt.length()+4, ' ');

            ssWiad.str("BLAD! Nie dalo sie otworzyc");
            ssWiad2.str("");   
            ssWiad2 << "pierwszego pliku - " << "\"" << argv[1] << "\"";
            cout << ssWiad.str() << " " << ssWiad2.str() << "." << endl;
            plikLogow << czasAkt << " :  --- " << ssWiad.str() << endl;
            plikLogow << wciecie << "    " << ssWiad2.str() << endl;

            OK = false;
        }
    }


    /* próba otwarcia drugiego pliku do porównania */

    if (OK==true)
    {
        plik2.open(argv[2], ios::in|ios::binary);

        if (plik2.is_open())  
            plikLogow << zapisAktCzasu() << " :  " <<
                         "Otwarcie drugiego pliku - " <<
                         "\"" << argv[2] << "\"" << endl;
        else   
        {
            czasAkt = zapisAktCzasu();
            wciecie = string(czasAkt.length()+4, ' ');

            ssWiad.str("BLAD! Nie dalo sie otworzyc");
            ssWiad2.str("");  
            ssWiad2 << "drugiego pliku - " << "\"" << argv[2] << "\"";
            cout << ssWiad.str() << " " << ssWiad2.str() << "." << endl;
            plikLogow << czasAkt << " :  --- " << ssWiad.str() << endl;
            plikLogow << wciecie << "    " << ssWiad2.str() << endl;

            OK = false;
        }
    }



    if (OK==true)
    {
        plik1.seekg(0, ios::end);   
        N1 = plik1.tellg();  
        plik1.seekg(0, ios::beg);  
        plik2.seekg(0, ios::end);   
        N2 = plik2.tellg();  
        plik2.seekg(0, ios::beg);   

        if (N1!=N2)   
        {
            cout << "BLAD! Pliki do porownania maja rozna dlugosc." << endl;
            plikLogow << zapisAktCzasu() << " :  " <<
                         "--- BLAD! Pliki maja rozna dlugosc" << endl;
            OK = false;
        }
    }


    /* weryfikacja, czy pliki do porównania nie są puste */

    if (OK==true)
        if (N1==0)   
        {
            cout << "BLAD! Pliki do porownania sa puste." << endl;
            plikLogow << zapisAktCzasu() << " :  " <<
                         "--- BLAD! Pliki sa puste" << endl;
            OK = false;
        }


    /* PORÓWNYWANIE PLIKÓW - wczytywanie danych z plików
       i obliczenia na nich, z pomiarem czasu */

    if (OK==true)
    {
        cout << "Trwa porownywanie plikow... ";
        plikLogow << zapisAktCzasu() << " :  " <<
                     "Rozpoczecie porownywania plikow" << endl;

        ileTykniec = clock();  

        IleBitow = 0;
        IleRoznic = 0;

        plik1.read(reinterpret_cast<char *>(bufor1), D);
        plik2.read(reinterpret_cast<char *>(bufor2), D);
        ileWczytano = plik1.gcount();

        while (ileWczytano > 0)  
        {
            for (i=0; i<ileWczytano; i++)
            {
                IleBitow += 8;   
                IleRoznic += ileRoznicBitow(bufor1[i],bufor2[i]);
                    //zliczaj różniące się bity
            }

            plik1.read(reinterpret_cast<char *>(bufor1), D);
            plik2.read(reinterpret_cast<char *>(bufor2), D);
            ileWczytano = plik1.gcount();
        } 

        BER = static_cast<double>(IleRoznic) /
              static_cast<double>(IleBitow);
            //oblicz Bit Error Rate

        ileTykniec = clock() - ileTykniec;  
        czasPorown = static_cast<double>(ileTykniec) /
                     static_cast<double>(CLOCKS_PER_SEC);
    }


    /* wypisanie / zapisanie wyników obliczeń */

    if (OK==true)
    {
        czasAkt = zapisAktCzasu();
        wciecie = string(czasAkt.length()+4, ' ');

        cout << endl;
        cout << "Zakonczono porownywanie plikow." << endl;
        cout << endl;
        plikLogow << czasAkt << " :  " <<
                     "Zakonczenie porownywania plikow" << endl;

        ssWiad.str("");   
        ssWiad << "Porownanych bitow:  " <<
                  zapisLiczbyZOdstepami(IleBitow);
        cout << ssWiad.str() << endl;
        plikLogow << wciecie << "    " << ssWiad.str() << endl;

        ssWiad.str("");   
        ssWiad << "Roznych bitow:      ";
        ssWiad.width( zapisLiczbyZOdstepami(IleBitow).length() );
        ssWiad << zapisLiczbyZOdstepami(IleRoznic);
        cout << ssWiad.str() << endl;
        plikLogow << wciecie << "    " << ssWiad.str() << endl;

        ssWiad.str("");  
        ssWiad << "Bit error rate (BER):  ";
        prec = ssWiad.precision(9);   
        ssWiad << BER*100 << " %";
        ssWiad.precision(prec);   
        cout << ssWiad.str() << endl;
        plikLogow << wciecie << "    " << ssWiad.str() << endl;

        ssWiad.str("");   
        ssWiad << "Czas porownywania plikow:  ";
        if (czasPorown<1)   
            ssWiad << czasPorown*1000 << " ms";
        else  
            ssWiad << czasPorown << " s";
        cout << ssWiad.str() << endl;
        plikLogow << wciecie << "    " << ssWiad.str() << endl;
    }


   

    if (plik1.is_open())
        if (plik2.is_open())
        {
            plik1.close();
            plik2.close();
            plikLogow << zapisAktCzasu() << " :  " <<
                         "Zamkniecie plikow" << endl;
        }
        else   
        {
            plik1.close();
            plikLogow << zapisAktCzasu() << " :  " <<
                         "Zamkniecie pierwszego pliku" << endl;
        }


   

    plikLogow << zapisAktCzasu() << " :  Zakonczenie programu" << endl;
    plikLogow << endl;


    /* zamknięcie pliku logów */

    plikLogow.close();



    return 0;
}