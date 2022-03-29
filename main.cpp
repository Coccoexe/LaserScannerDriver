/**
 * @file main.cpp
 * @author Alessio Cocco - 1219609 (alessio.cocco@studentiunipd.it)
 * @brief 
 * @date 04-12-2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include"LaserScannerDriver.cpp"
#include <stdlib.h> //rand

using namespace std;

double randomNum()
{
    return rand() % 100 + 1;
}

void get(LaserScannerDriver& l)
{
    try
    {
        vector<double> get1 = l.get_scan();
        for(int i=0; i<180/l.get_resolution(); i++)
            cout << get1[i] << " ";
        cout<<endl;
    }
    catch(exception){}
}

void output(const LaserScannerDriver& l)
{
        cout<<l<<endl;
}

void scan(LaserScannerDriver& l, int x, int n) //add x scan with a vector of n element of number d
{
    for(int i=0; i<x; i++)
    {
        vector<double> v(n);
        for(int i=0;i<n;i++)
            v[i] = randomNum();
        l.new_scan(v);
        l.getBuffer();
    }
}

void distance(const LaserScannerDriver& l)
{
    for(double d=0; d<=180; d=d+l.get_resolution())
            cout<<"Angolo "<<d<<" : "<<l.get_distance(d)<<endl;

}

int main()
{   
    cout<<"Test costruttore default: --------------------------------------------------------------------------------------------------------------"<<endl<<endl;
    //costruttore default
    cout<<"Costruttore di default: (lsdDef)"<<endl;
    LaserScannerDriver lsdDef;
    lsdDef.getBuffer();
    cout << endl;

    cout<<"Test new_scan in lsdDef: "<<endl;
    scan(lsdDef,10,50);
    cout<<endl;

    cout<<"Test operator<< "<<endl;
    output(lsdDef);
    cout<<endl;

    cout<<"Test get_scan in lsdDef: "<<endl; //mando in output solo il primo get_scan per rendere piu' pulita la console, rendo visibile con getBuffer la pulizia del buffer
    cout<<"scansione piu' vecchia: "<<endl;
    get(lsdDef);
    cout<<"buffer: "<<endl;
    lsdDef.getBuffer();
    for(int i=0; i<9; i++)
    {
        lsdDef.get_scan();
        lsdDef.getBuffer();
    }
    cout<<endl<<"Fine test costruttore Default -----------------------------------------------------------------------------------------------------"<<endl<<endl;
    cout<<endl<<"Test costruttore ------------------------------------------------------------------------------------------------------------------"<<endl<<endl;
    //costruttore
    cout<<"Costruttore:"<<endl;
    LaserScannerDriver lsd(0.7);
    lsd.getBuffer();
    cout << endl;

    cout<<"Test new_scan in lsd: "<<endl;
    scan(lsd,10,300);
    cout<<endl;

    cout<<"Test clear_buffer: "<<endl;
    lsd.clear_buffer();
    lsd.getBuffer();
    cout<<endl;

    cout<<"Inserimento parziale nel buffer: "<<endl;
    scan(lsd,5,300);
    lsd.get_scan();
    lsd.getBuffer();
    lsd.get_scan();
    lsd.getBuffer();
    cout<<endl;

    cout<<"Test clear_buffer con primo indice vuoto"<<endl; //mi dava problemi e controllo per sicurezza
    lsd.clear_buffer();
    lsd.getBuffer();
    cout<<endl;
    cout<<"Fine test costruttore ----------------------------------------------------------------------------------------------------------------"<<endl<<endl;

//test get scan (un sacco di valori!!)
    cout<<endl<<"Test get_distance con vettore riempito randomicamente"<<endl;
    LaserScannerDriver test(0.14);
    for(int i=0;i<4;i++)
    {
        vector<double> vct(test.size() + 1);
        for(int i=0;i<vct.size();i++)
            vct[i] = randomNum();
        test.new_scan(vct);

        cout<<"vettore inserito: "<<endl;
        cout<<test<<endl;
    }
    cout<<"Test get distance: "<<endl;
    distance(test);
    cout<<endl<<"Fine test get_distance"<<endl<<endl;


    //riempimento lsd per la copia
    cout<<"Riempimento lsd: "<<endl;
    scan(lsd,10,300);
    cout<<endl;

    //costruttore copia
    cout<<endl<<"Test costruttore copia-------------------------------------------------------------------------------------------------------------"<<endl<<endl;
    cout<<"Test costruttore di copia: copio lsd in lsdCopyCons"<<endl;
    LaserScannerDriver lsdCopyCons(lsd);

    cout<<"Buffer lsd: "<<endl;
    lsd.getBuffer();
    cout<<"Buffer lsdCopyCons: "<<endl;
    lsdCopyCons.getBuffer();
    cout<<endl;

    cout<<"Confronto lsd con copia: "<<endl;
    cout<<"lsd :"<<endl;
    output(lsd);
    cout<<"copia :"<<endl;
    output(lsdCopyCons);
    cout<<endl<<"Fine test costruttore copia -------------------------------------------------------------------------------------------------------"<<endl<<endl;

    //assegnamento copia
    cout<<endl<<"Test assegnamento copia -----------------------------------------------------------------------------------------------------------"<<endl<<endl;
    cout<<"Test assegnamento di copia: copio lsd in lsdCopy"<<endl;
    LaserScannerDriver lsdCopy(1);
    lsdCopy = lsd;

    cout<<"Buffer lsd: "<<endl;
    lsd.getBuffer();
    cout<<"Buffer lsdCopy: "<<endl;
    lsdCopy.getBuffer();
    cout<<endl;

    cout<<"Confronto lsd con copia: "<<endl;
    cout<<"lsd :"<<endl;
    output(lsd);
    cout<<"copia :"<<endl;
    output(lsdCopy);
    cout<<endl<<"Fine test assegnamento copia -----------------------------------------------------------------------------------------------------"<<endl<<endl;

    //costruttore spostamento
    cout<<endl<<"Test costruttore spostamento -----------------------------------------------------------------------------------------------------"<<endl<<endl;
    cout<<"Buffer di lsd: "<<endl;
    lsd.getBuffer();
    cout<<endl;

    cout<<"Test costruttore di spostamento: sposto lsd in lsdAssCons"<<endl;
    LaserScannerDriver lsdAssCons = move(lsd);

    cout<<"Buffer lsd: "<<endl;
    lsd.getBuffer();
    cout<<"Buffer lsdAssCons: "<<endl;
    lsdAssCons.getBuffer();
    cout<<endl<<"Fine test costruttore spostamento ------------------------------------------------------------------------------------------------"<<endl<<endl;

    //assegnamento spostamento
    cout<<endl<<"Test assegnamento spostamento ----------------------------------------------------------------------------------------------------"<<endl<<endl;
    cout<<"Buffer di lsdAssCons: "<<endl;
    lsdAssCons.getBuffer();
    cout<<endl;

    cout<<"Test assegnamento di spostamento: sposto lsdAssCons in lsdAss"<<endl;
    LaserScannerDriver lsdAss(0.5);
    lsdAss = move(lsdAssCons);

    cout<<"Buffer lsdAssCons: "<<endl;
    lsdAssCons.getBuffer();
    cout<<"Buffer lsdAss: "<<endl;
    lsdAss.getBuffer();
    cout<<endl<<"Fine test assegnamento spostamento -----------------------------------------------------------------------------------------------"<<endl<<endl;

    cout<<endl<<"Test eccezioni -------------------------------------------------------------------------------------------------------------------"<<endl<<endl;

    //eccezione costruttore 
    cout<<"Test exception costruttore"<<endl;
    try{LaserScannerDriver(20);}
    catch(exception){}
    cout<<endl;

    //eccezione get_scan
    lsdAss.clear_buffer();
    cout<<"Test excpetion get_scan"<<endl;
    get(lsdAss);   
    cout<<endl; 

    //eccezione get_distance
    cout<<"Test exception get_distance (vuoto)"<<endl;
    try{lsdAss.get_distance(1);}
    catch(exception){}
    cout<<endl;

    scan(lsdAss,1,10);
    cout<<endl<<"Test exception get_distance (angolo sbagliato)"<<endl;
    try{lsdAss.get_distance(200);}
    catch(exception){}
    cout<<endl;

    //eccezione getelementat
    cout<<"Test exception get_element_at (indice sbagliato)"<<endl;
    try{lsdAss.get_element_at(361);}
    catch(exception){}
    lsdAss.clear_buffer();
    cout<<endl<<"Test exception get_element_at (indice sbagliato)"<<endl;
    try{lsdAss.get_element_at(0);}
    catch(exception){}

    cout<<endl<<"Fine test eccezioni -------------------------------------------------------------------------------------------------------------"<<endl<<endl;

    cout<<"fine test\n";
    cout<<" "<<endl;
    cout<<"Alessio Cocco - 1219609 "<<endl;
    return 0;
}
