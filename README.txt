Daniel DeLayo, Catherine Feldman, Cameron Clarke, Tyler //7-31-2018

How to use this software suite to take output from a GEANT4 simulation and turn it into fitted envelopes:

1) Run a Geant4 Simulation with Tracking turned on (see branch feature-envelope in sbujlab/remoll)
2) Run the output through pruneTreeEnvelope.c (currently in sbujlab/rad_analysis github)
3) Make and call with ./reader filename to generate an envelope for an input z position
    a) Alternatively, envelope.sh can be edited and used
    b) reader.c is compiled with "make reader" which requires up-to-date libremoll.so and remolltypes.hh
4) Plug the vertex_stores.txt file into the SolidWorks VBA macros
5) Connect the resultant envelopes with a loft (allows connecting ngons)
    a) Catherine's updated version of Tyler's VBA script MakeEnvelopes is in the cadVBA subdirectory


Tyler's VBA Macros are located in the cadVBA folder, to use them:
1) Tools->Macro->Edit the .SWP file that you want to edit (example is CreateEnvelope1.swp in tyler/PREX/Envelopes/)
2) LoftStitch.swp is used to connect neighboring planes
