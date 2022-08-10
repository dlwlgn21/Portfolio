#include <Windows.h>
#include <sstream>
const wchar_t G_CLASS_NAME[] = L"MyWindowClass";
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

int WINAPI WinMain (
	_In_ HINSTANCE hInstance,				// OS�� �����Ų ���� �ν��Ͻ��� �ڵ�
	_In_opt_ HINSTANCE hPrevInstance,		// ���� �ν��Ͻ��� �ڵ�. �׻� NULL
	_In_ LPSTR lpCmdLine,					// �����. �������� �ڿ� �߰� ���
	_In_ int nShowCmd						// �Ϲ� ������� ǥ������, ��ü ȭ���̳� �ּ�ȭ ȭ������ ǥ�������� ���� ��
)
{
	//MessageBoxA (
	//	nullptr,							// �޽��� �ڽ��� ������ �θ� ������ ����. nullptr�̸� �ü���� �޽��� �ڽ��� ������
	//	"Hello World!",						// �޽��� �ڽ��� ǥ�õ� ����
	//	"SimpleWindow",						// ���� ǥ���ٿ� ǥ�õ� ����
	//	MB_ICONEXCLAMATION | MB_OK			// �޽��� �ڽ��� ����
	//);

	HWND hWnd;
	WNDCLASSEX wc;

	// STEP 1 - Register Window Class

	ZeroMemory(&wc, sizeof(wc));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = G_CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBoxA(
			nullptr,
			"FailedRegisterWNDCLASSEX",
			"Erorr",
			MB_ICONEXCLAMATION | MB_OK
		);
		return 0;
	}

	// STEP 2 - CreateWindow
	hWnd = CreateWindowEx(
		NULL,
		G_CLASS_NAME,
		L"MyAgainWindow",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hWnd == nullptr)
	{
		MessageBoxA(
			nullptr,
			"FailedCreateWindowEx",
			"Erorr",
			MB_ICONEXCLAMATION | MB_OK
		);
		return 0;
	}

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}


// STEP - 4 MakeWindowProc
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		{
			std::wostringstream oss;
			oss << "X : " << LOWORD(lp) << ", Y : " << HIWORD(lp);

			MessageBox(
				nullptr,
				oss.str().c_str(),
				L"Mouse Clikced!",
				MB_OK
			);
			break;
		}
	case WM_KEYDOWN:
		{
			std::wostringstream oss;
			oss << "Virtual Key = " << wp << ", Extra = " << std::hex << lp << "\n";
			
			OutputDebugString(oss.str().c_str());
			break;
		}
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}

	return 0;
}