#pragma once

/** @defgroup Clouds Clouds
 * @{
 * Functions for manipulating the clouds
 */
typedef struct
{
	double x;
	double y;
} Cloud;

/**
 * @brief Initializes a new cloud
 *
 * @param ix cloud x
 * @param iy cloud y
 * @return pointer to the new cloud created
 */
Cloud* new_cloud(double ix, double iy);
