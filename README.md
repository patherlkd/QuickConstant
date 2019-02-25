# QuickConstant

This repository contains a c++ code that computes the dissociation and association constants (according to Jong et al. (2010) https://onlinelibrary.wiley.com/doi/epdf/10.1002/jcc.21776), with units mol/liter and liter/mol respectively, based upon a simple 2 bead simulation with periodic boundary conditions. The coordinate files are expected to be in XYZ format i.e.
```
2
Atoms. Timestep: 0
1 <x1> <y1> <z1>
1 <x2> <y2> <z2>
```
where `<x>, <y>, <z>` are the cartesian coordinates.

# Usage

This repository contains an executable ***quickconstant***. By typing `quickconstant -h` or `quickconstant --help` you will see the command line options:
```
A program to compute the association and/or dissociation constant from a 2 bead simulation
Usage: quickconstant [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--xyzfilename TEXT REQUIRED
                              XYZ coordinate filename
  -l,--boxlength FLOAT REQUIRED
                              Box length of the simulation box
  -c,--cutoff FLOAT REQUIRED  Cut-off defining the dimer state
  -r,--radius FLOAT REQUIRED  excluded volume radius of particle
  -d,--lengthdim INT REQUIRED length units :  e.g. 10^-d where d = 9 for nano meters
```
e.g. to actually run the code you would execute:
```
quickconstant -f <xyzfilename> -c <cutoff> -l <boxlength> -d <lengthdim> -r <radius>
```
where `<xyzfilename` is the file name containing the cartesian coodinates for 2 beads at multiple timesteps, `<cutoff>` is the dimerization radius, i.e., if two beads have an inter-particle distance `<dr>` that is equal to or less than `<cutoff>` they are considered a dimer, `<boxlength>` is the simulation box length, `<radius>` is the excluded volume radius, and `<lengthdim>` is the unit of length e.g. 1 nm = 10^-9 m here `<lengthdim> = 9`.

# Results

Results will be displayed on the terminal e.g.
```
Number of trajectories containing dimers  <ndimer>
Number of trajectories not containing dimers  <nmono>
ka (association constant in units liters/mol) <ka>
kd (dissociation constant in units mol/liters)  <kd>
Number of trajectories analysed <ntraj>
```
