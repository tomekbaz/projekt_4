#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
using namespace std;



*/
string wielkimiLiterami(string S)
{
    for (int i=S.length(); i>=0; i--)
        if (S[i]>='a' && S[i]<='z')   
            S[i] = S[i]-32;   
    return S;
}



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

    strftime(t, D, "%d.%m.%Y", wCzasInfo);    
    strPom = t;
    if (strPom[0]=='0')   
        strPom.erase(0,1);   
    Wynik = Wynik + " " + strPom;

    strftime(t, D, "%H:%M:%S", wCzasInfo);     
    strPom = t;
    if (strPom[0]=='0')  
        strPom.erase(0,1);  
    Wynik = Wynik + " " + strPom;

    return Wynik;
}



int main(int argc, char *argv[])
{
    typedef  unsigned char  bajt;
       

    const string nazwaPlikuLogow = "log.txt";
    ofstream plikLogow;   
    ifstream plik1,plik2;  
    bool OK;   
              
    long N1,N2;   
    string komunikat;



    OK = true;
    cout << endl;


  /

    plikLogow.open(nazwaPlikuLogow.c_str(), ios::out|ios::app);




    plikLogow << zapisAktCzasu() << " :  Uruchomienie programu" << endl;


    

    if (argc!=3)   
    {
        if (argc==1)   
        {
            komunikat = "BLAD! Brak argumentow programu (wymaganych: 2)";
            cout << komunikat << "." << endl;
            plikLogow << zapisAktCzasu() << " :  --- " << komunikat << endl;
        }
        else   
        {
            string czas = zapisAktCzasu();
            string wciecie = string(czas.length()+4, ' ');
            komunikat = "BLAD! Nieprawidlowa liczba argumentow programu";
            cout << komunikat << " (" << argc-1 << ", wymaganych: 2)." << endl;
            plikLogow << czas << " :  --- " << komunikat << endl;
            plikLogow << wciecie <<
                         "    (" << argc-1 << ", wymaganych: 2)" << endl;
        }

        OK = false;
    }



    if (OK)
        if (wielkimiLiterami(argv[1]) == wielkimiLiterami(argv[2]))
         
        {
            komunikat = "BLAD! Nazwy plikow do porownania sa takie same";
            cout << komunikat << "." << endl;
            plikLogow << zapisAktCzasu() << " :  --- " << komunikat << endl;
            OK = false;
        }




    if (OK==true)
    {
        plik1.open(argv[1], ios::in|ios::binary);

        if (plik1.is_open())  
            plikLogow << zapisAktCzasu() << " :  " <<
                         "Otwarcie pierwszego pliku - " <<
                         "\"" << argv[1] << "\"" << endl;
        else   
        {
            komunikat = string("") +
                        "BLAD! Nie dalo sie otworzyc pierwszego " +
                        "pliku - " + "\"" + argv[1] + "\"";

            cout << komunikat << "." << endl;
            plikLogow << zapisAktCzasu() << " :  --- " << komunikat << endl;

            OK = false;
        }
    }




    if (OK==true)
    {
        plik2.open(argv[2], ios::in|ios::binary);

        if (plik2.is_open())
            plikLogow << zapisAktCzasu() << " :  " <<
                         "Otwarcie drugiego pliku - " <<
                         "\"" << argv[2] << "\"" << endl;
        else   
        {
            komunikat = static_cast<string>("") +
                        "BLAD! Nie dalo sie otworzyc drugiego " +
                        "pliku - " + "\"" + argv[2] + "\"";

            cout << komunikat << "." << endl;
            plikLogow << zapisAktCzasu() << " :  --- " << komunikat << endl;

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
            komunikat = "BLAD! Pliki do porownania maja rozna dlugosc";
            cout << komunikat << "." << endl;
            plikLogow << zapisAktCzasu() << " :  --- " << komunikat << endl;
            OK = false;
        }
    }








  

    if (plik1.is_open())
    {
        plik1.close();
        plikLogow << zapisAktCzasu() << " :  " <<
                     "Zamkniecie pierwszego pliku" << endl;
    }
    if (plik2.is_open())
    {
        plik2.close();
        plikLogow << zapisAktCzasu() << " :  " <<
                     "Zamkniecie drugiego pliku" << endl;
    }




    plikLogow << zapisAktCzasu() << " :  Zakonczenie programu" << endl;
    plikLogow << endl;



    plikLogow.close();



    return 0;
}
