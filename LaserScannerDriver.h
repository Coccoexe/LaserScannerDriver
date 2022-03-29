/**
 * @file LaserScannerDriver.h
 * @author Alessio Cocco - 1219609 (alessio.cocco@studentiunipd.it)
 * @brief 
 * @date 25-11-2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include<vector>
#include<iostream>

class LaserScannerDriver
{
    public:
        /**
        * @brief Construct a new Laser Scanner Driver object with standard resolution of 1
        * 
        */
        LaserScannerDriver();
        /**
        * @brief Construct a new Laser Scanner Driver object with custom resolution [0.1;1]
        * @throw std::exception if ang_res not in the range [0.1;1]
        * @param ang_res 
        */
        explicit LaserScannerDriver(const double& ang_res);             //explicit necessario per non permettere il cambio di risoluzione
        /**
        * @brief Destroy the Laser Scanner Driver object
        * 
        */
        ~LaserScannerDriver();
        /**
        * @brief Copy constructor
        * 
        * @param l 
        */
        LaserScannerDriver(const LaserScannerDriver& l);                //Costruttore di copia
        /**
        * @brief Move constructor
        * 
        * @param l 
        */
        LaserScannerDriver(LaserScannerDriver&& l);                     //Costruttore di spostamento
        /**
        * @brief Copy assignment
        * 
        * @param l 
        * @return LaserScannerDriver& 
        */
        LaserScannerDriver& operator=(const LaserScannerDriver& l);     //Assegnamento di copia
        /**
        * @brief move assignment
        * 
        * @param l 
        * @return LaserScannerDriver& 
        */
        LaserScannerDriver& operator=(LaserScannerDriver&& l);          //Assegnamento di spostamento
        /**
        * @brief add a scan in the buffer, if buffer full, replace the older one
        * 
        * @param scan 
        */
        void new_scan(const std::vector<double>& scan);
        /**
        * @brief Get the scan object
        * @throw std::exception if the user try to remove in an empty buffer
        * @return std::vector<double> 
        */
        std::vector<double> get_scan();
        /**
        * @brief set the object to the initial state
        * 
        */
        void clear_buffer();
        /**
        * @brief Get data of the angle saved in the newest scan
        * @throw std::exception if the input angle is < 0 or > 180 or if buffer is empty
        * @param ang 
        * @return double 
        */
        double get_distance(const double& ang) const;

    //added  functions
        /**
        * @brief Get the resolution object
        * 
        * @return double 
        */
        double get_resolution() const {return angle_resolution;};
        /**
        * @brief return the size of the scan, depend to resolution
        * 
        * @return int 
        */
        int size() const {return (int)((180/angle_resolution)+1);};
        /**
        * @brief 
        * 
        * @return true 
        * @return false 
        */
        bool isEmpty() const {return to_remove == -1;};
        /**
        * @brief Get the element at object
        * @throw std::exception if the buffer is empty
        * @param n 
        * @return double 
        */
        double get_element_at(int n) const;
        /**
        * @brief return the state of the buffer, 0 if a cell is empty , address if full
        * 
        */
        void getBuffer() const ;


    private:
        double angle_resolution;      //risoluzione -- e' una costante e rimane fissa per tutta la vita di un oggetto, viene solo cambiata nei costruttori e assegnamenti di copia e spostamento per evitare di lanciare eccezioni in caso non abbiano la stessa rioluzione. ma visto che si tratta di copia e spostamento e' come se venisse sovrascritto l'oggetto quindi ho deciso di permettere il cambio di risoluzione solo in quei casi
        const int BUFFER_DIM = 10; 
        int to_insert;                //contatore indice in cui inserire il prossimo scan -- invariante, deve rimanare tra 0 e 9   
        int to_remove;                //contatore indice in cui rimuovere il prossimo scan -- invariante, deve rimanere tra -1 e 9 (-1 se e' vuoto)
        double** buffer;              
};
std::ostream& operator<<(std::ostream& os,const LaserScannerDriver& l);

/* rappresentazione visiva del buffer ed esempio di gestione buffer nelle funzioni significative

buffer inizale: 

to_remove  
           to_insert
  [-1]        [0]       [1]       [2]       [3]       [4]       [5]       [6]       [7]       [8]       [9]
           [nullptr] [nullptr] [nullptr] [nullptr] [nullptr] [nullptr] [nullptr] [nullptr] [nullptr] [nullptr]

new_scan:
    to_insert++ e se vale 10 riporta a 0
    to_remove se caso iniziale settato a 0
    to_remove++ se buffer pieno elimino i dati della scans piu vecchia, salvo i nuovi e incremento
                se vale 10 riporta a 0

get_scan:
    to_remove-- e se vale -1 riporta a 9
    se to_remove == to_insert il buffer e' vuoto riporto to_insert a 0 e to_remove a -1          

clear_buffer
    se non e' gia come nel caso iniziale lo riporta al caso iniziale
    to_remove = -1;
    to_insert = 0;

*/