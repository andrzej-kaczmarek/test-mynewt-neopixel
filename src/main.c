/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "sysinit/sysinit.h"
#include "os/os.h"
#include "bsp/bsp.h"
#include "pwm/pwm.h"
#include "ws2812.h"

static uint32_t neopixel[32];
static struct os_callout reload_co;

static void
reload_event_func(struct os_event *ev)
{
    static int off = 0;
    int i, idx;

    for (i = 0; i < 32; i++) {
        idx = (i + off) % 32;
        neopixel[i] = idx % 8 ? 0x000000 : 0x000f00;
    }

    ws2812_write(neopixel);

    os_callout_reset(&reload_co, OS_TICKS_PER_SEC / 20);
    off++;
}

int
main(int argc, char **argv)
{
    int rc;

    sysinit();

    rc = ws2812_init();
    assert(rc == 0);

    os_callout_init(&reload_co, os_eventq_dflt_get(), reload_event_func, NULL);
    os_callout_reset(&reload_co, 0);

    while (1) {
        os_eventq_run(os_eventq_dflt_get());
    }

    assert(0);

    return rc;
}
