#include "oracle.h"

/*#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>*/


char *oracle_vsprintf(const char *fmt, va_list args)
{
	// va_list can be exhausted each time it is referenced
	// by a ...v style function. Since we use it twice, make a copy

	va_list args_copy;
	va_copy(args_copy, args);

	size_t size = vsnprintf(NULL, 0, fmt, args_copy) + 1;

	va_end(args_copy);

	char *str = calloc(1, size);
	vsnprintf(str, size, fmt, args);

	return str;
}

static void log_and_print(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char *log_string = oracle_vsprintf(fmt, args);
	va_end(args);

	if (log_string != NULL) {
		printf(log_string);
		FILE *tmp_file = NULL;
		if (fopen_s(&tmp_file, "Oracle.txt", "ab") == 0) {
			fprintf(tmp_file, log_string);
			fclose(tmp_file);
		}
	}
}

static char *get_registry_key_string(const HKEY hkey, const char *subkey, const char *value) {
	HKEY opened_key = NULL;
	LSTATUS e = RegOpenKeyExA(hkey, subkey, 0, KEY_QUERY_VALUE, &opened_key);
	if (e != ERROR_SUCCESS) return NULL;

	char *return_string = NULL;

	DWORD size = 0;
	DWORD type = 0;
	e = RegQueryValueExA(opened_key, value, NULL, &type, NULL, &size);
	if (e != ERROR_SUCCESS || size == 0 || type != REG_SZ) goto except;

	return_string = calloc(1, size + 1);
	e = RegQueryValueExA(opened_key, value, NULL, NULL, (LPBYTE)return_string, &size);
	if (e != ERROR_SUCCESS || size == 0) {
		free(return_string);
		return_string = NULL;
		goto except;
	}

except:
	RegCloseKey(opened_key);
	return return_string;
}

static uint32_t get_registry_key_dword(const HKEY hkey, const char *subkey, const char *value) {
	HKEY opened_key = NULL;
	LSTATUS e = RegOpenKeyExA(hkey, subkey, 0, KEY_QUERY_VALUE, &opened_key);
	if (e != ERROR_SUCCESS) return 0;

	DWORD ret_value = 0;
	DWORD size = 0;
	DWORD type = 0;
	e = RegQueryValueExA(opened_key, value, NULL, &type, NULL, &size);
	if (e != ERROR_SUCCESS || type != REG_DWORD) { ret_value = 0; goto except; }

	e = RegQueryValueExA(opened_key, value, NULL, NULL, (LPBYTE)&ret_value, &size);
	if (e != ERROR_SUCCESS || type != REG_DWORD) { ret_value = 0; goto except; }

except:
	RegCloseKey(opened_key);
	return ret_value;
}

static bool find_and_copy_str(const char *haystack, const char *needle, const uint32_t max_length, char *result)
{
	const char *substr = strstr(haystack, needle);
	if (substr == NULL) return false;

	substr += strlen(needle);
	snprintf(result, max_length, "%s", substr)	;
	return true;
}

static void print_adapter_information(const DISPLAY_DEVICEA dd)
{
	log_and_print("-Name:           %s\r\n", dd.DeviceString);

	char vid[5] = {0};
	if (find_and_copy_str(dd.DeviceID, "VEN_", 5, vid)) log_and_print("-Vendor ID:      %s\r\n", vid);
	char pid[5] = {0};
	if (find_and_copy_str(dd.DeviceID, "DEV_", 5, pid)) log_and_print("-Device ID:      %s\r\n", pid);

	char *registry_path = strstr(dd.DeviceKey, "\\");
	if (registry_path == NULL) return;
	registry_path = strstr(registry_path + 1, "\\");
	if (registry_path == NULL) return;
	registry_path = strstr(registry_path + 1, "\\");
	if (registry_path == NULL) return;

	char *driver_date = get_registry_key_string(HKEY_LOCAL_MACHINE, registry_path + 1, "DriverDate");
	if (driver_date) {
		log_and_print("-Driver Date:    %s\r\n", driver_date);
		free(driver_date);
	}

	char *driver_version = get_registry_key_string(HKEY_LOCAL_MACHINE, registry_path + 1, "DriverVersion");
	if (driver_version) {
		log_and_print("-Driver Version: %s\r\n", driver_version);
		free(driver_version);
	}
}

static void print_monitor_information(const DISPLAY_DEVICEA md)
{
	DEVMODEA dm = {0};
	dm.dmSize = sizeof(DEVMODEA);
	if (EnumDisplaySettingsA(md.DeviceName, ENUM_CURRENT_SETTINGS, &dm)) {
		log_and_print("    -Resolution:   %ldx%ld\r\n", dm.dmPelsWidth, dm.dmPelsHeight);
		log_and_print("    -Refresh Rate: %ldHz\r\n", dm.dmDisplayFrequency);
		log_and_print("    -Position:     [%ld %ld]\r\n", dm.dmPosition.x, dm.dmPosition.y);
	}
}

static void print_display_information(void)
{
	char cur_dev_id[128] = {0};
	uint32_t gpu_count = 0;
	uint32_t monitor_count = 0;

	DISPLAY_DEVICEA dd = {0};
	dd.cb = sizeof(DISPLAY_DEVICEA);
	for (int32_t di = 0; EnumDisplayDevicesA(NULL, di, &dd, 0); di++) {
		if (strcmp(dd.DeviceID, cur_dev_id)) {
			snprintf(cur_dev_id, 128, "%s", dd.DeviceID);
			if (cur_dev_id[0] != '\0') log_and_print("\r\n");

			monitor_count = 0;
			log_and_print("GPU %d\r\n", gpu_count++);
			print_adapter_information(dd);
		}

		DISPLAY_DEVICEA md = {0};
		md.cb = sizeof(DISPLAY_DEVICEA);
		if (EnumDisplayDevicesA(dd.DeviceName, 0, &md, 0)) {
			log_and_print("\r\n    Monitor %d\r\n", monitor_count++);
			if (md.DeviceString[0] == '\0') {
				log_and_print("    -Name:         Generic PnP Monitor\r\n");
			} else {
				log_and_print("    -Name:         %s\r\n", md.DeviceString);
			}
			print_monitor_information(dd);
		}
	}
}

static void print_cpu_information(void)
{
	char *cpu_name = get_registry_key_string(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString");
	if (cpu_name == NULL) {
		log_and_print("Couldn't get CPU information.");
		return;
	}
	while (strlen(cpu_name) > 0 && cpu_name[strlen(cpu_name) - 1] == ' ') cpu_name[strlen(cpu_name) - 1] = '\0';
	uint32_t speed = get_registry_key_dword(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "~MHz");

	log_and_print("CPU: %s (%u MHz)\r\n", cpu_name, speed);
	free(cpu_name);
}

static void print_os_information(void)
{
	char *product_name = get_registry_key_string(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");
	if (product_name == NULL) {
		log_and_print("Couldn't get OS information.");
		return;
	}

	char *display_version = get_registry_key_string(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "DisplayVersion");
	if (display_version == NULL) {
		log_and_print("OS:  Microsoft %s\r\n", product_name);
	} else {
		log_and_print("OS:  Microsoft %s (%s)\r\n", product_name, display_version);
	}

	free(product_name);
	free(display_version);
}

/*static uint32_t get_console_length()
{
    CONSOLE_SCREEN_BUFFER_INFO buffer_info = {0};
    if( GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buffer_info) != FALSE)
        return (uint32_t) ( (buffer_info.dwSize.X * ( buffer_info.dwCursorPosition.Y + 1)) - (buffer_info.dwSize.X - ( buffer_info.dwCursorPosition.X)) );
    else
        return 0;
}*/

int32_t oracleMain(void)
{	
	
	DeleteFile(L"Oracle.txt");
	//SetConsoleTitle(L"Oracle - Get System Information.");

	printf("\n------------------------------SYSTEM SPECS------------------------------\n");
	print_display_information();
	log_and_print("\r\n");
	print_cpu_information();
	print_os_information();
	printf("------------------------------END SYSTEM SPECS------------------------------\n\n");


	//printf("\r\nPress ENTER to terminate.");
	//getchar();

	return 0;
}
