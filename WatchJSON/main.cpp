// rapidjson에서 std::numeric_limits<>::max()를 사용하는데
// minwindef.h의 max(a, b)와 형태가 같아서 인자가 부족하다는 오류가 나왔음
// minwindef.h에서 NOMINMAX가 정의되어 있지 않으면 max, min을 정의한다고 했기 때문에 NOMINMAX를 정의 해주자
#define NOMINMAX

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"

wchar_t directoryName[100];
std::vector<std::wstring>dirList;

void WriteIniFile()
{
	//WritePrivateProfileString(L"DIRECTORY", L"1", L"C:\\Users\\KOCCA43\\source\\repos\\WatchJSON\\WatchJSON", L"C:\\Users\\KOCCA43\\source\\repos\\WatchJSON\\tmp.ini");
	WritePrivateProfileString(L"DIRECTORY", L"1", L"C:\\Users\\leejeongmo\\Desktop\\WatchJSON\\WatchJSON", L"C:\\Users\\leejeongmo\\Desktop\\WatchJSON\\setting.ini");
}

void ReadIniFile(wchar_t* out)
{
	//GetPrivateProfileString(L"DIRECTORY", L"1", L"cannot find key", directoryName, 100, L"C:\\Users\\KOCCA43\\source\\repos\\WatchJSON\\setting.ini");
	GetPrivateProfileString(L"DIRECTORY", L"1", L"cannot find key", out, 100, L"C:\\Users\\leejeongmo\\Desktop\\WatchJSON\\setting.ini");
}

void PrintJSONFileName()
{
	wcscat_s(directoryName, L"\\*.json"); // 와일드카드 문자: 어떤 동작에 대해, 여러 파일에 한꺼번에 지정하기 위함
	WIN32_FIND_DATA data;

	HANDLE hFind = FindFirstFile(directoryName, &data);
	int fileNumber = 1;
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	dirList.push_back(std::wstring(data.cFileName));
	++fileNumber;

	while (FindNextFile(hFind, &data))
	{
		dirList.push_back(std::wstring(data.cFileName));
		++fileNumber;
	}

	std::cout << "JSON FILE LIST" << std::endl;
	int num = 1;
	for (const std::wstring& s : dirList)
	{
		std::wcout << std::to_wstring(num) + std::wstring(L": ");
		std::wcout << s << std::endl;

		++num;
	}
	std::cout << std::endl;
}

std::wstring SelectJSONFile()
{
	int select = 0;
	std::cout << "출력할 json파일의 번호 입력: ";
	std::cin >> select;

	//std::wcout << dirList[select - 1] << std::endl;
	return dirList[select - 1];
}

void WriteJSONFile()
{
	rapidjson::Document d;
	d.SetObject();

	d.AddMember("name", "JSON FILE B", d.GetAllocator());
	d.AddMember("number", 2, d.GetAllocator());

	FILE* fp = nullptr;
	fopen_s(&fp, "b.json", "wb");
	char buffer[1024];

	rapidjson::FileWriteStream os(fp, buffer, sizeof(buffer));
	rapidjson::Writer<rapidjson::FileWriteStream>writer(os);
	d.Accept(writer);
	fclose(fp);
}

void ReadJSONFileAndPrint(std::wstring select)
{
	FILE* fp = nullptr;
	_wfopen_s(&fp, select.c_str(), L"rb");
	char buffer[1024];
	rapidjson::FileReadStream is(fp, buffer, sizeof(buffer));
	rapidjson::Document d;
	d.ParseStream(is);
	fclose(fp);

	std::cout << d["name"].GetString() << std::endl;
	std::cout << d["number"].GetInt() << std::endl;
}

int main()
{
	//WriteIniFile();

	// setting.ini 파일에 설정한 디렉토리명 가져오기
	ReadIniFile(directoryName);

	// 디렉토리에 있는 json 파일명만 출력
	PrintJSONFileName();

	// 특정 json 파일명 선택하면 화면에 출력
	std::wstring select = SelectJSONFile();

	//WriteJSONFile();

	// 선택한 json파일 화면에 출력
	ReadJSONFileAndPrint(select);

	return 0;
}