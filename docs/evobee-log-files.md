# EvoBee log files

## Output log file formats

As shown in the [General parameters](https://tim-taylor.github.io/evobee/evobee-config.html#general-parameters) section of the run configuration page, there are various types of logging data that may be requested from a run. The `log-flags` parameter specifies zero, one or more flags for different kinds of output. The output from all requested flags is recording in a single log file (with filename ending "-log.txt"). Each logging event appears as a separate line in the log file, and each line is a list of comma separated values (so the log file is in .csv format). The first item of every line in a single letter showing the corresponding log-flag associated with the line (e.g. 'Q', 'P', 'F', 'G', 'p', 'f', 'g', 'm', 'n') --- uppercase letters refer to full reporting formats, and lowercase letters to summary reporting formats.

To fully understand the specific format of each line, consult the corresponding methods in the `Logger` class.

A summary of some of the formats is shown below.

### log-flags=f  (Logger::logFlowersInterPhaseSummary)

 1. "f"
 2. generation number
 3. step number
 4. flower species ID
 5. flower species name
 6. number of plants of this species in this generation
 7. number of pollinated plants of this species at end of foraging phase in this generation

### log-flags=F  (Logger::logFlowersInterPhaseFull)

For each plant in the environment, emit a line with the following format at the end of each generation:

 1. "F"
 2. generation number
 3. plant unique ID
 4. plant species ID
 5. plant x position
 6. plant y position
 7. patch locality ID

These are followed by entries for each flower on the plant (NB the current implementation only allows a single flower on each plant):

 7. ":"
 8. flower unique ID
 9. flower pollination status ("P"=pollinated, "N"=not pollinated)
 10. flower marker point

This is followed by entries for each pollen grain in the flower's stigma:

 11. "~"
 12. pollen source flower species (marker point)
 13. number of pollen grains on stigma from this source species
 14. pollen source flower unique ID (only useful if there is only a single grain of pollen from this source species, as multiple grains may have come from multiple flowers)
 15. "~"

### log-flags=G  (Logger::logFlowersIntraPhaseFull)

For each plant in the environment, emit a line with the following format at regular intervals throughout a generation (determined by m_iLogUpdatePeriod):

 1. "G"
 2. generation number
 3. step number
 4. flower unique ID
 5. flower species ID
 6. flower x position
 7. flower y position
 8. flower marker point
 9. flower pollination status ("P"=pollinated, "N"=not pollinated)
 10. amount of pollen available on anther
 11. amount of pollen deposited on stigma
 12. amount of nectar available

### log-flags=m  (Logger::logFlowerMPsInterPhaseSummary)

 1. "m"
 2. generation number
 3. step number
 4. marker point
 5. number of plants with this flower marker point in this generation
 6. number of pollinated plants with this flower marker point at end of foraging phase in this generation
 7. number of plants in communal regions (non-refuges) with this flower marker point in this generation
 8. number of pollinated plants in communal regions (non-refuges) with this flower marker point at end of foraging phase in this generation

### log-flags=n (Logger::logFlowerInfoInterPhaseSummary)

 1. "n"
 2. generation number
 3. step number
 4. stimulus ID (as defined by the first field of the corresponding entry in the vis-data array)
 5. number of plants of this flower stimulus type (colour) in this generation
 6. number of pollinated plants of this flower stimulus type (colour)  at end of foraging phase in this generation
 7. number of plants in communal regions (non-refuges) of this flower stimulus type (colour)  in this generation
 8. number of pollinated plants in communal regions (non-refuges) of this flower stimulus type (colour)  at end of foraging phase in this generation
 9. auxiliary ID associated with this stimulus (as defined by ninth field in vis-data entry)
 10. dominant wavelength of this stimulus (as defined by second field in vis-data entry)
 11. number of landings on flowers of this type during the current generation

### log-flags=P (Logger::logPollinatorsInterPhaseFull)
(The output format with log-flags=P is the same as for log-flags=Q, except just a single line is logged for each pollinator at the end of each foraging phase rather than at every step of the foraging phase)

 1. "P"
 2. generation number
 3. step number
 4. pollinator species name
 5. pollinator ID
 6. pollinator x position
 7. pollinator y position
 8. pollinator heading
 9. number of flowers visited
 10. step number of most recent action
 11. flower marker point (if action=ON_FLOWER or DECLINED_FLOWER) or 0 (if action=NO_FLOWER_SEEN)
 12. reward received (if action=ON_FLOWER) or -1 (if action=DECLINED_FLOWER) or -2 (if action=NO_FLOWER_SEEN)
 13. "T" if pollinator judged flower to match its current target, otherwise "F"
 14. "//"
 15. pollinator's current target marker point
 16. "::"
 17. fields 17 onward record the pollinator's current visual preference data, in groups of three fields. The first field gives the marker point for which the following two fields apply, the second gives the probability of the pollinator landing on that marker point if it is the current target MP, and the third gives the probability of the pollinator landing on that marking point if it is not the current target MP. After these triplets have been recorded for every marker point that the pollinator knows about, the final field of the line in the log file is another "::"


### log-flags=p  (Logger::logPollinatorsInterPhaseSummary)

The first 3 fields are:

 1. "p"
 2. generation number
 3. pollinator ID

These are followed by entries for each plant species, each one comprising 4 further fields:

 4. flower species ID
 5. number of landings
 6. number of pollinations
 7. number of pollen grains of this species carried by the pollinator at end of foraging phase


### log-flags=Q  (Logger::logPollinatorsIntraPhaseFull)

 1. "Q"
 2. generation number
 3. step number
 4. pollinator species name
 5. pollinator ID
 6. pollinator x position
 7. pollinator y position
 8. pollinator heading
 9. number of flowers visited
 10. step number of most recent action
 11. flower marker point (if action=ON_FLOWER or DECLINED_FLOWER) or 0 (if action=NO_FLOWER_SEEN)
 12. reward received (if action=ON_FLOWER) or -1 (if action=DECLINED_FLOWER) or -2 (if action=NO_FLOWER_SEEN)
 13. "T" if pollinator judged flower to match its current target, otherwise "F"
 14. "//"
 15. pollinator's current target marker point
 16. "::"
 17. fields 17 onward record the pollinator's current visual preference data, in groups of three fields. The first field gives the marker point for which the following two fields apply, the second gives the probability of the pollinator landing on that marker point if it is the current target MP, and the third gives the probability of the pollinator landing on that marking point if it is not the current target MP. After these triplets have been recorded for every marker point that the pollinator knows about, the final field of the line in the log file is another "::"
