#include "Core/CGame.h"

// TODO
// Blending tester ¸¸µé±â

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	CGame Game{1280, 720};

	Game.Create(hInstance, WndProc, "GameBase");

	Game.AddObject2D(DirectX::XMFLOAT2(600.0f, 400.0f), "Asset/flight.png");

	Game.SetPlayerObject2D(0);

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
			DirectX::Keyboard::State KeyboardState{ Game.GetKeyboardState() };

			CObject2D* const PlayerObject2D{ Game.GetPlayerObject2D() };

			if (KeyboardState.Up) { PlayerObject2D->Translate(DirectX::XMVectorSet(0, +1, 0, 0)); }
			if (KeyboardState.Down) { PlayerObject2D->Translate(DirectX::XMVectorSet(0, -1, 0, 0)); }
			if (KeyboardState.Right) { PlayerObject2D->Translate(DirectX::XMVectorSet(+1, 0, 0, 0)); }
			if (KeyboardState.Left) { PlayerObject2D->Translate(DirectX::XMVectorSet(-1, 0, 0, 0)); }
			   
			Game.BeginRendering(BackgroundColor);

			Game.Draw();

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
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(Msg, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(Msg, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(Msg, wParam, lParam);
		break;
	default:
		break;
	}


	return DefWindowProc(hWnd, Msg, wParam, lParam);
}