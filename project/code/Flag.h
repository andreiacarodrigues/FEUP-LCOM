#pragma once
/** @defgroup Flag Flag
 * @{
 * Functions for manipulating the flag
 */
typedef struct
{
	double x;
	double y;
} Flag;
/**
 * @brief Initializes a new flag
 * @param ix flag x
 * @param iy flag y
 * @return pointer to the new flag created
 */
Flag* new_flag(double ix, double iy);
