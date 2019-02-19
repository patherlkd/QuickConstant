/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: ldavis
 *
 * Created on 19 February 2019, 17:50
 */

#include <cstdlib>
#include "CLI11.hpp"
#include <string>
#include <iostream>
#include <math.h>

/*
 * 
 */
int main(int argc, char* argv[]) {

    double l = 0.0; // Simulation box length
    double rc = 0.0; // cut-off defining dimer state
    int d = 0; // length units :  e.g. 10^-d where d = 9 for nano meters


    std::string xyzfilename = "";

    CLI::App quickCons{"A program to compute the association and/or dissociation constant from a 2 bead simulation"};

    CLI::Option* opt_xyzfilename = quickCons.add_option("-f,--xyzfilename", xyzfilename, "XYZ coordinate filename")->required();
    CLI::Option* opt_l = quickCons.add_option("-l,--boxlength", l, "Box length of the simulation box")->required();
    CLI::Option* opt_rc = quickCons.add_option("-c,--cutoff", rc, "Cut-off defining the dimer state")->required();
    CLI::Option* opt_d = quickCons.add_option("-d,--lengthdim", d, "length units :  e.g. 10^-d where d = 9 for nano meters")->required();

    CLI11_PARSE(quickCons, argc, argv);

    if (l <= 0.0) {
        std::cerr << "Box length must be > 0.0\n";
        exit(EXIT_FAILURE);
    }
    if (rc <= 0.0) {
        std::cerr << "Dimer cut-off must be > 0.0\n";
        exit(EXIT_FAILURE);
    }
    if (d <= 0) {
        std::cerr << "Length dimensions must be > 0\n";
        exit(EXIT_FAILURE);
    }
    if (xyzfilename.empty()) {
        std::cerr << "XYZ filename must be non-empty\n";
        exit(EXIT_FAILURE);
    }

    std::ifstream xyz(xyzfilename, std::ifstream::in);

    if (xyz.fail()) {
        std::cerr << "Cannot open: " << xyzfilename << "\n";
        exit(EXIT_FAILURE);
    }

    std::string line;

    int ntraj = 0; // Number of independent trajectories analysed
    int ndimer = 0; // Number of trajectories containing dimer
    int nmono = 0; // Number of trajectories containing separate monomers 

    int natom = 0;

    while (getline(xyz, line)) {// The main loop that reads the xyz file

        std::stringstream ss(line);
        std::string a;
        double x, y, z;
        double x1, y1, z1;
        double dx, dy, dz;

        ss >> a >> x >> y >> x;

        if (a == "Atoms.") {

            natom = 0;
            ntraj++;
        } else if (std::stoi(a) == 1) {
            natom++;

            if (natom == 1) {
                x1 = x;
                y1 = y;
                z1 = z;
            } else if (natom == 2) {


                dx = x1 - x;
                dy = y1 - y;
                dz = z1 - z;

                if (dx > l * 0.5) { // correct for periodic boundary conditions
                    dx = dx - l;
                } else if (dx <= -l * 0.5) {
                    dx = dx + l;
                }

                if (dy > l * 0.5) {
                    dy = dy - l;
                } else if (dy <= -l * 0.5) {
                    dy = dy + l;
                }

                if (dz > l * 0.5) {
                    dz = dz - l;
                } else if (dz <= -l * 0.5) {
                    dz = dz + l;
                }

                double r = sqrt(dx * dx + dy * dy + dz * dz);



                if (r <= rc) { // check if in dimer state
                    ndimer++;
                } else {
                    nmono++;
                }

            }

        }

    }

    double V = l * l*l;
    double Vd = (4.0 / 3.0) * M_PI * rc * rc * rc;

    double ka = 6.022140857 * ((double) ndimer / (double) (nmono))*(V - Vd) * pow(10.0, (23 + 3 - (d * 3)));
    double kd = 1.0 / ka;
    
    std::cout<<"Number of trajectories containing dimers: "<<ndimer<<"\n";
    std::cout<<"Number of trajectories not containing dimers: "<<nmono<<"\n";
    std::cout << "ka (association constant in units liters/mol): " << ka << "\n";
    std::cout << "kd (dissociation constant in units mol/liters): " << kd << "\n";
    std::cout << "Number of trajectories analysed: " << ntraj << "\n";

    xyz.close();

    return 0;
}

