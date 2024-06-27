#include "Header.h"
#include "Variables.h"
#include "resource.h"

// 1.34


void GetBase()
{
    module = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::offModuleBase); 
    base = VARS::memRead<uintptr_t>(module + offsets::offBase); 
}

void UpdateLabels(HWND hwnd)
{
    if (base == 0) { GetBase(); }
    
    rdi = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::offModuleForIterator);
    rdiObn = VARS::memRead<uintptr_t>(rdi + offsets::offForIterator);
    rdiOb = rdiObn >> 32;

    uint16_t iterator = static_cast<uint16_t>(rdiOb & 0xFFFF); 
    country = VARS::memRead<uintptr_t>(base + (iterator * 8));

    adm = VARS::memRead<int>(country + offsets::offAdm);
    dip = VARS::memRead<int>(country + offsets::offDip);
    mil = VARS::memRead<int>(country + offsets::offMil);
    cash = VARS::memRead<int>(country + offsets::offCash); cash /= 1000;
    man = VARS::memRead<int>(country + offsets::offManPower);

    int countryId = VARS::memRead<int>(country + offsets::offCountryId); 
    moduleProvince = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::offModuleProvince);
    baseProvince = VARS::memRead<uintptr_t>(moduleProvince + offsets::offBaseProvince);

    wstring ProvinceN;  
    char currentChar;
    provinceList.clear(); 
    provinceDev.clear(); 
    for (int i = 1; i < 5000; i++) 
    { 
        int ownerProvince = VARS::memRead<int>(baseProvince + (0x2E10 * i) + offsets::offProvinceOwner); 
        int capital = VARS::memRead<int>(baseProvince + (0x2E10 * i) + offsets::offProvincecapital);
        if (ownerProvince == countryId)  
        {
            DWORD_PTR currentAddress = (baseProvince + (0x2E10 * i) + offsets::offProvinceName); 

            ProvinceN = L""; 
            do {
                currentChar = VARS::memRead<char>(currentAddress);
                ProvinceN += currentChar;
                currentAddress += sizeof(char);
            } while (currentChar != L'\0');

            if (!ProvinceN.empty()) 
            { 
                ProvinceName = ProvinceN;  
                provinceList.push_back(ProvinceName); 
                provinceDev.push_back(i); 
                if (capital == 1) 
                {
                    CapitalName = ProvinceName;     
                    itIcap = i; 
                }
            }   
        }
    }
    sizeVec = provinceList.size(); 

    if (Freeze)
    {
        VARS::memWrite<int>(country + offsets::offAdm, Fadm);
        VARS::memWrite<int>(country + offsets::offDip, Fdip);
        VARS::memWrite<int>(country + offsets::offMil, Fmil);
        SetWindowText(g_hLabelFreeze, L"\u2713"); 
    }
    else
    {
        SetWindowText(g_hLabelFreeze, L"");
    }
    if (FreezeCash) 
    { 
        VARS::memWrite<int>(country + offsets::offCash, Fcash); 
        SetWindowText(g_hLabelFreezeCash, L"\u2713");
    }
    else
    {
        SetWindowText(g_hLabelFreezeCash, L"");
    }
    if (FreezeMan) 
    { 
        VARS::memWrite<int>(country + offsets::offManPower, Fman); 
        SetWindowText(g_hLabelFreezeMan, L"\u2713");
    }
    else
    {
        SetWindowText(g_hLabelFreezeMan, L"");
    }
    
    SetWindowText(g_hAdmLabel, (L"Adm: " + std::to_wstring(adm)).c_str());    
    SetWindowText(g_hDipLabel, (L"Dip: " + std::to_wstring(dip)).c_str());
    SetWindowText(g_hMilLabel, (L"Mil: " + std::to_wstring(mil)).c_str());
    SetWindowText(g_hCashLabel, (L"Cash: " + std::to_wstring(cash)).c_str());
    SetWindowText(g_hManLabel, (L"ManP: " + std::to_wstring(man)).c_str());

    //SetWindowText(g_hProvinceEdit, (L"Cap. " + CapitalName).c_str());
    if (key)
    {
        SendMessageW(hListBox, LB_RESETCONTENT, 0, 0);
        selectedIndex = 0;
        AddItemsToListBox(provinceList);
        key = !key;
        sizeVec = provinceList.size();
        sizeVecNew = sizeVec;
    }
    else
    {
        if (sizeVecNew != sizeVec)
        {
            selectedIndex = 0;
            key = !key;
        }
    }

    SetWindowText(g_hProvinceEdit, (provinceList.at(selectedIndex)).c_str());  

    provinceAdm = VARS::memRead<int>(baseProvince + (0x2E10 * provinceDev.at(selectedIndex)) + offsets::offProvinceAdm) / 1000; 
    provinceDip = VARS::memRead<int>(baseProvince + (0x2E10 * provinceDev.at(selectedIndex)) + offsets::offProvinceDip) / 1000;
    provinceMil = VARS::memRead<int>(baseProvince + (0x2E10 * provinceDev.at(selectedIndex)) + offsets::offProvinceMil) / 1000; 

    SetWindowText(g_hProvinceEditAdm, std::to_wstring(provinceAdm).c_str());  
    SetWindowText(g_hProvinceEditDip, std::to_wstring(provinceDip).c_str()); 
    SetWindowText(g_hProvinceEditMil, std::to_wstring(provinceMil).c_str()); 
}

void NewValues(int newAdm, int newDip, int newMil)
{
    if (newAdm > 0) { adm = newAdm; }
    if (newDip > 0) { dip = newDip; }
    if (newMil > 0) { mil = newMil; }

    VARS::memWrite<int>(country + offsets::offAdm, adm);
    VARS::memWrite<int>(country + offsets::offDip, dip);
    VARS::memWrite<int>(country + offsets::offMil, mil);
}

void NewValueCash(int newCash)
{
    cash = VARS::memRead<int>(country + offsets::offCash);
    if (newCash > 0) { cash = newCash * 1000; }
    VARS::memWrite<int>(country + offsets::offCash, cash);
}

void NewValueMan(int newMan)
{
    if (newMan > 0) { man = newMan; }
    VARS::memWrite<int>(country + offsets::offManPower, man);
}

void UPA()
{
    provinceAdm++; 
    VARS::memWrite<int>(baseProvince + (0x2E10 * provinceDev.at(selectedIndex)) + offsets::offProvinceAdm, provinceAdm * 1000);
}

void UPD()
{
    provinceDip++;
    VARS::memWrite<int>(baseProvince + (0x2E10 * provinceDev.at(selectedIndex)) + offsets::offProvinceDip, provinceDip * 1000);
}

void UPM()
{
    provinceMil++;
    VARS::memWrite<int>(baseProvince + (0x2E10 * provinceDev.at(selectedIndex)) + offsets::offProvinceMil, provinceMil * 1000);
}

void MA()
{
    if (provinceAdm >= 1)
        provinceAdm--;
    VARS::memWrite<int>(baseProvince + (0x2E10 * provinceDev.at(selectedIndex)) + offsets::offProvinceAdm, provinceAdm * 1000);
}

void MD()
{
    if (provinceDip >= 1)
        provinceDip--;
    VARS::memWrite<int>(baseProvince + (0x2E10 * provinceDev.at(selectedIndex)) + offsets::offProvinceDip, provinceDip * 1000);
}

void MM()
{
    if (provinceMil>=1)
        provinceMil--;
    VARS::memWrite<int>(baseProvince + (0x2E10 * provinceDev.at(selectedIndex)) + offsets::offProvinceMil, provinceMil * 1000);
}

void AddItemsToListBox(const std::vector<std::wstring>& provinces) 
{ 
    for (const auto& province : provinces) 
    {
        SendMessageW(hListBox, LB_ADDSTRING, 0, (LPARAM)province.c_str()); 
    }
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
    switch (message) 
    {
        case WM_PAINT: 
        { 
            PAINTSTRUCT ps; 
            HDC hdc = BeginPaint(hwnd, &ps); 

            //1
            HBRUSH hBorderBrush1 = CreateSolidBrush(RGB(0, 0, 0)); 
            HBRUSH hFillBrush1 = CreateSolidBrush(RGB(255, 255, 255)); 

            RECT rect1 = { 10, 10, 200, 145 }; 
            FrameRect(hdc, &rect1, hBorderBrush1); 

            InflateRect(&rect1, -1, -1); 
            FillRect(hdc, &rect1, hFillBrush1); 

            //2
            HBRUSH hBorderBrush2 = CreateSolidBrush(RGB(0, 0, 0)); 
            HBRUSH hFillBrush2 = CreateSolidBrush(RGB(255, 255, 255)); 

            RECT rect2 = { 240, 10, 450, 90 }; 
            FrameRect(hdc, &rect2, hBorderBrush2);  

            InflateRect(&rect2, -1, -1); 
            FillRect(hdc, &rect2, hFillBrush2); 

            //3
            HBRUSH hBorderBrush3 = CreateSolidBrush(RGB(0, 0, 0));  
            HBRUSH hFillBrush3 = CreateSolidBrush(RGB(255, 255, 255));  

            RECT rect3 = { 240, 110, 450, 190 }; 
            FrameRect(hdc, &rect3, hBorderBrush3); 

            InflateRect(&rect3, -1, -1); 
            FillRect(hdc, &rect3, hFillBrush3); 

            //4
            HBRUSH hBorderBrush4 = CreateSolidBrush(RGB(0, 0, 0)); 
            HBRUSH hFillBrush4 = CreateSolidBrush(RGB(255, 255, 255)); 

            RECT rect4 = { 10, 210, 200, 345 }; 
            FrameRect(hdc, &rect4, hBorderBrush4); 

            InflateRect(&rect4, -1, -1);
            FillRect(hdc, &rect4, hFillBrush4); 


            DeleteObject(hBorderBrush1); 
            DeleteObject(hFillBrush1); 
            DeleteObject(hBorderBrush2); 
            DeleteObject(hFillBrush2); 
            DeleteObject(hBorderBrush3);
            DeleteObject(hFillBrush3);

            EndPaint(hwnd, &ps); 

            break;
        }

        case WM_ERASEBKGND:
        {
            HDC hdc = (HDC)w_param;

            HBRUSH hBrush = CreateSolidBrush(RGB(23, 128, 109));

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            FillRect(hdc, &clientRect, hBrush);

            DeleteObject(hBrush);

            return TRUE;
        }

        case WM_GETMINMAXINFO:
        {
            MINMAXINFO* minMaxInfo = reinterpret_cast<MINMAXINFO*>(l_param);
            minMaxInfo->ptMinTrackSize.x = 480; 
            minMaxInfo->ptMinTrackSize.y = 400;

            minMaxInfo->ptMaxTrackSize.x = 480;
            minMaxInfo->ptMaxTrackSize.y = 400;

            break;
        }

        case WM_NCCREATE:
        {
            return DefWindowProc(hwnd, message, w_param, l_param);

            break;
        }

        case WM_CTLCOLORSTATIC: 
        {
            HDC hdcStatic = (HDC)w_param; 
            int controlId = GetDlgCtrlID((HWND)l_param); 

            if (controlId == IDC_YOURLABELID) { 
                SetTextColor(hdcStatic, RGB(0, 0, 0));  
                SetBkMode(hdcStatic, TRANSPARENT); 
                return (INT_PTR)GetStockObject(NULL_BRUSH);  
            }
            break;
        }

        case WM_CREATE:
        {
            HFONT hFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,   
                CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");   
            SendMessageW(hwnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));  

            HICON hIcon = LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_ICON1)); 
            SendMessageW(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);   


            // PowerPoints
            g_hAdmLabel = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE,
                30, 20, 100, 20, hwnd, NULL, NULL, NULL);
            g_hDipLabel = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE,
                30, 50, 100, 20, hwnd, NULL, NULL, NULL); 
            g_hMilLabel = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE,
                30, 80, 100, 20, hwnd, NULL, NULL, NULL); 

            g_hAdmEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,
                130, 20, 50, 20, hwnd, NULL, NULL, NULL);
            g_hDipEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,
                130, 50, 50, 20, hwnd, NULL, NULL, NULL);
            g_hMilEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,
                130, 80, 50, 20, hwnd, NULL, NULL, NULL);

            g_hButtonNewVal = CreateWindowW(L"BUTTON", L"Get Value", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                30, 110, 80, 25, hwnd, (HMENU)IDC_BUTTON, NULL, NULL); 
            g_hButtonFreeze = CreateWindowW(L"BUTTON", L"X", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                142, 110, 25, 25, hwnd, (HMENU)IDC_BUTTON_FREEZE, NULL, NULL);   
            g_hLabelFreeze = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE,
                172, 110, 25, 25, hwnd, NULL, NULL, NULL);

            g_hXLabel = CreateWindowW(L"STATIC", L"X - заморозить значение", WS_CHILD | WS_VISIBLE, 
                20, 155, 200, 20, hwnd, (HMENU)IDC_YOURLABELID, NULL, NULL); 
            g_hXGLabel = CreateWindowW(L"STATIC", L"\u2713 - заморозка включена", WS_CHILD | WS_VISIBLE, 
                20, 175, 200, 20, hwnd, (HMENU)IDC_YOURLABELID, NULL, NULL); 

            // Cash
            g_hCashLabel = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE,
                260, 20, 100, 20, hwnd, NULL, NULL, NULL);
            g_hCashEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,
                360, 20, 70, 20, hwnd, NULL, NULL, NULL);
            g_hButtonNewCash = CreateWindowW(L"BUTTON", L"Get Value", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                260, 50, 80, 25, hwnd, (HMENU)IDC_BUTTON_CASH, NULL, NULL);
            g_hButtonFreezeCash = CreateWindowW(L"BUTTON", L"X", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                382, 50, 25, 25, hwnd, (HMENU)IDC_BUTTON_FREEZE_CASH, NULL, NULL);
            g_hLabelFreezeCash = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE, 
                412, 50, 25, 25, hwnd, NULL, NULL, NULL); 

            // ManPower
            g_hManLabel = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE,
                260, 120, 100, 20, hwnd, NULL, NULL, NULL);
            g_hManEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,
                360, 120, 70, 20, hwnd, NULL, NULL, NULL);
            g_hButtonNewMan = CreateWindowW(L"BUTTON", L"Get Value", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                260, 150, 80, 25, hwnd, (HMENU)IDC_BUTTON_MAN, NULL, NULL);
            g_hButtonFreezeMan = CreateWindowW(L"BUTTON", L"X", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                382, 150, 25, 25, hwnd, (HMENU)IDC_BUTTON_FREEZE_MAN, NULL, NULL);
            g_hLabelFreezeMan = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE,
                412, 150, 25, 25, hwnd, NULL, NULL, NULL);

            // Provinces 
            g_hProvinceEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,  
                35, 220, 140, 24, hwnd, NULL, NULL, NULL); 
            g_hProvinceEditAdm = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER, 
                35, 280, 30, 20, hwnd, NULL, NULL, NULL);
            g_hProvinceEditDip = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER, 
                90, 280, 30, 20, hwnd, NULL, NULL, NULL);
            g_hProvinceEditMil = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER, 
                145, 280, 30, 20, hwnd, NULL, NULL, NULL);

            g_hButtonAdmPl = CreateWindowW(L"BUTTON", L"\u25B2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                40, 255, 20, 20, hwnd, (HMENU)IDC_BUTTON_SVVA, NULL, NULL);
            g_hButtonDipPl = CreateWindowW(L"BUTTON", L"\u25B2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                95, 255, 20, 20, hwnd, (HMENU)IDC_BUTTON_SVVD, NULL, NULL);
            g_hButtonMilPl = CreateWindowW(L"BUTTON", L"\u25B2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                150, 255, 20, 20, hwnd, (HMENU)IDC_BUTTON_SVVM, NULL, NULL);

            g_hButtonAdmMin = CreateWindowW(L"BUTTON", L"\u25BC", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                40, 305, 20, 20, hwnd, (HMENU)IDC_BUTTON_SVNA, NULL, NULL);
            g_hButtonDipMin = CreateWindowW(L"BUTTON", L"\u25BC", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                95, 305, 20, 20, hwnd, (HMENU)IDC_BUTTON_SVND, NULL, NULL);
            g_hButtonMilMin = CreateWindowW(L"BUTTON", L"\u25BC", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
                150, 305, 20, 20, hwnd, (HMENU)IDC_BUTTON_SVNM, NULL, NULL);

            break;
        }

        case WM_COMMAND:
        {
            if (LOWORD(w_param) == IDC_BUTTON) 
            {
                int intValueAdm = 0, intValueDip = 0, intValueMil = 0;
                wchar_t* bufferAdm = NULL; wchar_t* bufferDip = NULL; wchar_t* bufferMil = NULL;

                int textLengthAdm = GetWindowTextLengthW(g_hAdmEdit); 
                if (textLengthAdm > 0)
                {
                    bufferAdm = new wchar_t[textLengthAdm + 1];
                    GetWindowTextW(g_hAdmEdit, bufferAdm, textLengthAdm + 1); 
                    intValueAdm = std::stoi(bufferAdm); 
                }

                int textLengthDip = GetWindowTextLengthW(g_hDipEdit); 
                if (textLengthDip > 0)
                { 
                    bufferDip = new wchar_t[textLengthDip + 1];
                    GetWindowTextW(g_hDipEdit, bufferDip, textLengthDip + 1);  
                    intValueDip = std::stoi(bufferDip);
                }
            
                int textLengthMil = GetWindowTextLengthW(g_hMilEdit);
                if (textLengthMil > 0)
                {
                    bufferMil = new wchar_t[textLengthMil + 1];
                    GetWindowTextW(g_hMilEdit, bufferMil, textLengthMil + 1);  
                    intValueMil = std::stoi(bufferMil);
                }
            
                NewValues(intValueAdm, intValueDip, intValueMil);

                SetWindowTextW(g_hAdmEdit, L"");
                SetWindowTextW(g_hDipEdit, L"");
                SetWindowTextW(g_hMilEdit, L"");

                delete[] bufferAdm;  
                delete[] bufferDip;  
                delete[] bufferMil; 
            }

            else if (LOWORD(w_param) == IDC_BUTTON_FREEZE)
            {
                Fadm = adm;
                Fdip = dip;
                Fmil = mil; 

                Freeze = !Freeze;
            }

            else if (LOWORD(w_param) == IDC_BUTTON_CASH) 
            {
                int intValueCash = 0;
                wchar_t* bufferCash = NULL;

                int textLengthCash = GetWindowTextLengthW(g_hCashEdit);  
                if (textLengthCash > 0) 
                {
                    bufferCash = new wchar_t[textLengthCash + 1];
                    GetWindowTextW(g_hCashEdit, bufferCash, textLengthCash + 1);
                    intValueCash = std::stoi(bufferCash);
                }
                NewValueCash(intValueCash);
                SetWindowTextW(g_hCashEdit, L"");

                delete[] bufferCash;
            }

            else if (LOWORD(w_param) == IDC_BUTTON_FREEZE_CASH)
            {
                Fcash = cash * 1000;

                FreezeCash = !FreezeCash;
            }

            else if (LOWORD(w_param) == IDC_BUTTON_MAN)
            {
                int intValueMan = 0;
                wchar_t* bufferMan = NULL;

                int textLengthMan = GetWindowTextLengthW(g_hManEdit);
                if (textLengthMan > 0)
                {
                    bufferMan = new wchar_t[textLengthMan + 1];
                    GetWindowTextW(g_hManEdit, bufferMan, textLengthMan + 1);
                    intValueMan = std::stoi(bufferMan);
                }
                NewValueMan(intValueMan);
                SetWindowTextW(g_hManEdit, L"");

                delete[] bufferMan;
            }

            else if (LOWORD(w_param) == IDC_BUTTON_FREEZE_MAN)
            {
                Fman = man;

                FreezeMan = !FreezeMan;
            }

            else if (LOWORD(w_param) == IDC_BUTTON_SVVA) 
            {
                UPA(); 
            } 

            else if (LOWORD(w_param) == IDC_BUTTON_SVVD) 
            {
                UPD(); 
            }

            else if (LOWORD(w_param) == IDC_BUTTON_SVVM) 
            {
                UPM(); 
            }

            else if (LOWORD(w_param) == IDC_BUTTON_SVNA)
            {
                MA(); 
            } 

            else if (LOWORD(w_param) == IDC_BUTTON_SVND)
            {
                MD(); 
            }

            else if (LOWORD(w_param) == IDC_BUTTON_SVNM)
            {
                MM(); 
            }

            else if (HIWORD(w_param) == LBN_SELCHANGE) 
            {
                selectedIndex = SendMessageW(hListBox, LB_GETCURSEL, 0, 0);  
                if (selectedIndex != LB_ERR) 
                {
                    wchar_t buffer[MAX_PATH]; 
                    SendMessageW(hListBox, LB_GETTEXT, selectedIndex, (LPARAM)buffer); 
                }
            }

            break;
        }
    
        case WM_TIMER:
        {
            UpdateLabels(hwnd);
            break;
        }
        
        case WM_DESTROY:
        {
            KillTimer(hwnd, 1); 
            PostQuitMessage(0);
            break;
        }
        
        default: { return DefWindowProc(hwnd, message, w_param, l_param); }
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSW wc = { 0 };  
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";

    RegisterClassW(&wc);   

    HWND hwnd = CreateWindowW(L"MyWindowClass", L"Giperbola", WS_OVERLAPPEDWINDOW,   
        CW_USEDEFAULT, CW_USEDEFAULT, 480, 400, NULL, NULL, hInstance, NULL);

    hListBox = CreateWindowW( 
        L"LISTBOX",   
        L"",            
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS | WS_BORDER, 
        240, 210, 210, 130, 
        hwnd,         
        NULL,         
        hInstance,     
        NULL         
    );

    SetTimer(hwnd, 1, 100, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}