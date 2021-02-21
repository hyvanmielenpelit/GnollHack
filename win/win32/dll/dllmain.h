
#ifndef DLLMAIN_H
#define DLLMAIN_H


typedef void(__stdcall* ProgressCallback)(int);

extern "C"
{
#define DLL __declspec(dllexport)

	DLL int DoSomeCalc();

	typedef void(__stdcall* ProgressCallback)(int);

	DLL void DoWork(ProgressCallback progressCallback);

}

#endif //DLLMAIN_H
