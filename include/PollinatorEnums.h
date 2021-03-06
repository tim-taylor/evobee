/**
 * @file
 *
 * Declaration of enums associated with Pollinators:
 * PollinatorState, PollinatorConstancyType and PollinatorForagingStrategy
 */

#ifndef _POLLINATORENUMS_H
#define _POLLINATORENUMS_H

/**
 * The PollinatorState enum
 */
enum class PollinatorState {UNINITIATED, FORAGING, BOUT_COMPLETE};

/**
 * The PollinatorConstancyType enum
 */
enum class PollinatorConstancyType {NONE, SIMPLE, VISUAL};

/**
 * The PollinatorConstancyType enum
 */
enum class PollinatorForagingStrategy {RANDOM, NEAREST_FLOWER, RANDOM_FLOWER, RANDOM_GLOBAL};

/**
 * The PollinatorCurrentStatus enum
 */
enum class PollinatorCurrentStatus {ON_FLOWER, DECLINED_FLOWER, NO_FLOWER_SEEN};

/**
 * The PollinatorLearningStrategy enum
 */
enum class PollinatorLearningStrategy {DELIBERATIVE_DECISIVE, FICKLE_CIRCUMSPECT, STAY, STAY_RND, STAY_INNATE, NONE};

/**
 * The PollinatorStepType enum
 */
enum class PollinatorStepType {CONSTANT, LEVY};

/**
 * The PollinatorInnatePrefType enum
 */
enum class PollinatorInnatePrefType {GIURFA, FLAT, HOVERFLY, PRESET};


#endif /* _POLLINATORENUMS_H */
