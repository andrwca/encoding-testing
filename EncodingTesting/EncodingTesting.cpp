// EncodingTesting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <Windows.h>

#include "EncodingTesting.h"

int main()
{
	testCodePageConversion();
	testNormalization();

	return 0;
}

void testCodePageConversion()
{
	// This tests converting from UTF-16 to the local code page. This test will fail, if the system locale
	// is not set to Japanese. In this case, it'll attempt to convert the text into the local code page, 
	// which will fail, as the local code page will not contain the test character.

	std::cout << "Converting Japanese text into local code page." << std::endl;

	// Single unicode character.
	// Unicode character present in Unicode encodings, and the Japanese local code page 932 (Shift-JIS) https://en.wikipedia.org/wiki/Shift_JIS
    // https://www.fileformat.info/info/unicode/char/30c2/index.htm
	std::wstring s1 = L"ｼ";

	// Get the current code page
	int codePage = GetACP();
	std::cout << "Current code page: " << codePage << std::endl;

	// Get the size needed for the string to be converted into the local code page.
	// This will fail if the character is not present in the local code page.

	int size = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, s1.data(), -1, nullptr, 0, "?", nullptr);
	if (size < 1)
	{
		std::cout << "An error occured: " << GetLastError() << std::endl;
		return;
	}

	std::string multibyteString;
	multibyteString.resize(size);

	int usedDefaultChar = 0;
	int result = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, s1.data(), -1, &multibyteString[0], size, "?", &usedDefaultChar);
	if (result == 0)
	{
		std::cout << "An error occured: " << GetLastError() << std::endl;
		return;
	}

	// At this stage, if the system locale is set to Japanese, the string will be converted to the Japenese local code page. Note, at 
	// this stage, the string won't look right in the Visual Studio debugger, but it is correct.
	// 
	// If the system locale wasn't Japenese, the conversion would succeed, due to WC_NO_BEST_FIT_CHARS, but the character in the 
	// string that couldn't be found in the local code page will be replaced with the default character, '?'.

	std::cout << "Converted string: " << multibyteString.c_str() << std::endl;
	std::cout << "Used default character: " << usedDefaultChar << std::endl;
}

void testNormalization()
{
	// This tests the difference between a single unicode character and a combination of two unicode characters.
	// Where this looks the same visually, they are different in terms of byte comparison.

	std::cout << "Testing normalization." << std::endl;

	// Single unicode character.
	// https://www.fileformat.info/info/unicode/char/045e/index.htm
	std::wstring s1 = L"ў";

	// Two unicode characters combined.
	// https://www.fileformat.info/info/unicode/char/0443/index.htm
	// https://www.fileformat.info/info/unicode/char/0306/index.htm
	std::wstring s2 = L"ў";

	std::cout << "Length of s1: " << wcslen(s1.data()) << std::endl;
	std::cout << "Length of s2: " << wcslen(s2.data()) << std::endl;

	int result2 = _wcsnicmp(s1.data(), s2.data(), s1.length());
	std::cout << "Result of comparison s1 == s2: " << result2 << std::endl;

	// Normalize both strings.
	std::wstring s1Normalized;
	normalize(s1, s1Normalized);

	std::wstring s2Normalized;
	normalize(s2, s2Normalized);

	std::cout << "Length of normalized s1: " << wcslen(s1Normalized.data()) << std::endl;
	std::cout << "Length of normalized s2: " << wcslen(s2Normalized.data()) << std::endl;

	int result3 = _wcsnicmp(s1Normalized.data(), s2Normalized.data(), s1Normalized.length());
	std::cout << "Result of normalized comparison s1 == normalized s2: " << result3 << std::endl;
}

void normalize(const std::wstring string, std::wstring& normalizedString)
{
	// First get the length estimate of the normalized string.
	int s1NormalisedLength = NormalizeString(NormalizationC, string.data(), string.length(), nullptr, 0);
	normalizedString.resize(s1NormalisedLength);

	// Now normalize the string. This may need to be called multiple times.
	// See: https://learn.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-normalizestring
	NormalizeString(NormalizationC, string.data(), string.length(), &normalizedString[0], normalizedString.length());
}
