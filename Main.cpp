#include "Main.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI FileThread(LPVOID);
DWORD WINAPI ExtractThread(LPVOID);
DWORD WINAPI AddThread(LPVOID);

AppData app;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  HWND hWnd;
  WNDCLASS wndclass;
  MSG msg;
  INITCOMMONCONTROLSEX iccex;
  std::wstring lpszClass = app.CSetting.getString(kukdh1::Setting::ID_CAPTION);

  iccex.dwSize = sizeof(iccex);
  iccex.dwICC = ICC_WIN95_CLASSES | ICC_PROGRESS_CLASS | ICC_TREEVIEW_CLASSES;
  InitCommonControlsEx(&iccex);

  wndclass = { 0, WndProc, 0, 0, hInstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
               GetSysColorBrush(COLOR_BTNFACE), NULL, lpszClass.c_str() };

  RegisterClass(&wndclass);

  hWnd = CreateWindow(lpszClass.c_str(), lpszClass.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, nCmdShow);

  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}

BOOL Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {
  app.hButtonOpen = CreateWindow(WC_BUTTON, L"Open", WS_CHILD | WS_VISIBLE, 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)ID_BUTTON_OPEN, lpCreateStruct->hInstance, NULL);
  app.hButtonExctact = CreateWindow(WC_BUTTON, L"Extract", WS_CHILD | WS_VISIBLE | WS_DISABLED, 0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)ID_BUTTON_EXTRACT, lpCreateStruct->hInstance, NULL);
  
  // Pulsante analisi
  app.hButtonAnalyze = CreateWindow(WC_BUTTON, L"Analyze to JSON", WS_CHILD | WS_VISIBLE, 0, BUTTON_HEIGHT * 2, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)ID_BUTTON_ANALYZE, lpCreateStruct->hInstance, NULL);

  app.hTreeFileSystem = CreateWindow(WC_TREEVIEW, NULL, WS_CHILD | WS_VISIBLE, 0, BUTTON_HEIGHT * 3, 300, 400, hWnd, (HMENU)ID_TREE_FILESYSTEM, lpCreateStruct->hInstance, NULL);
  
  return TRUE;
}

void Cls_OnCommand(HWND hWnd, int id, HWND, UINT) {
  switch (id) {
    case ID_BUTTON_OPEN:
      // (mantieni il codice originale)
      break;

    case ID_BUTTON_EXTRACT:
      // (mantieni il codice originale)
      break;

    case ID_BUTTON_ANALYZE: {
      OPENFILENAME ofn = { 0 };
      WCHAR szFile[MAX_PATH] = { 0 };

      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = hWnd;
      ofn.lpstrFile = szFile;
      ofn.lpstrFilter = L"PAZ Files\0*.paz\0";
      ofn.nMaxFile = MAX_PATH;
      ofn.Flags = OFN_FILEMUSTEXIST;

      if (GetOpenFileName(&ofn)) {
        std::wstring jsonPath(szFile);
        jsonPath += L"_analysis.json";

        try {
          PazAnalyzer::analyzeAndLog(szFile, jsonPath);
          MessageBox(hWnd, L"Analysis Completed.", L"Success", MB_OK);
        } catch (std::exception & e) {
          MessageBoxA(hWnd, e.what(), "Error", MB_ICONERROR);
        }
      }
    } break;
  }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
    HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
    HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
    
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}