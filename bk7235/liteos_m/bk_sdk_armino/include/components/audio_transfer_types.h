/*
// Copyright (C) 2022 Beken Corporation
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

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	AUD_TRAS_SAMP_RATE_8K = 0,
	AUD_TRAS_SAMP_RATE_16K,
	AUD_TRAS_SAMP_RATE_MAX,
} audio_tras_samp_rate_t;

typedef struct {
	bool aec_enable;
	audio_tras_samp_rate_t samp_rate;
	int (*audio_send_mic_data)(unsigned char *data, unsigned int len);		/**< the api is called when collecting a frame mic packet data is complete */
} audio_tras_setup_t;

#ifdef __cplusplus
}
#endif

