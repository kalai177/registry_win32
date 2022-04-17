#include<windows.h>
#include<iostream>
#include <stdio.h>
#include<string>
#define TOTAL_BYTES_READ    1024
#define OFFSET_BYTES 1024
using namespace std;
void CreateKey(HKEY parentkey, LPCTSTR subkey) {
	DWORD new_sub_key = 0;
	DWORD dwDisposition = 0;
	HKEY hkey;
	try {
		new_sub_key = RegCreateKeyEx(
			parentkey,
			subkey,
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&hkey,
			&dwDisposition
			);
		if (new_sub_key != ERROR_SUCCESS) {
			throw("Subkey is not created succesffuly!!");
		}
		cout << "Subkey created successfully!!" << endl;
		RegCloseKey(hkey);
	}
	catch (LPCTSTR errormsg) {
		cout << errormsg << endl;
	}

}

void WriteIntoKeyDWORD(HKEY parentkey, LPCTSTR subkey, LPCTSTR title) {
	DWORD input_data;
	cout << "Enter the integer value to store in key: ";
	cin >> input_data;
	DWORD writeinkey = 0;
	HKEY hkey;
	try {
		writeinkey = RegOpenKeyEx(
			parentkey,
			subkey,
			0,
			KEY_WRITE,
			&hkey
		);
		if (writeinkey != ERROR_SUCCESS) {
			throw("Subkey is not created succesffuly!!");
		}
		else {
			if (ERROR_SUCCESS !=
				RegSetValueEx(
					hkey,
					title,
					0,
					REG_DWORD,
					reinterpret_cast<BYTE*>(&input_data),
					sizeof(input_data)))
			{
				cout << "Writed DWORD successfully";
				RegCloseKey(hkey);
			}
		}

	}
	catch (LPCTSTR errormsg) {
		cout << errormsg << endl;
	}
}
void ReadFromKeyDWORD(HKEY parentkey, LPCTSTR subkey, LPCTSTR title) {
	DWORD int_data_val = 0;
	DWORD len = sizeof(int_data_val);
	HKEY hkey;
	DWORD readfromkey = 0;
	try {
		readfromkey = RegOpenKeyEx(
			parentkey,
			subkey,
			0,
			KEY_READ,
			&hkey
		);

			if (readfromkey != ERROR_SUCCESS) {
				throw("Subkey is not created succesffuly!!");
		
			}

			readfromkey = RegQueryValueEx(
				hkey,
				title,
				NULL,
				NULL,
				(LPBYTE)(& int_data_val),
				&len
			);

			if (readfromkey == ERROR_SUCCESS) {
				cout << "DWORD data read from key: " << int_data_val << endl;
				RegCloseKey(hkey);
			}
	}
	catch (LPCTSTR errormsg) {
		cout << errormsg << endl;
	}

}
void DeleteKey(HKEY parentkey, LPCTSTR subkey) {
	DWORD deletekey = 0;
	try {
		deletekey = RegDeleteKeyEx(
			parentkey,
			subkey,
			KEY_WOW64_32KEY,
			0
		);
		if (deletekey != ERROR_SUCCESS) {
			throw("Key not deleted");

		}
		else {
			cout << "Key succesfully deleted!!" << endl;
		}
	}
	catch (LPCTSTR errormsg) {
		cout << errormsg << endl;
	}
}
void WriteIntoKeySTRING(HKEY parentkey, LPCTSTR subkey, LPCTSTR title) {
	DWORD writeinkey = 0;
	HKEY hkey;
	char* stringdata = new char[256];
	cout << "Enter the string data: ";
	cin >> stringdata;
	char* copy_string_data = new char[strlen(stringdata) + 1];
	strcpy_s(copy_string_data, strlen(stringdata) + 1, stringdata);

	try {
		writeinkey = RegOpenKeyEx(
			parentkey,
			subkey,
			0,
			KEY_WRITE,
			&hkey
			);
		if (writeinkey != ERROR_SUCCESS) {
			throw("Key not deleted");

		}
		else {

			if (ERROR_SUCCESS != RegSetValueEx(
				hkey,
				title,
				0,
				REG_SZ,
				(LPBYTE)copy_string_data,
				((((DWORD)strlen(copy_string_data) + 1)) * 2)
			)) {

				cout << "string has successfully written in key!!" << endl;
			}
			RegCloseKey(hkey);
			
		}
	}
	catch (LPCTSTR errormsg) {
		cout << errormsg << endl;
	}
}
void ReadFromKeySTRING(HKEY parentkey, LPCTSTR subkey, LPCTSTR title) {
	HKEY hkey;
	DWORD readfromkey = 0;
	DWORD len = TOTAL_BYTES_READ;
	DWORD readatalen = len;
	PWCHAR readbuffer = (PWCHAR)malloc(sizeof(PWCHAR) * len);

	try {
		readfromkey = RegOpenKeyEx(
			parentkey,
			subkey,
			0,
			KEY_READ,
			&hkey
		);
		if (readfromkey != ERROR_SUCCESS) {
			throw("Key not deleted");
		}

		else {
			readfromkey = RegQueryValueEx(
				hkey,
				title,
				NULL,
				NULL,
				(BYTE*)readbuffer,
				&readatalen
			);

		    while(readfromkey == ERROR_MORE_DATA) {
				len += OFFSET_BYTES;
				readbuffer = (PWCHAR)realloc(readbuffer, len);
				readfromkey = RegQueryValueEx(
					hkey,
					title,
					NULL,
					NULL,
					(BYTE*)readbuffer,
					&readatalen

				);

				}
			cout << "The string data read from key: ";
			printf("%s", readbuffer);
			cout << endl;
			RegCloseKey(hkey);
		}
	}
	catch (LPCTSTR errormsg) {
		cout << errormsg << endl;
	}
}
int main() {
	//DWORD data_val = 0;
	//PWCHAR string_data = NULL;
	CreateKey(HKEY_CURRENT_USER, L"RegisterSubKey");
	WriteIntoKeyDWORD(HKEY_CURRENT_USER, L"RegisterSubKey", L"IntValue");
	ReadFromKeyDWORD(HKEY_CURRENT_USER, L"RegisterSubKey", L"IntValue");
	WriteIntoKeySTRING(HKEY_CURRENT_USER, L"RegisterSubKey", L"StringValue");
	ReadFromKeySTRING(HKEY_CURRENT_USER, L"RegisterSubKey", L"StringValue");
	DeleteKey(HKEY_CURRENT_USER, L"RegisterSubKey");


   
}