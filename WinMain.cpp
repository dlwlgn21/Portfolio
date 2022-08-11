#include <Windows.h>
#include <d2d1.h>
#include <math.h>
#include <sstream>


#pragma comment (lib, "d2d1.lib")

const wchar_t G_CLASS_NAME[] = L"MyWindowClass";
ID2D1Factory* GP_D2D_FACTORY{};
ID2D1HwndRenderTarget* GP_RENDER_TARGET{};
ID2D1SolidColorBrush* GP_BRUSH{};
ID2D1RadialGradientBrush* GP_RADIAL_GRADIENT_BRUSH{};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
void OnPaint(HWND hWnd);

int WINAPI WinMain (
	_In_ HINSTANCE hInstance,				// OS가 실행시킨 현재 인스턴스의 핸들
	_In_opt_ HINSTANCE hPrevInstance,		// 이전 인스턴스의 핸들. 항상 NULL
	_In_ LPSTR lpCmdLine,					// 명령줄. 실행파일 뒤에 추가 명령
	_In_ int nShowCmd						// 일반 윈도우로 표시할지, 전체 화면이나 최소화 화면으로 표시할지에 대한 값
)
{
	//MessageBoxA (
	//	nullptr,							// 메시지 박스를 소유할 부모 윈도우 지정. nullptr이면 운영체제가 메시지 박스를 제어함
	//	"Hello World!",						// 메시지 박스에 표시될 내용
	//	"SimpleWindow",						// 제목 표시줄에 표시될 내용
	//	MB_ICONEXCLAMATION | MB_OK			// 메시지 박스의 형태
	//);

	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&GP_D2D_FACTORY
		);
	if (FAILED(hr))
	{
		MessageBoxA(
			nullptr,
			"Failed to create D2D Factory",
			"Error",
			MB_ICONEXCLAMATION | MB_OK
		);
		return 0;
	}

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
	RECT wr = { 0, 0, 640, 480 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);
	
	
	// STEP 2 - CreateWindow
	hWnd = CreateWindowEx(
		NULL,
		G_CLASS_NAME,
		L"Direct2D",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
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
	GetClientRect(hWnd, &wr);
	hr = GP_D2D_FACTORY->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hWnd, 
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)
		),
		&GP_RENDER_TARGET
	);

	if (FAILED(hr))
	{
		MessageBoxA(
			nullptr,
			"Failed to create D2D Factory",
			"Error",
			MB_ICONEXCLAMATION | MB_OK
		);
	}

	hr = GP_RENDER_TARGET->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Aqua),
		&GP_BRUSH
	);

	if (FAILED(hr))
	{
		MessageBoxA(
			nullptr,
			"Failed to CreateSolidColorBrush",
			"Error",
			MB_ICONEXCLAMATION | MB_OK
		);
	}


	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			OnPaint(hWnd);
		}
	}


	if (GP_RADIAL_GRADIENT_BRUSH != nullptr)
	{
		GP_RADIAL_GRADIENT_BRUSH->Release();
		GP_RADIAL_GRADIENT_BRUSH = nullptr;
	}

	if (GP_BRUSH != nullptr)
	{
		GP_BRUSH->Release();
		GP_BRUSH = nullptr;
	}

	if (GP_RENDER_TARGET != nullptr)
	{
		GP_RENDER_TARGET->Release();
		GP_RENDER_TARGET = nullptr;
	}
	if (GP_D2D_FACTORY != nullptr)
	{
		GP_D2D_FACTORY->Release();
		GP_D2D_FACTORY = nullptr;
	}

	return static_cast<int>(msg.wParam);
}
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	HRESULT hr;
	GP_RENDER_TARGET->BeginDraw();
	GP_RENDER_TARGET->Clear(D2D1::ColorF(0.f, 0.2f, 0.4f, 1.0f));
	
	ID2D1GradientStopCollection* pGraStopCollections{};
	D2D1_GRADIENT_STOP gradientStops[2];

	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Crimson, 1);
	gradientStops[1].position = 1.0;

	hr = GP_RENDER_TARGET->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pGraStopCollections
	);

	if (SUCCEEDED(hr))
	{
		hr = GP_RENDER_TARGET->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(50.f, 150.f),
				D2D1::Point2F(0, 0),
				50,
				50
			),
			pGraStopCollections,
			&GP_RADIAL_GRADIENT_BRUSH
		);
	}

	GP_BRUSH->SetOpacity(1.f);
	GP_BRUSH->SetColor(D2D1::ColorF(D2D1::ColorF::Aquamarine));
	GP_RENDER_TARGET->FillRectangle(
		D2D1::RectF(0.f, 0.f, 100.f, 100.f),
		GP_BRUSH
	);
	GP_BRUSH->SetOpacity(0.5f);
	GP_BRUSH->SetColor(D2D1::ColorF(D2D1::ColorF::LightYellow));

	GP_RENDER_TARGET->FillRectangle(
		D2D1::RectF(50.f, 50.f, 150.f, 150.f),
		GP_BRUSH
	);
	static float fAngle = 0.0f;

	GP_RENDER_TARGET->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(75.f + (sinf(fAngle) * 25), 150.f), 50.f, 50.f),
		GP_RADIAL_GRADIENT_BRUSH
	);

	fAngle += 0.02f;
	if (pGraStopCollections != nullptr)
	{
		pGraStopCollections->Release();
		pGraStopCollections = nullptr;
	}

	GP_RENDER_TARGET->EndDraw();
	EndPaint(hWnd, &ps);
}

// STEP - 4 MakeWindowProc
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	
	switch (msg)
	{
	//case WM_PAINT:
	//{
	//	OnPaint(hWnd);
	//	break;
	//}
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