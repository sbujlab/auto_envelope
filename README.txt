Daniel DeLayo, Catherine Feldman, Cameron Clarke //7-31-2018

How to use this software suite to take output from a GEANT4 simulation and turn it into fitted envelopes:

1) Run a Geant4 Simulation with Tracking turned on (see branch feature-envelope)
2) Run the output through pruneTreeEnvelope.c (currently in rad_analysis git)
3) make and call with ./reader filename to generate an envelope for an input z position
4) Use the output (vertex_storage.txt) to generate an envelope
