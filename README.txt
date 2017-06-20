Tyler Kutz, Cameron Clarke // 7-25-2016

How to use this software suite to take output from a GEANT4 simulation and turn it into fitted envelopes:

1) Count using counter.c the number of events (parents and ~9th daughters) that were generated and placed in the output_positions.txt file
2) run the plotPoints.c code (in batch mode) to go through all the data and plot only those that your GEANT4 simulation considered 'good' 
3) use the interactive ROOT script reader.c to select the positions of all the vertices that you want to save to file for different z's as starting points for the fit
4) run the MenvelopeFitLoop_batch.c code (in batch mode) to fit different z positions (using as a first guess the stored vertex points nearest to that working z position)
