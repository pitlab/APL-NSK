#pragma once

#include <iostream>
#include <string>


class CRegistry
{
	std::wstring GetStringValueFromHKLM(const std::wstring& regSubKey, const std::wstring& regValue);

};

