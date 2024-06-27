#ifndef VARIABLES_H
#define VARIABLES_H


uintptr_t rdi, rdiObn, rdiOb, country; 

HWND g_hAdmLabel, g_hDipLabel, g_hMilLabel, g_hXLabel, g_hXGLabel;
HWND g_hAdmEdit, g_hDipEdit, g_hMilEdit;
HWND g_hCashLabel, g_hCashEdit;
HWND g_hManLabel, g_hManEdit;
HWND g_hProvinceEdit;
HWND g_hProvinceEditAdm, g_hProvinceEditDip, g_hProvinceEditMil;

HWND g_hButtonNewVal, g_hButtonFreeze, g_hLabelFreeze; 
HWND g_hButtonNewCash, g_hButtonFreezeCash, g_hLabelFreezeCash;
HWND g_hButtonNewMan, g_hButtonFreezeMan, g_hLabelFreezeMan;
HWND g_hButtonAdmPl, g_hButtonDipPl, g_hButtonMilPl;
HWND g_hButtonAdmMin, g_hButtonDipMin, g_hButtonMilMin;
HWND g_hProvinceComboBox;  

uintptr_t module; 
uintptr_t base; 

int adm, dip, mil;
int cash, man;
int provinceAdm, provinceDip, provinceMil;

int Fadm, Fdip, Fmil;
int Fcash; int Fman;

bool Freeze = false;
bool FreezeCash = false;
bool FreezeMan = false;

uintptr_t rdx;

uintptr_t moduleProvince;
uintptr_t baseProvince;
int itI; 
int itIcap;
wstring ProvinceName; 
wstring CapitalName; 

std::vector<std::wstring> provinceList = { };
std::vector<int> provinceDev = { };  
void AddItemsToListBox(const std::vector<std::wstring>& provinces);
bool key = true;
int sizeVec; 
int sizeVecNew; 
HWND hListBox;
int selectedIndex = 0;

#endif