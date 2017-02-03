/*
 * Copyright (C) 2017 https://www.brobwind.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MCUCONF_H
#define MCUCONF_H

/*
 * GD32F1X0 drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 15...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

/*
 * HAL driver system settings.
 */
#define GD32_NO_INIT                       FALSE

/*
 * GPT driver system settings.
 */
#define GD32_GPT_USE_TIM2                  TRUE
#define GD32_GPT_USE_TIM3                  TRUE
#define GD32_GPT_TIM2_IRQ_PRIORITY         2
#define GD32_GPT_TIM3_IRQ_PRIORITY         2

/*
 * SERIAL driver system settings.
 */
#define GD32_SERIAL_USE_USART2             TRUE
#define GD32_SERIAL_USART2_PRIORITY        3

/*
 * ST driver system settings.
 */
#define GD32_ST_IRQ_PRIORITY               8
#define GD32_ST_USE_TIMER                  2

#endif /* MCUCONF_H */
