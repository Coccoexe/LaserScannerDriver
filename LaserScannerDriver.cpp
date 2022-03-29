/**
 * @file LaserScannerDriver.cpp
 * @author Alessio Cocco - 1219609 (alessio.cocco@studentiunipd.it)
 * @brief 
 * @date 28-11-2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include"LaserScannerDriver.h"
#include<math.h>    //round in get_distance

LaserScannerDriver::LaserScannerDriver()
{
    angle_resolution = 1;                           //set angle_resolution a 1, valore di default
    to_insert = 0;                                  //set to_insert a 0, condizione iniziale
    to_remove = -1;                                 //set to_remove a -1, condizione iniziale (buffer vuoto)
    buffer = new double*[BUFFER_DIM]{nullptr};
}

LaserScannerDriver::LaserScannerDriver(const double& ang_res)
{
    if(ang_res < 0.1 || ang_res > 1)                                    //eccezione se risoluzione non accettabile
    {
        std::cout<<"Resolution not in the range [0.1;1]"<<std::endl;
        throw std::exception();
    }
    angle_resolution = ang_res;                                         
    to_insert = 0;                                                      //set to_insert a 0, condizione iniziale
    to_remove = -1;                                                     //set to_remove a -1, condizione iniziale (buffer vuoto)
    buffer = new double*[BUFFER_DIM]{nullptr};
}

LaserScannerDriver::~LaserScannerDriver()
{   
    if(!isEmpty())                              //controllo se nel buffer e' presente qualcosa, se true cancello quello che c'e' dentro
    {
    for(int i=0; i < BUFFER_DIM; i++)
        if(buffer[i])
            delete[] buffer[i];
    }
    delete[] buffer;                            //cancello il buffer 
    buffer = nullptr;                           //buffer a nullptr
}

LaserScannerDriver::LaserScannerDriver(const LaserScannerDriver& l) //Costruttore copia
{
    angle_resolution = l.angle_resolution;
    buffer = new double*[BUFFER_DIM]{nullptr};         
    
    for(int i=0; i<BUFFER_DIM; i++)                    //for per copiare tutti i valori del buffer
    {
        if(l.buffer[i] != nullptr)                     //se una casella del buffer e' nullptr non la copio perche' quello nuovo e' gia inizializzato a nullptr
        {     
            buffer[i] = new double[size()]; 
            for(int j=0; j<size(); j++)
                buffer[i][j] = l.buffer[i][j];         //copia dei valori
        }
    }

    to_insert = l.to_insert;                           //copia delle invarianti
    to_remove = l.to_remove;                           //*
}

LaserScannerDriver::LaserScannerDriver(LaserScannerDriver&& l)  //Costruttore spostamento
{
    angle_resolution = l.angle_resolution;  //copio tutte le variabili private
    to_insert = l.to_insert;
    to_remove = l.to_remove;
    buffer = l.buffer;                      //faccio puntare il nuovo al vecchio buffer

    l.to_remove = -1;                       //re-inizializzo le invarianti di quello copiato
    l.to_insert = 0;                        //*
    l.buffer = nullptr;                     //tolgo il puntatore del vecchio al buffer che ho spostato
}

LaserScannerDriver& LaserScannerDriver::operator=(const LaserScannerDriver& l)  //Assegnamento copia
{
    angle_resolution = l.angle_resolution;
    double** p = new double*[BUFFER_DIM]{nullptr};       //uso un puntatore temporaneo per evitare il caso di auto-assegnamento
    for(int i=0; i<BUFFER_DIM; i++)
    {
        if(l.buffer[i] != nullptr)  
        {     
            p[i] = new double[size()]; 
            for(int j=0; j<size(); j++)
                p[i][j] = l.buffer[i][j];               //copia dei valori
        }
    }

    if(!isEmpty())                                      //Se il buffer non e' vuoto cancello il buffer del LSD in cui voglio spostare il buffer
        clear_buffer();
    delete[] buffer;

    buffer = p;                                         //faccio puntare al buffer temporaneo
    to_insert = l.to_insert;                            //copia delle invarianti
    to_remove = l.to_remove;                            //*

    return *this;
}

LaserScannerDriver& LaserScannerDriver::operator=(LaserScannerDriver&& l) //Assegnamento spostamento
{
    if(buffer != l.buffer)                              //controllo per evitare l'auto assegnamento
    {
        clear_buffer();                                 //cancello il buffer dell LSD in cui copiare
        delete[] buffer;

        angle_resolution = l.angle_resolution;          //copia delle invarianti
        to_insert = l.to_insert;                        //*
        to_remove = l.to_remove;                        //*

        buffer = l.buffer;                              //faccio puntare al buffer

        l.to_insert = 0;                                //re-inizializzo le invarianti
        l.to_remove = -1;                               //*
        l.buffer = nullptr;                             //faccio puntare a nullptr il vecchio oggetto LSD
    }
    
    return *this;
}

void LaserScannerDriver::new_scan(const std::vector<double>& scan)
{
    //crea un puntatore ausiliario in cui salva i dati del vettore
    double* storage = new double[size()];                   

    //copia dei valori nell array temporaneo
    if(scan.size() < size())                                //se il vettore passato ha meno elementi di quanti richiesti inizializza i mancanti a 0
    {
        int j = 0;
        while(j < scan.size())
        {
            storage[j] = scan[j];
            j++;
        }
        while(j < size())
        {
            storage[j] = 0;
            j++;
        }
    }
    else                                                    //altrimenti copia tutti i valori, i valori in eccesso vengono ignorati
    {
        for(int i=0; i < size(); i++)
            storage[i] = scan[i];
    }

    //se il buffer e' pieno elimina il vecchio
    if(to_insert == to_remove)
    {
        delete[] buffer[to_remove];
        buffer[to_remove++] = nullptr;
    }

    //assegno al buffer l'array temporaneo
    buffer[to_insert++] = storage;                          

    
    if(to_insert == 1 && to_remove == -1)   //Caso iniziale, devo incrementare to_remove che era inizializzato a -1
        to_remove++;

    if(to_remove > BUFFER_DIM - 1)          //controllo invariante to_remove
        to_remove = 0;

    if(to_insert > BUFFER_DIM - 1)          //controllo invariante to_insert
        to_insert = 0;   
}

std::vector<double> LaserScannerDriver::get_scan()
{
    //Buffer vuoto lancio eccezione
    if(isEmpty())
    {
        std::cout<<"Impossibile leggere...buffer vuoto!"<<std::endl;
        throw std::exception();
    }

    //copio i dati del buffer in un vector
    std::vector<double> rtn(size());
    for(int i=0; i<size(); i++)
        rtn[i] = buffer[to_remove][i];

    //cancello la casella del buffer da eliminare
    delete[] buffer[to_remove];
    buffer[to_remove++] = nullptr;

    if(to_remove > BUFFER_DIM - 1)  //controllo invariante to_remove
        to_remove = 0;

    if(to_remove == to_insert)      //controllo se ho svuotato il buffer, re-inizializzo le invarianti
    {
        to_remove = -1;
        to_insert = 0;
    }    

    return rtn;
}

void LaserScannerDriver::clear_buffer()
{
    if(!isEmpty())                          //controllo se nel buffer e' presente qualcosa, se true cancello quello che c'e' dentro  
    {
        for(int i=0; i<BUFFER_DIM; i++)
        {
            if(buffer[i])
                delete[] buffer[i];
            buffer[i] = nullptr;
        }
    }
    to_remove = -1;                         //re-inizializzo le invarianti
    to_insert = 0;                          //*
}   

double LaserScannerDriver::get_distance(const double& ang) const
{
    //lancio eccezione se angolo non rispetta i valori accettati
    if(ang < 0 || ang > 180)
    {
        std::cout<<"Angolo inserito non valido..."<<std::endl;
        throw std::exception();
    }
    //lancio eccezione se buffer vuoto
    if(isEmpty())
    {
        std::cout<<"Impossibile leggere...buffer vuoto!"<<std::endl;
        throw std::exception();
    }

    int position = round(ang/angle_resolution); //creo indice per la posizione a cui accedere

    if(position == size()) //necessario per risoluzione "brutte", se il decimale e' > 5 viene arrotondato al successivo che non esiste nel nostro caso, e' quindi necessario prendere il precedente
        position--;
    
    return get_element_at(position);
}

std::ostream& operator<<(std::ostream& os,const LaserScannerDriver& l)
{
    if(l.isEmpty())
        os<<"Impossibile leggere...buffer vuoto!";
    else
    {
        for(int i=0; i<l.size(); i++)
            os << l.get_element_at(i) << " ";   //ritorno i singoli double usando la funzione apposta
    }

    return os;
}

double LaserScannerDriver::get_element_at(int n) const
{
    //lancio eccezione se buffer vuoto
    if(isEmpty())
    {
        std::cout<<"Impossibile leggere...buffer vuoto!"<<std::endl;
        throw std::exception();
    }
    if(n < 0 || n > size())
    {
        std::cout<<"Indice Inserito non valido...deve essere [0;size]"<<std::endl;
        throw std::exception();
    }

    int last;               //indice che inizalizzo all'ultima scansione inserita
    if(to_insert == 0)
        last = BUFFER_DIM - 1;
    else
        last = to_insert - 1;

    return buffer[last][n]; //ritorno il double 
}

void LaserScannerDriver::getBuffer() const
{
    //controllo se c'e' un buffer
    if(buffer)
    {
        std::cout << "Buffer ---->";
        for(int i=0; i<BUFFER_DIM; i++)
        {
            if(buffer[i] == nullptr)                    //ritorna 0 se la cella del buffer punta a nullptr
                std::cout<<"[0] ";
            else
                std::cout<<"[" << buffer[i] << "] ";    //ritorna l'indirizzo del puntatore della cella del buffer
        }
    }
    else
    {
        std::cout<<"Buffer non esistente...";           
    }
    
    std::cout<<std::endl;
}
