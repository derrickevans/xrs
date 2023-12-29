/*
 * Main entry point for the windows platform.
 */
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdint.h>
#include <stdbool.h>

#define internal_func static
#define local_persist static
#define global_variable static

global_variable bool is_app_running = false;
global_variable int app_window_width;
global_variable int app_window_height;
global_variable void *pixel_buffer;
global_variable BITMAPINFO bitmap_info;

void xrs_render(void) {
	int *pixel = (int *) pixel_buffer;

	for (int y = 0; y < app_window_height; ++y) {
		for (int x = 0; x < app_window_width; ++x) {
			*pixel++ = 0x00000000;
		}
	}
}

LRESULT win32_window_proc(HWND window, UINT msg, WPARAM w_param, LPARAM l_param) {
	LRESULT result = 0;

	switch (msg) {
		case WM_CLOSE:
		{
			is_app_running = false;
		} break;

		case WM_DESTROY:
		{
			is_app_running = false;
		} break;

		case WM_KEYDOWN:
		{
			switch (w_param) {
				case VK_ESCAPE:
				{
					is_app_running = false;
				} break;
			}
		} break;

		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC device_context = BeginPaint(window, &paint);

			bitmap_info.bmiHeader = (BITMAPINFOHEADER) {
				.biSize = sizeof(BITMAPINFOHEADER),
				.biWidth = app_window_width,
				.biHeight = app_window_height,
				.biPlanes = 1,
				.biBitCount = 32,
				.biCompression = BI_RGB,
				.biSizeImage = 0,
				.biXPelsPerMeter = 0,
				.biYPelsPerMeter = 0,
				.biClrUsed = 0,
				.biClrImportant = 0
			};	
			StretchDIBits(device_context, 0, 0, app_window_width, app_window_height, 0, 0, app_window_width, app_window_height, pixel_buffer, &bitmap_info, DIB_RGB_COLORS, SRCCOPY);

			EndPaint(window, &paint);
		} break;

		default:
		{
			result = DefWindowProc(window, msg, w_param, l_param);
		} break;
	}


	return result;
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, PWSTR cmd_line, int cmd_show) {
	// Allocate the pixel buffer.
	pixel_buffer = VirtualAlloc(0, app_window_width * app_window_height * 4, MEM_COMMIT, PAGE_READWRITE);
	xrs_render();

	// Register WNDCLASS.
	WNDCLASSEX window_class = {0};
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = win32_window_proc;
	window_class.hInstance = instance;
	window_class.lpszClassName = L"xrs_window_class";

	if (!RegisterClassEx(&window_class)) {
		// TODO: Log error.
		return -1;
	}

	// Create the window.
	//DWORD window_style = WS_OVERLAPPEDWINDOW;
	DWORD window_style = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;

	// The size we want the client rect to be.
	RECT app_client_rect = {
		.left = 0,
		.top = 0,
		.right = 640,
		.bottom = 480
	};

	// Adjust the rect so the window is the correct size with the given client rect area.
	if (!AdjustWindowRect(&app_client_rect, WS_OVERLAPPEDWINDOW, false)) { 
		// Failed to adjust the window rect.
		return -4;
	}

	app_window_width = app_client_rect.right - app_client_rect.left;
	app_window_height = app_client_rect.bottom - app_client_rect.top;

	HWND window = CreateWindow(
		window_class.lpszClassName,
		L"XRS",
		window_style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		app_window_width,
		app_window_height,
		NULL,
		NULL,
		instance,
		NULL
	);

	if (!window) {
		// TODO: Maybe try to recreate?
		return -2;
	}

	ShowWindow(window, SW_SHOW);

	is_app_running = true;

	MSG msg;
	while (is_app_running) {
		// Handle messages.
		while (PeekMessage(&msg, window, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);	
		}

		// Render and other stuff.
		xrs_render();
		UpdateWindow(window);
	}	

	return 0;
}