#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib,"comctl32")

#include <windows.h>
#include <commctrl.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton;
	static HWND hTree;
	static BOOL bChangeColor;
	switch (msg)
	{
	case WM_CREATE:
		{
			InitCommonControls();
			hTree = CreateWindowEx(WS_EX_CLIENTEDGE,WC_TREEVIEW,0,
				WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_LINESATROOT,
				0, 0, 0, 0,hWnd,0,((LPCREATESTRUCT)lParam)->hInstance,0);
			TV_INSERTSTRUCT tv = { 0 };
			tv.hInsertAfter = TVI_LAST;
			tv.item.mask = TVIF_TEXT;
			tv.hParent = TVI_ROOT;
			tv.item.pszText = TEXT("Node0");
			const HTREEITEM hParent = TreeView_InsertItem(hTree, &tv);
			tv.hParent = hParent;
			tv.item.pszText = TEXT("Node1");
			const HTREEITEM hChild = TreeView_InsertItem(hTree, &tv);
			tv.hParent = hChild;
			tv.item.pszText = TEXT("Node2");
			const HTREEITEM hChildChild = TreeView_InsertItem(hTree, &tv);
			hButton = CreateWindow(TEXT("BUTTON"), TEXT("色を変更！"),
				WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDOK,
				((LPCREATESTRUCT)lParam)->hInstance, 0);
			SetFocus(hTree);
		}
		break;
	case WM_SIZE:
		MoveWindow(hTree, 10, 10, 256, 256, TRUE);
		MoveWindow(hButton, 10, 276, 256, 32, TRUE);
		break;
	case WM_NOTIFY:
		{
			LPNMHDR lpnmhdr = (LPNMHDR)lParam;
			if (lpnmhdr->hwndFrom == hTree && bChangeColor) {
				LPNMTREEVIEW lplv = (LPNMTREEVIEW)lParam;
				if (lplv->hdr.code == NM_CUSTOMDRAW) {
					LPNMTVCUSTOMDRAW lplvcd = (LPNMTVCUSTOMDRAW)lParam;
					if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
						return CDRF_NOTIFYITEMDRAW;
					if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) {
						if (lplvcd->nmcd.uItemState & CDIS_SELECTED) {
							// 選択項目の色を変更
							lplvcd->clrTextBk = RGB(200, 200, 255);
							lplvcd->clrText = RGB(0, 0, 0);
							return CDRF_DODEFAULT;
						}
					}
				}
			}
		}
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			bChangeColor = TRUE;
			// 背景色を変更
			SendMessage(hTree, TVM_SETBKCOLOR, 0, RGB(0, 0, 64));
			// 文字色を変更
			SendMessage(hTree, TVM_SETTEXTCOLOR, 0, RGB(255, 200, 200));
			MessageBox(hWnd, TEXT("確認"), 0, 0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}