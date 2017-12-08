// MIT License

// Copyright (c) 2017 Vadim Grigoruk @nesbox // grigoruk@gmail.com

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdlib.h>
#include <string.h>

#include <tic80.h>
#include "ticapi.h"
#include "tools.h"

#include "ext/gif.h"

static void onTrace(void* data, const char* text, u8 color)
{
	tic80* tic = (tic80*)data;

	if(tic->callback.trace)
		tic->callback.trace(text, color);
}

static void onError(void* data, const char* info)
{
	tic80* tic = (tic80*)data;

	if(tic->callback.error)
		tic->callback.error(info);
}

static void onExit(void* data)
{
	tic80* tic = (tic80*)data;

	if(tic->callback.exit)
		tic->callback.exit();
}

static u64 getFreq()
{
	return TIC_FRAMERATE;
}

static u64 TickCounter = 0;

static u64 getCounter()
{
	return TickCounter;
}

tic80* tic80_create(s32 samplerate)
{
	tic80_local* tic80 = malloc(sizeof(tic80_local));

	if(tic80)
	{
		memset(tic80, 0, sizeof(tic80_local));

		tic80->memory = tic_create(samplerate);

		{
			static const u8 Font[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x00, 0x30, 0x00, 0x00, 0x00, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0xf8, 0x50, 0xf8, 0x50, 0x00, 0x00, 0x00, 0x78, 0xa0, 0x70, 0x28, 0xf0, 0x00, 0x00, 0x00, 0x88, 0x10, 0x20, 0x40, 0x88, 0x00, 0x00, 0x00, 0x40, 0xa0, 0x68, 0x90, 0x68, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x40, 0x40, 0x20, 0x00, 0x00, 0x00, 0x40, 0x20, 0x20, 0x20, 0x40, 0x00, 0x00, 0x00, 0x20, 0xa8, 0x70, 0xa8, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x70, 0xc8, 0xc8, 0xc8, 0x70, 0x00, 0x00, 0x00, 0x30, 0x70, 0x30, 0x30, 0x78, 0x00, 0x00, 0x00, 0xf0, 0x18, 0x70, 0xc0, 0xf8, 0x00, 0x00, 0x00, 0xf8, 0x18, 0x30, 0x98, 0x70, 0x00, 0x00, 0x00, 0x30, 0x70, 0xd0, 0xf8, 0x10, 0x00, 0x00, 0x00, 0xf8, 0xc0, 0xf0, 0x18, 0xf0, 0x00, 0x00, 0x00, 0x70, 0xc0, 0xf0, 0xc8, 0x70, 0x00, 0x00, 0x00, 0xf8, 0x18, 0x30, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x70, 0xc8, 0x70, 0xc8, 0x70, 0x00, 0x00, 0x00, 0x70, 0xc8, 0x78, 0x08, 0x70, 0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x60, 0x20, 0x40, 0x00, 0x00, 0x10, 0x20, 0x40, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00, 0x78, 0x18, 0x30, 0x00, 0x30, 0x00, 0x00, 0x00, 0x70, 0xa8, 0xb8, 0x80, 0x70, 0x00, 0x00, 0x00, 0x70, 0xc8, 0xc8, 0xf8, 0xc8, 0x00, 0x00, 0x00, 0xf0, 0xc8, 0xf0, 0xc8, 0xf0, 0x00, 0x00, 0x00, 0x70, 0xc8, 0xc0, 0xc8, 0x70, 0x00, 0x00, 0x00, 0xf0, 0xc8, 0xc8, 0xc8, 0xf0, 0x00, 0x00, 0x00, 0xf8, 0xc0, 0xf0, 0xc0, 0xf8, 0x00, 0x00, 0x00, 0xf8, 0xc0, 0xf0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x78, 0xc0, 0xd8, 0xc8, 0x78, 0x00, 0x00, 0x00, 0xc8, 0xc8, 0xf8, 0xc8, 0xc8, 0x00, 0x00, 0x00, 0x78, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00, 0x00, 0xf8, 0x18, 0x18, 0xd8, 0x70, 0x00, 0x00, 0x00, 0xc8, 0xd0, 0xe0, 0xd0, 0xc8, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xf8, 0x00, 0x00, 0x00, 0xd8, 0xf8, 0xf8, 0xa8, 0x88, 0x00, 0x00, 0x00, 0xc8, 0xe8, 0xf8, 0xd8, 0xc8, 0x00, 0x00, 0x00, 0x70, 0xc8, 0xc8, 0xc8, 0x70, 0x00, 0x00, 0x00, 0xf0, 0xc8, 0xc8, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x70, 0xc8, 0xc8, 0xc8, 0x70, 0x08, 0x00, 0x00, 0xf0, 0xc8, 0xc8, 0xf0, 0xc8, 0x00, 0x00, 0x00, 0x78, 0xe0, 0x70, 0x38, 0xf0, 0x00, 0x00, 0x00, 0x78, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0xc8, 0xc8, 0xc8, 0xc8, 0x70, 0x00, 0x00, 0x00, 0xc8, 0xc8, 0xc8, 0x70, 0x20, 0x00, 0x00, 0x00, 0x88, 0xa8, 0xf8, 0xf8, 0xd8, 0x00, 0x00, 0x00, 0xc8, 0xc8, 0x70, 0xc8, 0xc8, 0x00, 0x00, 0x00, 0x68, 0x68, 0x78, 0x30, 0x30, 0x00, 0x00, 0x00, 0xf8, 0x30, 0x60, 0xc0, 0xf8, 0x00, 0x00, 0x00, 0x60, 0x40, 0x40, 0x40, 0x60, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x60, 0x20, 0x20, 0x20, 0x60, 0x00, 0x00, 0x00, 0x20, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x98, 0x98, 0x78, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0xc8, 0xc8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x78, 0xe0, 0xe0, 0x78, 0x00, 0x00, 0x00, 0x18, 0x78, 0x98, 0x98, 0x78, 0x00, 0x00, 0x00, 0x00, 0x70, 0xd8, 0xe0, 0x70, 0x00, 0x00, 0x00, 0x38, 0x60, 0xf8, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x70, 0x98, 0xf8, 0x18, 0x70, 0x00, 0x00, 0xc0, 0xf0, 0xc8, 0xc8, 0xc8, 0x00, 0x00, 0x00, 0x30, 0x00, 0x70, 0x30, 0x78, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x18, 0x98, 0x70, 0x00, 0x00, 0xc0, 0xc8, 0xf0, 0xc8, 0xc8, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x38, 0x00, 0x00, 0x00, 0x00, 0xd0, 0xf8, 0xa8, 0xa8, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xc8, 0xc8, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x70, 0xc8, 0xc8, 0x70, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xc8, 0xc8, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x78, 0x98, 0x98, 0x78, 0x18, 0x00, 0x00, 0x00, 0xf0, 0xc8, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x78, 0xe0, 0x38, 0xf0, 0x00, 0x00, 0x00, 0x60, 0xf8, 0x60, 0x60, 0x38, 0x00, 0x00, 0x00, 0x00, 0x98, 0x98, 0x98, 0x78, 0x00, 0x00, 0x00, 0x00, 0xc8, 0xc8, 0xd0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x88, 0xa8, 0xf8, 0xd8, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x70, 0x70, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x98, 0x98, 0x78, 0x18, 0x70, 0x00, 0x00, 0x00, 0xf8, 0x30, 0x60, 0xf8, 0x00, 0x00, 0x00, 0x30, 0x20, 0x60, 0x20, 0x30, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x60, 0x20, 0x30, 0x20, 0x60, 0x00, 0x00, 0x00, 0x00, 0x28, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
			memcpy(tic80->memory->font.data, Font, sizeof(tic_font));
		}

		return &tic80->tic;
	}

	return NULL;
}

TIC80_API void tic80_load(tic80* tic, void* cart, s32 size)
{
	tic80_local* tic80 = (tic80_local*)tic;

	tic80->tic.sound.count = tic80->memory->samples.size/sizeof(s16);
	tic80->tic.sound.samples = tic80->memory->samples.buffer;

	tic80->tic.screen = tic80->memory->screen;

	{
		tic80->tickData.error = onError;
		tic80->tickData.trace = onTrace;
		tic80->tickData.exit = onExit;
		tic80->tickData.data = tic80;

		tic80->tickData.start = 0;
		tic80->tickData.freq = getFreq;
		tic80->tickData.counter = getCounter;
		TickCounter = 0;
	}

	{
		tic80->memory->api.load(&tic80->memory->cart, cart, size, true);
		tic80->memory->api.reset(tic80->memory);
	}
}

TIC80_API void tic80_tick(tic80* tic, tic80_input input)
{
	tic80_local* tic80 = (tic80_local*)tic;

	tic80->memory->ram.vram.input.gamepad.data = input.data;
	
	tic80->memory->api.tick_start(tic80->memory, &tic80->memory->ram.sound);
	tic80->memory->api.tick(tic80->memory, &tic80->tickData);
	tic80->memory->api.tick_end(tic80->memory);

	tic80->memory->api.blit(tic80->memory, tic80->memory->api.scanline, tic80->memory->api.overlap, NULL);

	TickCounter++;
}

TIC80_API void tic80_delete(tic80* tic)
{
	tic80_local* tic80 = (tic80_local*)tic;

	tic_close(tic80->memory);

	free(tic80);
}
