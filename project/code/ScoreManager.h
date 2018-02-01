#pragma once

#include <minix/syslib.h>
#include <minix/drivers.h>
/** @defgroup Platform Platform
 * @{
 * Functions for managing the scores
 */
typedef struct
{
	int Score;
	int Day;
	int Month;
	int Year;
} Score;
/**
 * @brief Initializes the scores
 * @param Scores[] pointer to scores array
 */
void init_scores(Score* Scores[]);
/**
 * @brief Creates new score
 * @return pointer to the new score created
 */
Score * new_score();
/**
 * @brief Initializes the scores
 * @param Scores[] pointer to scores array
 * @param s pointer to score to add
 */
void add_score(Score* Scores[], Score * s);
/**
 * @brief Writes the scores in a file
 * @param Scores[] pointer to scores array
 */
void write_scores(Score* Scores[]);
/**
 * @brief Reads the scores from a file
 * @param Scores[] pointer to scores array
 */
void read_scores(Score* Scores[]);
