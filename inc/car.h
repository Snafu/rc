/*
 * car.h
 *
 *  Created on: 28.03.2014
 *      Author: Snafu
 */

#ifndef CAR_H_
#define CAR_H_

#include <types.h>


/*! \brief Initialize car control
 *
 */
extern int car_init(void);

/*! \brief Steer the car
 *
 *  \param percent Steering level in percent (negative = left, positive = right).
 */
extern void car_steer(int percent);

/*! \brief Set car throttle value
 *
 *  \param percent Throttle level in percent (negative = reverse, positive = forward).
 */
extern void car_throttle(int percent);

#endif /* CAR_H_ */
