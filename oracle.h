#pragma once

// Exported dependencies
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

char *oracle_vsprintf(const char *fmt, va_list args);

static void log_and_print(const char *fmt, ...);

static char *get_registry_key_string(const HKEY hkey, const char *subkey, const char *value);

static uint32_t get_registry_key_dword(const HKEY hkey, const char *subkey, const char *value);

static bool find_and_copy_str(const char *haystack, const char *needle, const uint32_t max_length, char *result);

static void print_adapter_information(const DISPLAY_DEVICEA dd);

static void print_monitor_information(const DISPLAY_DEVICEA md);

static void print_display_information(void);

static void print_cpu_information(void);

static void print_os_information(void);

//static uint32_t get_console_length();

int32_t oracleMain(void);