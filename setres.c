#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <width> <height>\n", argv[0]);
        return 1;
    }

    // 引数の検証（数値かつ正の整数か確認）
    char* endptr;
    long width = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || width <= 0) {
        fprintf(stderr, "Error: Invalid width value.\n");
        return 1;
    }
    long height = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || height <= 0) {
        fprintf(stderr, "Error: Invalid height value.\n");
        return 1;
    }

    // DEVMODE構造体の初期化
    DEVMODE devmode;
    ZeroMemory(&devmode, sizeof(devmode));
    devmode.dmSize = sizeof(DEVMODE);

    // 現在のディスプレイ設定を取得
    if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode)) {
        fprintf(stderr, "Error: Failed to get current display settings.\n");
        return 1;
    }

    // 解像度のみ上書き（色深度・リフレッシュレートは現状維持）
    devmode.dmPelsWidth = (DWORD)width;
    devmode.dmPelsHeight = (DWORD)height;

    // 1. 設定のテスト（実際に適用する前にサポートされているか確認）
    LONG result = ChangeDisplaySettings(&devmode, CDS_TEST);
    if (result == DISP_CHANGE_SUCCESSFUL) {
        // 2. テスト成功後に実際に適用
        result = ChangeDisplaySettings(&devmode, 0);
        if (result == DISP_CHANGE_SUCCESSFUL) {
            printf("✅ Resolution changed to %ldx%ld successfully.\n", width, height);
            return 0;
        } else {
            fprintf(stderr, "❌ Failed to apply settings. Error code: %ld\n", result);
            return 1;
        }
    } else {
        fprintf(stderr, "❌ Resolution %ldx%ld is not supported by the display driver.\n", width, height);
        return 1;
    }
}
