/*
 * Copyright (C) 2022 Beken Corporation
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <components/audio_transfer_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the audio transfer module
 *
 * This API init the resoure common:
 *   - Init audio transfer driver
 *   - Configure audio dac and adc paramter
 *   - Configure DMA to carry adc and dac data
 *   - Configure AEC
 *
 * @param setup audio transfer setup configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t audio_tras_init(audio_tras_setup_t setup);

/**
 * @brief     Deinit the audio transfer module
 *
 * This API init the resoure common:
 *   - Deinit audio transfer driver
 *   - Reset audio dac and adc paramter
 *   - Free DMA
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t audio_tras_deinit(void);

/**
 * @brief     Write speaker data received by wifi to audio dac
 *
 * @param dac_buff speaker data address
 * @param len the size of speaker data(byte)
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t audio_tras_write_spk_data(uint8_t *dac_buff, uint32_t len);

#ifdef __cplusplus
}
#endif

