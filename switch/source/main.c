#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <switch.h>
#include "pixels_operation.h"
#include "pcsx_rearmed.h"

static int key_map[14][2] = {
    {KEY_DUP, 1 << DKEY_UP},
    {KEY_DDOWN, 1 << DKEY_DOWN},
    {KEY_DLEFT, 1 << DKEY_LEFT},
    {KEY_DRIGHT, 1 << DKEY_RIGHT},
    {KEY_X, 1 << DKEY_TRIANGLE},
    {KEY_Y, 1 << DKEY_SQUARE},
    {KEY_A, 1 << DKEY_CIRCLE},
    {KEY_B, 1 << DKEY_CROSS},
    {KEY_MINUS, 1 << DKEY_SELECT},
    {KEY_PLUS, 1 << DKEY_START},
    {KEY_L, 1 << DKEY_L1},
    {KEY_ZL, 1 << DKEY_L2},
    {KEY_R, 1 << DKEY_R1},
    {KEY_ZR, 1 << DKEY_R2}
};

static int key_count = sizeof(key_map) / sizeof(key_map[0]);

short switch_input() {
    hidScanInput();
    u64 kDown = hidKeysHeld(CONTROLLER_P1_AUTO);
    short result = 0;
    
    for (int i = 0; i < key_count; i++) {
        if (kDown & key_map[i][0]) {
            result |= key_map[i][1];
        }
    }
    
    if (kDown & KEY_LSTICK && kDown & KEY_RSTICK) {
        pcsx_rearmed_stop();
    }
    return result;
}

extern void* switch_fb(void) {
    return gfxGetFramebuffer(NULL, NULL);
}

void switch_set_video_mode(int w, int h) {
    static int _w = 0, _h = 0;
    
    if (_w != w || _h != h) {
        printf("switch_set_video_mode: %ix%i\n", w, h);
        
        _w = w;
        _h = h;
        
        gfxExit();
        gfxInitResolution(_w, _h);
        gfxInitDefault();
    }
}

void switch_flip(void *buf_ptr, int w, int h, int fps, float vsps) {
    u32 fbw;
    
    u8 *fb = gfxGetFramebuffer(&fbw, NULL);
    
    static char fps_str[32];
    sprintf(fps_str, "fps: %i, vsps: %4.1f", fps, vsps);
    
    draw_string(fb, fbw, fps_str, 10, 10, 0xFF, 0xFF, 0xFF);
    
    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();
}

int main(int argc, char **argv) {
    gfxInitDefault();
    
#if DEBUG
    socketInitializeDefault();
    nxlinkStdio();
#endif
    
    char game[128], frameskip[2], game_path[256];
    FILE *file = fopen("sdmc:/switch/pcsx_rearmed/pcsx_rearmed.cfg", "r");
    fscanf(file, "game = %[^\n;]%*c\n", game);
    fscanf(file, "\nframeskip = %[^\n;]%*c", frameskip);
    fclose(file);
    
    sprintf(game_path, "sdmc:/switch/pcsx_rearmed/%s", game);
    
    pcsx_rearmed_init("sdmc:/switch/pcsx_rearmed", atoi(frameskip));
    
    bool ret = pcsx_rearmed_load_game(game_path);
    if (!ret) {
        goto exit;
    }
    
    pcsx_rearmed_run();
    pcsx_rearmed_stop();
    
exit:
#if DEBUG
    socketExit();
#endif
    gfxExit();
    return 0;
}

