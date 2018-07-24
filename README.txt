Daniel DeLayo, Catherine F, Cameron Clarke //7-23-2018

How to use this software suite to take output from a GEANT4 simulation and turn it into fitted envelopes:

1) Run a Geant4 Simulation with Tracking turned on (see branch feature-envelope)
2) Run the output through pruneTreeEnvelope.c (currently in rad_analysis git)
3) Compile reader and call with ./reader filename to generate an envelope for an input z position

4) run the MenvelopeFitLoop_batch.c code (in batch mode) to fit different z positions (using as a first guess the stored vertex points nearest to that working z position)
