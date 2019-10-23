#include "Core/CGame.h"

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	CGame Game{1280, 720};

	Game.Create(hInstance, WndProc, "GameBase");

	MSG Msg{};

	float BackgroundColor[]{ 0.0f, 0.0f, 0.0f, 1.0f };

	while (true)
	{
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&Msg);

			DispatchMessage(&Msg);
		}
		else
		{
			Game.BeginRendering(BackgroundColor);

			Game.EndRendering();
		}
	}

	return 0;
}

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}

		return 0;
	default:
		break;
	}


	return DefWindowProc(hWnd, Msg, wParam, lParam);
}