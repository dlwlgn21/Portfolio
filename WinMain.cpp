#include <Windows.h>

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


	return 0;
}
