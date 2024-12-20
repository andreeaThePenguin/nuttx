/****************************************************************************
 * boards/arm/nrf53/nrf5340-dk/src/nrf53_pwm.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <debug.h>
#include <errno.h>
#include <stddef.h>

#include <nuttx/timers/pwm.h>
#include <arch/board/board.h>

#include "nrf53_pwm.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NRF53_PWM (0)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: nrf53_pwm_setup
 *
 * Description:
 *   Initialize PWM and register the PWM device.
 *
 ****************************************************************************/

int nrf53_pwm_setup(void)
{
  static bool             initialized = false;
  struct pwm_lowerhalf_s *pwm         = NULL;
  int                     ret         = OK;

  /* Have we already initialized? */

  if (!initialized)
    {
      /* Call nrf53_pwminitialize() to get an instance of the PWM interface */

      pwm = nrf53_pwminitialize(NRF53_PWM);
      if (!pwm)
        {
          aerr("ERROR: Failed to get the NRF53 PWM lower half\n");
          ret = -ENODEV;
          goto errout;
        }

      /* Register the PWM driver at "/dev/pwm0" */

      ret = pwm_register("/dev/pwm0", pwm);
      if (ret < 0)
        {
          aerr("ERROR: pwm_register failed: %d\n", ret);
          goto errout;
        }

      /* Now we are initialized */

      initialized = true;
    }

errout:
  return ret;
}
