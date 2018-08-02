Daniel DeLayo, Catherine Feldman, Cameron Clarke //7-31-2018

How to use this software suite to take output from a GEANT4 simulation and turn it into fitted envelopes:

1) Run a Geant4 Simulation with Tracking turned on (see branch feature-envelope in sbujlab/remoll)
2) Run the output through pruneTreeEnvelope.c (currently in sbujlab/rad_analysis github)
3) make and call with ./reader filename to generate an envelope for an input z position
4) Use the output (vertex_storage.txt) to generate an envelope



5) VBA Macros - To make SolidWorks envelopes we need to have the same number of points in each z slice, they need to be very close to each other in x,y so that the envelope doesn't develop twists, and for physics concerns it should include almost every event (meaning that more events should be run, and the tree pruned before fitting)

Tyler's VBA Macros are located in the cadVBA folder, to use them:
1) Tools->Macro->Edit the .SWP file that you want to edit (example is CreateEnvelope1.swp in tyler/PREX/Envelopes/)
2) LoftStitch.swp is used to connect neighboring planes
